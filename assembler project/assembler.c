/*This assembler built by : Amit Moyal, ID 206086365, for course number 20465, learning group number 81, learning center 892. MAMAN 14.
This program is building object, entry and extern files out of assembly language code files (.as), Version 1, 29/3/2020*/
#include "types.h"

int  l;
Instruction code[MEM_SIZE-DATA_SIZE];
Instruction data[DATA_SIZE];
char *instructionList[]={"mov ","cmp ","add ","sub ","lea ","clr ","not ","inc ","dec ","jmp ","bne ","red ","prn ","jsr ","rts","stop"};
char *directiveList[]={".string ",".data ",".extern ",".entry "};
List labelTable, externTable;
char label[LINE_LENGTH];
enum Boolean labelFlag;
int errorFlag;
int firstMethod, secondMethod, firstVal, secondVal;


int main(int argc, char **argv){
	int filenum;
	for(filenum=1; filenum<argc; filenum++){
		FILE *fd;
		char buffer[LINE_LENGTH] = {0}, filename[20]={0}, writeLine[6] ={0};
		int i,ch,k=0,j,ic = 0, dc = 0, secondIc=0;
		int drctv = -1;
		int instruction, entryFlag = 0,externFlag = 0, externCount=0;
		labelTable.head = NULL;
		externTable.head = NULL;
		errorFlag = 0;

		/*No files error*/
		if(argc == 1){
			printf("No files entered\n");
			exit(0);
		}
		strcpy(filename,*(argv+filenum));
		strcat(filename,".as");
		/*Open File*/
		if(!(fd = fopen(filename,"r"))){
			printf("\nFile %s load was failed, only .as files can be compiled", *(argv+filenum));
			exit(0);
		}
	
		strcpy(filename,*(argv+filenum));
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
					while(buffer[j] != ',' && buffer[j]!='\0') j++;
				
					if(buffer[j] == '\0'){
						printf("No Operrands in : %s",buffer);
						errorFlag++;
						continue;
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


						if(firstMethod >1 && secondMethod > 1){
							l=1;
							code[ic+1].bits = firstVal<<7;
							code[ic+1].bits |= secondVal<<3;
							/*A flag*/
							code[ic+1].bits |= 1<<2;
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
		/*adjust the vlaue of data and string labels to ic+100*/
		addVal(&labelTable, ic+100);
		rewind(fd);
		secondIc = 0;
		while(!feof(fd)){
			int instruction, j=0;
			char label[LABEL_LENGTH];
			Node *node;
			l=0;
			k=0;
			i=0;
		

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
		
			if((drctv = isDirective(buffer+k))==lstring || drctv==ldata || drctv==lextern){
				continue;
			} else if(drctv!=-1) {
				entryFlag = 1;
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
							if(node->ltype != lextern){
								code[secondIc+1].bits = (node->value)<<3;
								code[secondIc+1].bits |= 1<<1;
							} else {
								externFlag = 1;
								code[secondIc+1].bits = externCount<<3;
								externCount++;
								code[secondIc+1].bits |= 1;
								addNode(&externTable, node->name, secondIc+1, lextern);
							}
							node=NULL;
						} else {
							printf("\nNo such data or string label %s in 1st operrand of : %s",label,buffer);
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
							if(node->ltype != lextern){
								code[secondIc+2].bits = (node->value)<<3;
								code[secondIc+2].bits |= 1<<1;
							} else {
								externFlag = 1;
								code[secondIc+2].bits = externCount<<3;
								externCount++;
								code[secondIc+2].bits |= 1;
								addNode(&externTable, node->name, secondIc+2, lextern);
							}
							node=NULL;
						} else {
							printf("\nNo such data or string label %s in 2nd operrand of : %s",label,buffer);
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
							if(node->ltype != lextern){
								code[secondIc+1].bits = (node->value)<<3;
								code[secondIc+1].bits |= 1<<1;
							} else {
								externFlag = 1;
								code[secondIc+1].bits = externCount<<3;
								externCount++;
								code[secondIc+1].bits |= 1;
								addNode(&externTable, node->name, secondIc+1, lextern);
							}
							node=NULL;
						} else {
							printf("\nNo such data or string label %s in operrand of : %s",label,buffer);
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

		/*---------------building the ob file--------------*/
		fclose(fd);
		strcpy(filename,*(argv+filenum));
		strcat(filename,".ob");
		if(!(fd = fopen(filename,"w"))){
			printf("\nCreating obj file error, %s", filename);
			exit(0);
		}

		fprintf(fd,"%d %d\n",ic,dc);
		secondIc=0;
		while(secondIc<ic){
			binToOc(code[secondIc],writeLine);
			fprintf(fd,"%d %s\n",secondIc+100,writeLine);
			secondIc++;
		}

		/*----------building the entry file--------------*/
		fclose(fd);
		if(entryFlag){
			Node *ptr = labelTable.head;
			strcpy(filename,*(argv+filenum));
			strcat(filename,".ent");

			if(!(fd = fopen(filename,"w"))){
				printf("\nCreating ent file error, %s", filename);
				exit(0);
			}

			while(ptr){
				if(ptr->ltype == lentry){
					fprintf(fd,"%s %d\n",ptr->name,ptr->value);
				}
				ptr=ptr->next;
			}
			fclose(fd);
		}

		/*------------building the extern file---------*/
	
		if(externFlag){
			Node *ptr = externTable.head;
			strcpy(filename,*(argv+filenum));
			strcat(filename,".ext");

			if(!(fd = fopen(filename,"w"))){
				printf("\nCreating ent file error, %s", filename);
				exit(0);
			}

			while(ptr){
				if(ptr->ltype == lextern){
					fprintf(fd,"%s %d\n",ptr->name,ptr->value+100);
				}
				ptr=ptr->next;
			}
			fclose(fd);
		}
	}
	return 0;
}
