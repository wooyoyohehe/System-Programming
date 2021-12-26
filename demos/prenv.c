/* program to print the environment and pid */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[], char *envp[])

{
	int i;

	printf("My PID is %d\nHere is my environment:\n",getpid());

	for (i=0; envp[i] != NULL; i++)
		printf("%s \n",envp[i]);
	exit(0);
}
