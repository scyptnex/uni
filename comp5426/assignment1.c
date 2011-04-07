#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256

int buf_read(char * buf, const char * end) {
    int count;
    count = fread(buf, 1, end - buf, stdin);
    if (count > 0 && i[count - 1] == '\n') {
        --count;
    }
    return count;
}

int main(int argc, char **argv) {



    ...


    if (numprocs == 1) {
        /* trivial single CPU case */

        ...

    } else if (myid == 0) {
        /* server case */

        ...


	/* read a block of chars to buf */
        read = buf_read(buf, buf + BUF_SIZE);

        /* initial allocation */
	allocatedJobs = 0;
        for(i = 0; i < numprocs - 1; i++) {
	      if (read){
		/* allocate a job to proc i+1 */
	          
                ...

                allocatedJobs++;

                /* read a block, but if the last read was a short item count, don't try to read again */
                
                ...

	      }
              else{
		/* send a zero length array to terminate the worker */

                ...

 
            }
        }

        /* while there is any job to be allocated or received */
        while(allocatedJobs) {                             
            /*after sending jobs, we open a blocking socket and waite for return*/
      
            ...

            /* status.MPI_SOURCE contains the sending process ID. We obtain the value received, 
               and add them to the result */           

            ...

	    allocatedJobs--;
 
            if (read){ 
	      /* Allocate a new job for it */

              ...
	      allocatedJobs++;
              /* read a block, but if the last read was a short item count, don't try to read again */
                
              ...

	    }
            else{
              /* send a zero length array to terminate the worker */

              ...

            }
        }
    } else {
        /* slave processor */
 
        ...

    }

    if (myid == 0) {
      /* print the output */

      ...
 

    }

    ...

}

