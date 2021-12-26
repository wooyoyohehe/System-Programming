/* Program to copy a file, using stdio */

#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	FILE *fd1,*fd2;		/* FILE descriptors */
	int c;				/* input buffer */

	if ((fd1=fopen(argv[1],"r")) == NULL) {
		puts("open1");
		exit(1);
		}

	if ((fd2=fopen(argv[2],"w"))== NULL) {
		puts("open2");
		exit(1);
		}


	while ((c=getc(fd1)) != EOF )
		putc(c,fd2);


fclose(fd2);
}
