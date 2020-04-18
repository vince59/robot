/*
** SIMBOT PROJECT, 2020
** sample_node.c
** File description: navigation node
** 
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "log_system.h"
#include "node.h"
#include "topic.h"
#include "navigation_node.h"

t_navigation *new_navigation_param(int topic_id)
{
    t_navigation *nav = malloc(sizeof(t_navigation));
    if (nav == NULL)
        handle_error("malloc");
    nav->topic_destination = topic_id;
    return nav;
}

t_destination_message *new_destination_message(int x, int y)
{
    t_destination_message *new_destination = malloc(sizeof(t_destination_message));
    if (new_destination == NULL)
        handle_error("malloc");
    new_destination->x = x;
    new_destination->y = y;
    return new_destination;
}

int log_destination_message(t_destination_message *destination, t_log *log_file)
{
    write_log(log_file, LEVEL_INFO, ON_SCREEN, "Send a new destination : x/y [%d,%d]", destination->x, destination->y);
    return EXIT_SUCCESS;
}

void *navigation_callback(void *arg)
{
    t_callback_param *param = arg;
    t_destination_message *message = param->message;
    write_log(param->log, LEVEL_INFO, FILE_ONLY, "New destination : x=%d, y=%d", message->x, message->y);
    return NULL;
}

// thread of the node
void *navigation_function(void *arg)
{
    struct node_info *node = arg;
    t_navigation *param = node->args;
    struct return_value *return_value;
    write_log(node->log, LEVEL_INFO, FILE_ONLY, "Starting navigation");
    subscribe_topic(node->topics, param->topic_destination, node, &navigation_callback);

    do
    {
        short_wait();
    } while (!node->end);
    write_log(node->log, LEVEL_INFO, FILE_ONLY, "En of navigation");
    return_value = malloc(sizeof(struct return_value));
    return_value->message = strdup("ok");
    return return_value;
}
