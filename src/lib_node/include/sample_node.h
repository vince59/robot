/*
** SIMBOT PROJECT, 2020
** sample_node.h
** File description: sample to create a new node
** 
*/

#ifndef SAMPLE_H
#define SAMPLE_H

// node parameter
struct sample_param
{
    int id;
    char *name;
};

// topic message
struct s_sample_message
{
    unsigned long id;
    char txt_message[100]; //do not add pointer in the structure (memory leak)
};

typedef struct s_sample_message t_sample_message;

extern int sample_topic[]; // topic id if node writes on it (not mandatory) 
extern void *sample_function(void *arg); // main function of the node (thread call back function)
#endif