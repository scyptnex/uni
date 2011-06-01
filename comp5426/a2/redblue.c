#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define T 3
#define TW 4
#define N T*TW
/* that is, if >R of any colour is in a grid, then the grid is won */
#define PERCENT 33
#define R (TW*TW*PERCENT)/100

#define WHITE 0
#define RED 1
#define BLUE 2

static uint8_t grid[N*N];

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

/* for debigging purposes */
void printGridRandom(){
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

int main(){
	srand(time(NULL));
	assignGrid();
	printGrid();
	printf("%d\n", R);
	return 0;
}
