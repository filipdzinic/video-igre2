#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cruid.h"

Igra popisIgara[100];
int brojIgara = 0;

int usporediPoId(const void* a, const void* b) {
    Igra* igraA = (Igra*)a;
    Igra* igraB = (Igra*)b;
    return (igraA->id - igraB->id);
}

int usporediPoGodini(const void* a, const void* b) {
	Igra* igraA = (Igra*)a;
	Igra* igraB = (Igra*)b;
	return (igraA->year - igraB->year);
}

int usporediPoCijeni(const void* a, const void* b) {
	Igra* igraA = (Igra*)a;
	Igra* igraB = (Igra*)b;
	return (igraA->price - igraB->price);
}

void stvoriIgru() {
	FILE* file = fopen(FILE_NAME, "a");

	if (file == NULL) {
		printf("\nERROR U DODAVANJU IGRE: Areska otvaranje datoteke\n");
		return;
	}

	Igra k;

	printf("\nUnesite ID igre: ");
	scanf("%d", &k.id);

	getchar(); // Čisti \n iz međuspremnika

	printf("\nUnesite ime igre, poželjno bez razmaka: ");
	fgets(k.name, sizeof(k.name), stdin);
	k.name[strcspn(k.name, "\n")] = 0;

	printf("\nUnesite zanr igre: ");
	fgets(k.genre, sizeof(k.genre), stdin);
	k.genre[strcspn(k.genre, "\n")] = 0;

	printf("\nUnesite godinu izdanja igre: ");
	scanf("%d", &k.year);

	printf("\nUnesite cijenu igre: ");
	scanf("%d", &k.price);

	// Zapis u datoteku
	fprintf(file, "%d %s %s %d %d\n", k.id, k.name, k.genre, k.year, k.price);
	fclose(file);

	// Ispis potvrde na ekran (ispravljen printf!)
	printf("\nIgra %s uspjesno dodana u datoteku.\n", k.name);
}

void citajIgru() {
	FILE* file = fopen(FILE_NAME, "r");

	if (file == NULL) {
		printf("\nREAD ERROR: Datoteka ne postoji, ili je prazna.\n");
		return;
	}

	Igra popisIgara[100];
	int brojIgara = 0;

	while (fscanf(file, "%d %s %s %d %d\n",
		&popisIgara[brojIgara].id,
		popisIgara[brojIgara].name,
		popisIgara[brojIgara].genre,
		&popisIgara[brojIgara].year,
		&popisIgara[brojIgara].price) != EOF) {
		brojIgara++;
		if (brojIgara >= 100) break;
	}

	fclose(file);

	if (brojIgara == 0) {
		printf("\nDatoteka je prazna.\n");
		return;
	}

	int odabir;
	printf("\nKako želite sortirati popis?\n");
	printf("1. Po ID-u\n");
	printf("2. Po godini izdanja\n");
	printf("3. Po cijeni\n");
	printf("Odabir: ");
	scanf("%d", &odabir);

	switch (odabir) {
	case 1:
		qsort(popisIgara, brojIgara, sizeof(Igra), usporediPoId);
		break;
	case 2:
		qsort(popisIgara, brojIgara, sizeof(Igra), usporediPoGodini);
		break;
	case 3:
		qsort(popisIgara, brojIgara, sizeof(Igra), usporediPoCijeni);
		break;
	default:
		printf("\nNeispravan odabir. Popis će biti prikazan nesortiran.\n");
		break;
	}

	printf("\n\n========== Popis igara ==========\n");
	for (int i = 0; i < brojIgara; i++) {
		printf("ID: %d Ime: %s Žanr: %s Godina: %d Cijena: %d\n",
			popisIgara[i].id,
			popisIgara[i].name,
			popisIgara[i].genre,
			popisIgara[i].year,
			popisIgara[i].price);
	}
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

		// cita se format s razmakom (%d %s) ili %d %[^\n] ako ima razmaka u imenu
		if (sscanf(linija, "%d %[^\n]", &trenutniID, trenutnoIme) >= 1) {

			// makni skriveni \r ili \n s kraja imena ako je ostao iz fgets-a
			trenutnoIme[strcspn(trenutnoIme, "\r\n")] = 0;

			if (trenutniID == trazeniID) {
				pronaden = 1;
				printf("Trenutno ime: %s\n", trenutnoIme);
				printf("Unesite novo ime: ");

				scanf("%[^\n]", trenutnoIme);
				while (getchar() != '\n'); // Čistimo buffer

				printf("\nIgra azurirana\n");
			}

			// dodajemo \n na kraj svake linije da se ne spoje
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
		char trenutniGenre[100];
		char trenutniYear[100];
		char trenutniPrice[100];

		// sscanf izvlači podatke iz pročitane linije
		if (sscanf(linija, "%d %s", &trenutniID, trenutnoIme, trenutniGenre, trenutniYear, trenutniPrice) == 2) {
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
