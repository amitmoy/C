#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MEM_SIZE 4096
#define DATA_SIZE 1024
#define LINE_LENGTH 80
#define LABEL_LENGTH 30

enum directives {lstring=0,ldata,lextern,lentry,lcode};

enum errors {none=0,collisionError,wrongLabel,wrongData,numberOverflow,wrongOperand};
enum instruction {mov=0,cmp,add,sub,lea,clr,inot,inc,dec,jmp,bne,red,prn,jsr,rts,stop};

enum addressingMethods {imm=0, direct, nonDirectReg, directReg};

enum Boolean {f=0,t};

typedef struct{
	int bits:15;
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
int isDirective(char *);
int addData(char *, Instruction *, int *, int);
void printList(List);
int addExtern(char *);
int whatInstruction(char *);
int readOperand(char *, int *, int *);
void printCode(Instruction);
Node * searchNode(List , char *);
