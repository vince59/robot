/*
** SIMBOT PROJECT, 2020
** node.h
** File description: node management 
** 
*/

#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "log_system.h"
#include "topic.h"

struct node_info
{
    pthread_t thread_id; // ID returned by pthread_create()
    int num;             // node num
    char *name;          // node name
    int end;             // 1 = force thread to stop
    void *args;          // argument of the node function
    t_log *log;          // log file to debug
    struct s_topics *topics;
    struct s_nodes *nodes;
};

struct return_value
{
    char *message;
};

struct s_nodes
{
    int nb_node;
    struct node_info *node_list;
    int max_node;
};

typedef struct s_nodes t_nodes;

t_nodes *init_node(int max_node, t_log *main_log_file);
int start_node(t_nodes *nodes, struct s_topics *topics, char *node_name, void *(*start_routine)(void *), void *args, t_log *main_log_file);
int close_node(t_nodes *nodes, t_log *main_log_file);
void main_node();
void short_wait();

#endif