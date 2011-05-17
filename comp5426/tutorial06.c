#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char**argv)
{
    int rank, numprocs, i, sbuf;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

    if (rank == 0) { 
	MPI_Request* requests = (MPI_Request*)malloc((numprocs-1)*sizeof(MPI_Request));
	MPI_Status* statuses = (MPI_Status*)malloc(sizeof(MPI_Status)*(numprocs-1));
	int* indices = (int *)malloc((numprocs-1)*sizeof(int));
	int* buf = (int *)malloc((numprocs-1)*sizeof(int));

	int j, ndone;
	int count;
	int w_src;
	int testNum;

	if (numprocs == 1){
	    printf("The number of processes need to be more than 1\n");
	    return;
	}

	printf("Please enter the testing number (a positive integer):\n");
	scanf("%d", &testNum);
	printf("The testing number entered is %d\n", testNum);
	if (testNum < 0){
	    printf("The testing number must be greater or equal to 0\n");
	    testNum = 0;
	}

	/* prepare to receive (non-blocking) messages from all workers */
	count = numprocs-1;
	for (i=0; i<numprocs-1; i++)
	    MPI_Irecv(buf+i, 1, MPI_INT, i+1, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i]);

	/* receive and check conditions */
	sbuf = 1;
	while(count > 0) {
	    MPI_Waitsome(numprocs-1, requests, &ndone, indices, statuses);
	    printf("ndone = %d\n", ndone);
	    for (i=0; i<ndone; i++) {
		j = indices[i];
		w_src = statuses[i].MPI_SOURCE;
		if (buf[j] < testNum){
		    MPI_Send(&sbuf, 1, MPI_INT, w_src, 0, MPI_COMM_WORLD);
		    printf("Msg (rbuf = %d) from %d with tag %d \n", buf[j], w_src, statuses[i].MPI_TAG);
		    MPI_Irecv(buf+j, 1, MPI_INT, w_src, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[j]);
		}
		else {
		    count--;
		    printf("count = %d and worker %d to be terminated.\n", count, w_src);
		}
	    }
	}

	/* terminate all workers */
	sbuf = 0;
	for (i=0; i<numprocs-1; i++)
	    MPI_Send(&sbuf, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD);

    }
    else {
	MPI_Status stat;
	int rbuf;
	sbuf = 0;
	MPI_Send(&sbuf, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	MPI_Recv(&rbuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
	while (rbuf){
	    sbuf++;
	    MPI_Send(&sbuf, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	    MPI_Recv(&rbuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
	}
	printf("worker %d is terminated.\n", rank);
    }
    MPI_Finalize();
    return 0;
}
	

