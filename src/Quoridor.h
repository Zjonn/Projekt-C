/*
 * Quoridor.h
 *
 *  Created on: 21.01.2017
 *      Author: danie_000
 */

#ifndef QUORIDOR_H_
#define QUORIDOR_H_

#include <gtk/gtk.h>

#define SIZE 17

typedef struct field* Field;

typedef struct pos* Pos;

struct field {
	GtkWidget *button;
	int i;
};

struct pos{
	int x;
	int y;
};

extern Pos PLAYER;

extern Pos OPPONENT;

extern Field pola[SIZE][SIZE];

#endif /* QUORIDOR_H_ */
