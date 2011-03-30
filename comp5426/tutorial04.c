#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int InsertionSort(int *arr,int size) {
	int 	j,P,tmp,operations;

	operations = 0;
	for(P = 1; P < size; P++) {
		operations++;
		tmp = arr[P];
		for(j = P; j > 0 && arr[j-1] > tmp; j-- ) {
			operations++;
			arr[j] = arr[j-1];
		}
		arr[j] = tmp;
	}
	return operations;
}

int arrSize(int *arr) {
	int size = 0;
	while(*arr++ != '\0') {
		size++;
	}
	return size;

}

int printArr(int *arr) {
	int	count = 0;
	while(arr[count] != '\0') {
		printf("%i\n",arr[count]);
		count++;
	}

}

void readFile(char *filename, int *arr) {
	FILE		*fp			= fopen(filename,"r");
	int			curr		= 0;
	int			count		= 0;
	while(fscanf(fp,"%i",&curr) == 1) {
		arr[count] = curr;
		count++;
	}
	arr[count] = '\0';
}


int main(int argc , char **argv)
{
	int 		numprocs,myid;
	int 		larrsize 		= 0;
	int 		garrsize 		= 0;
	int 		lnumops 		= 0;
	int			gnumops			= 0;
	int			*garr			= (int *)calloc(atoi(argv[1])+1,sizeof(int));
	int 		*larr;
	int i;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	if(myid == 0) {
		readFile(argv[argc-1],garr);
		printf("Read in a file with %i valid elements\n",arrSize(garr));
		garrsize = arrSize(garr);
		larrsize = (int)(garrsize/numprocs);
	}
	
	//process 0 bradcasts garrsize and larrsize to all other processes
	MPI_Bcast(&larrsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	printf("for processor %i there are %i local items\n",myid,larrsize);
	
	larr = (int *)calloc(larrsize,sizeof(int));
	
	//garr is scattered to all processes
	MPI_Scatter(garr, larrsize, MPI_INT, larr, larrsize, MPI_INT, 0, MPI_COMM_WORLD);
	for(i=0; i<larrsize; i++){
	//	printf("%d ",larr[i]);
	}
	//printf("\n");
	lnumops = InsertionSort(larr,larrsize);

	//locally sorted larr gathered to process 0
	MPI_Gather(larr, larrsize, MPI_INT, garr, larrsize, MPI_INT, 0, MPI_COMM_WORLD);

	if(myid == 0) {
		lnumops += InsertionSort(garr,garrsize);
	}
	printf("for processor %i it took %i operations to sort\n",myid,lnumops);
	MPI_Reduce(&lnumops,&gnumops,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	
	
	
	if(myid == 0) {
		printf("All up there were %i operations\n",gnumops);
	}
	
		
	printArr(garr);
	MPI_Finalize();
	return 0;
}
