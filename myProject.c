#include "types.h"

#define MEM_SIZE 4096
#define DATA_SIZE 1024
#define LINE_LENGTH 80

int ic = 0, dc = 0, l;
Boolean lableflag;
Instruction code[MEM_SIZE-DATA_SIZE];
Instruction data[DATA_SIZE];
char *instructionList[]={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
char *directiveList[]={".string",".data",".extern",".entry"};
List labelTable;
Boolean labelFlag;
char label[LINE_LENGTH];
enum errors errorFlag;


int main(int argc, char **argv){
	FILE *fd, *fdob, *fdext, *fdent;
	char buffer[LINE_LENGTH] = {0};
	int i,ch,k=0;
	int drctv;
	
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
	
	/*------------First Read-----------*/
	while(!feof(fd)){
		i = 0;
		l = 1;
		
		/*-----Reads the line to the buffer-----*/
		while((ch = fgetc(fd))!=EOF && ch!='\n'){
			if(i<LINE_LENGTH){
				buffer[i++] = ch;
			}else{
				printf("Line length overflow");
				break;
			}
		}
		
		buffer[i]='\0';

		/*checks if the instruction labeled*/
		if(isLabel(buffer)){
			labelFlag.bit = 1;
		}else{
			labelFlag.bit = 0;
		}
		
		if(labelFlag.bit){
			while(buffer[k++]!=':');
			if((drctv = isDirective(buffer[k]))==lstring || drctv==ldata){
				addNode(&labelTable, label, ic, drctv);
				printf("\n string drc\n");
			}
		}
	}
	return 0;
}
