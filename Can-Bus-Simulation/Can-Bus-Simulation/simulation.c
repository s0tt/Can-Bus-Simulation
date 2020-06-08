#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include "can.h"
#include "nachrichten.h"
#include "simulation.h"

int Zeit = 0;
cannachricht *laufzeiger;

void simulationsMenue() {
	char auswahl;
	Zeit = 0;
	int boolean = 1;
	laufzeiger = nachrichtenanfang;

	if (laufzeiger != NULL) {
		cls();
		printf("SIMULATION:\n");
		printf("Zeitschritt: %d\n\n", Zeit);
		printf("\n[+]N""\x84""chster Zeitschritt  [E]ditieren von Inhalt\n[H]inzuf""\x81""gen spontanen Nachricht [B]eenden\nAuswahl: ");

		while (boolean) {
		seekfunc();
		auswahl = tolower(getchar());
		
		switch (auswahl) {
			case '+':
				cls();
				Zeit++;
				printf("SIMULATION:\n");
				printf("Zeitschritt: %d\n\n", Zeit);
				simuliereZeitschritt();
				printf("\n[+]N""\x84""chster Zeitschritt  [E]ditieren von Inhalt\n[H]inzuf""\x81""gen spontanen Nachricht [B]eenden\nAuswahl: ");	
				seekfunc();
				break;

			case 'e':
				cls();
				printf("Wählen sie die zu bearbeitende zyklische Nachricht:\n");
				zeigeNachrichten(1);
				printf("SIMULATION:\n");
				Zeit++;
				printf("Zeitschritt: %d\n\n", Zeit);			
				simuliereZeitschritt();
				printf("\n[+]N""\x84""chster Zeitschritt  [E]ditieren von Inhalt\n[H]inzuf""\x81""gen spontanen Nachricht [B]eenden\nAuswahl: ");
				break;

			case 'h':
				cls();
				printf("Wählen sie die spontane Nachricht:\n");
				zeigeNachrichten(0);
				Zeit++;
				printf("SIMULATION:\n");
				printf("Zeitschritt: %d\n\n", Zeit);
				simuliereZeitschritt();
				printf("\n[+]N""\x84""chster Zeitschritt  [E]ditieren von Inhalt\n[H]inzuf""\x81""gen spontanen Nachricht [B]eenden\nAuswahl: ");
				break;

			case 'b':
				cls();
				boolean = 0;
				break;

			default:
				cls();
				break;

			}
		}
	}
	else {
		cls();
		printf("Es sind keine Nachrichten zum Simulieren vorhanden!\n");
	}
}

void simuliereZeitschritt() {

	//Iteriere durch die Liste um zyklische Nachrichten anzuzeigen
	laufzeiger = nachrichtenanfang;
	while (laufzeiger != NULL) {

		if (laufzeiger->sendeart == 1) {
			if (laufzeiger->sendezyklus == 5) {
				if ((Zeit % 5) == 0) {
					printf("%d,%d,%d,%s,%d,%d,%d,%d,%d\n", laufzeiger->id.prioritaet, laufzeiger->id.empfaenger, laufzeiger->id.sender, laufzeiger->name, laufzeiger->werte.min, laufzeiger->werte.max, laufzeiger->werte.defaultwert, laufzeiger->sendeart, laufzeiger->sendezyklus);
				}
			}
			if (laufzeiger->sendezyklus == 10) {
				if ((Zeit % 10) == 0) {
					printf("%d,%d,%d,%s,%d,%d,%d,%d,%d\n", laufzeiger->id.prioritaet, laufzeiger->id.empfaenger, laufzeiger->id.sender, laufzeiger->name, laufzeiger->werte.min, laufzeiger->werte.max, laufzeiger->werte.defaultwert, laufzeiger->sendeart, laufzeiger->sendezyklus);
				}
			}
			if (laufzeiger->sendezyklus == 15) {
				if ((Zeit % 15) == 0) {
					printf("%d,%d,%d,%s,%d,%d,%d,%d,%d\n", laufzeiger->id.prioritaet, laufzeiger->id.empfaenger, laufzeiger->id.sender, laufzeiger->name, laufzeiger->werte.min, laufzeiger->werte.max, laufzeiger->werte.defaultwert, laufzeiger->sendeart, laufzeiger->sendezyklus);
				}
			}
		}
		else if (laufzeiger->sendeart == 0) {
			if (laufzeiger->werte.sendestatus == 1) {
				printf("%d,%d,%d,%s,%d,%d,%d,%d,%d\n",laufzeiger->id.prioritaet, laufzeiger->id.empfaenger, laufzeiger->id.sender, laufzeiger->name, laufzeiger->werte.min, laufzeiger->werte.max, laufzeiger->werte.defaultwert, laufzeiger->sendeart, laufzeiger->sendezyklus);
				laufzeiger->werte.sendestatus = 0;
			}

		}
		laufzeiger = laufzeiger->nach;
	}
}

