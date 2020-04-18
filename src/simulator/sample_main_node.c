/*
** SIMBOT PROJECT, 2020
** main_node.c
** File description: main node
** 
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "node.h"
#include "log_system.h"
#include "sample_node.h"
#include "topic.h"

#define MAX_NODE 10
#define MAX_SUBSCRIBER 10       // max number of subscriber / topic
#define MAX_TOPIC 3             // max number of topics
#define MAX_BUFFER_MESSAGE 2000 // max total unread message
#define MAX_TOPIC_MESSAGE 1000  // max unread message for a topic

void sample_main_node()
{
    int i;
    struct sample_param sample_node[MAX_NODE];

    t_log *main_log_file = create_log("main");
    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Start main node");
    t_topics *topics = init_topic(MAX_TOPIC, MAX_SUBSCRIBER, MAX_TOPIC_MESSAGE, MAX_BUFFER_MESSAGE, main_log_file);
    printf("%d\n",topics->max_topic);
    t_nodes *nodes = init_node(MAX_NODE, main_log_file);
    for (i = 0; i < MAX_TOPIC; i++)
    {
        char name[20];
        sprintf(name, "topic_%02d", i);
        sample_topic[i] = add_topic(topics, name, main_log_file);
    }
    for (i = 0; i < MAX_NODE; i++)
    {
        char name[20];
        sprintf(name, "node_%02d", i);
        sample_node[i].id = i;
        sample_node[i].name = strdup(name);
        start_node(nodes, topics, name, &sample_function, &sample_node[i], main_log_file);
    }
    sleep(5);
    close_node(nodes, main_log_file);
    delete_all_topic(topics, main_log_file);
    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "End of main node");
    close_log(main_log_file);
}
