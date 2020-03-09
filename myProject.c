#include "types.h"

#define MEM_SIZE 4096
#define DATA_SIZE 1024
#define LINE_LENGTH 80

int ic = 0, dc = 0, l;
Boolean lableflag;
Instruction code[MEM_SIZE-DATA_SIZE];
Instruction data[DATA_SIZE];
List labelTable;

int main(int argc, char **argv){
	FILE *fd, *fdob, *fdext, *fdent;
	char buffer[LINE_LENGTH] = {0};
	int i,ch;
	Node *ptr;
	
	/*No files error*/
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
		l = 1;

		while((ch = fgetc(fd))!=EOF && ch!='\n'){
			if(i<LINE_LENGTH){
				buffer[i++] = ch;
			}else{
				printf("Line characters overflow");
				break;
			}
		}

		buffer[i]='\0';
	}
	return 0;
}
