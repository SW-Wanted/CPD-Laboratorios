/*
 * main.c -- main program function
 */

#include "task.h"

/*
 * main program
 */

int main() {
    char cmd[16];
    char id[MAX_ID];
    int priority;
    t_task *lists[MAX_PRIORITY]; /* array of pointers to task lists */

    memset(lists, 0, sizeof(lists));

    while (scanf("%s", cmd) == 1) {
        if (strcmp(cmd, "new") == 0) {
            scanf("%d %s", &priority, id);
            tsk_new(lists, priority, id);
        } else if (strcmp(cmd, "list") == 0) {
            scanf("%d", &priority);
            tsk_list(lists, priority);
        } else if (strcmp(cmd, "complete") == 0) {
            scanf("%s", id);
            tsk_complete(lists, id);
        }
        else if ((strcmp(cmd, "exit") == 0) || (strcmp(cmd, "quit") == 0)) {
            break;
        }
        else {
            printf("Syntax:\n");
            printf("\n");
            printf("  new <priority> <task_id> : new task\n");
            printf("  list <priority>          : list tasks with priority\n");
            printf("  complete <task_id>       : mark task as complete\n");
            printf("\n");
            printf("  exit | quit              : exit program\n");
        }
    }
    return 0;
}