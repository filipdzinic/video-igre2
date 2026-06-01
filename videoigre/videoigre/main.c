#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "cruid.h"

// KONCEPT 11 Korištenje enum-a za izbornik
enum opcijaIzbornika {
	NOVA_IGRA = 1,
	CITANJE_IGRE,
	UPDATE_IGRE,
	BRISANJE_IGRE,
	ISPIS,
	IZLAZ
};

// KONCEPT 10 izbornik
void menu() {

	printf("\n!!!!!!!!!!!!!!!!!!!!");
	printf("\nDobrodosli u game library!");
	printf("\n====================");
	printf("\n1. Dodaj igru");
	printf("\n2. Prikazi igre");
	printf("\n3. Uredi igre");
	printf("\n4. Obrisi igre");
	printf("\n5. Velicina datoteke");
	printf("\n6. Izlaz");
	printf("\n====================");
	printf("\nOdabir: ");
}

// switch case, sa enumom, ovo je dugacko. sadrzi i koncept 20 fseek i 22 feof/ferror
int main() {
	int izbor;
	FILE* f;

	do {
		menu();
		scanf(" %d", &izbor);
		getchar();

		switch (izbor) {

		case NOVA_IGRA:
			stvoriIgru();
			break;

		case CITANJE_IGRE:
			citajIgru();
			break;

		case UPDATE_IGRE:
			updateIgru();
			break;

		case BRISANJE_IGRE:
			obrisiIgru();
			break;

			// KONCEPT 20. FSEEK SEEK_END FTELL REWIND
			// KONCEPT 22. FEOF/FERROR
		case ISPIS:
			f = fopen("igre.txt", "rb");

			if (f == NULL) {
				perror("Greska pri otvaranju datoteke");
				break;
			}

			fseek(f, 0, SEEK_END);
			printf("Velicina: %ld bajtova\n", ftell(f));

			rewind(f);

			Igra igra;

			// feof/ferror/fread
			while (fread(&igra, sizeof(Igra), 1, f) == 1) {
				printf("%s - %.2f\n", igra.name, igra.price);
			}

			if (feof(f)) {
				printf("\nKraj datoteke\n");
			}

			if (ferror(f)) {
				perror("Greska pri citanju datoteke");
			}

			fclose(f);
			break;

		case IZLAZ:
			printf("\nIzlaz iz programa\n");
			free(popisIgara);
			popisIgara = NULL;
			break;

		default:
			printf("\nNeispravan odabir\n");
		}

	} while (izbor != 5);

	return 0;
}
