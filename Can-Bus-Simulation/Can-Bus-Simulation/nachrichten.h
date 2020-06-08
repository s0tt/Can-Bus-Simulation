#pragma once
#define MAX_NAME 40
#define MAX_BESCHREIBUNG_n 41

#ifndef nachrichten_h
#define nachrichten_h

typedef struct wertebereich {
	int min;
	int max;
	int defaultwert;
	int inhalt;
	int sendestatus; //1 gesendet / 0 ausstehend
} nachrichtenwerte;

typedef struct identitaet {
	unsigned int prioritaet : 8;
	unsigned int sender : 8;
	unsigned int empfaenger : 8;
}nachrichtenid;

typedef struct canmessage {
	char name[MAX_NAME];
	int sendeart; //1 - zyklisch, 0 -spontan
	int sendezyklus;
	nachrichtenwerte werte;
	nachrichtenid id;
	struct canmessage *nach;
	struct canmessage *vor;
}cannachricht;

cannachricht *nachrichtenanfang;

int hinzufuegen_n(unsigned int prioriaet, unsigned int empfaenger, unsigned int sender, char name[MAX_NAME], int sendeart, int sendezyklus, int min, int max, int defaultwert);
int dateneingabe_n();
void loeschen_n(cannachricht *zeiger_n);
int ansehen_n();
int editieren_n(cannachricht *zeiger_n);
int speichernachrichten();
int ladenachrichten(FILE *datei);

#endif