#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define T 2
#define TW 3
#define N (T*TW)
/* that is, if >R of any colour is in a grid, then the grid is won */
#define PERCENT 33
#define R ((TW*TW*PERCENT)/100)

#define WHITE 0
#define RED 1
#define BLUE 2

/* All threads use dual lock synchronisation */
/* Dual locking prevents the race condition where the synchroniser receives a falso hold because it happened to unlock the mutex befoe the thread got a chance to lock it */

static pthread_t moveThreads[N];
static pthread_mutex_t redLocks[N];/* each movement thread has a lock for red movement */
static pthread_mutex_t blueLocks[N];/* and blue */
static pthread_cond_t moveSyncs[N];/* but the synchs can be shared */

static pthread_t checkThreads[T*T];
static pthread_mutex_t checkLocks[T*T];
static pthread_mutex_t doneLocks[T*T];
static pthread_cond_t checkSyncs[T*T];

static int going;/* true to start with, if a thread finds saturation then we flag this and terminate all movement threads */
static int stopcall;/* this is how check threads inform the synchroniser they found something, it is separate from going to prevent race conditions */
static int quadrant;/* successful quadrants will write their ID here when they find saturation */

static uint8_t grid[N*N];/* the actual grid */

/* randomly assigns red, blue and white to the grid */
void assignGridRandom(){
	int x, y;
	int numRed = (N*N)/3;
	int numBlue = (N*N)/3;
	int numWhite = (N*N) - numRed - numBlue;
	int tmp;
	for(x=0; x<N; x++){
		for(y=0; y<N; y++){
			tmp = rand()%(numRed + numBlue + numWhite);
			if(tmp < numRed){
				grid[x + y*N] = RED;
				numRed--;
			}
			else if(tmp < numBlue + numRed){
				grid[x + y*N] = BLUE;
				numBlue--;
			}
			else{
				grid[x + y*N] = WHITE;
				numWhite--;
			}
		}
	}
}

void assignGridEven(){
	int x,y;
	int cur;
	int start = WHITE;
	for(x=0; x<N; x++){
		cur = start;
		start = (start+1)%3;
		for(y=0; y<N; y++){
			grid[x + y*N] = cur;
			cur = (cur+1)%3;
		}
	}
}

/* for debigging purposes */
void printGrid(){
	int x, y;
	char out;
	for(y=0; y<N; y++){
		for(x=0; x<N; x++){
			out = '.';
			if(grid[y*N + x] == RED) out = 'r';
			else if(grid[y*N + x] == BLUE) out = 'b';
			printf("%c", out);
		}
		printf("\n");
	}
}

void *checking(void * arg)
{
	int id = (int) arg;
	int red, blue;
	while(1){
		pthread_cond_wait(&checkSyncs[id], &checkLocks[id]);
		if(!going){
			break;
		}
		red = rand()%30;
		printf("c%d: got %d\n", id, red);
		if(red == 0){
			stopcall = 0;
			quadrant = id;
		}
		pthread_cond_wait(&checkSyncs[id], &doneLocks[id]);
		/*printf("c%d: resetting\n", id);*/
	}
	printf("c%d: bye\n", id);
	pthread_exit(NULL);
}

/* for movement threads:
 * 		Wait for start (blue) signal
 *		if saturation has been found, exit loop and terminate
 *		move all reds in (id) row
 *		wait for red signal
 *		move all blues in (id) column
 *		return to start
 */
void *movement(void * arg) 
{
	int i;
	int id = (int) arg;
	while(1){
		pthread_cond_wait(&moveSyncs[id], &blueLocks[id]);
		if(!going){
			break;
		}
		for(i=0; i<N; i++){
			if(grid[i + id*N] == RED && grid[(i+1)%N + id*N] == WHITE){
				grid[i + id*N] = WHITE;
				grid[(i+1)%N + id*N] = RED;
			}
		}
		pthread_cond_wait(&moveSyncs[id], &redLocks[id]);
		for(i=0; i<N; i++){
			if(grid[id + i*N] == BLUE && grid[id + ((i+1)%N)*N] == WHITE){
				grid[id + i*N] = WHITE;
				grid[id + ((i+1)%N)*N] = BLUE;
			}
		}
	}
	printf("m%d: bye\n", id);
	pthread_exit(NULL);
}

void hold(int num, pthread_mutex_t* lcks){
	int i;
	for(i=0; i<num; i++){
		pthread_mutex_lock(&lcks[i]);
	}
}
void advance(int num, pthread_mutex_t* lcks, pthread_cond_t* syncs){
	int i;
	for(i=0; i<num; i++){
		pthread_mutex_unlock(&lcks[i]);
		pthread_cond_signal(&syncs[i]);
	}
}
/* the ultimate synchroniser */
void lockstep(int num, pthread_mutex_t* lcks, pthread_cond_t* syncs){
	hold(num, lcks);
	advance(num, lcks, syncs);
}

int main(){
	/* to make threads joinable */
	int t, c;/* iterators */
	int iteration;/* not to be confused with the iterators */
	pthread_attr_t attr;
	
	/* init */
	srand(time(NULL));
	assignGridRandom();
	going = 1;
	stopcall = 1;
	iteration = 0;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(t=0; t<N; t++){
		pthread_cond_init (&moveSyncs[t], NULL);
		pthread_mutex_init(&redLocks[t], NULL);
		pthread_mutex_init(&blueLocks[t], NULL);
	}
	for(c=0; c<T*T; c++){
		pthread_cond_init(&checkSyncs[c], NULL);
		pthread_mutex_init(&checkLocks[c], NULL);
		pthread_mutex_init(&doneLocks[c], NULL);
	}
	
	/* create threads, lock locks initially */
	/* The logic is: the only reason a lock should be unlocked is if that thread is waiting at a sync point */
	for(t=0; t<N; t++){
		pthread_mutex_lock(&redLocks[t]);
		pthread_mutex_lock(&blueLocks[t]);
		pthread_create(&moveThreads[t], &attr, movement, (void *)t);
	}
	for(c=0; c<T*T; c++){
		pthread_mutex_lock(&checkLocks[c]);
		pthread_mutex_lock(&doneLocks[c]);
		pthread_create(&checkThreads[c], &attr, checking, (void *)c);
	}
	
	/* Time to make som dataflows */
	while(going){
		hold(N, blueLocks);
		printGrid();
		lockstep(T*T, checkLocks, checkSyncs);/* prime and begin all check threads */
		lockstep(T*T, doneLocks, checkSyncs);/* block for all check thread completion */
		if(!stopcall){
			going = 0;
		}
		if(going){
			advance(N, blueLocks, moveSyncs);/* strangely, when we lockstep the blue locks, we start the red stage */
			lockstep(N, redLocks, moveSyncs);/* and vice versa */
			iteration++;
		}
	}
	advance(N, blueLocks, moveSyncs);
	lockstep(T*T, checkLocks, checkSyncs);
	printf("m: %d was saturated after %d iteration(s)\n", quadrant, iteration);
	
	/* Join all threads for termination */
	for(t=0; t<N; t++){
		pthread_join(moveThreads[t], NULL);
	}
	for(c=0; c<T*T; c++){
		pthread_join(checkThreads[c], NULL);
	}
	
	/* cleaning */
	for(t=0; t<N; t++){
		pthread_mutex_destroy(&redLocks[t]);
		pthread_mutex_destroy(&blueLocks[t]);
		pthread_cond_destroy(&moveSyncs[t]);
	}
	pthread_attr_destroy(&attr);
	return 0;
}
