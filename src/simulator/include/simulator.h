/*
** SIMBOT PROJECT, 2020
** simulator.h
** File description: robot simulator
** 
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "mapper.h"

int init_simulator();
int close_simulator();
void draw_map(t_cell_struct **matrix,int size);
int get_robot_avatar(int orientation);
#endif



