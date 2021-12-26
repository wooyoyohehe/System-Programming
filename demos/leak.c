// Dynamic allocation of a 2D 3x5 array, row at a time
// It has a leak, check with
// valgrind --tool=memcheck --leak-check=full

#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv) {

	int **array, i,j;

	array =(int **)  malloc(sizeof(int *) * 3); 
	array[0] = (int *) malloc (sizeof (int) *5);
	array[1] = (int *) malloc (sizeof (int) *5);
	array[2] = (int *) malloc (sizeof (int) *5);

	for (i=0; i<=2; i++) {
		 for (j=0; j<=4; j++) array[i][j]=i+j;
	}
	for (i=0; i<=2; i++) {
		 printf("row %d ",i);
		 for (j=0; j<=4; j++) printf("%d ",array[i][j]);
		 printf("\n");
	}
	free(array[0]); free(array[1]); free(array[2]); 
}
