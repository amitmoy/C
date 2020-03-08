#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct{
	char bit:1;
} Boolean;

typedef struct{
	unsigned bits:15;
} Instruction;


int islable(char *);

