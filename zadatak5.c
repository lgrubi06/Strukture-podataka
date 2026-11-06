#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define SUCCESS 0
#define EMPTY_STACK 1
#define INVALID_INPUT 2
#define MALLOC_ERROR -1
#define FILE_OPEN_ERROR -2
#define SCANF_ERROR -3    // <--- DODANO: Definicija SCANF_ERROR

typedef struct stackItem{
	float value;
	struct stackItem* next;
}StackItem;

typedef struct{
	StackItem *top;
}Stack;

int push(Stack* stack, float x);
int pop(Stack* stack, float* x);
int evaluatePostfixFromFile(const char* filename, float* result);

int main(){
	float result = 0;
	int status = evaluatePostfixFromFile("postfix.txt", &result);

	if(status != SUCCESS){
		return status;
	}

	// ISPRAVLJENO: umjesto %d stavljamo %g jer je result tipa float
	printf("Rezultat: %g\n", result);

	return SUCCESS;
}

int push(Stack* stack, float x){
	StackItem* item = NULL;
	item = (StackItem*)malloc(sizeof(StackItem));

	if(item == NULL){
		printf("Malloc error!\n");
		return MALLOC_ERROR;
	}

	item->value = x;
	item->next = stack->top;
	stack->top = item;

	return SUCCESS; // <--- ISPRAVLJENO: vraæen ispravan naziv makroa
}

int pop(Stack* stack, float* x){
	StackItem* top;
	top = stack->top;

	if(top == NULL){
		printf("Prazan stog!\n");
		return EMPTY_STACK;
	}

	*x = top->value;
	stack->top = top->next;
	free(top);

	return SUCCESS;
}

int evaluatePostfixFromFile(const char* filename, float* result){
	int size = 0;
	float firstOperand = 0, secondOperand = 0, parsedValue = 0; // (ispravljen naziv firtOperand u firstOperand)
	char currItem[10] = { 0 };
	char postfix[MAX_LINE] = { 0 };
	char* postfixPointer = postfix;
	FILE* filePointer = NULL;
	Stack stack = { .top = NULL };

	filePointer = fopen(filename, "r");

	if(filePointer == NULL){
		printf("Datoteka se ne moze otvoriti.\n");
		return FILE_OPEN_ERROR;
	}

	fgets(postfix, MAX_LINE, filePointer);
	fclose(filePointer);

	printf("Postfix: %s", postfix);
	
	while(*postfixPointer != '\0'){

		while(*postfixPointer == ' ' || *postfixPointer == '\t' || *postfixPointer == '\n' || *postfixPointer == '\r'){
			postfixPointer++;
		}

		if(*postfixPointer == '\0'){
			break;
		}

		if(sscanf(postfixPointer, "%s %n", currItem, &size) != 1){
			printf("sscanf error.\n");
			return SCANF_ERROR;
		}

		if(currItem[0] >= '0' && currItem[0] <= '9'){
			sscanf(currItem, "%f", &parsedValue);
			if(push(&stack, parsedValue)) return MALLOC_ERROR;
		}

		else if(currItem[0] == '+' || currItem[0] == '-' || currItem[0] == '*' || currItem[0] == '/'){
			if(pop(&stack, &secondOperand) || pop(&stack, &firstOperand)){
				printf("Pokusaj citanja iz praznog stoga.\n");
				return EMPTY_STACK;
			}
			if(currItem[0] == '+'){
				if(push(&stack, firstOperand + secondOperand)) return MALLOC_ERROR;
			}
			else if(currItem[0] == '-'){
				if(push(&stack, firstOperand - secondOperand)) return MALLOC_ERROR;
			}
			else if(currItem[0] == '*'){
				if(push(&stack, firstOperand * secondOperand)) return MALLOC_ERROR;
			}
			else{
				if(push(&stack, firstOperand / secondOperand)) return MALLOC_ERROR;
			}
		}
		else{
			printf("Nevaljan znak u izrazu.\n");
			return INVALID_INPUT;
		}
		postfixPointer += size;
	}
	
	if(pop(&stack, result)) return EMPTY_STACK;

	return SUCCESS;
}