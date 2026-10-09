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

char buffer[50]={0};

FILE *fp;
fp=fopen("popis.txt", "r");

if(!fp){
	printf("Greška");
	return -1;
}

while(fgets(buffer, 50, fp)!=NULL){
	brojac ++;
}

fclose(fp);

stud *Student;
Student=(stud*)malloc(brojac*sizeof(stud));

if(!Student){
	printf("Greška");
	return -2;
}

fp=fopen("popis.txt", "r");

for(i=0; i<brojac; i++){
	fscanf(fp, " %s %s %d", Student[i].name, Student[i].lastName, &Student[i].bodovi);
}

fclose(fp);

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