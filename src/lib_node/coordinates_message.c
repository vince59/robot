/*
** SIMBOT PROJECT, 2020
** sample_node.c
** File description: navigation node
** 
*/

#include <stdlib.h>
#include "log_system.h"
#include "coordinates_message.h"

t_coordinates_message *new_coordinates_message(int x, int y)
{
    t_coordinates_message *new_destination = malloc(sizeof(t_coordinates_message));
    if (new_destination == NULL)
        handle_error("malloc");
    new_destination->x = x;
    new_destination->y = y;
    return new_destination;
}

int log_coordinates_message(t_coordinates_message *destination, t_log *log_file)
{
    write_log(log_file, LEVEL_INFO, FILE_ONLY, "   [x=%d,y=%d]", destination->x, destination->y);
    return EXIT_SUCCESS;
}