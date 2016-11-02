#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#define size 21 /* The size of the diamond */

int main() {

	int width = size+1;

	// keep track of the parallel processes.
	int th_id; // the id of the current thread;

	//the total number of threads;
    int nthreads;

	char print_char = '*';
	char trailing_char = ' ';
	char cr = '\n';

	// Calculate the area of the diamond length is the size 20 width is 20 + 1 for newline chars
	int area = size*width;

    // store the full shape in a single dimensional array, boosts memory efficiency.
	char shape[area+1];

	int i;

	// X-axis
	int x;
	// Y-axis
	int y;
	// pre calculate the midpoint since it is relatively fixed in size.
    int half = size / 2;
	// Add clock to the program to keep track of how quickly the program runs.
	clock_t t;
	#pragma omp parallel private(th_id) shared(shape)
	{
	    //Collect the start time in processor. We are only interested in overall time, not individual processors.
        #pragma omp barrier
        if ( th_id == 0 ) {
            t = clock();
        }
        //printf("area=%d\n",area);

	    // retrieve the number of threads from OpenMP
	    nthreads = omp_get_num_threads();
	    // retrieve the current thread id from OpenMP
        th_id = omp_get_thread_num();

        //#pragma omp parallel for

        // Calculate the range that the current processor is is responsible for.
        //int lb = floor(area / nthreads) * th_id;
        //int ub = floor(area / nthreads) * (th_id + 1);
        int chunk = floor(area / nthreads);
        //printf("p%d lb=%d,ub=%d\n",th_id,lb,ub);
        #pragma omp for schedule(dynamic, chunk)
        for(i = 0;i < area;i++){

            x = i / width;
            y = i % width;

    //		printf("%3d ",i);
    		//printf("p%d (%2d,%2d)",th_id, x,y);
            if( y == size) {
                shape[i] = cr;
            }
            //assign the print char * to the correct index in the shape.
            else if (
               y >= -x + half
               && y <= x + half
               && y >= x - half
               && y <= -x + 3 * half
            )
            {
                /*#pragma omp critical
                {
                    printf("p%d (%d,%d)",th_id,x,y);
                }*/
                shape[i]=print_char;
            }
            //put a space in place if it should not be a newline or the printed char.
            else
            {
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
