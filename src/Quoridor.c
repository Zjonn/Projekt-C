/*
 ============================================================================
 Name        : Quoridor.c
 Author      : Daniel Dubiel
 Version     : 1.0
 Copyright   : PizzaWare licence
 Description : Gra Quoridor w C
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "GUI.h"
#include "Quoridor.h"

Field pola[SIZE][SIZE];
Pos PLAYER;
Pos OPPONENT;

int main(int argc, char *argv[]) {
	initPlayers();

	gtk_init(&argc, &argv);

	init();

	gtk_main();

	return EXIT_SUCCESS;
}

void initPlayers() {
	PLAYER = malloc(sizeof(Pos*));
	PLAYER->x = SIZE;
	PLAYER->y = SIZE;

	OPPONENT = malloc(sizeof(Pos*));
	OPPONENT->x = 0;
	OPPONENT->y = 0;
}
