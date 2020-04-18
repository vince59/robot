/*
** SIMBOT PROJECT, 2020
** sample_node.h
** File description: navigation node
** 
*/

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "log_system.h"

// node parameter
struct s_navigation_param
{
    int topic_destination;
};

typedef struct s_navigation_param t_navigation;

// topic message
struct s_destination_message
{
    int x,y; // destination parameter
};

typedef struct s_destination_message t_destination_message;
 
extern void *navigation_function(void *arg); // main function of the node

t_navigation *new_navigation_param(int topic_id);
t_destination_message *new_destination_message(int x, int y);
int log_destination_message(t_destination_message *destination, t_log *log_file);
void *navigation_callback(void *arg);

#endif