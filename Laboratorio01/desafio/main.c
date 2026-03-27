/*
 * main_cpd.c -- cpd-terminal: simple parallel terminal
 *
 * Stage 1: fork/exec child processes in background, orderly exit command.
 * Stage 2: monitor thread that records execution times via wait().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commandlinereader.h"
#include "list.h"

#define MAX_ARGS    7       /* pathname + up to 5 args + NULL */
#define BUFFER_SIZE 512     /* buffer for readLineArguments   */

/* Shared state between main thread and monitor thread                 */
list_t          *process_list;
int              numChildren = 0;   /* children currently running      */
int              done        = 0;   /* set to 1 when user types exit   */
pthread_mutex_t  lock = PTHREAD_MUTEX_INITIALIZER;

/* Monitor Thread */
void *monitor(void *arg)
{
    while (1) {
        pthread_mutex_lock(&lock);
        int children = numChildren;
        int finished = done;
        pthread_mutex_unlock(&lock);

        if (children == 0) {
            if (finished) break;  /* exit received and no children → stop */
            sleep(1);             /* active wait: sleep 1 second          */
            continue;
        }

        /* at least one child running — block until one terminates */
        int status;
        pid_t pid = wait(&status);
        if (pid > 0) {
            time_t endtime = time(NULL);
            pthread_mutex_lock(&lock);
            update_terminated_process(process_list, pid, endtime);
            numChildren--;
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

/* Main Thread */
int main(void)
{
    char path[BUFFER_SIZE];
    char  buffer[BUFFER_SIZE];
    char *args[MAX_ARGS];
    int   nargs;

    process_list = lst_new();

    /* launch the monitor thread */
    pthread_t monitor_tid;
    pthread_create(&monitor_tid, NULL, monitor, NULL);

    /* command reading loop */
    while ((nargs = readLineArguments(args, MAX_ARGS, buffer, BUFFER_SIZE)) >= 0) {
        if (nargs == 0) continue;  /* empty line */

        /* exit command: signal the monitor and leave the loop */
        if (strcmp(args[0], "exit") == 0) {
            pthread_mutex_lock(&lock);
            done = 1;
            pthread_mutex_unlock(&lock);
            break;
        }

        /* launch child process */
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Error: could not create child process\n");
            continue;
        }
        
        if (pid == 0) {
            /* -- child process -- */
            execv(args[0], args);

            /* only reached if execv or access fails */
            fprintf(stderr, "Error executing: %s\n", args[0]);
            exit(1);
        }

        /* -- parent process -- */
        time_t starttime = time(NULL);
        pthread_mutex_lock(&lock);
        insert_new_process(process_list, pid, starttime);
        numChildren++;
        pthread_mutex_unlock(&lock);
    }

    /* wait for the monitor thread to finish (after all children exit) */
    pthread_join(monitor_tid, NULL);

    /* print final report: pid and execution time of each child */
    lst_print(process_list);
    lst_destroy(process_list);

    return 0;
}