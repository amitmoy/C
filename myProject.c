#include "types.h"

int ic = 0, dc = 0, l, firstMethod, secondMethod;
Instruction code[MEM_SIZE-DATA_SIZE];
Instruction data[DATA_SIZE];
char *instructionList[]={"mov ","cmp ","add ","sub ","lea ","clr ","not ","inc ","dec ","jmp ","bne ","red ","prn ","jsr ","rts ","stop "};
char *directiveList[]={".string ",".data ",".extern ",".entry "};
List labelTable;
char label[LINE_LENGTH];
enum Boolean labelFlag;
int errorFlag;
int firstMethod, secondMethod, firstVal, secondVal;


int main(int argc, char **argv){
	FILE *fd;
	char buffer[LINE_LENGTH] = {0};
	int i,ch,k=0,j;
	int drctv = -1;
	int instruction;
	int mask = 1;
	
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
		int check;
		i = 0;
		l = 1;
		k = 0;
		j = 0;
		
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
			
			code[ic].bits = instruction<<11;
			while(buffer[k] == ' ' || buffer[k] == '\t') k++;
			k = k + strlen(instructionList[instruction]);
			if(instruction>=0 && instruction<=4){
				j=k;
				l=2;
				printf("\n%s\n",buffer);
				while(buffer[j] != ',' && buffer[j]!='\0') j++;
				
				if(buffer[j] == '\0'){
					/*error no operands*/
				} else {
					buffer[j++] = '\0';

					/*-------Adrressing errors--------*/
					if(readOperand(buffer+k, &firstMethod, &firstVal) || readOperand(buffer+j, &secondMethod, &secondVal)){
						printf("\nWrong Operands on : %s", buffer);
						errorFlag++;
						continue;
					}
					if((secondMethod == imm && instruction != 1) || (firstMethod!=1 && instruction == 4)){
						printf("\nInvalid addressing method on : %s", buffer);
						errorFlag++;
						continue;
					}

					code[ic].bits |= (1<<firstMethod)<<7;  /*build the first info word*/
					code[ic].bits |= (1<<secondMethod)<<3;  /*-----------------------*/
					code[ic].bits |= 1<<2;			/*-------------------------*/

					printCode(code[ic]);

					if(firstMethod >1 && secondMethod > 1){
						l=1;
						code[ic+1].bits = firstVal<<7;
						code[ic+1].bits |= secondVal<<3;
						/*A flag*/
						code[ic+1].bits |= 1<<2;

						printCode(code[ic+1]);

					}else{
						l=2;
						if(firstMethod == imm){
							code[ic+1].bits = firstVal<<3;
						} else if(firstMethod != direct){
							code[ic+1].bits = firstVal<<7;
						} 

						if(secondMethod == imm){
							code[ic+2].bits = secondVal<<3;
						} else if(secondMethod != direct){
							code[ic+2].bits = secondVal<<3;
						}
						/*A flag*/
						code[ic+1].bits |= 1<<2;
						code[ic+2].bits |= 1<<2;

						printCode(code[ic+1]);
						printCode(code[ic+2]);
					}
					
				}
			}else if(instruction>=5 && instruction<=13){
				/*------------Addressing methods error------------*/
				if(readOperand(buffer+k, &secondMethod, &secondVal)){
					printf("\nWrong Operand on : %s", buffer);
					errorFlag++;
					continue;
				}
				if((secondMethod == 0 && instruction != 12) || (secondMethod == 3 && (instruction==9||instruction==10||instruction==13))){
					printf("\nInvalid addressing method on : %s", buffer);
					errorFlag++;
					continue;
				}

				l=1;
				code[ic].bits |= (1<<secondMethod)<<3;
				/*A flag*/
				code[ic].bits |= 1<<2;
				if(secondMethod != direct){
					code[ic+1].bits = secondVal<<3;
				} 
				code[ic+1].bits |= 1<<2;
			}else{
				/*no opperands*/
			}
		}	

	}
	while(ic--) printf("\n%d is : %u\n",ic,code[ic].bits);
	return 0;
}
