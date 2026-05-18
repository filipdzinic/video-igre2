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

	fprintf(file, "%d %s\n", k.id, k.ime);
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

	while (fscanf(file, "%d %[^\n]\n)", &k.id, k.ime) != EOF) {
		printf("ID: %d Ime: %s\n", k.id, k.ime);
	}

	fclose(file);
}

void updateIgru() {
	FILE* file = fopen(FILE_NAME, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (file == NULL || temp == NULL) {
		printf("\nUPDATE ERROR: Greska otvaranje datoteke.\n");
		if (file) fclose(file);
		if (temp) fclose(temp);
		return;
	}

	int trazeniID;
	int pronaden = 0;
	char linija[256];

	printf("\nUnesite ID igre za izmjenu: ");
	if (scanf("%d", &trazeniID) != 1) {
		printf("\nNeispravan unos ID-a!\n");
		fclose(file);
		fclose(temp);
		return;
	}

	while (getchar() != '\n'); // Čišćenje buffera

	while (fgets(linija, sizeof(linija), file) != NULL) {
		int trenutniID;
		char trenutnoIme[100];

		// Čitamo format s RAZMAKOM (%d %s) ili %d %[^\n] ako ima razmaka u imenu
		if (sscanf(linija, "%d %[^\n]", &trenutniID, trenutnoIme) >= 1) {

			// Makni skriveni \r ili \n s kraja imena ako je zaostao iz fgets-a
			trenutnoIme[strcspn(trenutnoIme, "\r\n")] = 0;

			if (trenutniID == trazeniID) {
				pronaden = 1;
				printf("Trenutno ime: %s\n", trenutnoIme);
				printf("Unesite novo ime: ");

				scanf("%[^\n]", trenutnoIme);
				while (getchar() != '\n'); // Čistimo buffer

				printf("\nIgra azurirana\n");
			}

			// KLJUČNO: Eksplicitno dodajemo \n na kraj svake linije da se ne spoje!
			fprintf(temp, "%d %s\n", trenutniID, trenutnoIme);
		}
	}

	fclose(file);
	fclose(temp);

	if (pronaden) {
		remove(FILE_NAME);
		rename("temp.txt", FILE_NAME);
	}
	else {
		remove("temp.txt");
		printf("\nIgra nije pronadjena\n");
	}
}

/* void obrisiIgru() {
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
	while (getchar() != '\n');
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
} */

void obrisiIgru() {
	FILE* file = fopen(FILE_NAME, "r");
	FILE* temp = fopen("temp.txt", "w");

	if (file == NULL || temp == NULL) {
		printf("\nDELETE ERROR: Greska pri otvaranju datoteke.\n");
		if (file) fclose(file);
		if (temp) fclose(temp);
		return;
	}

	int trazeniID;
	int pronaden = 0;
	char linija[256];

	printf("\nUnesite ID igre za brisanje: ");
	if (scanf("%d", &trazeniID) != 1) {
		printf("\nNeispravan unos ID-a!\n");
		fclose(file);
		fclose(temp);
		return;
	}

	// očisti buffer nakon scanf-a
	while (getchar() != '\n');

	// citanje datoteke liniju po liniju
	while (fgets(linija, sizeof(linija), file) != NULL) {
		int trenutniID;
		char trenutnoIme[100];

		// sscanf izvlači podatke iz pročitane linije
		if (sscanf(linija, "%d %s", &trenutniID, trenutnoIme) == 2) {
			if (trenutniID == trazeniID) {
				pronaden = 1;
				continue; // Preskoči upisivanje ove linije u temp (brišemo je)
			}
		}

		// Ako linija ne odgovara traženom ID-u, prepiši je u temp datoteku
		fprintf(temp, "%s", linija);
	}

	fclose(file);
	fclose(temp);

	if (pronaden) {
		if (remove(FILE_NAME) != 0) {
			printf("\nERROR: Ne mogu obrisati originalnu datoteku. Provjeri dozvole.\n");
			return;
		}
		if (rename("temp.txt", FILE_NAME) != 0) {
			printf("\nERROR: Ne mogu preimenovati temp.txt u %s.\n", FILE_NAME);
			return;
		}
		printf("\nIgra uspjesno obrisana!\n");
	}
	else {
		remove("temp.txt"); // temp ne treba
		printf("\nIgra s ID-om %d nije pronadjena u datoteci.\n", trazeniID);
	}
}
