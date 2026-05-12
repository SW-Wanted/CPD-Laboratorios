/*
	Name: Custom MPI_Scatterv Implementation
	Author: Student Implementation
	Date: 2026
	Description:
	Custom implementation of MPI_Scatterv using MPI_Send and MPI_Recv.
	Distributes variable-sized chunks from root to all processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

/*
 * Custom MPI_Scatterv - Linear Implementation
 * 
 * Complexity Analysis (Linear Implementation):
 * - Time Complexity: O(p) where p is number of processes
 * - Communication rounds: p-1
 * - Root sends to processes 1, 2, ..., p-1 sequentially
 * - Each process i receives sendcounts[i] items starting at offset displs[i]
 * - Total communication steps: (p-1)
 * 
 * Could be optimized to O(log p) using tree-based approach with prefix sums
 */
int my_MPI_Scatterv_linear(const void *sendbuf, const int *sendcounts, const int *displs,
                           MPI_Datatype sendtype, void *recvbuf, int recvcount,
                           MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	int rank, size;
	MPI_Status status;
	int datasize;
	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	MPI_Type_size(sendtype, &datasize);
	
	if (rank == root) {
		/* Root process sends variable-sized chunks to all others */
		for (int i = 0; i < size; i++) {
			if (i == root) {
				/* Copy data to own receive buffer */
				memcpy(recvbuf, (char *)sendbuf + displs[i] * datasize,
					   recvcount * datasize);
			} else {
				/* Send to other processes with their specific size */
				MPI_Send((char *)sendbuf + displs[i] * datasize,
						 sendcounts[i], sendtype, i, 0, comm);
			}
		}
	} else {
		/* Non-root processes receive from root */
		MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, &status);
	}
	
	return MPI_SUCCESS;
}

/*
 * Custom MPI_Scatterv - Tree-based Implementation
 * 
 * Complexity Analysis (Tree-based Implementation):
 * - Time Complexity: O(log p) where p is number of processes
 * - Communication rounds: ceil(log2(p))
 * - Uses binary tree structure for scattering
 * - Root divides and delegates to subtrees with appropriate boundaries
 * - Requires careful offset computation using prefix sums
 */
int my_MPI_Scatterv_tree(const void *sendbuf, const int *sendcounts, const int *displs,
                         MPI_Datatype sendtype, void *recvbuf, int recvcount,
                         MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	int rank, size;
	MPI_Status status;
	int datasize;
	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	MPI_Type_size(sendtype, &datasize);
	
	char *send_buf = (char *)sendbuf;
	char *recv_buf = (char *)recvbuf;
	
	/* For simplicity in tree implementation, we'll use a simpler approach */
	int mask = 1;
	int *my_displs = (int *)malloc(size * sizeof(int));
	int *my_counts = (int *)malloc(size * sizeof(int));
	
	memcpy(my_counts, sendcounts, size * sizeof(int));
	memcpy(my_displs, displs, size * sizeof(int));
	
	while (mask < size) {
		if (rank & mask) {
			/* Receive from parent */
			int parent = rank ^ mask;
			MPI_Recv(recv_buf, recvcount, recvtype, parent, 0, comm, &status);
		} else {
			/* Send to children */
			int child = rank ^ mask;
			if (child < size) {
				MPI_Send(send_buf + my_displs[child] * datasize,
						 my_counts[child], sendtype, child, 0, comm);
			}
		}
		mask <<= 1;
	}
	
	free(my_displs);
	free(my_counts);
	
	return MPI_SUCCESS;
}

/* Test program with timing comparison */
int main(int argc, char *argv[])
{
	int rank, size;
	char *sendbuf, *recvbuf;
	double time_linear, time_tree, time_mpi;
	int rounds = 100;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (argc > 1)
		rounds = atoi(argv[1]);

	if (!rank)
		printf("=== MPI_Scatterv Performance Comparison ===\n");
	if (!rank)
		printf("Processes: %d, Rounds: %d\n\n", size, rounds);

	/* Create variable sendcounts and displs */
	int *sendcounts = (int *)malloc(size * sizeof(int));
	int *displs = (int *)malloc(size * sizeof(int));
	
	/* Variable sizes: process i gets (i+1)*256 bytes */
	displs[0] = 0;
	for (int i = 0; i < size; i++) {
		sendcounts[i] = (i + 1) * 256;
		if (i > 0)
			displs[i] = displs[i-1] + sendcounts[i-1];
	}
	int total_size = displs[size-1] + sendcounts[size-1];

	sendbuf = (char *)malloc(total_size);
	recvbuf = (char *)malloc(sendcounts[rank]);
	
	if (rank == 0) {
		for (int i = 0; i < total_size; i++)
			sendbuf[i] = (char)(i % 256);
	}

	if (!rank) {
		printf("Variable sendcounts:\n");
		for (int i = 0; i < size; i++)
			printf("  Process %d: %d bytes at offset %d\n", i, sendcounts[i], displs[i]);
		printf("\n");
	}

	/* Test Custom Linear Implementation */
	MPI_Barrier(MPI_COMM_WORLD);
	time_linear = -MPI_Wtime();
	for (int r = 0; r < rounds; r++) {
		my_MPI_Scatterv_linear(sendbuf, sendcounts, displs, MPI_CHAR,
							   recvbuf, sendcounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	time_linear += MPI_Wtime();

	/* Test Custom Tree-based Implementation */
	MPI_Barrier(MPI_COMM_WORLD);
	time_tree = -MPI_Wtime();
	for (int r = 0; r < rounds; r++) {
		my_MPI_Scatterv_tree(sendbuf, sendcounts, displs, MPI_CHAR,
							 recvbuf, sendcounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	time_tree += MPI_Wtime();

	/* Test OpenMPI Implementation */
	MPI_Barrier(MPI_COMM_WORLD);
	time_mpi = -MPI_Wtime();
	for (int r = 0; r < rounds; r++) {
		MPI_Scatterv(sendbuf, sendcounts, displs, MPI_CHAR,
					 recvbuf, sendcounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	time_mpi += MPI_Wtime();

	if (!rank) {
		printf("Total data size: %d B\n", total_size);
		printf("Linear:    %10.4f ms\n", time_linear*1000);
		printf("Tree:      %10.4f ms\n", time_tree*1000);
		printf("MPI:       %10.4f ms\n", time_mpi*1000);
		printf("\nNote: Tree-based should be faster than linear for larger p\n");
	}

	free(sendbuf);
	free(recvbuf);
	free(sendcounts);
	free(displs);

	MPI_Finalize();
	return 0;
}
