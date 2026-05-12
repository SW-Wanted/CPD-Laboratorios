/*
	Name: Custom MPI_Scatter Implementation
	Author: Student Implementation
	Date: 2026
	Description:
	Custom implementation of MPI_Scatter using MPI_Send and MPI_Recv.
	Distributes equal-sized chunks from root to all processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

/*
 * Custom MPI_Scatter - Linear Implementation
 * 
 * Complexity Analysis (Linear Implementation):
 * - Time Complexity: O(p) where p is number of processes
 * - Communication rounds: p-1
 * - Root sends to processes 1, 2, ..., p-1 sequentially
 * - Each process i receives sendcount items starting at offset i*sendcount
 * - Total communication steps: (p-1)
 * 
 * Could be optimized to O(log p) using tree-based approach
 */
int my_MPI_Scatter_linear(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                          void *recvbuf, int recvcount, MPI_Datatype recvtype,
                          int root, MPI_Comm comm)
{
	int rank, size;
	MPI_Status status;
	int datasize;
	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	MPI_Type_size(sendtype, &datasize);
	
	if (rank == root) {
		/* Root process sends to all others */
		for (int i = 0; i < size; i++) {
			if (i == root) {
				/* Copy data to own receive buffer */
				memcpy(recvbuf, (char *)sendbuf + i * sendcount * datasize, 
					   recvcount * datasize);
			} else {
				/* Send to other processes */
				MPI_Send((char *)sendbuf + i * sendcount * datasize, 
						 sendcount, sendtype, i, 0, comm);
			}
		}
	} else {
		/* Non-root processes receive from root */
		MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, &status);
	}
	
	return MPI_SUCCESS;
}

/*
 * Custom MPI_Scatter - Tree-based Implementation
 * 
 * Complexity Analysis (Tree-based Implementation):
 * - Time Complexity: O(log p) where p is number of processes
 * - Communication rounds: ceil(log2(p))
 * - Uses binary tree structure for scattering
 * - Root divides and delegates to subtrees
 */
int my_MPI_Scatter_tree(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                        void *recvbuf, int recvcount, MPI_Datatype recvtype,
                        int root, MPI_Comm comm)
{
	int rank, size;
	MPI_Status status;
	int datasize;
	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	MPI_Type_size(sendtype, &datasize);
	
	char *send_buf = (char *)sendbuf;
	char *recv_buf = (char *)recvbuf;
	
	/* Binary tree scatter - forward pass */
	int mask = 1;
	while (mask < size) {
		if (rank & mask) {
			/* Receive from parent */
			int parent = rank ^ mask;
			MPI_Recv(recv_buf, recvcount, recvtype, parent, 0, comm, &status);
		} else {
			/* Send to children and keep segment */
			int child = rank ^ mask;
			if (child < size) {
				int child_segment = (rank ^ mask) * sendcount * datasize;
				MPI_Send(send_buf + child_segment, sendcount, sendtype, child, 0, comm);
			}
		}
		mask <<= 1;
	}
	
	return MPI_SUCCESS;
}

/* Test program with timing comparison */
int main(int argc, char *argv[])
{
	int rank, size, sendcount;
	char *sendbuf, *recvbuf;
	double time_linear, time_tree, time_mpi;
	int rounds = 1000;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (argc > 1)
		rounds = atoi(argv[1]);

	int sizes[] = {64, 256, 1024, 4096, 16384, 65536};
	
	if (!rank)
		printf("=== MPI_Scatter Performance Comparison ===\n");
	if (!rank)
		printf("Processes: %d, Rounds: %d\n\n", size, rounds);

	for (int s = 0; s < 6; s++) {
		sendcount = sizes[s];
		int total_size = sendcount * size;
		
		sendbuf = (char *)malloc(total_size);
		recvbuf = (char *)malloc(sendcount);
		
		if (rank == 0) {
			for (int i = 0; i < total_size; i++)
				sendbuf[i] = (char)(i % 256);
		}

		/* Test Custom Linear Implementation */
		MPI_Barrier(MPI_COMM_WORLD);
		time_linear = -MPI_Wtime();
		for (int r = 0; r < rounds; r++) {
			my_MPI_Scatter_linear(sendbuf, sendcount, MPI_CHAR, 
								   recvbuf, sendcount, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		time_linear += MPI_Wtime();

		/* Test Custom Tree-based Implementation */
		MPI_Barrier(MPI_COMM_WORLD);
		time_tree = -MPI_Wtime();
		for (int r = 0; r < rounds; r++) {
			my_MPI_Scatter_tree(sendbuf, sendcount, MPI_CHAR,
							    recvbuf, sendcount, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		time_tree += MPI_Wtime();

		/* Test OpenMPI Implementation */
		MPI_Barrier(MPI_COMM_WORLD);
		time_mpi = -MPI_Wtime();
		for (int r = 0; r < rounds; r++) {
			MPI_Scatter(sendbuf, sendcount, MPI_CHAR,
					    recvbuf, sendcount, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		time_mpi += MPI_Wtime();

		if (!rank) {
			printf("SendCount: %6d B | Linear: %8.4f ms | Tree: %8.4f ms | MPI: %8.4f ms\n",
				   sendcount, time_linear*1000, time_tree*1000, time_mpi*1000);
		}

		free(sendbuf);
		free(recvbuf);
	}

	if (!rank)
		printf("\nNote: Tree-based should be faster than linear for larger p\n");

	MPI_Finalize();
	return 0;
}
