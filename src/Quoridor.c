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

field pola[SIZE][SIZE];
point PLAYER;
point OPPONENT;

int main(int argc, char *argv[]) {

	initPlayers();

	gtk_init(&argc, &argv);

	init();

	gtk_main();

	init(argc, argv);

	return EXIT_SUCCESS;
}

void initPlayers() {
	PLAYER = malloc(sizeof(point*));
	PLAYER->x = SIZE-1;
	PLAYER->y = SIZE-1;

	OPPONENT = malloc(sizeof(point*));
	OPPONENT->x = 0;
	OPPONENT->y = 0;
}
