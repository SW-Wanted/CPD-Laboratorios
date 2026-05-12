/*
	Name: Send and Receive Message
	Copyright: 2021-22
	Author: Prof. Joao Costa
	Date: 13/05/22 10:28
	Description:
	Simple experience to understand the Message Passing Interface.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

	MPI_Status status;
	int id, p,
		i, rounds;
	double secs;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (argc != 2)
	{
		if (!id)
			printf("Command line: %s <n-rounds>\n", argv[0]);
		MPI_Finalize();
		exit(1);
	}
	rounds = atoi(argv[1]);

	MPI_Barrier(MPI_COMM_WORLD);
	secs = -MPI_Wtime();

	int sizes[] = {1, 64, 256, 1024, 4096, 16384, 65536};
	for (int s = 0; s < 7; s++)
	{
		char *buf = malloc(sizes[s]);
		// ... Send/Recv com buf de tamanho sizes[s]
		double bw = (sizes[s] * rounds * 2.0) / (secs * 1e6); // MB/s
		if (!id)
			printf("Size=%d B, BW=%.2f MB/s\n", sizes[s], bw);
		free(buf);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	secs += MPI_Wtime();

	if (!id)
	{
		printf("Rounds= %d, N Processes = %d, Time = %12.6f sec,\n",
			   rounds, p, secs);
		printf("Average time per Send/Recv = %6.2f us\n",
			   secs * 1e6 / (2 * rounds * p));
	}
	MPI_Finalize();
	return 0;
}
