#include "types.h"
enum readingStatus {pre, read, post, minus, backslash};
enum readOperandStatus {preop, prenum, num, readlabel, reg, postop, prereg, regread, postreg};
extern char *directiveList[];
extern char label[];
extern char *instructionList[];
extern List labelTable;

/*gets the current line the compiler reads and telling if there is a lable*/
int isLabel(char *str){
	int i = 0, k;
	char labelBuffer[LINE_LENGTH] = {0};

	while(isalpha(str[i]) || isdigit(str[i])){
		i++;
	}

	strncpy(labelBuffer,str,i);	

	/*checks if the label not named like instruction*/
	for(k=0;k<16;k++){
		if(!(strncmp(labelBuffer, instructionList[k], strlen(instructionList[k]))))
			return 0;
		
	}
	
	if(str[i]==':' && i>0){
		strcpy(label,labelBuffer);
		return 1;
	} else {
		return 0;
	}
}

/*returns what directive is detected*/
int isDirective(char *str){
	int k = 0,i;

	while(str[k]==' '||str[k]=='\t') k++;

	/*checks the whole directives*/
	for(i=0;i<4;i++){
		if(!(strncmp((str + k),directiveList[i],strlen(directiveList[i])))){
			return i;
		}
	}
	return -1;/*if no directive recognized*/
}

/*this function gets the line, the data array, data counter and the directive and adds the data to data array*/
int addData(char *str, Instruction *data, int *DC, int drctv){
	char num[LINE_LENGTH-5] = {0};
	int k=0,i=0,val,origindc = *DC;
	enum readingStatus status = pre;
	if(drctv == ldata){
		while(1){
			/*reading chars and decide what to do with each state*/
			switch(status){
				/*ready to start read numbers meanwhile can read whitechars*/
				/*if digit was readed then store it and state become read, if read minus state become minus and store it aswell*/
				case pre:
					if(isdigit(str[i])){
						num[k++] = str[i++];
						status = read;
					} else if(str[i]=='-') {	
						num[k++] = str[i++];
						status = minus;
					} else if(str[i]==' ' || str[i]=='\t'){
						i++;
					} else {
						*DC = origindc;
						return wrongData;
					}
					break;
				/*reading number state, can read digits and stores it, when white char appear adds the number to data array and state become post*/
				/*if ',' appear store the number in data array and state become pre, if '/0' then return 0 which means no error*/
				case read:
					if(isdigit(str[i])){
						num[k++] = str[i++];
					} else if(str[i] == ' ' || str[i] == '\t'){
						if((val = atoi(num))<32767 && val> -32768){
							data[*DC].bits = val;
							*DC = *DC + 1;
							memset(num, 0, k * sizeof (int));
							k = 0;
						} else {
							*DC = origindc;
							return numberOverflow;
						} 
						status = post;
						i++;
					} else if(str[i] == ',') {
						if((val = atoi(num))<32767 && val> -32768){
							data[*DC].bits = val;
							*DC = *DC + 1;
							memset(num, 0, k * sizeof (int));
							k = 0;
						} else {
							*DC = origindc;
							return numberOverflow;
						} 
						status = pre;
						i++;
					} else if(str[i] == '\0') {
						if((val = atoi(num))<32767 && val> -32768){
							data[*DC].bits = val;
							*DC = *DC + 1;
							memset(num, 0, k * sizeof (int));
							k = 0;
						} else {
							*DC = origindc;
							return numberOverflow;
						} 
						return none;
					} else {
						*DC = origindc;
						return wrongData;
					}
					break;
				/*post state, can read ',' and then state become pre, otherwise if whitechar keep read*/
				case post:
					if(str[i] == ','){
						status = pre;
						i++;
					} else if (str[i] == ' ' || str[i] == '\t') {
						i++;
					} else {
						*DC = origindc;
						return wrongData;
					}
					break;
				/*minus state can read only digits*/
				case minus:
					if(!(isdigit(str[i]))){
						*DC = origindc;
						return wrongData;
					} else {
						num[k++] = str[i++];
						status = read;
					}
					break;
			}
		}
	} else if(drctv == lstring) {
		while(1){
			switch(status){
				/*pre status waiting for " to start read the string*/
				case pre:
					if(str[i] == ' ' || str[i] == '\t'){
						i++;
					} else if(str[i] == '\"'){
						status = read;
						i++;
					} else {
						*DC = origindc;
						return wrongData;
					}
					break;
				/*read status, reading the string, insert each char to data array, change status if '\' or '"' appear*/
				case read:
					if(str[i] == '\\'){
						status = backslash;
						i++;
					} else if(str[i] == '"'){
						data[*DC].bits = '\0';
						*DC = *DC + 1;
						status = post;
						i++;
					} else {
						data[*DC].bits = str[i];
						*DC = *DC + 1;
						i++;
					}
					break;
				/*post status can read only whitechars*/
				case post:
					if(str[i]== ' ' || str[i] == '\t'){
						i++;
					} else if(str[i] == '\0'){
						return none;
					} else {
						*DC = origindc;
						return wrongData;
					}
					break;
				/*backslash status, if another \ appear then adds one \ if '"' appear then adds ", else adds the  \ and the char that appeared*/
				case backslash:
					if(str[i] == '"'){
						data[*DC].bits = str[i];
						*DC = *DC + 1;
						status = read;
						i++;
					} else if(str[i] == '\\') { 
						data[*DC].bits = '\\';
						*DC = *DC + 1;
						status = read;
						i++;
					} else {
						data[*DC].bits = '\\';
						*DC = *DC + 1;
						data[*DC].bits = str[i];
						*DC = *DC + 1;
						status = read;
						i++;
					}
			}/*end of switch*/
		}/*end of while*/
	}
	return -1;
}

