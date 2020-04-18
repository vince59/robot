/*
** SIMBOT PROJECT, 2020
** sample_node.c
** File description: sample to create a new node
** 
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "log_system.h"
#include "node.h"
#include "topic.h"
#include "sample_node.h"

int sample_topic[10];

// create the message
t_sample_message *new_sample_message(t_sample_message *mess)
{
    t_sample_message *new_mess = malloc(sizeof(t_sample_message));
    if (new_mess == NULL)
        handle_error("malloc");
    *new_mess = *mess;
    return new_mess;
}

void *sample_event(void *arg)
{
    t_callback_param *param=arg;
    t_sample_message *message=param->message;
    write_log(param->log, LEVEL_INFO, FILE_ONLY, "Message content : %s", message->txt_message);
    return NULL;
}

// thread of the node
void *sample_function(void *arg)
{
    struct node_info *node = arg;
    struct return_value *return_value;
    t_sample_message message, *new_mess;

    unsigned long nb_message = 0;
    int i;

    write_log(node->log, LEVEL_INFO, FILE_ONLY, "Hello, my name is %s and I am started", node->name);
    for (i = 0; i < node->topics->max_topic; i++) // loop only for the example, can be remove if you subscribe only on one topic
        subscribe_topic(node->topics, sample_topic[i], node, &sample_event);
    node->started=1; //node is ready
    do
    {
        short_wait();
        for (i = 0; i < node->topics->max_topic; i++) // loop only for the example, can be remove if you write only on one topic
        {
            // create un new message with some datas
            nb_message++;
            message.id = uniqid(nb_message);
            sprintf(message.txt_message, "Message %lu from %s, id :%lu", nb_message, node->name, message.id);
            new_mess = new_sample_message(&message);
            // send message on topic
            write_log(node->log, LEVEL_INFO, FILE_ONLY, "Send '%s' on topic %s", new_mess->txt_message, node->topics->topics[i]->name);
            write_topic(node->topics, sample_topic[i], new_mess, node->log);
        }
    } while (!node->end);
    // give a return value
    write_log(node->log, LEVEL_INFO, FILE_ONLY, "Bye, I finished my job");
    return_value = malloc(sizeof(struct return_value));
    return_value->message = strdup("ok");
    return return_value;
}
