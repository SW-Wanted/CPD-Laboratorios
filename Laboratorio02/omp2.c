/*
	Name: Lab#2
	Copyright: 2021-22
	Author: Prof. Joao Costa
	Date: 06/04/22 17:24
	Description: 
		Parallelizing an inner loop with dependences
	
			for (iter=0; iter<numiter; iter++) {
				for (i=0; i<size-1; i++) {
					V[i] = f( V[i], V[i+1] );
				}
			}
*/

#include<stdio.h>
#include<stdlib.h>

#define TOTALSIZE 1000
#define NUMITER 200

/*
* DUMMY FUNCTION
*/
#define f(x,y)	((x+y)/2.0)


/* MAIN: PROCESS PARAMETERS */
int main(int argc, char *argv[]) {

  /* VARIABLES */
  int i, iter;

  /* DECLARE VECTOR AND AUX DATA STRUCTURES */
  double *src = (double *) malloc(TOTALSIZE * sizeof(double));
  double *dst = (double *) malloc(TOTALSIZE * sizeof(double));
  double *tmp;

  /* 1. INITIALIZE VECTOR */
  for(i = 0; i < TOTALSIZE; i++) {
    src[i] = 0.0 + i;
  }

  /* 2. ITERATIONS LOOP */
  for(iter = 0; iter < NUMITER; iter++) {
    /* 2.1. PROCESS ELEMENTS */
    #pragma omp parallel for
    for(i = 0; i < TOTALSIZE-1; i++) {
      dst[i] = f(src[i], src[i+1]);
    }
    dst[TOTALSIZE-1] = src[TOTALSIZE-1];
    tmp = src;
    src = dst;
    dst = tmp;
    /* 2.2. END ITERATIONS LOOP */
  }

  /* 3. OUTPUT FINAL VALUES */
  printf("Output:\n"); 
  for(i = 0; i < TOTALSIZE; i++) {
    printf("%4d %f\n", i, src[i]);
  }

  /* 4. FREE MEMORY */
  free(src);
  free(dst);
}