/*adds the extern label*/
int addExtern(char *str){
	int i=0,k=0;
	enum readingStatus status = pre;
	char exlabel[LINE_LENGTH] = {0};
	while(str[i]!='.') i++;
	i=i + strlen(".extern");
	printf("addex start %d, ,%s,",i,str+i);
	while(1){
		switch(status){
			case pre:
				if(str[i] == ' ' || str[i] == '\t'){
					i++;
				} else if(isalpha(str[i]) || isdigit(str[i])){
					exlabel[k++] = str[i++];
					status = read;
				} else {
					return wrongLabel;
				}
				break;
			case read:
				if(isalpha(str[i]) || isdigit(str[i])){
					exlabel[k++] = str[i++];
				} else if(str[i] == ' ' || str[i] == '\t'){
					exlabel[k] = '\0';
					status = post;
					i++;
				} else if(str[i]=='\0'){
					exlabel[k] = '\0';
					addNode(&labelTable, exlabel, -1, lextern);
					return none;
				} else {
					return wrongLabel;
				}
				break;
			case post:
				if(str[i] == ' ' || str[i] == '\t'){
					i++;
				} else if(str[i] == '\0'){
					addNode(&labelTable, exlabel, -1, lextern);
					return none;
				} else {
					return wrongLabel;
				}
		}
	}
	return none;
}

/*returns the instruction code of the instruction*/
int whatInstruction(char *str){
	int i=0,k;

	while(str[i] == ' ' || str[i] == '\t') i++;
	
	for(k=0; k<16; k++){
		if(!strncmp(str+i,instructionList[k], strlen(instructionList[k])))
			return k;
	}
	
	return -1;
}


int readOperand(char *str, int *method, int *value){
	enum readOperandStatus status = pre;
	char val[LINE_LENGTH] = {0};
	int i=0,k=0;
	enum Boolean nonDirect = f;
	while(1){
		switch(status){
			case preop:
				if(str[i] == ' ' || str[i] == '\t'){
					i++;
				} else if(str[i] == '#'){
					i++;
					status = prenum;
				} else if(str[i] == 'r') {
					i++;
					status = regread;
				} else if(isalpha(str[i])){
					val[k++] = str[i++];
					status = readlabel;
				} else if(str[i] == '*') {
					nonDirect = t;
					i++;
					status = prereg;
				} else {
					return wrongOperand;
				}
				break;
			case prenum:
				if(str[i] == '-' || isdigit(str[i])){
					val[k++] = str[i++];
					status = num;
				} else if(str[i] == '+'){
					i++;
					status = num;
				} else {
					return wrongOperand;
				}
				break;
			case num:
				if(isdigit(str[i])){
					val[k++] = str[i++];
				} else if(str[i] == '\t' || str[i] == ' '){
					if(str[i-1] == '-') return wrongOperand;
					i++;
					val[k] = '\0';
					*value = atoi(val);
					*method = imm;
					status = postop;
				} else if(str[i] == '\0'){
					if(str[i-1] == '-') return wrongOperand;
					val[k] = '\0';
					*value = atoi(val);
					*method = imm;
					return none;
				}else{
					return wrongOperand;
				}
				break;
			case prereg:
				if(str[i] == 'r'){
					val[k++] = str[i++];
					status = regread;
				} else {
					return wrongOperand;
				}
				break;
			case regread:
				if(isdigit(str[i]) && (str[i]-'0')>=0 && (str[i]-'0')<=7){
					*value = str[i] - '0';
					if(nonDirect == t){
						*method = nonDirectReg;
					} else {
						*method = directReg;
					}
					val[k++] = str[i++];
					status = postreg;
				}else if(nonDirect==f && (isalpha(str[i]) || isdigit(str[i]))){
					val[k++] = str[i++];
					status = readlabel;
				} else if(str[i]=='\0') {
					if(nonDirect == t){
						return wrongOperand;
					} else {
						*method = direct;
						return none;
					}
				}else{
					return wrongOperand;
				}
				break;
			case postop:
				if(str[i] == '\0'){
					return none;
				} else if(str[i] == '\t' || str[i] == ' ') {
					i++;
				} else {
					return wrongOperand;
				}
				break;
			case readlabel:
				if(str[i] == ' ' || str[i] == '\t'){
					i++;
					status = postop;
					*method = direct;
				} else if(str[i] == '\0') {
 					*method = direct;
					return none;
				} else if(isdigit(str[i]) || isalpha(str[i])){
					val[k++] = str[i++];
				} else {
				}
				break;
			case postreg:
				if(str[i] == ' ' || str[i] == '\t'){
					i++;
					status = postop;
				} else if(nonDirect==f && (isalpha(str[i]) || isdigit(str[i]))){
					val[k++] = str[i++];
					status = readlabel;
				} else if(str[i] == '\0') {
					return none;
				} else {
					return wrongOperand;
				}
		}
	}
	return -1;
}
			
