#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

typedef struct Term{
	int coef;
	int exp;
	struct Term *next;
}Term;

void insertTerm(Term *head, int coef, int exp);
int readPolynomial(FILE *file, Term *head);
void addPolynomials(Term *result, const Term *a, const Term *b);
void multiplyPolynomials(Term *result, const Term *a, const Term *b);
void printPolynomial(const Term *poly);
void freePolynomial(Term *head);
int loadPolynomials(const char *fileName, Term *head1, Term *head2);
void printResults(const Term *poly1, const Term *poly2, const Term *sum, const Term *product);

int main(int argc, char *argv[])
{
    const char *fileName = (argc > 1) ? argv[1] : "polynomials.txt";

    Term poly1Head = { 0, 0, NULL };
    Term poly2Head = { 0, 0, NULL };
    Term sumHead = { 0, 0, NULL };
    Term productHead = { 0, 0, NULL };

    if (loadPolynomials(fileName, &poly1Head, &poly2Head) != 0) {
        freePolynomial(&poly1Head);
        freePolynomial(&poly2Head);
        return EXIT_FAILURE;
    }

    addPolynomials(&sumHead, poly1Head.next, poly2Head.next);
    multiplyPolynomials(&productHead, poly1Head.next, poly2Head.next);

    printResults(poly1Head.next, poly2Head.next, sumHead.next, productHead.next);

    freePolynomial(&poly1Head);
    freePolynomial(&poly2Head);
    freePolynomial(&sumHead);
    freePolynomial(&productHead);
    return EXIT_SUCCESS;
}

void insertTerm(Term *head, int coef, int exp){

	if(coef==0)
		return;

	Term *prev=head;
	while(prev->next!=NULL && prev->next->exp>exp)
		prev=prev->next;

	if(prev->next!=NULL && prev->next->exp==exp){

		prev->next->coef+=coef;

		if(prev->next->coef==0){

			Term *temp=prev->next;
			prev->next=temp->next;

			free(temp);
		}

		return;

	}

	Term *newTerm=malloc(sizeof(Term));
	if(newTerm==NULL){
		fprintf(stderr, "Greška, nedovoljno memorije.\n");
		exit(EXIT_FAILURE);
	}

	newTerm->coef=coef;
	newTerm->exp=exp;
	newTerm->next=prev->next;
	prev->next=newTerm;

}

int readPolynomial(FILE *file, Term *head){
	char line[MAX_LINE];

	do{
		if(fgets(line, sizeof line, file)==NULL)
			return 0;
	}while(strspn(line, " \t\r\n")==strlen(line));

	char *cursor=line;
	int coef, exp, charsRead;

	while(sscanf(cursor, "%d%*[x^ ]%d%n", &coef, &exp, &charsRead)==2){
		insertTerm(head, coef, exp);
		cursor+=charsRead;
	}
	return 1;
}

void addPolynomials(Term *result, const Term *a, const Term *b){
	for(; a!=NULL; a=a->next)
		insertTerm(result, a->coef, a->exp);
	for(; b!=NULL; b=b->next)
		insertTerm(result, b->coef, b->exp);
}

void multiplyPolynomials(Term *result, const Term *a, const Term *b){
	for(const Term *p=a; p!=NULL; p=p->next)
		for(const Term *q=b; q!=NULL; q=q->next)
			insertTerm(result, p->coef * q->coef, p->exp+q->exp);
}

void printPolynomial(const Term *poly){
	if(poly==NULL){
		printf("0\n");
		return;
	}

	for(int first=1; poly!=NULL; poly=poly->next, first=0){

		int coef=poly->coef;

		if(first)
			printf("%s", coef<0? "-" : "");
		else
			printf("%c", coef<0? '-' : '+');

		int absCoef=coef<0? -coef:coef;

		if(poly->exp==0)
			printf("%d", absCoef);
		else{
			if(absCoef!=1)
				printf("%d", absCoef);
			printf("x");
			if(poly->exp!=1)
				printf("^%d", poly->exp);
		}
	}
	printf("\n");
}

void freePolynomial(Term *head){
	while (head->next!=NULL){
		Term *temp=head->next;
		head->next=temp->next;
		free(temp);
	}
}

int loadPolynomials(const char *fileName, Term *head1, Term *head2)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Greška: ne mogu otvoriti datoteku '%s'.\n", fileName);
        return -1;
    }

    if (!readPolynomial(file, head1) || !readPolynomial(file, head2)) {
        fprintf(stderr, "Greška: datoteka mora sadržavati dva polinoma (dva retka).\n");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

void printResults(const Term *poly1, const Term *poly2, const Term *sum, const Term *product)
{
    printf("P1(x)        = "); printPolynomial(poly1);
    printf("P2(x)        = "); printPolynomial(poly2);
    printf("P1(x)+P2(x)  = "); printPolynomial(sum);
    printf("P1(x)*P2(x)  = "); printPolynomial(product);
}