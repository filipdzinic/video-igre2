#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cruid.h"

void stvoriIgru() {
	FILE* file = fopen(FILE_NAME, "a");

	if (file == NULL) {
		printf("\nERROR U DODAVANJU IGRE: Greska otvaranje datoteke\n");
		return;
	}

	Igra k;

	printf("\nUnesite ID igre: ");
	scanf("%d", &k.id);

	getchar();

	printf("\nUnesite ime igre: ");
	fgets(k.ime, sizeof(k.ime), stdin);

	k.ime[strcspn(k.ime, "\n")] = 0;

	fprintf(file, "%d|%s\n", k.id, k.ime);
	printf(file, "%d %s dodano u datoteku.\n", k.id, k.ime);

	fclose(file);

	printf("Igra uspjesno dodana\n");
}

void citajIgru() {

	FILE* file = fopen(FILE_NAME, "r");

	if (file == NULL) {
		printf("\nREAD ERROR: Datoteka ne postoji, ili je prazna.\n");
		return;
	}

	Igra k;

	printf("\n========== Popis igara ==========\n");

	while (fscanf(file, "%d|%[^\n]\n)", &k.id, k.ime) != EOF) {
		printf("ID: %d | Ime: %s\n", k.id, k.ime);
	}

	fclose(file);
}

void updateIgru() {
	FILE* file = fopen(FILE_NAME, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (file == NULL || temp == NULL) {
		printf("\nUPDATE ERROR: Greska otvaranje datoteke. Temp file ne postoji\n");
		return;
	}

	int trazeniID;
	int pronaden = 0;

	Igra k;

	printf("\nUnesite ID igre za izmjenu: ");
	scanf("%d", &trazeniID);

	while (fscanf(file, "%d %s", &k.id, k.ime) != EOF) {
		if (k.id == trazeniID) {
			pronaden = 1;
			printf("\nUnesite novo ime: ");
			scanf("%s, k.ime");

			printf("\nIgra azurirana\n");
		}

		fprintf(temp, "%d %s\n", k.id, k.ime);
	}

	fclose(file);
	fclose(temp);

	// stvara novi temp, kopira iz stare u temp te mijenja ime tempa
	remove(FILE_NAME);
	rename("temp.txt", FILE_NAME);

	if (!pronaden) {
		printf("\nIgra nije pronadjena\n");
	}
}

void obrisiIgru() {
	FILE* file = fopen(FILE_NAME, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (file == NULL || temp == NULL) {
		printf("\nDELETE ERROR: Greska pri otvaranju datoteke\n");
		return;
	}

	int trazeniID;
	int pronaden = 0;
	
	Igra k;

	printf("\nUnesite ID igre za brisanje: \n");
	scanf("%d", &trazeniID);

	while (fscanf(file, "%d %s", &k.id, k.ime) != EOF) {
		if (k.id == trazeniID) {
			pronaden = 1;
			continue;
		}

		fprintf(temp, "%d %s\n", k.id, k.ime);
	}

	fclose(file);
	fclose(temp);

	// stvara novi temp, kopira iz stare u temp te mijenja ime tempa
	remove(FILE_NAME);
	rename("temp.txt", FILE_NAME);

	if (pronaden) {
		printf("\nIgra obrsiana\n");
	}
	else {
		printf("\nIgra nije pronadjena\n");
	}
}