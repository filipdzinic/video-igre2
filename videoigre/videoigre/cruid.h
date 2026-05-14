#ifndef CRUID_H
#define CRUID_H

#define FILE_NAME "igre.txt"

typedef struct {
	int id;
	char ime[50];
} Igra;

void stvoriIgru();
void citajIgru();
void updateIgru();
void obrisiIgru();

#endif