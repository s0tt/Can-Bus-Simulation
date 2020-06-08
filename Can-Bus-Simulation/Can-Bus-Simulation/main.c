#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "can.h"
#include "knoten.h"
#include "nachrichten.h"
#include "simulation.h"


void menue();

void main() {
	menue();
}

void menue() {
	int auswahl = 0;
	int auswahl_sim = 0;
	int boolean = 1;
	int boolean2 = 1;
	FILE *datei;
	char eingabe[20];
	int rueckgabe;
	int rueckgabe_n;

	while (boolean) {
		printf("CAN-BUS-Programm:\n");
		printf("1. CAN-Bus-Aufbau\n");
		printf("2. Can-Nachrichten\n");
		printf("3. K-Matrix erstellen/laden\n");
		printf("4. Simulation\n");
		printf("5. Beenden\n");
		scanf("%d", &auswahl);
		switch (auswahl) {

		//CAN-KNOTEN
		case 1:
			cls();
			boolean2 = 1;
			while (boolean2) {
				printf("CAN-Bus-Aufbau\n");
				printf("1. CAN-Bus Element hinzuf""\x81gen""\n");
				printf("2. CAN-Bus Elemente ansehen\n");
				printf("3. CAN-Bus Elemente editieren\n");
				printf("4. CAN-Bus Elemente l""\x94""schen\n");
				printf("5. Zur""\x81""ck zum Hauptmen""\x81""\n");
				seekfunc();
				scanf("%d", &auswahl);
				switch (auswahl) {

				case 1:
					cls();
					rueckgabe = dateneingabe();
					if (rueckgabe == 0) {
						cls();
						printf("Einf""\x81""gen erfolgreich\n");
						
					}
					else if (rueckgabe == 2) {
						cls();
						printf("Die Adresse ist schon vergeben! Das Element wurde nicht angelegt!\n");
					}else{
						printf("Fehler beim Einf""\x81""gen\n");
					}
					
					break;

				case 2:
					cls();
					ansehen();
					break;

				case 3:
					cls();
					printf("W""\x84""hlen sie das gew""\x81""nschte Element:\n");
					rueckgabe = ansehen();
					if (rueckgabe == 1) {
						printf("Adress""\x84""nderung erfolgreich!\n");
						boolean = 0;
					}

					break;

				case 4:
					cls();
					printf("W""\x84""hlen sie das gew""\x81""nschte Element:\n");
					ansehen();
					break;

				case 5:
					cls();
					boolean2 = 0;
					break;

				default:
					cls();
					printf("Falsche Auswahleingabe!\n");
					break;
				}
			}
			break;

		//CAN-Nachrichten
		case 2:
			cls();
			boolean2 = 1;
			while (boolean2) {
				printf("CAN-Nachrichten\n");
				printf("1. CAN-Nachricht hinzuf""\x81gen""\n");
				printf("2. CAN-Nachrichten ansehen\n");
				printf("3. CAN-Nachricht editieren\n");
				printf("4. CAN-Nachricht l""\x94""schen\n");
				printf("5. Zur""\x81""ck zum Hauptmen""\x81""\n");
				scanf("%d", &auswahl);
				switch (auswahl) {

				case 1:
					rueckgabe_n = dateneingabe_n();
					if (rueckgabe_n == 0) {
						cls();
						printf("Einf""\x81""gen erfolgreich!\n");
					}
					else if(rueckgabe_n == 2) {
						printf("Die Adresse ist schon vergeben! Die Nachricht wurde nicht angelegt!\n");
					}
					else {
						printf("Fehler beim Einf""\x81""gen!\n");
					}

					break;

				case 2:
					cls();
					ansehen_n();
					break;

				case 3:
					cls();
					printf("W""\x84""hlen sie das gew""\x81""nschte Element:\n");
					if (ansehen_n() == 2) {
						printf("Adress""\x84""nderung erfolgreich!\n");
						boolean = 0;
					}else
					break;

				case 4:
					cls();
					printf("W""\x84""hlen sie das gew""\x81""nschte Element:\n");
					ansehen_n();
					break;

				case 5:
					cls();
					boolean2 = 0;
					break;

				default:
					cls();
					printf("Falsche Auswahleingabe!\n");
					break;
				}
			}

			break;

		//K-Matrix
		case 3:
			cls();
			int boolean2 = 1;

			while (boolean2) {
				printf("K-Matrix erstellen/laden\n");
				printf("1. K-Matrix erstellen\n");
				printf("2. K-Matrix laden\n");
				printf("3. Zur""\x81""ck zum Hauptmen""\x81""\n");
				scanf("%d", &auswahl);
				switch (auswahl) {

				//K-Matrix erstellen
				case 1:
					cls();
					speicherknoten();
					speichernachrichten();
					break;

				//K-Matrix auslesen
				case 2:
					cls();
					printf("Hinweis: Die Text-Datei muss im programmeigenen Format vorliegen\n");
					printf("Namen der zu ladenden Datei mit Dateityp ein (max. 16 Zeichen + .txt):\n");
					seekfunc();
					fgets(eingabe, 20, stdin);
					entferneZeichen(eingabe, '\n');
					datei = fopen(eingabe, "r");
					if (datei != NULL) {
						cls();
						if (ladeknoten(datei) == 0) {
							
							printf("Knoten erfolgreich geladen!\n");
						}
						if (ladenachrichten(datei) == 0) {
							printf("Nachrichten erfolgreich geladen!\n");
						}
						fclose(datei);
					}
					else {
						cls();
						printf("Fehler beim Laden der Datei...\n");
					}
					boolean2 = 0;
					break;
				case 3:
					boolean2 = 0;
					break;

				default:
					cls();
					printf_s("Falsche Eingabe! Bitte w\x84hlen sie erneut:\n");
					break;
				}

			}
			break;

		//Simulation
		case 4:
			cls();
			printf("SIMULATION:\n");
			printf("1. Simulation starten\n");
			printf("2. Zurück zum Hauptmenü\n");
			decimalEingabe("Auswahl:", &auswahl_sim, 1, 1, 2);
			if (auswahl_sim == 1) {
				simulationsMenue();
			}
			else {
				cls();
			}

			//Reaktiviere den Sendestatus der Spontanen Nachrichten für nächste Simulation
			break;

		//Beenden
		case 5:
			boolean = 0;
			exit(0);
			break;

		}
	}
}
