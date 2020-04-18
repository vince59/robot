/*
** SIMBOT PROJECT, 2020
** simulator.c
** File description: robot simulator
** 
*/

#include <stdlib.h>
#include <stdio.h>
#include "mapper.h"

#define MATRIX_SIZE 10
#define VERSION     "1.0"

t_cell_struct **simMap; // Simulator matrix to store the map
t_pos_struct simPos;    // Robot position inside the simulator map

void init_simulator(){
	printf("Simulator Enabled\n");
	printf("Version :%s\n",VERSION);
	simMap = gen_map(MATRIX_SIZE);
	init_map(simMap,MATRIX_SIZE);
	simPos.x=2;
	simPos.y=3;
	simPos.orientation=N;
	
	simMap[simPos.x][simPos.y].obstacle=ROBOT;
	display_map(simMap,MATRIX_SIZE);
}

void move(int orientation) {
	int x,y;
	
	x=simPos.x;
	y=simPos.y;
	switch (orientation) {
		case N  :y--;       break;  // Nord
		case NE :x--; y--;  break;  // Nord Est
		case E  :x++;       break;  // Est
		case SE :x++; y++;  break;  // SudEst
		case S  :y++;       break;  // Sud
		case SO :x--; y++;  break;  // Sud Ouest
		case O  :x--;       break;  // Ouest
		case NO :x--; y--;  break;  // Nord Ouest
	}
	
	if (x>=0 && y>=0 && x<MATRIX_SIZE && y<MATRIX_SIZE && simMap[x][y].obstacle==FREE) {
		simMap[simPos.x][simPos.y].obstacle=FREE;
		simPos.x=x;
		simPos.y=y;
		simMap[simPos.x][simPos.y].obstacle=ROBOT;
	}
	
	display_map(simMap,MATRIX_SIZE);
	printf("Move simu");
}

void detect(){
	
}