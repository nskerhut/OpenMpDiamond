#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#define SIZE 21 /* The size of the diamond */
//Define the characters that will be printed.
#define print_char '*'
#define trailing_char ' '
#define cr '\n'
// Define the with to be the size plus one.
#define WIDTH (SIZE+1)
// Calculate the area of the diamond length is the size 20 width is 20 + 1 for newline chars
#define AREA (SIZE*WIDTH)
#define HALF (SIZE/2)
int main() {

	// keep track of the parallel processes.
	int th_id; // the id of the current thread;

	//the total number of threads;
    int nthreads;

    // store the full shape in a single dimensional array, boosts memory efficiency.
	char shape[AREA+1];

	int i;

	// X-axis
	int x;
	// Y-axis
	int y;
	// pre calculate the midpoint since it is relatively fixed in size.
    //int lb, ub;
    int chunk;
	// Add clock to the program to keep track of how quickly the program runs.
	clock_t t;
	#pragma omp parallel default(shared) private(th_id,i) shared(shape,nthreads,chunk)
	{
	    // retrieve the number of threads from OpenMP
	    nthreads = omp_get_num_threads();
	    // retrieve the current thread id from OpenMP
        th_id = omp_get_thread_num();

	    //Collect the start time in processor. We are only interested in overall time, not individual processors.
        #pragma omp barrier
        if ( th_id == 0 ) {
            printf("area=%d\n",AREA);
            t = clock();
        }
        //printf("area=%d\n",area);

        #pragma omp barrier
        chunk = floor(AREA/nthreads);


        //#pragma omp parallel for

        // Calculate the range that the current processor is is responsible for.

        //printf("p%d lb=%d,ub=%d\n",th_id,lb,ub);
        #pragma omp barrier
        /*lb = floor(AREA / nthreads) * th_id;
        ub = floor(AREA / nthreads) * (th_id + 1);
        #pragma omp critical
        {
            printf("p%d [%d,%d]\n",th_id,lb,ub);
        }
        #pragma omp barrier
*/
        #pragma omp parallel for schedule(static, chunk)
        for(i = 0;i < AREA;i++){
        //for(i = lb;i < ub;i++){

            x = i / WIDTH;
            y = i % WIDTH;

    //		printf("%3d ",i);
    		//printf("p%d (%2d,%2d)",th_id, x,y);

    		//if the y-axis is equal the WIDTH - 1 then place the carriage return.
            if( y == SIZE) {
                #pragma omp critical
                shape[i] = cr;
            }
            //assign the print char * to the correct index in the shape.
            else if (
               y >= -x + HALF
               && y <= x + HALF
               && y >= x - HALF
               && y <= -x + 3 * HALF
            )
            {
                #pragma omp critical
                shape[i]=print_char;
            }
            //put a space in place if it should not be a newline or the printed char.
            else
            {
                #pragma omp critical
                shape[i]=trailing_char;
            }
        }

        // processor 0 is responsible for providing the IO.
        #pragma omp barrier
        if ( th_id == 0 ) {
          // Display the time it took to build the shape.
          t = clock() - t;
          printf("\n%s\n",shape);

          printf("Built in %d threads\n",nthreads);
          printf("elapsed time: %f\n",((float)t)/CLOCKS_PER_SEC);
        }
	}

}
