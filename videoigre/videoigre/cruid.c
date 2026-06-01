#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cruid.h"

// dinamicki zauzeto, potrebno za kon.17 u citajIgru
Igra* popisIgara = NULL;
int brojIgara = 0;
int kapacitet = 0;

// sljedece 3 funkcije "usporedi" su za sortiranje. pretvaranje pokazivaca za qsort
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

// za koncept 25 rekurziva
float rekurzivnaUkupnaCijena(const Igra* igre, int indeks) {
	if (igre == NULL || indeks < 0) {
		return 0.0f; //bazni slucaj
	}

	return igre[indeks].price +
		rekurzivnaUkupnaCijena(igre, indeks - 1);
}

void stvoriIgru() {
	FILE* file = fopen(FILE_NAME, "a");

	if (file == NULL) {
		printf("\nERROR U DODAVANJU IGRE: Greska otvaranje datoteke\n");
		return;
	}

	Igra k;

	printf("\nUnesite ID igre: ");
	scanf("%d", &k.id);

	getchar(); // Čisti \n iz međuspremnika

	printf("\nUnesite ime igre, bez razmaka: ");
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

	// ispis
	printf("\nIgra %s uspjesno dodana u datoteku.\n", k.name);
}

// KONCEPT 17 malloc calloc free, KONCEPT 23, 24 qsort bsearch, KONCEPT 25 rekurziva
void citajIgru() {

	FILE* file = fopen(FILE_NAME, "r");

	if (file == NULL) {
		printf("\nREAD ERROR: Datoteka ne postoji, ili je prazna.\n");
		return;
	}

	// reset
	brojIgara = 0;
	kapacitet = 2;

	popisIgara = malloc(kapacitet * sizeof(Igra));

	if (popisIgara == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		fclose(file);
		return;
	}

	Igra temp;

	while (fscanf(file, "%d %s %s %d %d",
		&temp.id,
		temp.name,
		temp.genre,
		&temp.year,
		&temp.price) != EOF) {

		if (brojIgara >= kapacitet) {
			kapacitet += 2;

			Igra* novi = realloc(popisIgara,
				kapacitet * sizeof(Igra));

			if (novi == NULL) {
				printf("Greska pri realloc!\n");
				free(popisIgara);
				fclose(file);
				return;
			}

			popisIgara = novi;
		}

		popisIgara[brojIgara] = temp;
		brojIgara++;
	}

	fclose(file);

	if (brojIgara == 0) {
		printf("\nDatoteka je prazna.\n");
		return;
	}

	// ---------------- UI ----------------
	int odabir;

	printf("\nKako zelite sortirati popis?\n");
	printf("1. Po ID-u\n");
	printf("2. Po godini izdanja\n");
	printf("3. Po cijeni\n");
	printf("4. Trazi pojedinacan ID\n");
	printf("5. Sveukupna cijena\n");
	printf("Odabir: ");
	scanf("%d", &odabir);

	switch (odabir) {
		// KONCEPT 23. QSORT
	case 1:
		qsort(popisIgara, brojIgara, sizeof(Igra), usporediPoId);
		break;

	case 2:
		qsort(popisIgara, brojIgara, sizeof(Igra), usporediPoGodini);
		break;

	case 3:
		qsort(popisIgara, brojIgara, sizeof(Igra), usporediPoCijeni);
		break;

		// KONECPT 24. BSEARCH
	case 4: {
		int trazeniId;

		printf("Unesite ID igre: ");
		scanf("%d", &trazeniId);

		qsort(popisIgara, brojIgara, sizeof(Igra), usporediPoId);

		Igra kljuc;
		kljuc.id = trazeniId;

		Igra* rezultat = bsearch(
			&kljuc,
			popisIgara,
			brojIgara,
			sizeof(Igra),
			usporediPoId
		);

		if (rezultat)
			printf("Pronadena igra: %s\n", rezultat->name);
		else
			printf("Igra nije pronadena.\n");

		break;
	}
		  // KONCEPT 25, REKURZIVA
	case 5:
		printf("\n[REKURZIVNI REZULTAT] Ukupna cijena svih igara: %.2f\n",
			rekurzivnaUkupnaCijena(popisIgara, brojIgara - 1));
		break;

	default:
		printf("\nNeispravan odabir. Popis ce biti nesortiran.\n");
		break;
	}

	// ---------------- ISPIS ----------------
	printf("\n\n========== Popis igara ==========\n");

	for (int i = 0; i < brojIgara; i++) {
		printf("ID: %d Ime: %s Zanr: %s Godina: %d Cijena: %d\n",
			popisIgara[i].id,
			popisIgara[i].name,
			popisIgara[i].genre,
			popisIgara[i].year,
			popisIgara[i].price);
	}
}

// sadrzi KONCEPT 21 rename remove
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

	while (getchar() != '\n'); // ciscenje buffera

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
				while (getchar() != '\n'); // ciscenje buffera

				printf("\nIgra azurirana\n");
			}

			// \n na kraj svake linije da se ne spoje
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

// isto KON.21
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

	// očisti buffer
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
				continue; // preskace se upisivanje ove linije u temp (brišemo je)
			}
		}

		// ako linija ne odgovara trazenom ID-u, prepiši je u temp datoteku
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
