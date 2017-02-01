/*
 * FileMenager.c
 *
 *  Created on: 29.01.2017
 *      Author: danie_000
 */

#include <stdlib.h>
#include <stdio.h>

#include "Quoridor.h"
#include "FileMenager.h"

int printError(FILE *f, char gameName[]);

int saveFile(char gameName[]) {
	FILE *f = fopen(gameName, "w");

	if (!printError(f, gameName)) {
		return 0;
	}

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			fprintf(f, "%d", pola[i][j]->i);
		}
		fprintf(f, "\n");
	}
	return 1;
}

int loadFile(char gameName[]) {
	FILE *f = fopen(gameName, "r");

	if (!printError(f, gameName)) {
		return 0;
	}

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			int tmp = fgetc(f);
			if (tmp == '\n')
				tmp = fgetc(f);
			pola[i][j]->i = tmp;
		}
	}
	return 1;
}

int printError(FILE *f, char gameName[]) {
	if (!f) {
		printf("Nie moge uzyskac dostepu do pliku: %s", gameName);
		return 0;
	}
	printf("Otworzono plik: %s", gameName);
	return 1;
}
