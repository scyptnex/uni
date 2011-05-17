#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_TAG 1
#define BLOCK_SIZE 64

#define MASTER 0

/* slightly modified this method so it didnt fail miserably */
/* Lifted directly from assignment 1 */
size_t buf_read(char* buf, size_t len) {
    size_t count;
    count = fread(buf, 1, len, stdin);
    if (count > 0 && buf[count - 1] == '\n') {
        --count;
    }
    return count;
}

/* same as assign 1 */
unsigned long sumBuffer(char* buf, size_t len){
	size_t i;
	unsigned long ret = 0;
	for(i=0; i<len; i++){
		ret = ret + buf[i] - '0';
	}
	return ret;
}

/* the super excellent elegant recursive summation */
/* As stated previously, the collapse of a number is at most 10* the number of digits, which means a number must be approx (2^64)/10 digits long or approx 1670 terraBytes*/
char finishSummation(unsigned long sum){
	char smallBuf[32];
	size_t smallLen;
	sprintf(smallBuf, "%li", sum);
	smallLen = strlen(smallBuf);
	if(smallLen < 2) return smallBuf[0];
	return finishSummation(sumBuffer(smallBuf, smallLen));/* Elegant, no? */
}

int main(int argc, char *argv[])
{
	/*mpi variables*/
	int myid;
	int numProcs;
	MPI_Status nullStatus;

	/*master variables*/
	MPI_Request* allReqs;
	MPI_Status* allStats;

	int* sumBuffers;
	int* demanders;

	int numOut;
	int finCount;
	int curID;
	time_t start,cur;

	/*Worker variables*/
	int workAmount;

	/*collective variables*/
	int i;
	char readBuffer[BUF_SIZE + 1];/*+1 for good luck*/
	int curBufSize;
	unsigned long total;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if{InumProcs == 1){/* ther is only 1 master*/
		while((curBufSize = buf_read(readBuffer, BLOCK_SIZE)) > 0){
			total += sumBuffer
		}
	}
	else if(myid == MASTER){/*i am the master, ther is at least 1 worker*/

		allReqs = (MPI_Request*)malloc((numProcs-1)*sizeof(MPI_Request));
		allStats = (MPI_Status*)malloc((numProcs-1)*sizeof(MPI_Status));

		sumBuffers = (int*)malloc((numProcs-1)*sizeof(int));
		demanders = (int*)malloc((numProcs-1)*sizeof(int));

		total = 0;
		numOut = numProcs-1;

		srand ( time(NULL) );
		time (&start);
		for(i=1; i<numProcs; i++){/*line up requests for the reverberator*/
			MPI_Irecv(&sumBuffers[i-1], 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &allReqs[i-1]);
		}

		while(numOut > 0){
			MPI_Waitsome(numProcs-1, allReqs, &finCount, demanders, allStats);
			numOut -= finCount;/* lets presume for now that there are no more workloads to allocate */
			time(&cur);
			for(i = 0; i < finCount; i++){
			   	curID = allStats[i].MPI_SOURCE;
				total += sumBuffers[curID-1];
			   	printf("%d, up to %d", curID, total);
				if(difftime(cur,start) < 10){
					assignment = 1 + rand()%4;
					printf(", assigning %d\n", assignment);
					MPI_Send(&assignment, 1, MPI_INT, curID, DEFAULT_TAG, MPI_COMM_WORLD);
					numOut++;
					MPI_Irecv(&sumBuffers[curID-1], 1, MPI_INT, curID, MPI_ANY_TAG, MPI_COMM_WORLD, &allReqs[curID-1]);
				}
				else{
					assignment = 0;
					printf(", terminating\n");
					MPI_Send(&assignment, 1, MPI_INT, curID, DEFAULT_TAG, MPI_COMM_WORLD);
				}
			}
		}
		free(allReqs);
		free(allStats);
		free(sumBuffers);
		free(demanders);
	}
	
	/*if i am a worker*/
	else{
		result = 0;
		while(1){/*break when exiting*/
			MPI_Send(&result, 1, MPI_INT, MASTER, DEFAULT_TAG, MPI_COMM_WORLD);
			MPI_Recv(&workAmount, 1, MPI_INT, MASTER, DEFAULT_TAG, MPI_COMM_WORLD, &nullStatus);
			if(workAmount == 0) break;/*termination condition*/
			else{
				printf("(%d) i will sleep %d\n", myid, workAmount);
				sleep(workAmount);
				result++;
			}
		}
		printf("(%d) goodbye\n", myid);
	}

	MPI_Finalize();
	return 0;

}

