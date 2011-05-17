#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int 		myid, numprocs;
	int		x;

	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD, &myid);

	if (myid == 0){
		fprintf(stdout,"Please enter the value of x\n");
		scanf("%d",&x);
		printf("the value of x is %d\n", x);
	}

	if (numprocs == 1) {
		/* trivial single CPU case */
		int i, result = 0;
		for (i=1; i<=x; i++)
			result += i;

		printf("one process and the final result is %d\n", result);

	} else 
	if(myid == 0){
		MPI_Request* 	reqs = (MPI_Request*)malloc((numprocs-1)*sizeof(MPI_Request));
		MPI_Status* 	stat=(MPI_Status*)malloc((numprocs-1)*sizeof(MPI_Status));
		int* 			indices = (int*)malloc((numprocs-1)*sizeof(int));
		int* 			buf = (int*)malloc((numprocs-1)*sizeof(int));
		int 			i, j, t, count, numRunningProcs, wkrid;
		int 		result = 0;

		for(i = 0; i < numprocs-1; i++)
		{
			/* send the problem size x to workers */
			MPI_Send(&x, 1, MPI_INT, i+1, 10, MPI_COMM_WORLD);

			/*open an unblocking socket and wait for return from that worker*/
	   		MPI_Irecv(&buf[i], 1, MPI_INT, i+1, MPI_ANY_TAG, MPI_COMM_WORLD, &reqs[i]);
		}

		numRunningProcs = numprocs - 1;

		/* while there is any job to be allocated or received */
		while (numRunningProcs){
			/* wait for partial results from workers*/	
			MPI_Waitsome(numprocs-1, reqs, &count, indices, stat);

			numRunningProcs -= count;
			for(i = 0; i < count; i++){
			   	wkrid = stat[i].MPI_SOURCE;
				t = stat[i].MPI_TAG;
				j = indices[i];
				result += buf[j];
			   	printf("partial result %d received from worker id = %d with tag = %d\n", buf[j], wkrid, t);
			}
		}

		printf("The final result is %d\n", result);

		free(buf);
		free(reqs);
		free(indices);
		free(stat);
	}
	else{ 
		/* worker processes */
		MPI_Status status; 
		int x, i, strt, fnsh;  
		int result = 0;

		/* receive the size y from master */
		MPI_Recv(&x, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

		strt = (myid-1) * x / (numprocs-1) + 1;
		fnsh = myid * x / (numprocs-1);
 		for (i=strt; i<=fnsh; i++)
			result += i;

		/* send the result to master */
           	MPI_Send(&result, 1, MPI_INT, 0, myid, MPI_COMM_WORLD);
	}


	MPI_Finalize();
	return 0;

}

