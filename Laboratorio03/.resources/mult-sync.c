#include <stdlib.h>

#define N 1000
#define SEED 0xFC94E3BF

int main(int argc, char** argv)
{
	int acc, i, j, k;
	
	float* A = malloc(N * N * sizeof(float));
	float* B = malloc(N * N * sizeof(float));
	float* C = malloc(N * N * sizeof(float));
	
	srandom(SEED);
	
	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			A[j + i * N] = (float) random() / RAND_MAX;
			B[j + i * N] = (float) random() / RAND_MAX;
		}
	}

#pragma omp parallel private(i, j, k)
	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
#pragma omp single
			acc = 0;

#pragma omp for reduction(+ : acc)
			for (k = 0; k < N; ++k)
				acc += A[k + i * N] * B[j + k * N];

#pragma omp single
			C[j + i * N] = acc;

#pragma omp barrier
		}
	}
	
	free(A);
	free(B);
	free(C);
	
	return 0;
}
