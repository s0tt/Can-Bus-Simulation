#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "can.h"
#include "nachrichten.h"
#include "knoten.h"


//Globaler Listenanfang Zeiger
cannachricht *nachrichtenanfang = NULL;

int dateneingabe_n() {
	unsigned int prioritaet;
	unsigned int empfaenger;
	unsigned int sender;
	int min;
	int max;
	int sendeart;
	int sendezyklus = 0;
	int defaultwert;
	char name[MAX_NAME];
	int boolean = 1;

	// Daten eingaben
	printf("\nBitte geben sie die Daten ein:\n");
	printf("ID der CAN-Nachricht (24 Bit):\n");
	printf("   1. Priori""\x84""t der Nachricht (0-255): ");
	achtbiteingabe(&prioritaet);

	printf("   2. Empf""\x84""nger der Nachricht (0-255): ");
	achtbiteingabe(&empfaenger);

	printf("   3. Sender der Nachricht (0-255): ");
	achtbiteingabe(&sender);

	StringEingabe("\nName der Nachricht (40 Zeichen):", &name, MAX_NAME);

	printf("\nWertebereich der Nachricht (Min, Max und Wertebereich):\n");
	printf("   Minimalwert der Nachricht: ");
	seekfunc();
	scanf("%d", &min);
	printf("   Maximalwert der Nachricht: ");

	seekfunc();
	boolean = 1;

	while (boolean) {
		seekfunc();
		scanf("%d", &max);
		if (max < min) {
			printf("Der Maximalwert kann nicht kleiner als der Minimalwert sein!\n Wiederholen sie die Eingabe:\n");
		}
		else {
			boolean = 0;
		}
	}


	decimalEingabe("   Standartwert der Nachricht (zwischen Min - Max): ", &defaultwert, 1, min, max);


	printf("\nSendeart der Nachricht (0 f""\x81""r spontan / 1 f""\x81""r zyklisch):\n");
	seekfunc();
	boolean = 1;
	while (boolean) {
		seekfunc();
		scanf("%d", &sendeart);
		if (sendeart < 0 || sendeart > 1) {
			printf("Es gibt nur die Sendearten 0 und 1, wiederholen sie die Eingabe:\n");
		}
		else {
			if (sendeart == 1) {
				boolean = 1;
				printf("Geben sie die Sendezeit der zyklischen Nachricht ein (5ms,10ms,15ms):\n");
				while (boolean) {
					seekfunc();
					scanf("%d", &sendezyklus);
					if (sendezyklus == 5 || sendezyklus == 10 || sendezyklus == 15) {
						boolean = 0;
					}
					else {
						printf("Falsche Eingabe! Geben sie 5, 10 oder 15 ms an!\n");
					}
				}
			}
			else {

				boolean = 0;
			}

		}
	}


	return (hinzufuegen_n(prioritaet, empfaenger, sender, name, sendeart, sendezyklus, min, max, defaultwert));

}

