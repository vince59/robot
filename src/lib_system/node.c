/*
** SIMBOT PROJECT, 2020
** node.c
** File description: node management
** 
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "node.h"
#include "log_system.h"

#define MILI_SEC 1000000L         // 1 miliseconde
#define NODE_WAIT MILI_SEC * 100L // 100 miliseconds (0.1 second)

void short_wait()
{
    struct timespec sleeptime = {0, (long)NODE_WAIT};
    nanosleep(&sleeptime, NULL);
}

t_nodes *init_node(int max_node, t_log *main_log_file)
{
    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Init nodes, max nodes : %d", max_node);
    t_nodes *nodes = malloc(sizeof(t_nodes));
    if (nodes == NULL)
        handle_error("malloc");
    nodes->node_list = calloc(max_node, sizeof(struct node_info));
    if (nodes->node_list == NULL)
        handle_error("calloc");
    nodes->nb_node = 0;
    nodes->max_node = max_node;
    return nodes;
}

int start_node(t_nodes *nodes, t_topics *topics, char *node_name, void *(*start_routine)(void *), void *args, t_log *main_log_file)
{
    int s, i;
    pthread_attr_t attr;

    s = pthread_attr_init(&attr);
    if (s != 0)
        handle_error("(%d) pthread_attr_init", s);

    nodes->nb_node++;
    if (nodes->nb_node > nodes->max_node)
        handle_error("max thread limit reached");
    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Create a new node (%s) #%d", node_name, nodes->nb_node);
    i = nodes->nb_node - 1;
    nodes->node_list[i].num = i;
    nodes->node_list[i].name = strdup(node_name);
    nodes->node_list[i].args = args;
    nodes->node_list[i].end = 0;
    nodes->node_list[i].log = create_log(node_name);
    nodes->node_list[i].nodes = nodes;
    nodes->node_list[i].topics = topics;
    s = pthread_create(&nodes->node_list[i].thread_id, &attr,
                       start_routine, &nodes->node_list[i]);
    if (s != 0)
        handle_error("pthread_create");

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        handle_error("pthread_attr_destroy");
    return (EXIT_SUCCESS);
}

int close_node(t_nodes *nodes, t_log *main_log_file)
{
    int i, s;
    void *ret_val;

    write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Close all nodes");
    for (i = 0; i < nodes->nb_node; i++)
    {
        nodes->node_list[i].end = 1;
        s = pthread_join(nodes->node_list[i].thread_id, &ret_val);
        if (s != 0)
            handle_error("pthread_join");
        write_log(main_log_file, LEVEL_INFO, ON_SCREEN, "Close node %d (%s); returned value was %s",
                  nodes->node_list[i].num, nodes->node_list[i].name, ((struct return_value *)ret_val)->message);
        free(((struct return_value *)ret_val)->message);
        free(ret_val);
        free(nodes->node_list[i].name);
        close_log(nodes->node_list[i].log);
    }
    free(nodes);
    return (EXIT_SUCCESS);
}