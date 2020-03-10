#include "types.h"

extern char *directiveList[];
extern char label[];
extern char *instructionList[];

/*gets the current line the compiler reads and telling if there is a lable*/
int isLabel(char *str){
	int i = 0, k;

	while(isalpha(str[i]) || isdigit(str[i])){
		i++;
	}

	/*checks if the label not named like instruction*/
	for(k=0;k<16;k++){
		if(!(strncmp(str,(instructionList[k]),i)))
			return wrongLabel;
	}
	
	if(str[i]==':' && i>0){
		strncpy(label,str,i);
		return 1;
	} else {
		return 0;
	}
}

/*returns what directive is detected*/
int isDirective(char *str){
	int k = 0,i;
	while(str[k]==' ') k++;

	/*checks the whole directives*/
	for(i=0;i<4;i++){
		if(!(strncmp((str + k),directiveList[i],strlen(directiveList[i])))){
			return i;
		}
	}
	return -1;/*if no directive recognized*/
}
