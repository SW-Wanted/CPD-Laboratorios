/*
 * task.c - implementation of the integer task functions
 */

#include "task.h"

void tsk_new(t_task **lists, int priority, char *id) {
    t_task *t;
    
    t = malloc(sizeof(t_task));
    strncpy(t->id, id, MAX_ID);
    t->next = lists[priority];
    lists[priority] = t;
}

void tsk_list(t_task **lists, int priority) {
    int p;
    t_task *t;

    p = MAX_PRIORITY - 1;
    while (p >= priority) {
        t = lists[p];
        while (t != NULL) {
            printf("%d %s\n", p, t->id);
            t = t->next;
        }
        p--;
    }
}

void tsk_complete(t_task **lists, char *id) {
    int p;
    t_task **t;

    p = 0;
    while (p < MAX_PRIORITY) {
        t = &lists[p];
        while (*t != NULL) {
            if (strcmp((*t)->id, id) == 0) {
                t_task *tmp = *t;
                *t = tmp->next;
                free(tmp);
                return;
            }
            t = &(*t)->next;
        }
        p++;
    }
    printf("TAREFA INEXISTENTE\n");
}