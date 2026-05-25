#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "cruid.h"

// OBAVEZAN KONCEPT 11: Korištenje enum-a za izbornik
enum opcijaIzbornika {
	NOVA_IGRA = 1,
	CITANJE_IGRE,
	UPDATE_IGRE,
	BRISANJE_IGRE,
	IZLAZ
};

// izbornik
void menu() {

	printf("\n!!!!!!!!!!!!!!!!!!!!");
	printf("\nDobrodosli u game library!");
	printf("\n====================");
	printf("\n1. Dodaj igru");
	printf("\n2. Prikazi igre");
	printf("\n3. Uredi igre");
	printf("\n4. Obrisi igre");
	printf("\n5. Izlaz");
	printf("\n====================");
	printf("\nOdabir: ");
}

// switch case, sa enumom
int main() {
	int izbor;

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

		case IZLAZ:
			printf("\nIzlaz iz programa\n");
			break;

		default:
			printf("\nNeispravan odabir\n");
		}

	} while (izbor != 5);

	return 0;
}

/*
Koncepti koji su odrađeni (direktno, ne slučajno)
- 1
- 11

*/
