#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv){
	FILE *fd, *fdob, *fdext, *fdent;

	/*No arguments error*/
	if(argc == 1){
		printf("No files entered\n");
		exit(0);
	}
	
	/*Open File*/
	if(!(fd = fopen(*(argv+1),"r"))){
		printf("File load was failed\n");
		exit(0);
	}
	
	return 0;
}