void SendestatusWdh() {
	laufzeiger = nachrichtenanfang;
	while (laufzeiger != NULL) {
		if (laufzeiger->sendeart == 0) {
			laufzeiger->werte.sendestatus = 0;
		}
	}
}

void zeigeNachrichten(int auswahl) { //0 - spontane Nachrichten zeigen | 1- zyklische nachrichten zeigen
	laufzeiger = nachrichtenanfang;
	cannachricht *suchZeiger;
	int boolean = 1;
	char eingabe;

	do {
		if (nachrichtenanfang != NULL) {
			if (laufzeiger->sendeart == auswahl) {
				printf("Nachrichten-ID: %d-%d-%d\n", DezimalzuBinaer(laufzeiger->id.prioritaet), DezimalzuBinaer(laufzeiger->id.empfaenger), DezimalzuBinaer(laufzeiger->id.sender));
				printf("   Priorit""\x84""t: %d\n", laufzeiger->id.prioritaet);
				printf("   Empf""\x84""nger: %d\n", laufzeiger->id.empfaenger);
				printf("   Sender: %d\n", laufzeiger->id.sender);
				printf("Nachrichten-Name: %s\n", laufzeiger->name);
				printf("Nachrichten-Wertebereich: \n   Minimalwert: %d\n   Maximalwert: %d\n   Standartwert: %d\n", laufzeiger->werte.min, laufzeiger->werte.max, laufzeiger->werte.defaultwert);
				printf("NACHRICHTE-INHALT: %d \n", laufzeiger->werte.inhalt);
				printf("Nachrichten-Sendeart: %d \n", laufzeiger->sendeart);
				if (laufzeiger->sendeart == 1) {
					printf("   Sendezyklus: %d \n", laufzeiger->sendezyklus);
				}
				if (auswahl == 1) {
					printf("\n[V]orheriges      [N]""\x84""chstes      [E]ditiere Inhalt      [B]eenden\n"); //Mehrere Strings da sonst Fehlinterpretation als zu große Hexzahl erfolgt
					printf("Auswahl [V,N,E,B]:");
				}
				else if (auswahl == 0) {
					printf("\n[V]orheriges      [N]""\x84""chstes      [S]enden im CAN     [B]eenden\n"); //Mehrere Strings da sonst Fehlinterpretation als zu große Hexzahl erfolgt
					printf("Auswahl [V,N,S,B]:");
				}
				seekfunc();

				eingabe = tolower(getchar());
				switch (eingabe) {
				case 'v':
					if (laufzeiger->vor != NULL) {
						laufzeiger = laufzeiger->vor;
						cls();
					}
					else {
						cls();
						printf("Das ist das erste Element!\n");
					}
					break;

				case 'n':
					if (laufzeiger->nach != NULL) {
						laufzeiger = laufzeiger->nach;
						cls();
					}
					else {
						cls();
						printf("Das ist das letzte Element!\n");
					}
					break;
				case 'e':
					decimalEingabe("\nGeben sie den neuen Inhalt ein:", &(laufzeiger->werte.inhalt), 1, laufzeiger->werte.min, laufzeiger->werte.max);
					cls();
					break;

				case 'b':
					cls();
					boolean = 0;
					break;

				case 's':
					cls();
					decimalEingabe("\nGeben sie den Inhalt für die Spontane Nachricht an", &(laufzeiger->werte.inhalt), 1, laufzeiger->werte.min, laufzeiger->werte.max);
					laufzeiger->werte.sendestatus = 1;
					boolean = 0;


				default:
					cls();
					break;
				}

			}else {
				suchZeiger = laufzeiger;

				//Suche ob weitere Elemente gleicher Sendeart in der Liste VOR dem Element sind
				if (eingabe == 'v') {
					while (suchZeiger != NULL && suchZeiger->sendeart != auswahl) {
						suchZeiger = suchZeiger->vor;
					}
					if (suchZeiger != NULL) {
							laufzeiger = suchZeiger;
						}
						else {
							printf("Das ist das erste Element!\n");
							laufzeiger = laufzeiger->nach;
						}
					

				//Suche ob weitere Elemente gleicher Sendeart in der Liste NACH dem Element sind
				}else if (eingabe == 'n') {
				

					while (suchZeiger != NULL && suchZeiger->sendeart != auswahl) {
						suchZeiger = suchZeiger->nach;
					}

					if (suchZeiger != NULL) {
						laufzeiger = suchZeiger;
					}
					else {
							printf("Das ist das letzte Element!\n");
							laufzeiger = laufzeiger->vor;
						}
					}

				}	
			
		}else {
				boolean = 0;
				printf("Es sind keine Nachrichten vorhanden!\n");
			}
		
		} while (boolean);

}
