#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum labelType {lcode,ldata,lextrn,lentry};

enum errors {collisionError=1};

typedef struct{
	char bit:1;
} Boolean;

typedef struct{
	unsigned bits:15;
} Instruction;


typedef struct Node{
	struct Node *next;
	char name[81];
	enum labelType ltype;
	int value;
} Node;

typedef struct{
	Node *head;
} List;

int addNode(List *, char *, int, int);
int islable(char *);

