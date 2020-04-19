/*
** SIMBOT PROJECT, 2020
** sample_node.h
** File description: simulation node
** 
*/

#ifndef simulation_H
#define simulation_H

#include "log_system.h"
#include "coordinates_message.h"

// node parameter
struct s_simulation_param
{
    int destination_topic;
    int position_topic;
};

typedef struct s_simulation_param t_simulation;
 
extern void *simulation_function(void *arg); // main function of the node
t_simulation *new_simulation_param(int destination_topic, int position_topic);
void *destination_callback(void *arg);

#endif