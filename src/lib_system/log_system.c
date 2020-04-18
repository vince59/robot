/*
** SIMBOT PROJECT, 2020
** utils.c
** File description: miscellaneous useful functions
** 
*/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "log_system.h"

#define HEADER_SIZE 58

struct timestamp getTimestamp()
{
    char timebuffer[32] = {0};
    struct tm *tmval = NULL;
    struct tm gmtval = {0};

    struct timespec curtime = {0};
    struct timestamp ts;

    // Get current time
    clock_gettime(CLOCK_REALTIME, &curtime);
    // Set the fields
    ts.seconds = curtime.tv_sec;
    ts.milliseconds = round(curtime.tv_nsec / 1.0e6);

    if ((tmval = gmtime_r(&ts.seconds, &gmtval)) != NULL)
    {
        // Build the first part of the time
        strftime(timebuffer, sizeof timebuffer, "%Y-%m-%d %H:%M:%S", &gmtval);

        // Add the milliseconds part and build the time string
        snprintf(ts.timestring, sizeof ts.timestring, "%s.%09li", timebuffer, curtime.tv_nsec);
    }
    return ts;
}


void handle_error(const char *fmt, ...)
{
    int size = 0;
    char *p = NULL;
    va_list ap;

    /* Determine required size */

    va_start(ap, fmt);
    size = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (size < 0)
        exit(EXIT_FAILURE);

    size++; /* For '\0' */
    p = malloc(size);
    if (p == NULL)
        exit(EXIT_FAILURE);

    va_start(ap, fmt);
    size = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (size < 0)
        free(p);

    perror(p);
    exit(EXIT_FAILURE);
}

t_log *create_log(char *name)
{
    t_log *log_file = malloc(sizeof(t_log));
    if (log_file == NULL)
        handle_error("malloc");
    log_file->name = calloc(strlen(name) + 1, sizeof(char));
    if (log_file->name == NULL)
        handle_error("calloc");
    strcpy(log_file->name,name);
    char *dir = "./log/";
    char *ext = ".log";
    char *path = calloc(strlen(dir) + strlen(name) + strlen(ext) + 1, sizeof(char));
    if (log_file->name == NULL)
        handle_error("calloc");
    strcpy(path, dir);
    strcat(path, name);
    strcat(path, ext);
    log_file->p_file = fopen(path, "w");
    if (log_file->name == NULL)
        handle_error("Cannot open file %s", path);
    free(path);
    return log_file;
}

int write_log(t_log *log_file, int level, int screen, char *fmt, ...)
{
    int l;
    char spaces[HEADER_SIZE];

    if (level > LOG_LEVEL)
        return EXIT_SUCCESS;
    char header[300], txt_level[5];
    struct timestamp ts;

    switch (level)
    {
    case LEVEL_ERROR:
        strcpy(txt_level, "ERRO");
        break;
    case LEVEL_WARNING:
        strcpy(txt_level, "WARN");
        break;
    case LEVEL_INFO:
        strcpy(txt_level, "INFO");
        break;
    }
    ts = getTimestamp();

    l=30-strlen(ts.timestring);
    if (l>0)
    {
        sprintf(spaces,"%*s",l," ");
        sprintf(ts.timestring, "%s%s", ts.timestring, spaces);
    }
    sprintf(header, "%s %s (%s)", ts.timestring, txt_level, log_file->name);
    l=HEADER_SIZE-strlen(header);
    if (l>0)
    {
        sprintf(spaces,"%*s",l," ");
        sprintf(header, "%s%s -> ", header,spaces);
    }
    else
    {
        sprintf(header, "%s -> ", header);
    }
    

    va_list args;
    if (log_file->p_file != NULL)
    {
        fprintf(log_file->p_file, "%s", header);
        va_start(args, fmt);
        vfprintf(log_file->p_file, fmt, args);
        fprintf(log_file->p_file, "\n");
        fflush(log_file->p_file);
    }
    if (screen || (log_file->p_file == NULL))
    {
        printf("%s", header);
        va_start(args, fmt);
        vprintf(fmt, args);
        printf("\n");
    }
    va_end(args);
    return EXIT_SUCCESS;
}

int close_log(t_log *log_file)
{
    fclose(log_file->p_file);
    free(log_file->name);
    return EXIT_SUCCESS;
}
