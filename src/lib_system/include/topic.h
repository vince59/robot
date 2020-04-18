/*
** SIMBOT PROJECT, 2020
** topic.h
** File description: topic management
** 
*/

#ifndef TOPIC_H
#define TOPIC_H

#include <pthread.h>
#include "log_system.h"
#include "node.h"

struct thread_info
{
	pthread_t thread_id; // ID returned by pthread_create()
	int end_thread;		 // 1 = force thread to stop
	void *args;			 // thread arguments
	t_log *log;			 // log file to debug
};

struct s_topic_struct
{
	char *name;
	struct s_subscriber **subscriber;
	int nb_subscriber;
	pthread_mutex_t mutex;
};

typedef struct s_topic_struct t_topic; //

struct message_reference
{
	void *adr;		   // message adresse
	int nb_reader;	   // nb subscriber who have read the message
	int nb_subscriber; // Number of subscriber who have to read the message
	unsigned long int message_uniqid;
};

struct s_callback_param
{
	t_log *log;
	void *message;
};

typedef struct s_callback_param t_callback_param;

struct s_topics
{
	struct message_reference *message_collector;
	t_topic **topics;
	int nb_topic;
	pthread_mutex_t memory_mutex;
	struct thread_info *th_memory;
	unsigned long message_counter;
	int max_topic;
	int max_subscriber;
	int max_topic_message; 
	int max_buffer_message;
};

typedef struct s_topics t_topics;

struct s_subscriber
{
	int *message; // message queue for a subscriber
	int nb_message;					// number of message in a queue
	int start;						// begin of the queue
	int end;						// end of the queue
	void *(*callback)(void *);
	struct thread_info *t_info;
	struct node_info *node;
	int topic_id;
	char *name;
	pthread_mutex_t mutex;
	t_topics *topics;
};

typedef struct s_subscriber t_subscriber;

unsigned long uniqid(unsigned long u_id);
t_topics *init_topic(int max_topic, int max_subscriber, int max_topic_message, int max_buffer_message, t_log *main_log_file);
int add_topic(t_topics *topics, const char *name, t_log *main_log_file);
int delete_all_topic(t_topics *topics, t_log *main_log_file);
int add_message(t_topics *topics, void *message, int nb_message, t_log *log_file);
int write_topic(t_topics *topics, int topic_id, void *message, t_log *log_file);
int subscribe_topic(t_topics *topics, int topic_id, struct node_info *node, void *(*callback)(void *));
void *topic_manager(void *arg);
void *virtual_memory_manager(void *arg);
void free_virtual_memory();
#endif