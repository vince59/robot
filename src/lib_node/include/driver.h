/*
** SIMBOT PROJECT, 2020
** driver.h
** File description: robot driver function 
** 
*/

#ifndef DRIVER_H
#define DRIVER_H

struct detector_struct {
	int left,right,front,back;
};

typedef struct detector_struct t_detector_struct; // detector type

void move(int orientation);
void detect(t_detector_struct* detector);

#endif