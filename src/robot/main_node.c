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
#include "navigation_node.h"
#include "topic.h"

#define MAX_NODE 10
#define MAX_SUBSCRIBER 10       // max number of subscriber / topic
#define MAX_TOPIC 3             // max number of topics
#define MAX_BUFFER_MESSAGE 2000 // max total unread message
#define MAX_TOPIC_MESSAGE 1000  // max unread message for a topic

void main_node()
{
    int target;
    t_log *main_log_file = create_log("main");
    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Start main node");
    t_topics *topics = init_topic(MAX_TOPIC, MAX_SUBSCRIBER, MAX_TOPIC_MESSAGE, MAX_BUFFER_MESSAGE, main_log_file);
    t_nodes *nodes = init_node(MAX_NODE, main_log_file);
    target = add_topic(topics, "target", main_log_file);
    t_navigation *param = new_navigation_param(target);
    start_node(nodes, topics, "navigation", &navigation_function, param, main_log_file);
    t_destination_message *destination = new_destination_message(10, 13);
    log_destination_message(destination, main_log_file);
    write_topic(topics, target, destination, main_log_file);

    sleep(2);
    close_node(nodes, main_log_file);
    delete_all_topic(topics, main_log_file);
    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "End of main node");
    close_log(main_log_file);
}