int hinzufuegen_n(unsigned int prioritaet, unsigned int empfaenger, unsigned int sender, char name[MAX_NAME], int sendeart, int sendezyklus, int min, int max, int defaultwert) {
	int boolean = 1;
	int fallart = 0;
	cannachricht *einordnungsposition;
	cannachricht *adressposition;
	einordnungsposition = nachrichtenanfang;

	//Liste leer
	if (nachrichtenanfang == NULL) {
		nachrichtenanfang = malloc(sizeof(cannachricht));
		if (nachrichtenanfang == NULL) {
			cls();
			printf("Es konnte kein Speicher bereitgestellt werden!");
		}
		else {
			nachrichtenanfang->werte.defaultwert = defaultwert;
			nachrichtenanfang->werte.inhalt = defaultwert;
			nachrichtenanfang->werte.min = min;
			nachrichtenanfang->werte.max = max;
			nachrichtenanfang->werte.sendestatus = 0;

			nachrichtenanfang->id.sender = sender;
			nachrichtenanfang->id.prioritaet = prioritaet;
			nachrichtenanfang->id.empfaenger = empfaenger;

			nachrichtenanfang->sendeart = sendeart;
			nachrichtenanfang->sendezyklus = sendezyklus;
			strcpy(nachrichtenanfang->name, name);

			nachrichtenanfang->nach = NULL;
			nachrichtenanfang->vor = NULL;
			return 0;
		}
	}

	//Bereits Elemente in der Liste
	else {

		adressposition = malloc(sizeof(cannachricht));
		if (adressposition == NULL) {
			cls();
			printf("Es konnte kein Speicher bereitgestellt werden!");
		}
		else {
			//Einordnung des Elements (Prioritätssortiert) von vorne nach hinten
			while (einordnungsposition->id.prioritaet < prioritaet && einordnungsposition->nach != NULL) {
				einordnungsposition = einordnungsposition->nach;
			}



			if (einordnungsposition->id.prioritaet < prioritaet) {
				// Einordnung nach Element
				adressposition->vor = einordnungsposition;
				adressposition->nach = einordnungsposition->nach;
				if (einordnungsposition->nach != NULL) {
					einordnungsposition->nach->vor = adressposition;
				}
				einordnungsposition->nach = adressposition;

				//Werte einfügen
				adressposition->werte.defaultwert = defaultwert;
				adressposition->werte.inhalt = defaultwert;
				adressposition->werte.min = min;
				adressposition->werte.max = max;
				adressposition->werte.sendestatus = 0;

				adressposition->id.sender = sender;
				adressposition->id.prioritaet = prioritaet;
				adressposition->id.empfaenger = empfaenger;

				adressposition->sendeart = sendeart;
				adressposition->sendezyklus = sendezyklus;
				strcpy(adressposition->name, name);

				return 0;
			}
			else if (einordnungsposition->id.prioritaet > prioritaet) {
				// Einordnung vor Element
				if (einordnungsposition == nachrichtenanfang) {
					nachrichtenanfang = adressposition;
				}
				adressposition->nach = einordnungsposition;
				adressposition->vor = einordnungsposition->vor;

				if (einordnungsposition->vor != NULL) {
					einordnungsposition->vor->nach = adressposition;
				}

				einordnungsposition->vor = adressposition;

				//Werte einfügen
				adressposition->werte.defaultwert = defaultwert;
				adressposition->werte.inhalt = defaultwert;
				adressposition->werte.min = min;
				adressposition->werte.max = max;
				adressposition->werte.sendestatus = 0;

				adressposition->id.sender = sender;
				adressposition->id.prioritaet = prioritaet;
				adressposition->id.empfaenger = empfaenger;

				adressposition->sendeart = sendeart;
				adressposition->sendezyklus = sendezyklus;
				strcpy(adressposition->name, name);

				return 0;

			}
			else {
				return 2;
			}

		}
	}
}

