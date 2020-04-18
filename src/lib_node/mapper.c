/*
** SIMBOT PROJECT, 2020
** mapper.c
** File description: mapper to create the maps
** 
*/

#include <stdlib.h>
#include <stdio.h>
#include "mapper.h"

// allocate memory to a new map
t_cell_struct **gen_map(int size) {
	t_cell_struct **matrix;
    int l;

    matrix = malloc(sizeof (t_cell_struct *)*size);
    for (l=0; l<size; l++) 
		matrix[l]=malloc(sizeof (t_cell_struct)*size);
	
	return matrix;
}

// display map to debug purpose
void display_map(t_cell_struct **matrix,int size) {
    int l,c;
    
    for (l=0; l<size; l++) {
        for (c=0; c<size; c++)
            printf("%d ",matrix[c][l].obstacle);
		printf("\n");
    }
    printf("\n\n");
}

// init a exiting map
void init_map(t_cell_struct **matrix,int size) {
    int l,c;
    
    for (l=0; l<size; l++) 
        for (c=0; c<size; c++) 
            matrix[c][l].obstacle=FREE;
}