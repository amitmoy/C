#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 80

int main(int argc, char **argv){
	FILE *fd, *fdob, *fdext, *fdent;
	char buffer[LINE_LENGTH] = {0};
	int i,ch,line=1;
	
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
	
	/*Reading the text line by line*/
	while(!feof(fd)){
		i = 0;

		while((ch = fgetc(fd))!=EOF && ch!='\n'){

			if(i<LINE_LENGTH){
				buffer[i++] = ch;
			}else{
				printf("\nLine %d is too long\n",line);
				break;
			}
		}

		buffer[i]='\0';
		printf("%s\n",buffer);
		line++;
	}
	return 0;
}