int ansehen_n() {
	cannachricht *zeiger;
	zeiger = nachrichtenanfang;
	int boolean = 1;


	do {
		if (nachrichtenanfang != NULL) {
			printf("Nachrichten-ID: %d-%d-%d\n", DezimalzuBinaer(zeiger->id.prioritaet), DezimalzuBinaer(zeiger->id.empfaenger), DezimalzuBinaer(zeiger->id.sender));
			printf("   Priorit""\x84""t: %d\n", zeiger->id.prioritaet);
			printf("   Empf""\x84""nger: %d\n", zeiger->id.empfaenger);
			printf("   Sender: %d\n", zeiger->id.sender);
			printf("Nachrichten-Name: %s\n", zeiger->name);
			printf("Nachrichten-Wertebereich: \n   Minimalwert: %d\n   Maximalwert: %d\n   Standartwert: %d\n", zeiger->werte.min, zeiger->werte.max, zeiger->werte.defaultwert);
			printf("Nachrichten-Inhalt: %d \n", zeiger->werte.inhalt);
			printf("Nachrichten-Sendeart: %d \n", zeiger->sendeart);
			if (zeiger->sendeart == 1) {
				printf("   Sendezyklus: %d \n", zeiger->sendezyklus);
			}
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
				loeschen_n(zeiger);
				if (nachrichtenanfang != NULL) {
					zeiger = nachrichtenanfang;
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
				if ((editieren_n(zeiger) == 1)) {
					boolean = 0;
					return 1;
				}
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
			printf("Es sind keine Nachrichten vorhanden!\n");
		}
	} while (boolean);

}

void loeschen_n(cannachricht *zeiger_n) {

	//Element davor & danach
	if (zeiger_n->vor != NULL && zeiger_n->nach != NULL) {
		zeiger_n->vor->nach = zeiger_n->nach;
		zeiger_n->nach->vor = zeiger_n->vor;
	}

	//Element nur danach
	if (zeiger_n->vor == NULL && zeiger_n->nach != NULL) {
		nachrichtenanfang = zeiger_n->nach;
		zeiger_n->nach->vor = NULL;
	}

	//Element nur davor
	if (zeiger_n->vor != NULL && zeiger_n->nach == NULL) {
		zeiger_n->vor->nach = NULL;
	}

	//Kein Element davor & danach
	if (zeiger_n->vor == NULL && zeiger_n->nach == NULL) {
		nachrichtenanfang = NULL;
	}

	cls();
	printf("L""\x94""chen von der Nachricht: %s erfolgreich!\n\n", zeiger_n->name);
	free(zeiger_n);

}

int editieren_n(cannachricht *zeiger_n) {
	int auswahl;
	int adresseingabe;
	int boolean = 1;
	int boolean2 = 1;
	int zähler = 1;
	unsigned int prioritaet;

	while (boolean) {
		cls();
		printf("Nachrichten-ID:\n");
		printf("   [1.] Priorit""\x84""t: %d\n", zeiger_n->id.prioritaet);
		printf("   [2.] Empf""\x84""nger: %d\n", zeiger_n->id.empfaenger);
		printf("   [3.] Sender: %d\n", zeiger_n->id.sender);
		printf("[4.] Nachrichten-Name: %s\n", zeiger_n->name);
		printf("Nachrichten-Werte:\n");
		printf("   [5.] Inhalt: %d\n", zeiger_n->werte.inhalt);
		printf("   [6.] Minimalwert: %d\n", zeiger_n->werte.min);
		printf("   [7.] Maximalwert: %d\n", zeiger_n->werte.max);
		printf("   [8.] Standartwert: %d\n", zeiger_n->werte.defaultwert);
		printf("[9.] Nachrichten-Sendeart: %d\n", zeiger_n->sendeart);
		if (zeiger_n->sendeart == 1) {
			printf("[10.] Nachrichten-Sendezyklus: %d\n", zeiger_n->sendezyklus);
		}
		printf("[11.] Bearbeitung beenden");

		printf("\n\nWas wollen sie bearbeiten? (1-11): ");
		seekfunc();
		scanf("%d", &auswahl);

		switch (auswahl) {
		case 1:
			//WICHTIG: ES WIRD AUCH DAS ELEMENT IN DER LISTE VERSCHOBEN (NICHT NUR DIE ADRESSE GEÄNDERT)

			//Setze Verschiebenzeiger auf aktuelles Element

			printf("Geben sie die neue Prioritaet des CAN-Elements ein (8-bit):\n");
			achtbiteingabe(&prioritaet);
			if ((hinzufuegen_n(prioritaet, zeiger_n->id.empfaenger, zeiger_n->id.sender, zeiger_n->name, zeiger_n->sendeart, zeiger_n->sendezyklus, zeiger_n->werte.min, zeiger_n->werte.max, zeiger_n->werte.defaultwert)) == 2) {
				printf("Es existiert bereits ein Element mit dieser Adresse!\n");
			}
			else {
				loeschen(zeiger_n);
			}
			printf("Adress""\x84""nderung erfolgreich!");
			boolean = 0;
			return 1;
			break;

		case 2:
			printf("Geben sie den neuen Empf""\x84""nger ein (1-255):");
			achtbiteingabe((zeiger_n->id.prioritaet));
			printf("Bearbeitung erfolgreich!");
			break;


		case 3:
			printf("Geben sie den neuen Sender ein (1-255):");
			achtbiteingabe(zeiger_n->id.sender);
			printf("Bearbeitung erfolgreich!");
			break;

		case 4:
			StringEingabe("Geben sie den Namen der Nachricht ein:", zeiger_n->name, MAX_NAME);
			break;

		case 5:
			decimalEingabe("Geben sie den neuen Inhalt ein:", zeiger_n->werte.inhalt, 1, zeiger_n->werte.min, zeiger_n->werte.max);
			break;

		case 6:
			boolean2 = 1;
			printf("Geben sie den neuen Minimalwert ein:\n");
			while (boolean2) {
				seekfunc();
				scanf("%d", &(zeiger_n->werte.min));
				if (zeiger_n->werte.max < zeiger_n->werte.min) {
					printf("Der Minmalwert kann nicht gr""\x94""""\xE1""er als der Maximalwert sein!\nWiederholen sie die Eingabe:");
				}
				else {
					boolean2 = 0;
				}
			}
			break;

		case 7:
			boolean2 = 1;
			printf("Geben sie den neuen Maximalwert ein:\n");
			while (boolean2) {
				seekfunc();
				scanf("%d", &(zeiger_n->werte.max));
				if ((zeiger_n->werte.max) < (zeiger_n->werte.min)) {
					printf("Der Maximalwert kann nicht kleiner als der Minimalwert sein!\nWiederholen sie die Eingabe:");
				}
				else {
					boolean2 = 0;
				}
			}
			break;

		case 8:
			decimalEingabe("Geben sie den neuen Standartwert ein:", &(zeiger_n->werte.defaultwert), 1, zeiger_n->werte.min, zeiger_n->werte.max);
			break;

		case 9:
			decimalEingabe("Geben sie die neue Nachrichten Senderart ein (0 f""\x81""r spontan/1 f""\x81""r zyklisch)", &(zeiger_n->sendeart), 1, 0, 1);
			break;

		case 10:
			if (zeiger_n->sendeart == 1) {
				boolean = 1;
				printf("Geben sie die Sendezeit der zyklischen Nachricht ein (5ms,10ms,15ms):\n");
				while (boolean) {
					seekfunc();
					scanf("%d", &(zeiger_n->sendezyklus));
					if (zeiger_n->sendezyklus != 5 || zeiger_n->sendezyklus != 10 || zeiger_n->sendezyklus != 15) {
						printf("Falsche Eingabe! Geben sie 5, 10 oder 15 ms an!");
					}
					else {
						boolean = 0;
					}
				}
			}
			else if (zeiger_n->sendeart == 0) {
				printf("Die Nachricht ist nicht vom Sendetyp zyklisch!");
			}
			break;

		default:
			printf("Falsche Eingabe!");
		}

	}

}

int speichernachrichten() {
	FILE *kmatrix;
	cannachricht *zeiger;

	kmatrix = fopen("kmatrix.txt", "a+");
	fputs("NACHRICHTEN:\n", kmatrix);
	fputs("Priorität\tEmpfänger\tSender\tName \t\t Min.\t Max.\t Default.\t Sendeart\t Sendezyklus\n", kmatrix);
	fputs("____________________________________________________________________________________________________________________________\n", kmatrix);
	zeiger = nachrichtenanfang;

	while (zeiger != NULL) {
		fprintf(kmatrix, "\n%d,\t", zeiger->id.prioritaet);
		schreibeLeer(kmatrix, 8 - stellenzahl(zeiger->id.prioritaet));

		fprintf(kmatrix, "%d,\t", zeiger->id.empfaenger);
		schreibeLeer(kmatrix, 8 - stellenzahl(zeiger->id.empfaenger));

		fprintf(kmatrix, "%d,\t", zeiger->id.sender);
		schreibeLeer(kmatrix, 5 - stellenzahl(zeiger->id.empfaenger));
		fputs(zeiger->name, kmatrix);

		fprintf(kmatrix, ",");
		schreibeLeer(kmatrix, 9 - strlen(zeiger->name));

		fprintf(kmatrix, "%d,\t", zeiger->werte.min);
		schreibeLeer(kmatrix, 4 - stellenzahl(zeiger->werte.min));

		fprintf(kmatrix, "%d,\t", zeiger->werte.max);
		schreibeLeer(kmatrix, 4 - stellenzahl(zeiger->werte.max));

		fprintf(kmatrix, "%d,\t", zeiger->werte.defaultwert);
		schreibeLeer(kmatrix, 8 - stellenzahl(zeiger->werte.defaultwert));

		fprintf(kmatrix, "%d,\t", zeiger->sendeart);
		schreibeLeer(kmatrix, 8 - stellenzahl(zeiger->sendeart));

		fprintf(kmatrix, "%d,\t", zeiger->sendezyklus);

		fprintf(kmatrix, "\n");
		printf("Speichere Nachricht: %s erfolgreich....\n", zeiger->name);
		zeiger = zeiger->nach;

	}
	fputs("---------------------------------------------------------------------\n", kmatrix);
	fclose(kmatrix);
	return 0;
}

int ladenachrichten(FILE *datei) {

	char ausgelesen[100];
	int zeilenzähler = 0;
	int zeigerzähler = 2;
	char *zeiger;
	char name[MAX_NAME];
	unsigned int prioritaet;
	unsigned int empfaenger;
	unsigned int sender;
	int min;
	int max;
	int sendeart;
	int sendezyklus = 0;
	int defaultwert;
	int boolean = 1;

	if (fgets(ausgelesen, 100, datei) != NULL) {
		while (ausgelesen[0] != 45) {
			if (zeilenzähler > 4 && ausgelesen[0] != '\n') {
				zeigerzähler = 2;
				//String bereinigen
				entferneZeichen(ausgelesen, '\t');
				entferneZeichen(ausgelesen, '\n');
				entferneZeichen(ausgelesen, '\0');
				entferneZeichen(ausgelesen, ' ');

				//String anhand von Komma in Daten zerlegen
				zeiger = strtok(ausgelesen, ",");
				prioritaet = atoi(zeiger);	//Wandelt Zahl als String in eine Dezimalzahl um


				while (zeiger != NULL && zeigerzähler < 10) {
					switch (zeigerzähler) {
					case 2:
						zeiger = strtok(NULL, ",");
						empfaenger = atoi(zeiger);
						break;
					case 3:
						zeiger = strtok(NULL, ",");
						sender = atoi(zeiger);
						break;
					case 4:
						zeiger = strtok(NULL, ",");
						strcpy(name, zeiger);
						break;
					case 5:
						zeiger = strtok(NULL, ",");
						min = atoi(zeiger);
						break;
					case 6:
						zeiger = strtok(NULL, ",");
						max = atoi(zeiger);
						break;
					case 7:
						zeiger = strtok(NULL, ",");
						defaultwert = atoi(zeiger);
						break;
					case 8:
						zeiger = strtok(NULL, ",");
						sendeart = atoi(zeiger);
						break;
					case 9:
						zeiger = strtok(NULL, ",");
						sendezyklus = atoi(zeiger);
						break;
					}
					zeigerzähler++;
				}

				hinzufuegen_n(prioritaet, empfaenger, sender, name, sendeart, sendezyklus, min, max, defaultwert);
			}
			zeilenzähler++;
			fgets(ausgelesen, 100, datei);
		}

		return 0;
	}
	else {
		return 1;
	}
}