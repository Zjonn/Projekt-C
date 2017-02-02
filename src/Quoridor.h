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

typedef struct field* field;

typedef struct point* point;

struct field {
	GtkWidget *button;
	int i;
};

struct point{
	int x;
	int y;
};

extern point PLAYER;

extern point OPPONENT;

extern field pola[SIZE][SIZE];

void initPlayers();

#endif /* QUORIDOR_H_ */
