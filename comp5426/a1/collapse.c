#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256
#define DEFAULT_TAG 1

/* slightly modified this method so it didnt fail miserably */
size_t buf_read(char* buf, size_t len) {
    size_t count;
    count = fread(buf, 1, len, stdin);
    if (count > 0 && buf[count - 1] == '\n') {
        --count;
    }
    return count;
}

unsigned long sumBuffer(char* buf, size_t len){
	size_t i;
	unsigned long ret = 0;
	for(i=0; i<len; i++){
		ret = ret + buf[i] - '0';
	}
	return ret;
}

/*
By this stage the number is stored in an unsigne dlong, whose max value is roughly 1.85*10^20 and hence at most 20 digits, this a 32 byte buffer can store it
With such a small buffer it would be pointless to add to the network, this method simply calls itself recursively
*/
char finishSummation(unsigned long sum){
	char smallBuf[32];
	size_t smallLen;
	sprintf(smallBuf, "%li", sum);
	smallLen = strlen(smallBuf);
	if(smallLen < 2) return smallBuf[0];
	return finishSummation(sumBuffer(smallBuf, smallLen));/* Elegant, no? */
}

/* That's the Processor ID, not the worker's number */
unsigned long receivePartialSum(int procID, MPI_Status* statusp){
	unsigned long ret;
	MPI_Recv(&ret, 1, MPI_UNSIGNED_LONG, procID, DEFAULT_TAG, MPI_COMM_WORLD, statusp);
	return ret;
}

int main(int argc, char **argv) {

	/*mpi variables*/
	int myid;
	int numProcs;
	MPI_Status status;

	/*my variables*/
	char readBuffer[BUF_SIZE + 1];
	size_t read;
	unsigned long sum = 0;
	unsigned long tempSum;
	int numWorkers, rrStage, jobsPending, i;
	unsigned int terminate;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	numWorkers = numProcs - 1;
	rrStage = 0;/*RoundRobin stage: that is worker 1, i.e. we send to ID rrStage + 1;*/
	jobsPending = 0;
	terminate = 0;

	/* if i am the master*/
	if(myid == 0){

		/* read a block of chars to buf */
		while((read = buf_read(readBuffer, BUF_SIZE)) != 0){

			if(numWorkers > 0){
				/* This happens when we have assigned a job to all workers in round robin and have yet to come back and retreive results */
				if(jobsPending >= numWorkers){
					sum += receivePartialSum(rrStage + 1, &status);
					jobsPending--;
				}
				MPI_Send(&read, 1, MPI_UNSIGNED, rrStage+1, DEFAULT_TAG, MPI_COMM_WORLD);/* Send the length of the buffer to the worker.  In termination this is 0 */
				MPI_Send(readBuffer, BUF_SIZE, MPI_CHAR, rrStage+1, DEFAULT_TAG, MPI_COMM_WORLD);
				rrStage = (rrStage + 1)%numWorkers;
				jobsPending++;
			}

			else{
				sum += sumBuffer(readBuffer, read);
			}
		}
		for(i=0; i<numWorkers; i++){
			rrStage = (rrStage + numWorkers - 1)%numWorkers;
			if(jobsPending > 0){
				sum += receivePartialSum(rrStage + 1, &status);
				jobsPending--;
			}
			MPI_Send(&terminate, 1, MPI_UNSIGNED, rrStage+1, DEFAULT_TAG, MPI_COMM_WORLD);/* terminate workers by sending the signal */
		}
		printf("%c\n", finishSummation(sum));
	}
	
	/*if i am a worker*/
	else{
		while(1){ /* infinite loop is broken by receiving terminate signal */
			MPI_Recv(&read, 1, MPI_UNSIGNED, 0, DEFAULT_TAG, MPI_COMM_WORLD, &status);
			if(read == terminate) break;
			MPI_Recv(readBuffer, BUF_SIZE, MPI_CHAR, 0, DEFAULT_TAG, MPI_COMM_WORLD, &status);
			tempSum = sumBuffer(readBuffer, read);
			MPI_Send(&tempSum, 1, MPI_UNSIGNED_LONG, 0, DEFAULT_TAG, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return 0;
}
