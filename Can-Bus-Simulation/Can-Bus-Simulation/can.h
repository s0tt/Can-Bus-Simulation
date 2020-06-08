#pragma once

#ifndef can_h
#define can_h

#define _CRT_SECURE_NO_WARNINGS
#define cls() (system("cls"))
#define seekfunc() (fseek(stdin, 0, SEEK_END))

void entferneZeichen(char* string, char zeichen);
int DezimalzuBinaer(int zahl);
int StringEingabe(char text[], char *speicherort, int laenge);
int achtbiteingabe(unsigned int *speicherort);
int decimalEingabe(char text[],int *speicherort, int wertebereichAN, int min, int max);
int schreibeLeer(FILE *datei, int anzahl);
int stellenzahl(int zahl);

#endif 