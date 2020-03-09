#include "types.h"

extern char *directivesList[];
extern char label[];
extern enum error errorFlag;

/*gets the current line the compiler reads and telling if there is a lable*/
int isLabel(char *str){
	int i = 0;

	while(isalpha(str[i]) || isdigit(str[i])){
		i++;
	}

	if(str[i]==':' && i>0){
		strncpy(label,str,i);
		return 1;
	} else {
		return 0;
	}
}

checks
int isDirective(char *str){
	int k = 0,i;
	while(str[k]==' ') k++;
	
	/*checks the whole directives*/
	for(i=0;i<4;i++){
		if(!(strncmp(str[k],directivesList[i],strlen(directivesList[i])))){
			return i;
		}
	}
	return -1;/*if no directive recognized*/
}
