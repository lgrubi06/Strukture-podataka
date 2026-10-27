#define _CRT_SECURE_NO_WARNINGS

/*
Prethodnom zadatku dodati funkcije:
A. dinamicki dodaje novi element iza odredenog elementa,
B. dinamicki dodaje novi element ispred odredenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. cita listu iz datoteke.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20
#define MALLOC_FAILED (-1)
#define FILE_NOT_OPENED (-2)

typedef struct person* Position;

typedef struct person{
	char name[MAX];
	char lastName[MAX];
	int birthYear;
	Position next;
}person;

Position newPerson();
Position findLastName(Position first, char* lastName);

int insertBegin(Position head);
int printList(Position first);
int deleteList(Position head);

int insertAfter(Position head);
int insertBefore(Position head);
int sortList(Position head);
int writeToFile(Position first, char* nameOfFile);
int readFromFile(Position head, char* nameOfFile);

int main(){

person head={"", "", 0, NULL};

insertBegin(&head);
insertAfter(&head);
insertBefore(&head);
sortList(&head);
printList(head.next);
writeToFile(head.next, "popis.txt");
readFromFile(&head, "popis.txt");
printList(head.next);
deleteList(&head);

return 0;
}

Position newPerson(){
	Position newNode=NULL;

	newNode=(Position)malloc(sizeof(person));
	if(!newNode){
		printf("greska pri alokaciji memorije!\n");
		return NULL;
	}

	printf("Ime: ");
	scanf("%19s", newNode->name);
	printf("Prezime: ");
	scanf("%19s", newNode->lastName);
	printf("Godina rodenja: ");
	scanf("%d", &newNode->birthYear);

	newNode->next=NULL;
	return newNode;
}

Position findLastName(Position first, char* lastName){
	Position current=first;

	while(current){
		if(strcmp(current->lastName, lastName)==0)
			return current;
		current=current->next;
	}
	return NULL;
}

int insertBegin(Position head){
	Position newNode=NULL;

	printf("\nUnos osobe na pocetak liste:\n");
	newNode=newPerson();
	if(!newNode)
		return MALLOC_FAILED;

	newNode->next=head->next;
	head->next=newNode;

	return 0;
}

int printList(Position first){
	Position current=first;
	int counter=0;

	printf("\nLista: ");
	if(!current)
		printf("(prazna)\n");
	printf("\n");

	while(current){
		printf("%s %s %d\n", current->name, current->lastName, current->birthYear);
		counter++;
		current=current->next;
	}
	return counter;
}

int deleteList(Position head){
	Position temp=NULL;

	while(head->next){
		temp=head->next;
		head->next=temp->next;
		free(temp);
	}
	return 0;
}

int insertAfter(Position head){
	char lastName[MAX]="";
	Position target=NULL;
	Position newNode=NULL;

	printf("\nPrezime osobe iza koje se dodaje: ");
	scanf("%19s", lastName);

	target=findLastName(head->next, lastName);
	if(!target){
		printf("Osoba '%s' nije pronadena.\n", lastName);
		return 1;
	}

	newNode=newPerson();
	if(!newNode)
		return MALLOC_FAILED;

	newNode->next=target->next;
	target->next=newNode;

	return 0;
}

int insertBefore(Position head){
	char lastName[MAX]="";
	Position previous=head;
	Position newNode=NULL;

	printf("\nPrezime osobe ispred koje se dodaje: ");
	scanf("%19s", lastName);

	while(previous->next && strcmp(previous->next->lastName, lastName)!=0)
		previous=previous->next;

	if(!previous->next){
		printf("Osoba '%s' nije pronadena.\n", lastName);
		return 1;
	}

	newNode=newPerson();
	if(!newNode)
		return MALLOC_FAILED;

	newNode->next=previous->next;
	previous->next=newNode;

	return 0;
}

static int compareByLastName(Position a, Position b){
	int result=strcmp(a->lastName, b->lastName);

	if(result!=0)
		return result;

	return strcmp(a->name, b->name);
}

int sortList(Position head){
	Position end=NULL;
	Position previous=NULL;
	Position current=NULL;
	Position temp=NULL;

	while(head->next!=end){
		previous=head;
		current=head->next;

		while(current->next!=end){
			if(compareByLastName(current, current->next)>0){
				temp=current->next;
				previous->next=temp;
				current->next=temp->next;
				temp->next=current;
				current=temp;
			}
			previous=current;
			current=current->next;
		}
		end=current;
	}
	return 0;
}

int writeToFile(Position first, char* nameOfFile){
	FILE* filePointer=NULL;
	Position current=first;

	filePointer=fopen(nameOfFile, "w");
	if(!filePointer){
		printf("Ne mogu otvoriti datoteku '%s' za pisanje.\n", nameOfFile);
		return FILE_NOT_OPENED;
	}

	while(current){
		fprintf(filePointer, "%s %s %d\n", current->name, current->lastName, current->birthYear);
		current=current->next;
	}

	fclose(filePointer);

	return 0;
}

int readFromFile(Position head, char* nameOfFile){
	FILE* filePointer=NULL;
	Position newNode=NULL;
	Position last=head;
	int counter=0;

	filePointer=fopen(nameOfFile, "r");
	if(!filePointer){
		printf("Ne mogu otvoriti datoteku '%s' za citanje.\n", nameOfFile);
		return FILE_NOT_OPENED;
	}

	deleteList(head);

	while(1){
		newNode=(Position)malloc(sizeof(person));
		if(!newNode){
			fclose(filePointer);
			return MALLOC_FAILED;
		}

		if(fscanf(filePointer, "%19s %19s %d", newNode->name, newNode->lastName, &newNode->birthYear)!=3){
			free(newNode);
			break;
		}

		newNode->next=NULL;
		last->next=newNode;
		last=newNode;
		counter++;
	}

	fclose(filePointer);

	return counter;
}