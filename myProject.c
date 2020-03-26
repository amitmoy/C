#include "types.h"

int ic = 0, dc = 0, l, firstMethod, secondMethod;
Instruction code[MEM_SIZE-DATA_SIZE];
Instruction data[DATA_SIZE];
char *instructionList[]={"mov ","cmp ","add ","sub ","lea ","clr ","not ","inc ","dec ","jmp ","bne ","red ","prn ","jsr ","rts ","stop "};
char *directiveList[]={".string ",".data ",".extern ",".entry "};
List labelTable;
char label[LINE_LENGTH];
enum Boolean labelFlag;
enum errors errorFlag;


int main(int argc, char **argv){
	FILE *fd;
	char buffer[LINE_LENGTH] = {0};
	int i,ch,k=0;
	int drctv = -1;
	int instruction;
	
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
				addData((buffer+k), data, &dc, drctv);
				continue;
			}
		} else {
			drctv = isDirective(buffer);
			if(drctv == lstring || drctv == ldata){
				while(buffer[k]!='.') k++;
				k = k + strlen(directiveList[drctv]);
				addData((buffer+k), data, &dc, drctv);
				continue;
			}
		}

		printf("\ndrctv = %d, %s", drctv, buffer);
		/*checks if .entry or .extern, if entry continue, if external adds the label*/
		if(drctv==lentry){
			if(labelFlag)
				printf("Warning: Label before entry does not do anything");
			continue;
		} else if(drctv == lextern){
			if(labelFlag){
				printf("Warning: Label before extern does not do anything");
			}
			addExtern(buffer);
			continue;
		} 

		/*------Its instruction-----*/
		drctv = lcode;
		if(labelFlag==t){
			addNode(&labelTable, label, ic+100, drctv);
			while(buffer[k++]!=':');
		}

		if((instruction = whatInstruction(buffer+k))==-1){
			/*--------error-------*/
		} else {
			code[ic++].bits = instruction<<11;
			while(buffer[k] == ' ' || buffer[k] == '\t') k++;
			k = k + strlen(instructions[instruction]);
			if(instruction>=0 && instruction<=4){
				/*two opperands*/
			}else if(instruction>=5 && instruction<=13){
				/*one opperand*/
			}else{
				/*no opperands*/
			}
		}	

	}
	while(ic--) printf("\n%d is : %u\n",ic,code[ic].bits);
	return 0;
}
