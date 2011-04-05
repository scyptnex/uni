#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int 		myid, numprocs;
	int			chunk_size = 10;
	int			x = 0;
	int			curValue = 1;


	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD, &myid);

	if(myid == 0)
	{
		int* 			lSum = (int*)calloc(numprocs - 1, sizeof(int));
		MPI_Request* 	reqs = (MPI_Request*)calloc(numprocs - 1, sizeof(MPI_Request));
		int* 			indices = (int *)malloc((numprocs-1)*sizeof(int));
		MPI_Status * 	stat=(MPI_Status*)malloc(sizeof(MPI_Status)*(numprocs - 1));
		int 			i, count, numRunningProcs;
		long int 		result;

		fprintf(stdout,"Please enter the value of x\n");
		fscanf(stdin,"%i",&x);

		count = numprocs - 1;
		for (i = 0; i < count; i++)
			indices[i] = i;

		result = 0;
		numRunningProcs = 0;

		while (numRunningProcs || (curValue + chunk_size - 1) <= x) /*While there is still work to do */
		{
			for(i = 0; i < count && (curValue + chunk_size - 1) <= x; i++)
			{
				MPI_Send(&curValue, 1, MPI_INT, indices[i] + 1, 0, MPI_COMM_WORLD);
				numRunningProcs++;
				curValue += chunk_size;
				/*after sending the work, we open an unblocking socket and wait for return*/
				MPI_Irecv((lSum + indices[i]), 1, MPI_INT, indices[i] + 1, 1, MPI_COMM_WORLD, reqs + indices[i]);
			}

			MPI_Testsome(numprocs - 1, reqs, &count, indices, stat);

			numRunningProcs -= count;
			/* indices is an array with count entries containing the id's of
				the processes that have finished. We obtain the values that
				the finished processors have returned , and add them to the result */
			for(i = 0; i < count; i++)
				result += *(lSum + indices[i]);
		}

		while (curValue <= x)
			result += curValue++;

		curValue = 0;
		for(i = 0; i < numprocs - 1; i++)	/* send the termination signal */
			MPI_Send(&curValue, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);

		printf("The final result is %ld\n", result);
		free(lSum);
		free(reqs);
		free(indices);
		free(stat);
	}
	else
	{
		MPI_Status stat;
		MPI_Request req;

		while (1)
		{
			int	i, curValue, sum = 0;

			MPI_Recv(&curValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

			if (curValue == 0)
				break;

			for (i = 0; i < chunk_size; i++)
				sum += curValue++;

			MPI_Isend(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &req);
			MPI_Wait(&req, &stat);
		}
	}

	MPI_Finalize();
	return 0;

}

