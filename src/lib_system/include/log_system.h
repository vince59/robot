/*
** SIMBOT PROJECT, 2020
** log_system.h
** File description: miscellaneous useful log functions
** 
*/

#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#include <stdio.h>
//log level
#define LEVEL_ERROR   1
#define LEVEL_WARNING 2
#define LEVEL_INFO    3
//print log on screen
#define FILE_ONLY 0
#define ON_SCREEN 1

struct timestamp
{
    time_t seconds;
    long milliseconds;
    char timestring[46];
};

struct s_log {
    FILE *p_file;
    char *name;
};

typedef struct s_log t_log;

void handle_error(const char *fmt, ...);
t_log *create_log(char *name);
int write_log(t_log *log_file, int level, int screen, char *fmt, ...);
int close_log(t_log *log_file);
struct timestamp getTimestamp();
#endif