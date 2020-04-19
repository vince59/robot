/*
** SIMBOT PROJECT, 2020
** sample_node.h
** File description: navigation node
** 
*/

#ifndef COORDINATES_MESSAGE_H
#define COORDINATES_MESSAGE_H
#include "log_system.h"

struct s_coordinates_message
{
    int x,y; 
};

typedef struct s_coordinates_message t_coordinates_message;

t_coordinates_message *new_coordinates_message(int x, int y);
int log_coordinates_message(t_coordinates_message *destination, t_log *log_file);

#endif