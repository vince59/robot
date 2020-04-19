/*
** SIMBOT PROJECT, 2020
** sample_node.c
** File description: simulation node
** 
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "log_system.h"
#include "node.h"
#include "topic.h"
#include "simulation_node.h"
#include "simulator.h"

t_simulation *new_simulation_param(int destination_topic, int position_topic)
{
    t_simulation *nav = malloc(sizeof(t_simulation));
    if (nav == NULL)
        handle_error("malloc");
    nav->destination_topic = destination_topic;
    nav->position_topic = position_topic;
    return nav;
}

void *destination_callback(void *arg)
{
    t_callback_param *param = arg;
    t_coordinates_message *message = param->message;
    write_log(param->log, LEVEL_INFO, FILE_ONLY, "New destination to go x/y [%d,%d]", message->x, message->y);
    return NULL;
}

// thread of the node
void *simulation_function(void *arg)
{
    init_simulator();
    int x=0,y=0;
    struct node_info *node = arg;
    t_simulation *param = node->args;
    struct return_value *return_value;
    write_log(node->log, LEVEL_INFO, FILE_ONLY, "Starting simulation");
    subscribe_topic(node->topics, param->destination_topic, node, &destination_callback);
    node->started=1; //node is ready
    do
    {
        x++; y++;
        write_topic(node->topics, param->position_topic, new_coordinates_message(x, y), node->log);
        short_wait();
    } while (!node->end);
    write_log(node->log, LEVEL_INFO, FILE_ONLY, "End of simulation");
    return_value = malloc(sizeof(struct return_value));
    return_value->message = strdup("ok");
    close_simulator();
    return return_value;
}
