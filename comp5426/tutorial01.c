#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


#define MAX_BUFFER_SIZE		256
#define MASTER				0
#define DEFAULT_TAG			1

int main (int argc , char **argv) {
    int myid, numProcs, src;
	int size = MAX_BUFFER_SIZE;
    char msg[MAX_BUFFER_SIZE];
	char procn[MAX_BUFFER_SIZE];
	char hostn[MAX_BUFFER_SIZE];
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    if (myid == 0)
    {
		for (src = 1; src < numProcs; src++)
		{
			MPI_Recv(msg, MAX_BUFFER_SIZE, MPI_CHAR, src, DEFAULT_TAG, MPI_COMM_WORLD, &status);
			printf("Greetings from %s!\n", msg);
		}
	}
	else
	{
		MPI_Get_processor_name(procn, &size);
		gethostname(hostn, MAX_BUFFER_SIZE);
		sprintf(msg, "slave %d, %s on %s", myid, hostn, procn);
		MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, MASTER, DEFAULT_TAG, MPI_COMM_WORLD);
	}

    MPI_Finalize();
    return 0;
}


