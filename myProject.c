#include "types.h"

int ic = 0, dc = 0, l, firstMethod, secondMethod, secondIc;
Instruction code[MEM_SIZE-DATA_SIZE];
Instruction data[DATA_SIZE];
char *instructionList[]={"mov ","cmp ","add ","sub ","lea ","clr ","not ","inc ","dec ","jmp ","bne ","red ","prn ","jsr ","rts","stop"};
char *directiveList[]={".string ",".data ",".extern ",".entry "};
List labelTable;
char label[LINE_LENGTH];
enum Boolean labelFlag;
int errorFlag;
int firstMethod, secondMethod, firstVal, secondVal;


int main(int argc, char **argv){
	FILE *fd,*fdw;
	char buffer[LINE_LENGTH] = {0}, filename[20]={0};
	int i,ch,k=0,j;
	int drctv = -1;
	int instruction;
	int pointer = 0;
	
	/*No files error*/
	if(argc == 1){
		printf("No files entered\n");
		exit(0);
	}
	strcpy(filename,*(argv+1));
	
	/*Open File*/
	if(!(fd = fopen(*(argv+1),"r"))){
		printf("File %s load was failed\n", *(argv+1));
		exit(0);
	}
	
	strcpy(filename,*(argv+1));
	/*------------First Read-----------*/
	while(!feof(fd)){
		i = 0;
		l = 0;
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
		if(buffer[0] == '\n'){
			continue;
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
				addNode(&labelTable, label, ic+100, drctv);
				while(buffer[k]!='.') k++;
				k = k + strlen(directiveList[drctv]);
				addData((buffer+k), data, code, &dc, &ic, drctv);
				continue;
			}
		} else {
			drctv = isDirective(buffer);
			if(drctv == lstring || drctv == ldata){
				while(buffer[k]!='.') k++;
				k = k + strlen(directiveList[drctv]);
				addData((buffer+k), data, code, &dc, &ic, drctv);
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
				/*A flag*/
				code[ic+1].bits |= 1<<2;
			}else{
				while(buffer[k] == '\t' || buffer[k] == ' ') k++;
				if(buffer[k]!='\0'){
					errorFlag++;
					printf("\nNo operrands allowed on : %s", buffer);
					continue;
				}
				code[ic].bits |= 1<<2;	
			}
			ic+=l+1;
			
		}
		if(ic>=MEM_SIZE-DATA_SIZE){
			printf("\nMemory overflow error");
			exit(0);
		}	
	}
	
	/*-----Prepare for second read------*/
	if(errorFlag){
		printf("\nCompile Errors detected");
		exit(0);
	}
	
	rewind(fd);
	printList(labelTable);
	secondIc = 0;
	while(!feof(fd)){
		int instruction;
		char label[LABEL_LENGTH];
		l=0;
		k=0;
		i=0;
		int j = 0;
		Node *node;

		/*reading line to the buffer*/
		while((ch = fgetc(fd))!=EOF && ch!='\n'){
			if(i<LINE_LENGTH){
				buffer[i++] = ch;
			}else{
				printf("Line length overflow");
				break;
			}
		}
		
		buffer[i]='\0';
					
		/*checks if there is label*/
		if(isLabel(buffer)){	
			labelFlag = t;
			while(buffer[k++]!=':');
		}else{
			labelFlag = f;
		}
		
		if((drctv = isDirective(buffer+k))==lstring || drctv==ldata){
			l=sizeOfCode(buffer+k, drctv);
			secondIc+=l;
			continue;
		} else if(drctv!=-1) {
			if(addEntry(buffer+k, &labelTable)){
				errorFlag++;
				printf("\nNo such label on : %s", buffer);
				continue;
			}
		}

		if((instruction = whatInstruction(buffer+k))==-1){
			/*--------error-------*/
		} else{
			if(instruction>=0 && instruction<=4){
				while(buffer[k] == ' ' || buffer[k] == '\t') k++;
				k = k + strlen(instructionList[instruction]);
				j=k;
				while(buffer[j]!=',') j++;
				buffer[j]='\0';
				readOperand(buffer+k, &firstMethod, &firstVal);
				readOperand(buffer+j+1, &secondMethod, &secondVal);
				if(firstMethod == direct){
					j=0;
					while(buffer[k] == ' ' || buffer[k] == '\t') k++;
					while(buffer[k]!=',' && buffer[k]!=' ' && buffer[k]!='\t') label[j++] = buffer[k++];
					label[j]='\0';
					if((node=searchNode(&labelTable,label))){
						code[secondIc+1].bits = node->value;
					} else {
						printf("No such label %s", label);
						errorFlag++;
					}
				}
				j=0;
				if(secondMethod == direct){
					while(buffer[k++]!=',');
					while(buffer[k] == ' ' || buffer[k] == '\t') k++;
					while(buffer[k]!='\0' && buffer[k]!=' ' && buffer[k]!='\t') label[j++] = buffer[k++];
					label[j]='\0';
					if((node=searchNode(&labelTable,label))){
						code[secondIc+2].bits = node->value;
					} else {
						printf("No such label in 2nd operrand of: %s",buffer);
						errorFlag++;
					}
				}
				if(firstMethod > 1 && secondMethod > 1){
					l=2;
				} else {
					l=3;
				}
			} else if(instruction<=13){
				while(buffer[k] == ' ' || buffer[k] == '\t') k++;
				k = k + strlen(instructionList[instruction]);
				readOperand(buffer+k, &firstMethod, &firstVal);
				if(firstMethod == direct){
					while(buffer[k] == ' ' || buffer[k] == '\t') k++;
					while(buffer[k]!='\0' && buffer[k]!=' ' && buffer[k]!='\t') label[j++] = buffer[k++];
					label[j]='\0';
					if(!(node=searchNode(&labelTable,label))){
						code[secondIc+1].bits = node->value;
					} else {
						printf("No such label in: %s",buffer);
						errorFlag++;
					}
				}
				l=2;
			} else{
				l=1;
			}
			secondIc+=l;	
		}				
	}

	if(errorFlag){
		printf("\nCompiling error detected");
		exit(0);
	}
	str
	fopen
	while(pointer<secondIc){
	return 0;
}
