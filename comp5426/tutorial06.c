#include "mpi.h"
#include <stdio.h>

#include <stdlib.h>

int main(argc, argv)
int argc;
char **argv;
{
    int rank, size, i, sbuf, cnt;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    sbuf = rank; 
    if (rank == size-1) { /* Note the master is the proc with id=size-1 (but not the one with id=0) */
	MPI_Request* requests = (MPI_Request*)malloc((size-1)*sizeof(MPI_Request));
	MPI_Status* statuses = (MPI_Status*)malloc(sizeof(MPI_Status)*(size-1));
	int* indices = (int *)malloc((size-1)*sizeof(int));
	int* buf = (int *)malloc((size-1)*sizeof(int));

	int j, ndone;

	cnt = (size-1)*10;
	for (i=0; i<size-1; i++)
	    MPI_Irecv(buf+i, 1, MPI_INT, i,
		       MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i]);
	while(cnt > 0) {
	    MPI_Waitsome(size-1, requests, &ndone, indices, statuses);
	    for (i=0; i<ndone; i++) {
		j = indices[i];
		printf("Msg (sbuf = %d) from %d with tag %d\n", buf[j], j, statuses[i].MPI_TAG);
		MPI_Irecv(buf+j, 1, MPI_INT, j, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[j]);
	    }
	    cnt -= ndone;
	}

    }
    else {
	for (i=0; i<10; i++) 
	    MPI_Send( &sbuf, 1, MPI_INT, size-1, i, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
	

