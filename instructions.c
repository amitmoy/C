#include "types.h"

/*gets the current line the compiler reads and telling if there is a lable*/
int islable(char *str){
	int i = 0;

	while(isalpha(str[i]) || isdigit(str[i])){
		i++;
	}

	if(str[i]==':' && i>0)
		return 1;
	else
		return 0;
}
