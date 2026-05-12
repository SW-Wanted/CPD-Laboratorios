/*
	Name: Custom MPI_Bcast Implementation
	Author: Student Implementation
	Date: 2026
	Description:
	Custom implementation of MPI_Bcast using MPI_Send and MPI_Recv.
	Broadcasts a buffer from root process to all other processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

/*
 * Custom MPI_Bcast - Linear Implementation
 * 
 * Complexity Analysis (Linear Implementation):
 * - Time Complexity: O(p) where p is number of processes
 * - Communication rounds: p-1
 * - Root sends to processes 1, 2, ..., p-1 sequentially
 * - Total communication steps: (p-1)
 * 
 * Could be optimized to O(log p) using tree-based approach
 */
int my_MPI_Bcast_linear(void *buffer, int count, MPI_Datatype datatype, 
                         int root, MPI_Comm comm)
{
	int rank, size;
	MPI_Status status;
	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	
	if (rank == root) {
		/* Root process sends to all others */
		for (int i = 0; i < size; i++) {
			if (i != root) {
				MPI_Send(buffer, count, datatype, i, 0, comm);
			}
		}
	} else {
		/* Non-root processes receive from root */
		MPI_Recv(buffer, count, datatype, root, 0, comm, &status);
	}
	
	return MPI_SUCCESS;
}

/*
 * Custom MPI_Bcast - Tree-based Implementation
 * 
 * Complexity Analysis (Tree-based Implementation):
 * - Time Complexity: O(log p) where p is number of processes
 * - Communication rounds: ceil(log2(p))
 * - Uses binary tree structure for broadcasting
 * - Each process can send to 2 children processes in parallel
 */
int my_MPI_Bcast_tree(void *buffer, int count, MPI_Datatype datatype,
                      int root, MPI_Comm comm)
{
	int rank, size;
	MPI_Status status;
	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	
	/* Binary tree broadcast */
	int mask = 1;
	while (mask < size) {
		if (rank & mask) {
			/* Receive from parent: rank with mask bit off */
			int parent = rank ^ mask;
			MPI_Recv(buffer, count, datatype, parent, 0, comm, &status);
		} else {
			/* Send to children */
			int child = rank ^ mask;
			if (child < size) {
				MPI_Send(buffer, count, datatype, child, 0, comm);
			}
		}
		mask <<= 1;
	}
	
	return MPI_SUCCESS;
}

/* Test program with timing comparison */
int main(int argc, char *argv[])
{
	int rank, size, count;
	char *buffer;
	double time_linear, time_tree, time_mpi;
	int rounds = 1000;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (argc > 1)
		rounds = atoi(argv[1]);

	int sizes[] = {64, 256, 1024, 4096, 16384, 65536};
	
	if (!rank)
		printf("=== MPI_Bcast Performance Comparison ===\n");
	if (!rank)
		printf("Processes: %d, Rounds: %d\n\n", size, rounds);

	for (int s = 0; s < 6; s++) {
		count = sizes[s];
		buffer = (char *)malloc(count);
		memset(buffer, rank, count);

		/* Test Custom Linear Implementation */
		MPI_Barrier(MPI_COMM_WORLD);
		time_linear = -MPI_Wtime();
		for (int r = 0; r < rounds; r++) {
			my_MPI_Bcast_linear(buffer, count, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		time_linear += MPI_Wtime();

		/* Test Custom Tree-based Implementation */
		MPI_Barrier(MPI_COMM_WORLD);
		time_tree = -MPI_Wtime();
		for (int r = 0; r < rounds; r++) {
			my_MPI_Bcast_tree(buffer, count, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		time_tree += MPI_Wtime();

		/* Test OpenMPI Implementation */
		MPI_Barrier(MPI_COMM_WORLD);
		time_mpi = -MPI_Wtime();
		for (int r = 0; r < rounds; r++) {
			MPI_Bcast(buffer, count, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		time_mpi += MPI_Wtime();

		if (!rank) {
			printf("Size: %6d B | Linear: %8.4f ms | Tree: %8.4f ms | MPI: %8.4f ms\n",
				   count, time_linear*1000, time_tree*1000, time_mpi*1000);
		}

		free(buffer);
	}

	if (!rank)
		printf("\nNote: Tree-based should be faster than linear for larger p\n");

	MPI_Finalize();
	return 0;
}
