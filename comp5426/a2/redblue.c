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

static uint8_t grid[N*N];
static pthread_mutex_t locks[N*N];/* a lock for every grid point */
static pthread_t threads[T*T]; /* a thread for every tile */

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
	for(x=0; x<N; x++){
		for(y=0; y<N; y++){
			out = '.';
			if(grid[y*N + x] == RED) out = 'r';
			else if(grid[y*N + x] == BLUE) out = 'b';
			printf("%c", out);
		}
		printf("\n");
	}
}

void *do_work(void * arg) 
{
	int mid = (int) arg;
	int xt = mid%T;
	int yt = mid/T;
	int blue, red;
	int x, y;
	int xd, yd;
	blue = 0;
	red = 0;
	for(x=xt*TW; x<(xt+1)*TW; x++){
		for(y=yt*TW; y<(yt+1)*TW; y++){
			if(pthread_mutex_trylock(&locks[x + y*N])){/* we did not gain the lock */
				/* do nothing */
			}
			else{/* We gained the lock */
				if(grid[x + y*N] != WHITE){
					xd = (x+1)%N;
					yd = y;
					if(grid[x + y*N] == BLUE){
						xd = x;
						yd = (y+1)%N;
					}
					/* now try to get the destination's lock */
					if(pthread_mutex_trylock(&locks[xd + yd*N])){/* we did not get the destination lock */
						/* do nothing */
					}
					else{/* we got the destination lock */
						if(grid[xd + yd*N] == WHITE){
							grid[xd + yd*N] = grid[x + y*N];
							grid[x + y*N] = WHITE;
						}
						pthread_mutex_unlock(&locks[xd + yd*N]);
					}
				}
				pthread_mutex_unlock(&locks[x + y*N]);
			}
			if(grid[y*N + x] == RED) red++;
			else if(grid[y*N + x] == BLUE) blue++;
		}
	}
	printf("%d (%d, %d) :r=%d, b=%d\n", mid, xt, yt, red, blue);
	pthread_exit(NULL);
}

int main(){
	/* to make threads joinable */
	int t, m;/* iterators */
	pthread_attr_t attr;
	
	srand(time(NULL));
	assignGridRandom();
	printGrid();
	printf("%d\n", R);
	
	/* init */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(m=0; m<N*N; m++){
		pthread_mutex_init(&locks[m], NULL);
	}
	
	/* create and join threads */
	for(t=0; t<T*T; t++){
		pthread_create(&threads[t], &attr, do_work, (void *)t);
	}
	
	for(t=0; t<T*T; t++){
		pthread_join(threads[t], NULL);
	}
	
	printGrid();
	
	/* cleaning */
	for(m=0; m<N*N; m++){
		pthread_mutex_destroy(&locks[m]);
	}
	pthread_attr_destroy(&attr);
	return 0;
}
