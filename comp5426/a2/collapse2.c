#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DEFAULT_TAG 1
#define BLOCK_SIZE 64

#define MASTER 0
#define TERMINATE 0

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

	unsigned long* sumBuffers;
	int* demanders;
	int finCount;
	int curFin;
	int maxAmmount;/* each worker cannot be assigned more than this ammount */

	/*collective variables*/
	int i;
	int stillgoing;/* will count thee number of workers possibly working or otherwise unoccupied */
	char readBuffer[BLOCK_SIZE + 1];/*+1 for good luck*/
	int curBufSize;
	unsigned long total;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if(myid == MASTER){/*i am a master*/
	
		total = 0;
		stillgoing = 0;
		
		if(numProcs == 1){
			while((curBufSize = buf_read(readBuffer, BLOCK_SIZE)) != 0){
				total += sumBuffer(readBuffer, curBufSize);
			}
		}
		else{
			/*allocate arrays*/
			allReqs = (MPI_Request*)malloc((numProcs-1)*sizeof(MPI_Request));
			allStats = (MPI_Status*)malloc((numProcs-1)*sizeof(MPI_Status));
			sumBuffers = (unsigned long*)malloc((numProcs-1)*sizeof(unsigned long));
			demanders = (int*)malloc((numProcs-1)*sizeof(int));
			
			for(i=1; i<numProcs; i++){/*line up requests for the reverberator*/
				MPI_Irecv(&sumBuffers[i-1], 1, MPI_UNSIGNED_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, &allReqs[i-1]);
				stillgoing++;
			}
			
			while(stillgoing > 0){
				MPI_Waitsome(numProcs-1, allReqs, &finCount, demanders, allStats);
				/*printf("M: %d finished this round\n", finCount);*/
				for(i = 0; i < finCount; i++){
					curFin = allStats[i].MPI_SOURCE;
					MPI_Recv(&maxAmmount, 1, MPI_INT, curFin, DEFAULT_TAG, MPI_COMM_WORLD, &nullStatus);
					/*printf("M: %d has solved to %ld and requested %d", curFin, sumBuffers[curFin-1], maxAmmount);*/
					total += sumBuffers[curFin-1];
					curBufSize = buf_read(readBuffer, maxAmmount);
					MPI_Send(&curBufSize, 1, MPI_INT, curFin, DEFAULT_TAG, MPI_COMM_WORLD);
					if(curBufSize == 0){
						stillgoing--;
						/*printf(", we shall terminate it\n");*/
					}
					else{
						/*printf(", sending %d\n", curBufSize);*/
						MPI_Send(readBuffer, BLOCK_SIZE, MPI_CHAR, curFin, DEFAULT_TAG, MPI_COMM_WORLD);
						MPI_Irecv(&sumBuffers[curFin-1], 1, MPI_UNSIGNED_LONG, curFin, MPI_ANY_TAG, MPI_COMM_WORLD, &allReqs[curFin-1]);
					}
				}
			}
			
			/*free arrays*/
			free(allReqs);
			free(allStats);
			free(sumBuffers);
			free(demanders);
		}
		
		printf("%c\n", finishSummation(total));
	}
	else{/*i am a worker*/
		srand( time(NULL) );
		total = 0;
		while(1){/*break when exiting*/
			MPI_Send(&total, 1, MPI_UNSIGNED_LONG, MASTER, DEFAULT_TAG, MPI_COMM_WORLD);/* send the total ammount */
			curBufSize = rand()%(BLOCK_SIZE/2) + BLOCK_SIZE/2;/*generate a random work ammount, to speed things up we say its between max and max/2*/
			/*printf("%d: requesting %d\n", myid, curBufSize);*/
			MPI_Send(&curBufSize, 1, MPI_INT, MASTER, DEFAULT_TAG, MPI_COMM_WORLD);/* tell the master the most i am willing to work */
			MPI_Recv(&curBufSize, 1, MPI_INT, MASTER, DEFAULT_TAG, MPI_COMM_WORLD, &nullStatus);/* see how much i will actually work */
			if(curBufSize == TERMINATE) break;/* i'm fired */
			MPI_Recv(readBuffer, BLOCK_SIZE, MPI_CHAR, 0, DEFAULT_TAG, MPI_COMM_WORLD, &nullStatus);
			total = sumBuffer(readBuffer, curBufSize);
		}
		/*printf("%d: goodbye\n", myid);*/
	}
	MPI_Finalize();
	return 0;

}
