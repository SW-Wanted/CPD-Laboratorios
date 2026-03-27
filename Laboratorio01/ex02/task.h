/*
 * task.h - definitions and declarations of the integer task 
 */

#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 64
#define MAX_PRIORITY 6


/* t_task - represents a single task */
typedef struct s_task{
    char id[MAX_ID];
    struct s_task *next;
} t_task;



/* tsk_new - create a new task */
void tsk_new(t_task **lists, int priority, char *id);

/* tsk_list - list tasks with a specific priority */
void tsk_list(t_task **lists, int priority);

/* tsk_complete - mark a task as complete */
void tsk_complete(t_task **lists, char *id);

#endif