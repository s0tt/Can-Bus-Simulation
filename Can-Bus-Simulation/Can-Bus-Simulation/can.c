#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include "can.h"

void entferneZeichen(char* string, char zeichen) {
	char* target = string;
	for (; (*target = *string) != 0; string++)
		if (*target != zeichen) target++;
}

int DezimalzuBinaer(int zahl)
{

		int rest, z = 1, binaerzahl = 0;
		while (zahl != 0)
		{
			rest = zahl % 2;
			zahl /= 2;
			binaerzahl += rest*z;
			z *= 10;
		}
		return binaerzahl;
	
}

int StringEingabe(char text[], char *speicherort, int laenge) {
	printf(text);
	printf("\n");
	seekfunc();
	fgets(speicherort, laenge, stdin);

	if (speicherort[strlen(speicherort) - 1] == '\n') {
		speicherort[strlen(speicherort) - 1] = '\0';
	}
	return 1;
}

int achtbiteingabe(unsigned int *speicherort) {
	int boolean = 1;
	while (boolean) {
		seekfunc();
		scanf("%d", speicherort);
		if ((*speicherort) < 0 || (*speicherort) > 255) {
			printf("Sie d""\x81""rfen maximal einen 8-bit gro""\xE1""en Integer eingeben!\nWiederholen sie die Eingabe:\n");
		}
		else {
			boolean = 0;
		}
	}
	return 1;
}

int decimalEingabe(char text[], int *speicherort, int wertebereichAN, int min, int max) { //WertebereichAN == 1 bedeutet eingrenzung der eingabe
	int boolean = 1;

	printf(text);
	if (wertebereichAN == 1) {
		while (boolean) {
			seekfunc();
			scanf("%d", speicherort);
			if (*speicherort >= min && *speicherort <= max) {
				boolean = 0;
			}
			else {
				printf("Wiederholen sie die Eingabe! \nDer Wert muss zwischen %d und %d liegen\n", min, max);
			}

		}
	}
	else if (wertebereichAN == 0) {
		seekfunc();
		scanf("%d", speicherort);
	}

}

int schreibeLeer(FILE *datei, int anzahl) {
	for (int i = 0; i < anzahl; i++) {
		fprintf(datei, " ");
	}
}

int stellenzahl(int zahl) {
	int stellen = floor(log10(abs(zahl))) + 1;
	return stellen;
}