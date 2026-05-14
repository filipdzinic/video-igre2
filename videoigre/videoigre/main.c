#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "cruid.h"

void menu() {

	printf("\n====================");
	printf("\nDobrodosli u game library! Sljedeci put samo skinite STEAM");
	printf("\n====================");
	printf("\n1. Dodaj igru");
	printf("\n2. Prikazi igre");
	printf("\n3. Uredi igre");
	printf("\n4. Obrisi igre");
	printf("\n5. Izlaz");
	printf("\n====================");
	printf("\nOdabir: ");
}

int main() {
	int izbor;

	do {
		menu();
		scanf("%d", &izbor);

		switch (izbor) {

		case 1:
			stvoriIgru();
			break;

		case 2:
			citajIgru();
			break;

		case 3:
			updateIgru();
			break;

		case 4:
			obrisiIgru();
			break;

		case 5:
			printf("\nIzlaz iz programa\n");
			break;

		default:
			printf("\nNeispravan odabir\n");
		}

	} while (izbor != 5);

	return 0;
}