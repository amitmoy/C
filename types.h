#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum directives {lstring=0,ldata,lextrn,lentry,lcode};

enum errors {none=0,collisionError};

typedef struct{
	char bit:1;
} Boolean;

typedef struct{
	unsigned bits:15;
} Instruction;


typedef struct Node{
	struct Node *next;
	char name[81];
	enum directives ltype;
	int value;
} Node;

typedef struct{
	Node *head;
} List;

int addNode(List *, char *, int, int);
int isLabel(char *);

