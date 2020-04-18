/*
** SIMBOT PROJECT, 2020
** topic.c
** File description: topic system
** 
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "log_system.h"
#include "topic.h"

unsigned long uniqid(unsigned long u_id)
{
    struct timeval t;
    unsigned long id;
    gettimeofday(&t, NULL);
    id = ((t.tv_sec * 1000 * 1000) + (t.tv_usec * 1000)) << 42;
    id |= (u_id % 16777216) << 24;
    return id;
}

t_topics *init_topic(int max_topic, int max_subscriber, int max_topic_message, int max_buffer_message, t_log *main_log_file)
{
    int i, s;
    pthread_attr_t attr;
    t_topics *topics = malloc(sizeof(t_topics));
    if (topics == NULL)
        handle_error("malloc");
    topics->nb_topic = 0;
    topics->message_counter = 0;
    topics->max_topic = max_topic;
    topics->max_subscriber = max_subscriber;
    topics->max_topic_message = max_topic_message;
    topics->max_buffer_message = max_buffer_message;
    topics->topics = calloc(max_topic, sizeof(t_topic *));
    if (topics->topics == NULL)
        handle_error("calloc");

    topics->message_collector = calloc(max_buffer_message, sizeof(struct message_reference));
    if (topics->message_collector == NULL)
        handle_error("calloc");
    topics->topics = calloc(max_topic, sizeof(t_topic));
    if (topics->topics == NULL)
        handle_error("calloc");

    for (i = 0; i < max_buffer_message; i++)
        topics->message_collector[i].adr = NULL;
    pthread_mutex_init(&topics->memory_mutex, NULL);

    topics->th_memory = malloc(sizeof(struct thread_info));
    if (topics->th_memory == NULL)
        handle_error("malloc");

    write_log(main_log_file, LEVEL_INFO, FILE_ONLY, "Start virtual memory manager");
    s = pthread_attr_init(&attr);
    if (s != 0)
        handle_error("(%d) pthread_attr_init", s);
    topics->th_memory->args = topics;
    topics->th_memory->end_thread = 0;
    topics->th_memory->log = create_log("garbage_collector");
    s = pthread_create(&topics->th_memory->thread_id, &attr,
                       &virtual_memory_manager, topics->th_memory);
    if (s != 0)
        handle_error("pthread_create");

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        handle_error("pthread_attr_destroy");

    return topics;
}

void *virtual_memory_manager(void *arg)
{
    struct thread_info *th_info = arg;
    t_topics *topics = th_info->args;
    struct return_value *return_value;
    int i;
    write_log(th_info->log, LEVEL_INFO, FILE_ONLY, "Virtual memory manager started");
    do
    {
        short_wait();
        pthread_mutex_lock(&topics->memory_mutex);
        for (i = 0; i < topics->max_buffer_message; i++)
        {
            if (topics->message_collector[i].adr != NULL && topics->message_collector[i].nb_subscriber == topics->message_collector[i].nb_reader)
            {
                write_log(th_info->log, LEVEL_INFO, FILE_ONLY, "Free message %lu", topics->message_collector[i].message_uniqid);
                free(topics->message_collector[i].adr);
                topics->message_collector[i].adr = NULL;
                topics->message_collector[i].nb_subscriber = 0;
                topics->message_collector[i].nb_reader = 0;
            }
        }
        pthread_mutex_unlock(&topics->memory_mutex);
    } while (!th_info->end_thread);

    write_log(th_info->log, LEVEL_INFO, FILE_ONLY, "Virtual memory will stop, clear unreaded messages");
    for (i = 0; i < topics->max_buffer_message; i++)
        if (topics->message_collector[i].adr != NULL)
        {
            write_log(th_info->log, LEVEL_INFO, FILE_ONLY, "Free message %lu", topics->message_collector[i].message_uniqid);
            free(topics->message_collector[i].adr);
        }
    return_value = malloc(sizeof(struct return_value));
    return_value->message = strdup("ok");
    return return_value;
}

int add_topic(t_topics *topics, const char *name, t_log *main_log_file)
{
    if (topics->nb_topic < topics->max_topic)
    {
        t_topic *topic = malloc(sizeof(t_topic));
        if (topic == NULL)
            handle_error("malloc");
        topic->name = strdup(name);
        topic->nb_subscriber = 0;
        topic->subscriber = calloc(topics->max_subscriber, sizeof(struct s_subscriber *));
        if (topic->subscriber == NULL)
            handle_error("calloc");
        topics->topics[topics->nb_topic] = topic;
        topics->nb_topic++;
        write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "New topic : %s", topic->name);
    }
    else
        handle_error("Max number of topics reached (%d)", topics->max_topic);
    return topics->nb_topic - 1;
}

int add_message(t_topics *topics, void *message, int nb_subscriber, t_log *log_file)
{
    pthread_mutex_lock(&topics->memory_mutex);
    int i = 0;
    while (topics->message_collector[i].adr != NULL && i < topics->max_buffer_message)
        i++;
    if (i >= topics->max_buffer_message)
        i = -1;
    else
    {
        topics->message_counter++;
        write_log(log_file, LEVEL_INFO, FILE_ONLY, "Add message %lu in memory", topics->message_counter);
        topics->message_collector[i].adr = message;
        topics->message_collector[i].nb_subscriber = nb_subscriber;
        topics->message_collector[i].nb_reader = 0;
        topics->message_collector[i].message_uniqid = topics->message_counter;
    }
    pthread_mutex_unlock(&topics->memory_mutex);
    return i;
}

int push_message(t_topics *topics, int topic_id, int subscriber_id, int message_id, t_log *log_file)
{
    if (topics->topics[topic_id]->subscriber[subscriber_id]->nb_message >= topics->max_topic_message)
    {
        write_log(log_file, LEVEL_ERROR, FILE_ONLY, "Cannot add message to %s backlog, it is full %d/%d",
                  topics->topics[topic_id]->name, topics->max_topic_message, topics->max_topic_message);
        return EXIT_FAILURE;
    }
    pthread_mutex_lock(&topics->topics[topic_id]->subscriber[subscriber_id]->mutex); // pour empêcher la lecture ou l'ecriture de message pour ce topic
    topics->topics[topic_id]->subscriber[subscriber_id]->nb_message++;
    topics->topics[topic_id]->subscriber[subscriber_id]->end =
        (topics->topics[topic_id]->subscriber[subscriber_id]->end < topics->max_topic_message) ? topics->topics[topic_id]->subscriber[subscriber_id]->end + 1 : 0;
    topics->topics[topic_id]->subscriber[subscriber_id]->message[topics->topics[topic_id]->subscriber[subscriber_id]->end] = message_id;
    pthread_mutex_unlock(&topics->topics[topic_id]->subscriber[subscriber_id]->mutex);
    return EXIT_SUCCESS;
}

int write_topic(t_topics *topics, int topic_id, void *message, t_log *log_file)
{
    int subscriber_id, s, final_state = EXIT_SUCCESS;
    int nb_subscriber = topics->topics[topic_id]->nb_subscriber;

    if (nb_subscriber == 0)
    {
        write_log(log_file, LEVEL_INFO, FILE_ONLY, "No subcriber on this topic");
        free(message);
        return EXIT_SUCCESS;
    }

    int message_id = add_message(topics, message, nb_subscriber, log_file);

    if (message_id < 0)
    {
        write_log(log_file, LEVEL_ERROR, FILE_ONLY, "Cannot create more message, virtual memory buffer is full %d/%d!", topics->max_buffer_message, topics->max_buffer_message);
        return EXIT_FAILURE;
    }

    for (subscriber_id = 0; subscriber_id < nb_subscriber; subscriber_id++)
    {
        s = push_message(topics, topic_id, subscriber_id, message_id, log_file);
        if (s == EXIT_FAILURE)
            final_state = s;
    }
    return final_state;
}

int subscribe_topic(t_topics *topics, int topic_id, struct node_info *node, void *(*callback)(void *))
{
    pthread_mutex_lock(&topics->topics[topic_id]->mutex); // pour empecher deux noeuds de souscrire en même temps au même topic
    if (topics->topics[topic_id]->nb_subscriber >= topics->max_subscriber)
    {
        write_log(node->log, LEVEL_ERROR, FILE_ONLY, "Can't add new subscriber, maximum limit reached (%d/%d)", topics->max_subscriber, topics->max_subscriber);
        pthread_mutex_unlock(&topics->topics[topic_id]->mutex);
        return EXIT_FAILURE;
    }
    t_subscriber *subscriber = malloc(sizeof(t_subscriber));
    if (subscriber == NULL)
        handle_error("malloc");

    char listener_name[100];
    subscriber->nb_message = 0;
    subscriber->start = 0;
    subscriber->end = 0;
    subscriber->callback = callback;
    subscriber->node = node;
    subscriber->topic_id = topic_id;
    subscriber->name = strdup(node->name);
    subscriber->topics = topics;
    pthread_mutex_init(&subscriber->mutex, NULL);
    pthread_mutex_lock(&subscriber->mutex); // pour bloquer la lecture ou l'ecriture de message tant que la souscription n'est pas terminée
    int s;
    pthread_attr_t attr;
    subscriber->t_info = malloc(sizeof(struct thread_info));
    if (subscriber->t_info == NULL)
        handle_error("malloc");
    subscriber->message=calloc(topics->max_topic_message,sizeof(int));
    write_log(node->log, LEVEL_INFO, FILE_ONLY, "Start topic manager for subscriber %s on topic %s", subscriber->node->name, topics->topics[topic_id]->name);
    s = pthread_attr_init(&attr);
    if (s != 0)
        handle_error("(%d) pthread_attr_init", s);
    subscriber->t_info->args = subscriber;
    subscriber->t_info->end_thread = 0;

    sprintf(listener_name, "%s_%s", subscriber->node->name, topics->topics[topic_id]->name);
    subscriber->t_info->log = create_log(listener_name);
    topics->topics[topic_id]->subscriber[topics->topics[topic_id]->nb_subscriber] = subscriber;
    s = pthread_create(&subscriber->t_info->thread_id, &attr,
                       &topic_manager, subscriber);
    if (s != 0)
        handle_error("pthread_create");

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        handle_error("pthread_attr_destroy");

    topics->topics[topic_id]->nb_subscriber++;
    pthread_mutex_unlock(&subscriber->mutex);
    pthread_mutex_unlock(&topics->topics[topic_id]->mutex);
    return EXIT_SUCCESS;
}

void *topic_manager(void *arg)
{
    t_subscriber *subscriber = arg;
    struct return_value *return_value;
    int message_id;
    write_log(subscriber->t_info->log, LEVEL_INFO, FILE_ONLY, "Topic manager started");
    do
    {
        short_wait();
        if (subscriber->nb_message > 0)
        {
            pthread_mutex_lock(&subscriber->mutex);              // bloque la lecture du message si un message est en cours d'écriture par un noeud pour ce subscriber
            message_id = subscriber->message[subscriber->start]; // get the first massage in the topic buffer
            write_log(subscriber->t_info->log, LEVEL_INFO, FILE_ONLY, "New incoming message : %lu",
                      subscriber->topics->message_collector[message_id].message_uniqid);
            void *(*fun_ptr)(void *) = subscriber->callback;                       // create the pointer to the call back funtion
            t_callback_param param;                                                // callback function parameter
            param.log = subscriber->t_info->log;                                   // log for the callback function
            param.message = subscriber->topics->message_collector[message_id].adr; // get the pointer on the message store in the vitual memory (home made garbage collector)
            pthread_mutex_unlock(&subscriber->mutex);

            (*fun_ptr)(&param); // callback function

            pthread_mutex_lock(&subscriber->mutex);
            pthread_mutex_lock(&subscriber->topics->memory_mutex);         // bloque l'accés à la mémoire
            subscriber->topics->message_collector[message_id].nb_reader++; // to say that we have read the message
            subscriber->nb_message--;
            if (subscriber->nb_message > 0) // if there is more message, move the read head (virtual memory is a circular array)
                subscriber->start = (subscriber->start < subscriber->topics->max_buffer_message) ? subscriber->start + 1 : 0;
            pthread_mutex_unlock(&subscriber->topics->memory_mutex);
            pthread_mutex_unlock(&subscriber->mutex);
        }
    } while (!subscriber->t_info->end_thread);
    write_log(subscriber->t_info->log, LEVEL_INFO, FILE_ONLY, "Topic manager stoped");
    return_value = malloc(sizeof(struct return_value));
    return_value->message = strdup("ok");
    return return_value;
}

int delete_all_topic(t_topics *topics, t_log *main_log_file)
{
    int i, j, s;
    void *ret_val;

    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Close all topics (%d)", topics->nb_topic);
    for (i = 0; i < topics->nb_topic; i++)
    {
        write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Stop %s listeners", topics->topics[i]->name);
        for (j = 0; j < topics->topics[i]->nb_subscriber; j++)
        {
            topics->topics[i]->subscriber[j]->t_info->end_thread = 1;
            s = pthread_join(topics->topics[i]->subscriber[j]->t_info->thread_id, &ret_val);
            if (s != 0)
                handle_error("pthread_join");
            write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Close listener %s; returned value was %s",
                      topics->topics[i]->subscriber[j]->name, ((struct return_value *)ret_val)->message);
            free(((struct return_value *)ret_val)->message);
            free(ret_val);
            close_log(topics->topics[i]->subscriber[j]->t_info->log);
            free(topics->topics[i]->subscriber[j]->t_info);
            free(topics->topics[i]->subscriber[j]);
        }
        free(topics->topics[i]->subscriber);
    }
    topics->th_memory->end_thread = 1;
    s = pthread_join(topics->th_memory->thread_id, &ret_val);
    if (s != 0)
        handle_error("pthread_join");
    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Virtual memory manager stopped; returned value was %s",
              ((struct return_value *)ret_val)->message);
    free(((struct return_value *)ret_val)->message);
    free(ret_val);
    close_log(topics->th_memory->log);
    free(topics->th_memory);
    free(topics->message_collector);
    free(topics->topics);
    free(topics);
    return EXIT_SUCCESS;
}
