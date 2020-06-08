#pragma once

#define MAX_NAME_KNOTEN 10+1
#define MAX_BESCHREIBUNG 40+1

#ifndef knoten_h
#define knoten_h

//Struct CAN-BUS
typedef struct can {
	unsigned int adresse : 8;
	char name[MAX_NAME_KNOTEN];
	char beschreibung[MAX_BESCHREIBUNG];
	struct can *nach;
	struct can *vor;
}canbuselement;

int dateneingabe();
int hinzufuegen(int adresse, char name[MAX_NAME_KNOTEN], char beschreibung[MAX_BESCHREIBUNG]);
int ansehen();
void loeschen(canbuselement *zeiger);
int editieren(canbuselement *zeiger);
int speicherknoten();
int ladeknoten(FILE *datei);
#endif