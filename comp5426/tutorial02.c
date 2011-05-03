#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_TAG			1

int main (int argc , char **argv) {
    int myid, numprocs, source ,destination;
    int token = 0;
    MPI_Status status;
    MPI_Init(&argc,&argv); // Used to send the command line argumenys to all procs
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs); // Initializes the number of procs in the group 
											 // Specified by mpirun
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);	 // Initialize the rank of this process in the group
  
    if(myid == 0)
    {
		token = 1;
        printf("Hello World! This is processor %i from %i - I have the token now.  I make it: %d\n",myid,numprocs,token);
        MPI_Send(&token, 1, MPI_INT, (myid + 1)%numprocs , DEFAULT_TAG, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, numprocs - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
		printf("At the end the token is %d\n", token);
    } else {
        MPI_Recv(&token, 1, MPI_INT, myid - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
        printf("Hello World! This is processor %i from %i - I have the token now: %d to %d\n",myid,numprocs,token,token + 1);
		token += 1;
        MPI_Send(&token, 1, MPI_INT, (myid + 1)%numprocs , DEFAULT_TAG, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}


