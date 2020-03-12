#include "types.h"

int ic = 0, dc = 0, l;
Instruction code[MEM_SIZE-DATA_SIZE];
Instruction data[DATA_SIZE];
char *instructionList[]={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
char *directiveList[]={".string",".data",".extern",".entry"};
List labelTable;
char label[LINE_LENGTH];
enum Boolean labelFlag;
enum errors errorFlag;


int main(int argc, char **argv){
	FILE *fd;
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
		k = 0;
		
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

		/*-----label check-----*/
		if(isLabel(buffer)){
			labelFlag = t;
		}else{
			labelFlag = f;
		}

		/*-----string and data directive check------*/
		if(labelFlag==t){
			while(buffer[k++]!=':');
			
			if((drctv = isDirective(buffer+k))==lstring || drctv==ldata){
				addNode(&labelTable, label, dc, drctv);
				while(buffer[k]!='.') k++;
				k = k + strlen(directiveList[drctv]);
				printf("\n %s",buffer+k);
				printf("\n data status: %d", addData((buffer+k), data, &dc, drctv));
			}
		} else {
			drctv = isDirective(buffer);
		}

	}
	while(dc--) printf("\n data : %c",(char) data[dc].bits);
	return 0;
}
