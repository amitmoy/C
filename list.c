#include "types.h"

extern enum error errorFlag;
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

	 

	
