#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILENAME		10
#define MAX_INTLEN			256


const char* baseFilename = "infile";

FILE * idfopen(int myid);

int main (int argc , char *argv[])
{
	int current, count, myid, numProcs, numElements, lSum, gSum;
	FILE *inFile;
	int x, i, tmp, sum;
	char buffer[MAX_INTLEN];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	lSum = gSum = 0;
	if (myid == 0)
	{
		printf("Please enter the number of elements to read in\n");
		
		//read from the standard input an int to numElements
	 	scanf ("%d",&x);

	}

	// process 0 bradcasts numElements to all other processes
	MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	inFile = idfopen(myid);
	// all processes read a number of numElements integers from 
	// file infileI where I is equal to process's myid and then
	// do the local sum which is stored in lSum.
	sum = 0;
	for(i = 0; i<x; i++){
		fgets(buffer, MAX_FILENAME, inFile);
		printf("%d buffer = %s", myid, buffer);
		tmp = atoi(buffer);
		sum += tmp;
	}


	// global sum - store it in gSum and print it on the screen by process 0
	MPI_Reduce(&sum, &tmp, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(myid == 0){
		printf("sum: %d\n", tmp);
	}


	fclose(inFile);

	MPI_Finalize();

	return 0;
}


FILE * idfopen(int myid)
{
	FILE *FP;
	char *filename = (char *)malloc(MAX_FILENAME * sizeof(char));

	sprintf(filename, "%s%d", baseFilename, myid);
	FP = fopen(filename,"r");
	free(filename);

	return FP;
}
