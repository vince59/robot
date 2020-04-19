/*
** SIMBOT PROJECT, 2020
** main.c
** File description: main file
** 
*/

#include <unistd.h>
#include <string.h>
#include "node.h"
#include "log_system.h"
#include "simulation_node.h"
#include "coordinates_message.h"
#include "topic.h"

#define MAX_NODE 10
#define MAX_SUBSCRIBER 10       // max number of subscriber / topic
#define MAX_TOPIC 3             // max number of topics
#define MAX_BUFFER_MESSAGE 2000 // max total unread message
#define MAX_TOPIC_MESSAGE 1000  // max unread message for a topic

int main ( void ) {
    t_log *main_log_file = create_log("main");
    write_log(main_log_file, LEVEL_INFO, FILE_ONLY, "Start main node");
    t_topics *topics = init_topic(MAX_TOPIC, MAX_SUBSCRIBER, MAX_TOPIC_MESSAGE, MAX_BUFFER_MESSAGE, main_log_file);
    t_nodes *nodes = init_node(MAX_NODE, main_log_file);
    int destination_topic = add_topic(topics, "destination", main_log_file);
    int position_topic = add_topic(topics, "position", main_log_file);
    t_simulation *param = new_simulation_param(destination_topic,position_topic);
    start_node(nodes, topics, "simulation", &simulation_function, param, main_log_file);
    t_coordinates_message *destination_message = new_coordinates_message(10, 13);
    write_log(main_log_file, LEVEL_INFO, FILE_ONLY, "Send message :");
    log_coordinates_message(destination_message, main_log_file);
    write_topic(topics, destination_topic, destination_message, main_log_file);
    sleep(2);
    close_node(nodes, main_log_file);
    delete_all_topic(topics, main_log_file);
    write_log(main_log_file, LEVEL_INFO, FILE_ONLY, "End of main node");
    close_log(main_log_file);
	return 1;
}
