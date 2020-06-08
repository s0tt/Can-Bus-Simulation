#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "can.h"
#include "knoten.h"


canbuselement *listenanfang = NULL;

int dateneingabe() {
	unsigned int adresse;
	char name[MAX_NAME_KNOTEN];
	char beschreibung[MAX_BESCHREIBUNG];
	int boolean = 1;

	// Daten eingaben
	printf("Bitte geben sie die Daten ein:\n");
	printf("Adresse des CAN-Elements (0-255):\n");
	achtbiteingabe(&adresse);

	StringEingabe("Name des CAN-Elements (10 Zeichen):", &name, MAX_NAME_KNOTEN);

	StringEingabe("Beschreibung des CAN-Elements (40 Zeichen):", &beschreibung, MAX_BESCHREIBUNG);

	return ((hinzufuegen(adresse, name, beschreibung)));
	
}

int hinzufuegen(int adresse, char name[MAX_NAME_KNOTEN], char beschreibung[MAX_BESCHREIBUNG]) {
	int boolean = 1;
	canbuselement *einordnungsposition;
	canbuselement *adressposition;
	einordnungsposition = listenanfang;

	//DATENEINGABE AUSGELAGERT

	//Liste leer
	if (listenanfang == NULL) {
		listenanfang = malloc(sizeof(canbuselement));
		if (listenanfang == NULL) {
			cls();
			printf("Es konnte kein Speicher bereitgestellt werden!");
		}
		else {
			listenanfang->adresse = adresse;
			strcpy(listenanfang->name, name);
			strcpy(listenanfang->beschreibung, beschreibung);
			listenanfang->nach = NULL;
			listenanfang->vor = NULL;
			return 0;
		}
	}

	//Bereits Elemente in der Liste
	else {

		adressposition = malloc(sizeof(canbuselement));
		if (adressposition == NULL) {
			cls();
			printf("Es konnte kein Speicher bereitgestellt werden!");
		}
		else {
			//Einordnung des Elements (Adressensortiert) von vorne nach hinten
			while (einordnungsposition->adresse < adresse && einordnungsposition->nach != NULL) {
				einordnungsposition = einordnungsposition->nach;
			}



			if (einordnungsposition->adresse < adresse) {
				// Einordnung nach Element
				adressposition->vor = einordnungsposition;
				adressposition->nach = einordnungsposition->nach;
				if (einordnungsposition->nach != NULL) {
					einordnungsposition->nach->vor = adressposition;
				}
				einordnungsposition->nach = adressposition;
				adressposition->adresse = adresse;
				strcpy(adressposition->name, name);
				strcpy(adressposition->beschreibung, beschreibung);
				return 0;

			}
			else if (einordnungsposition->adresse > adresse) {
				// Einordnung vor Element
				if (einordnungsposition == listenanfang) {
					listenanfang = adressposition;
				}
				adressposition->nach = einordnungsposition;
				adressposition->vor = einordnungsposition->vor;

				if (einordnungsposition->vor != NULL) {
					einordnungsposition->vor->nach = adressposition;
				}

				einordnungsposition->vor = adressposition;

				adressposition->adresse = adresse;
				strcpy(adressposition->name, name);
				strcpy(adressposition->beschreibung, beschreibung);
				return 0;

			}
			else {
				return 2;
			}


		}
	}
}

int ansehen() {
	canbuselement *zeiger;
	zeiger = listenanfang;
	int boolean = 1;


	do {
		if (listenanfang != NULL) {
			printf("CAN-Adresse: %d\n", zeiger->adresse);
			printf("CAN-Name: %s\n", zeiger->name);
			printf("CAN-Beschreibung: %s\n", zeiger->beschreibung);
			printf("\n[V]orheriges      [N]""\x84""chstes      [E]ditieren      [L]""\x94""schen      [B]eenden\n"); //Mehrere Strings da sonst Fehlinterpretation als zu große Hexzahl erfolgt
			printf("Auswahl [V,N,E,L,B]:");
			seekfunc();

			switch (tolower(getchar())) {
			case 'v':
				if (zeiger->vor != NULL) {
					zeiger = zeiger->vor;
					cls();
				}
				else {
					cls();
					printf("Das ist das erste Element!\n");
				}
				break;
			case 'l':
				loeschen(zeiger);
				if (listenanfang != NULL) {
					zeiger = listenanfang;
				}
				break;

			case 'n':
				if (zeiger->nach != NULL) {
					zeiger = zeiger->nach;
					cls();
				}
				else {
					cls();
					printf("Das ist das letzte Element!\n");
				}
				break;

			case 'e':
				return(editieren(zeiger));
				break;

			case 'b':
				boolean = 0;
				cls();

			default:
				cls();
				break;
			}

		}
		else {
			boolean = 0;
			printf("Der CAN-Bus ist leer!\n");
		}
	} while (boolean);

}

