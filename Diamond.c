#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

int main() {
	int size=21;
	int width = size+1;

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

	for(i=0;i < area;i++){
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
	t = clock() - t;	
	
	printf("elapsed time: %f",((float)t)/CLOCKS_PER_SEC);

	printf("\n%s\n",shape);
}
