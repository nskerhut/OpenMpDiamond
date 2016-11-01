#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main() {
	int size=21;
	int width = size+1;

	int th_id, nthreads;

	char print_char = '*';
	char trailing_char = ' ';
	char cr = '\n';

	int area = size*width;

	char shape[area+1];

	int i;
	int x;
	int y;

	clock_t t;

	t = clock();

	int half = size / 2;

//	printf("area=%d\n",area);
    #pragma omp parallel private(th_id)
	{
	    nthreads = omp_get_num_threads();
        th_id = omp_get_thread_num();

    //#pragma omp parallel for
        int lb = floor(area / nthreads) * th_id;
        int ub = (floor(area / nthreads) * th_id + 1) - 1;

        for(lb;i <= ub;i++){
            x = i / width;
            y = i % width;

    //		printf("%3d ",i);
    //		printf("(%2d,%2d)",x,y);



            if( y == size) {
    //			printf("\\\n");

                shape[i] = cr;
            }
            else if (
               y >= -x + half
               && y <= x + half
               && y >= x - half
               && y <= -x + 3 * half
            )
            {
    //			printf("*");

                shape[i]=print_char;
            }
            else
            {
    //			printf("-");

                shape[i]=trailing_char;
            }
        }
        #pragma omp barrier
        if ( th_id == 0 ) {
          nthreads = omp_get_num_threads();
          printf("There are %d threads\n",nthreads);

          t = clock() - t;

            printf("elapsed time: %f",((float)t)/CLOCKS_PER_SEC);

            printf("\n%s\n",shape);
        }
	}

}
