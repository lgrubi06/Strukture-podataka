//1. Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime, prezime, apsolutni i relativni broj bodova. Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju. relatvan_br_bodova = br_bodova/max_br_bodova*100


#include <stdio.h>
#include <stdlib.h>
#define LINE 20

typedef struct{
	char name[LINE];
	char lastName[LINE];
	int bodovi;
}stud;

int main(){

int brojac=0;
int i, max;

char buffer[50]={0}; //privremeni spremnik za redak

FILE *fp;
fp=fopen("popis.txt", "r");

//ako datoteka ne postoji ili se nije uspjela otvorit prekini program i ispisi gresku
if(!fp){
	printf("Greška");
	return -1;
}

//broji retke
while(fgets(buffer, 50, fp)!=NULL){
	brojac ++;
}

fclose(fp);

stud *Student;
Student=(stud*)malloc(brojac*sizeof(stud));

//ako malloc nije uspio dobit memoriju NULL i pisemo return -2 da razlikujemo greske
if(!Student){
	printf("Greška");
	return -2;
}

fp=fopen("popis.txt", "r");

for(i=0; i<brojac; i++){
	fscanf(fp, " %s %s %d", Student[i].name, Student[i].lastName, &Student[i].bodovi);
}

fclose(fp);

//trazimo maks da bi mogli izracunat relativni broj bodova
max=Student[0].bodovi;
for(i=1; i<brojac; i++){
	if(Student[i].bodovi > max){
		max=Student[i].bodovi;
	}
}

for(i=0; i<brojac; i++){
	printf("%s %s %d %.2f%%\n", Student[i].name, Student[i].lastName, Student[i].bodovi, (double)Student[i].bodovi/max*100);
}

free(Student);
return 0;
}