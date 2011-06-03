#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define T 2
#define TW 3
#define N T*TW
/* that is, if >R of any colour is in a grid, then the grid is won */
#define PERCENT 33
#define R (TW*TW*PERCENT)/100

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
static int quadrant;/* successful quadrants will write their ID here when they find saturation */

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
			going = 0;
			quadrant = id;
		}
		pthread_cond_wait(&checkSyncs[id], &doneLocks[id]);
		printf("c%d: resetting\n", id);
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
	int id = (int) arg;
	while(1){
		pthread_cond_wait(&moveSyncs[id], &blueLocks[id]);
		if(!going){
			break;
		}
		printf("m%d: red\n", id);
		pthread_cond_wait(&moveSyncs[id], &redLocks[id]);
		printf("m%d: blue\n", id);
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
	going = 1;
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
	hold(N, blueLocks);
	while(going){
		lockstep(T*T, checkLocks, checkSyncs);/* prime and begin all check threads */
		lockstep(T*T, doneLocks, checkSyncs);/* block for all check thread completion */
		if(going){
			advance(N, blueLocks, moveSyncs);/* strangely, when we lockstep the blue locks, we start the red stage */
			lockstep(N, redLocks, moveSyncs);/* and vice versa */
			iteration++;
			hold(N, blueLocks);
		}
	}
	advance(N, blueLocks, moveSyncs);
	lockstep(T*T, checkLocks, checkSyncs);
	printf("m: %d was saturated on iteration %d\n", quadrant, iteration);
	
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
