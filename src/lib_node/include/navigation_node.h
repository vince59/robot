/*
** SIMBOT PROJECT, 2020
** sample_node.h
** File description: navigation node
** 
*/

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "log_system.h"
#include "coordinates_message.h"

// node parameter
struct s_navigation_param
{
    int topic_destination;
};

typedef struct s_navigation_param t_navigation;
 
extern void *navigation_function(void *arg); // main function of the node
t_navigation *new_navigation_param(int topic_id);
void *navigation_callback(void *arg);

#endif