void loeschen(canbuselement *zeiger) {

	//Element davor & danach
	if (zeiger->vor != NULL && zeiger->nach != NULL) {
		zeiger->vor->nach = zeiger->nach;
		zeiger->nach->vor = zeiger->vor;
	}else

	//Element nur danach
	if (zeiger->vor == NULL && zeiger->nach != NULL) {
		listenanfang = zeiger->nach;
		zeiger->nach->vor = NULL;
	}else

	//Element nur davor
	if (zeiger->vor != NULL && zeiger->nach == NULL) {
		zeiger->vor->nach = NULL;
	}else

	//Kein Element davor & danach
	if (zeiger->vor == NULL && zeiger->nach == NULL) {
		listenanfang = NULL;
	}

	cls();
	printf("L""\x94""chen von CAN-Bus Element: %d.%s erfolgreich!\n\n", zeiger->adresse, zeiger->name);
	free(zeiger);

}

int editieren(canbuselement *zeiger) {
	int auswahl;
	int adresseingabe = 0;
	int boolean = 1;
	int zähler = 1;


	while (boolean) {
		cls();

		printf("[1.] CAN-Adresse: %d\n", zeiger->adresse);
		printf("[2.] CAN-Name: %s\n", zeiger->name);
		printf("[3.] CAN-Beschreibung: %s\n", zeiger->beschreibung);
		printf("[4.] Bearbeitung beenden");

		printf("\nWas wollen sie bearbeiten? (1,2,3,4 )");
		seekfunc();
		scanf("%d", &auswahl);

			switch (auswahl) {
			case 1:
				//WICHTIG: Elemént muss verschoben werden (Grund: Adresssortiert) -> Lösung: Erstellen neuen Elements mit alten Werten + neue Adresse

				printf("Geben sie die neue Adresse des CAN-Elements  ein (8-bit):\n");
				achtbiteingabe(&adresseingabe);

				//Neues Element mit alten Parametern + neue Adresse
				if (hinzufuegen(adresseingabe, zeiger->name, zeiger->beschreibung) == 2) {
					cls();
					printf("Es existiert bereits ein Element mit dieser Adresse!\n");
					return 0;
				}
				else {
					loeschen(zeiger);
					cls();
					boolean = 0;
					return 1;
				}		

				break;

			case 2:
				StringEingabe("Geben sie den neuen Namen ein (10 Zeichen):", zeiger->name, MAX_NAME_KNOTEN);
				printf("Bearbeitung erfolgreich!");
				break;


			case 3:
				StringEingabe("Geben sie die neue Beschreibung ein (40 Zeichen):", zeiger->beschreibung, MAX_BESCHREIBUNG);
				printf("Bearbeitung erfolgreich!");

				break;
			case 4:
				cls();
				boolean = 0;
				break;

			default:
				printf("Falsche Eingabe!");
			}

	}
}

int speicherknoten() {
	FILE *kmatrix;
	canbuselement *zeiger;

	kmatrix = fopen("kmatrix.txt", "w+");
	fputs("KNOTEN:\n", kmatrix);
	fputs("ID\tName\t\tBeschreibung\n", kmatrix);
	fputs("_____________________________________________________\n", kmatrix);
	zeiger = listenanfang;

	while (zeiger != NULL) {
		fprintf(kmatrix, "\n%d,\t", zeiger->adresse);
		fputs(zeiger->name, kmatrix);
		fprintf(kmatrix, ",\t\t");
		fputs(zeiger->beschreibung, kmatrix);
		fprintf(kmatrix, "\n");
		printf("Speichere Element %d.%s erfolgreich....\n", zeiger->adresse, zeiger->name);
		zeiger = zeiger->nach;

	}
	fputs("-----------------------------------------------------\n", kmatrix);
	fclose(kmatrix);
	return 0;
}

int ladeknoten(FILE *datei) {

	char ausgelesen[75];
	int zeilenzähler = 0;
	int stellen = 0;
	char *zeiger;
	unsigned int adresse;
	char name[MAX_NAME_KNOTEN];
	char beschreibung[MAX_BESCHREIBUNG];

	if (fgets(ausgelesen, 75, datei) != NULL) {
		while (ausgelesen[0] != 45) {
			if (zeilenzähler > 3 && ausgelesen[0] != '\n') {

				//String bereinigen
				entferneZeichen(ausgelesen, '\t');
				entferneZeichen(ausgelesen, '\n');
				entferneZeichen(ausgelesen, '\0');

				//String anhand von Komma in Daten zerlegen
				zeiger = strtok(ausgelesen, ",");
				adresse = atoi(zeiger);	//Wandelt Zahl als String in eine Dezimalzahl um

				zeiger = strtok(NULL, ",");
				strcpy(name, zeiger);

				zeiger = strtok(NULL, ",");
				strcpy(beschreibung, zeiger);

				hinzufuegen(adresse, name, beschreibung);
			}

			fgets(ausgelesen, 75, datei);
			zeilenzähler++;
		}
	}
	return 0;
}