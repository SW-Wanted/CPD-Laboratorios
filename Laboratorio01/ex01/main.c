/*
 * main.c -- main program function
 */


#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "list.h"


/*
 * main program
 */

int main(int argc, char *argv[])
{
  list_t *list;
 
  printf("<<BEGIN>>\n");

  /* initialize list */
  list = lst_new();

  /* insert several elements into list */
  insert_new_process(list, 1, time(NULL));
  sleep(1);
  insert_new_process(list, 2, time(NULL));
  sleep(2);
  insert_new_process(list, 3, time(NULL));
  sleep(2);
  insert_new_process(list, 0, time(NULL));
  sleep(1);
  insert_new_process(list, 5, time(NULL));

  /* print list 'list' */
  lst_print(list);

  /* remove elements from list */
  update_terminated_process(list, 1, time(NULL));
  sleep(1);
  update_terminated_process(list, 2, time(NULL));
  sleep(2);
  update_terminated_process(list, 5, time(NULL));

  /* print list */
  lst_print(list);

  /* clean up list */
  lst_destroy(list);

  printf("<<END>>\n");

  return 0;
}
