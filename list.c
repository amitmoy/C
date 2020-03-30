#include "types.h"

extern int errorFlag;
extern char *instructionList[];

/*adds the new node if label was not found in list*/
int addNode(List *list, char *str, int ic, int type){
	Node *ptr = list->head;
	Node *prev;
	Node *newNode;

	
	while(ptr){
		if(!(strcmp(str,ptr->name))){
			return collisionError;
		}
		prev=ptr;
		ptr=ptr->next;
	}

	if(!(newNode = (Node *) malloc(sizeof(Node)))){
		printf("Memorry alocating error");
		exit(0);
	}

	strcpy(newNode->name,str);
	newNode->value=ic;
	newNode->ltype=type;

	if(!(list->head)){
		list->head=newNode;
	} else {
		prev->next=newNode;
	}
	
	return 0;
} 

void printList(List list){
	int i=1;
	Node *ptr = list.head;
	if(!(list.head)){
		printf("\nList is empty\n");
	} else {
		while(ptr){
			printf("\nNode #%d label is {%s}, Directive:%d , ic: %d", i++, ptr->name, ptr->ltype, ptr->value);
			ptr = ptr->next;
		}
	}
}	 

Node * searchNode(List *list, char *label){
	Node *ptr = list->head;
	while(ptr){
		if((!strcmp(ptr->name, label)) && (ptr->ltype == lstring || ptr->ltype == ldata)) return ptr;
		ptr = ptr->next;
	}
	return NULL;
}

void printCode(Instruction toPrint){
	unsigned mask = 1<<14;
	int i;
	for(i=0; i<15; i++){
		if(toPrint.bits & (mask)){
			putchar('1');
		} else {
			putchar('0');
		}
		mask = mask >> 1;
	}
	putchar('\n');
}

void addVal(List *list, int val){
	Node *ptr = list->head;
	while(ptr){
		if(ptr->ltype == lstring || ptr->ltype == ldata){
			ptr->value += val;
		}
		ptr = ptr->next;
	}
}
