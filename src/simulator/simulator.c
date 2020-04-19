/*
** SIMBOT PROJECT, 2020
** simulator.c
** File description: robot simulator
** 
*/

#include <stdlib.h>
#include <stdio.h>
#include "ansi_escape.h"
#include "simulator.h"
#include "mapper.h"

#define MATRIX_SIZE 30
#define VERSION "1.0"

t_cell_struct **simMap; // Simulator matrix to store the map
t_pos_struct simPos;	// Robot position inside the simulator map

int draw_robot_avatar(int orientation)
{
	switch (orientation)
	{
	case N:
		putchar('^'); // Nord
		break;
	case NE:
		printf("\u2510"); // Nord Est
		break;
	case E:
		putchar('>'); // Est
		break;
	case SE:
		 printf("\u2518"); // SudEst
		break;
	case S:
		putchar('V'); // Sud
		break;
	case SO:
	printf("\u2514");
		// Sud Ouest
		break;
	case O:
		putchar('<'); // Ouest
		break;
	case NO:
		printf("\u250C"); // Nord Ouest
		break;
	}
	return EXIT_FAILURE;
}

int init_simulator()
{
	setupConsole();
	clearScreen();
	moveTo(1, 1);
	setTextColorBright(1);
	setTextColor(YELLOW_TXT);
	puts("     _______. __  .___  ___.  __    __   __        ");
	puts("    /       ||  | |   \\/   | |  |  |  | |  |      ");
	puts("   |   (----`|  | |  \\  /  | |  |  |  | |  |      ");
	puts("    \\   \\    |  | |  |\\/|  | |  |  |  | |  |    ");
	puts(".----)   |   |  | |  |  |  | |  `--'  | |  `----./ ");
	puts("|_______/    |__| |__|  |__|  \\______/  |_______/ \n\n");

	simMap = gen_map(MATRIX_SIZE);
	init_map(simMap, MATRIX_SIZE);
	simPos.x = 2;
	simPos.y = 3;
	simPos.orientation = NO;

	simMap[simPos.x][simPos.y].obstacle = ROBOT;
	simMap[7][7].obstacle = OBSTACLE;
	draw_map(simMap, MATRIX_SIZE);
	return EXIT_SUCCESS;
}

void move(int orientation)
{
	int x, y;

	x = simPos.x;
	y = simPos.y;
	switch (orientation)
	{
	case N:
		y--;
		break; // Nord
	case NE:
		x--;
		y--;
		break; // Nord Est
	case E:
		x++;
		break; // Est
	case SE:
		x++;
		y++;
		break; // SudEst
	case S:
		y++;
		break; // Sud
	case SO:
		x--;
		y++;
		break; // Sud Ouest
	case O:
		x--;
		break; // Ouest
	case NO:
		x--;
		y--;
		break; // Nord Ouest
	}

	if (x >= 0 && y >= 0 && x < MATRIX_SIZE && y < MATRIX_SIZE && simMap[x][y].obstacle == FREE)
	{
		simMap[simPos.x][simPos.y].obstacle = FREE;
		simPos.x = x;
		simPos.y = y;
		simMap[simPos.x][simPos.y].obstacle = ROBOT;
	}

	draw_map(simMap, MATRIX_SIZE);
	printf("Move simu");
}

// display map to debug purpose
void draw_map(t_cell_struct **matrix, int size)
{
	int l, c;
	int row, col;
	getCursorPosition(&row, &col);
	setTextColorBright(0);
	setTextColor(YELLOW_TXT);
	setBackgroundColor(BLUE_BKG);
	col = 10;
	moveTo(row, col);
	for (l = 0; l < size; l++)
	{
		for (c = 0; c < size; c++)
		{
			switch (matrix[c][l].obstacle)
			{
			case FREE:
				setBackgroundColor(BLUE_BKG);
				setTextColor(BLUE_TXT);
				puts(" ");
				break;
			case OBSTACLE:
				setBackgroundColor(BLACK_BKG);
				setTextColor(BLACK_TXT);
				puts(" ");
				break;
			case ROBOT:
				setTextColor(YELLOW_TXT);
				setBackgroundColor(BLUE_BKG);
				draw_robot_avatar(simPos.orientation);
				break;
			}
			moveTo(row, ++col);
		}
		row++;
		col = 10;
		moveTo(row, col);
	}
	moveTo(row + 2, 1);
}

int close_simulator()
{
	restoreConsole();
	return EXIT_SUCCESS;
}

void detect()
{
}