#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20

typedef struct person* Position;

typedef struct person{
	char ime[MAX];
	char prezime[MAX];
	int godina;
	Position next;
}person;

int insertBegin(Position head);
int printList(Position first);
Position novaOsoba(void);
int insertEnd(Position head);
Position pronadiPrezime(Position first, char* prezime);
int deletePerson(Position head, char* prezime);
int deleteList(Position head);

int main(void){

	person head = { "", "", 0, NULL };

	insertBegin(&head);
	insertEnd(&head);
	printList(head.next);
	deleteList(&head);

	return 0;
}

Position novaOsoba(void){
	Position nova=(Position)malloc(sizeof(person));

	if(!nova)
		return NULL;

	printf("Ime: ");
	scanf("%19s", nova->ime);
	printf("Prezime: ");
	scanf("%19s", nova->prezime);
	printf("Godina rodenja: ");
	scanf("%d", &nova->godina);

	nova->next=NULL;
	return nova;
}

int insertBegin(Position head){
	Position nova=novaOsoba();

	if(!nova)
		return -1;

	nova->next=head->next;
	head->next=nova;

	return 0;
}

int printList(Position current){
	while(current){
		printf("%s %s %d\n", current->ime, current->prezime, current->godina);
		current=current->next;
	}

	return 0;
}

int insertEnd(Position head){
	Position nova=novaOsoba();

	if(!nova)
		return -1;

	while(head->next)
		head=head->next;

	head->next=nova;

	return 0;
}

Position pronadiPrezime(Position first, char* prezime){

	while(first){

		if(strcmp(first->prezime, prezime)==0){
			return first;
		}
		first=first->next;
	}
	return NULL;
}

int deletePerson(Position head, char* prezime){
	Position prev=head;
	Position trenutni=head->next;

	while(trenutni && strcmp(trenutni->prezime, prezime)!=0){
		prev=trenutni;
		trenutni=trenutni->next;
	}
	if(!trenutni)
		return -1;

	prev->next=trenutni->next;

	free(trenutni);

	return 0;
}

int deleteList(Position head){
	Position temp = NULL;

	while(head->next){
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return 0;
}