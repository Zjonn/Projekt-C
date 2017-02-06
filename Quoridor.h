/*
 * Quoridor.h
 *
 *  Created on: 21.01.2017
 *      Author: danie_000
 */

#ifndef QUORIDOR_H_
#define QUORIDOR_H_

#include <gtk/gtk.h>

#define FIELD_SIZE 17

typedef struct field* field;

typedef struct point* point;

struct field
{

    GtkWidget *button, *wallButton;
    int i;
};

struct point
{
    int x;
    int y;
};

extern point PLAYER;

extern point OPPONENT;

extern field pola[FIELD_SIZE][FIELD_SIZE];

extern int isClient;

char opponentNick[50];

void imReady();

void initPlayers();

void exitGame();

int waitForStart();

int canStartGame();

void StartGameClient();

void StartGameServer();

void opponentDisconnect();

int isOpponentConnected();

void prepareToSendMessenge(GtkWidget *w, GtkWidget *text);

int analyzeMessenge();

#endif /* QUORIDOR_H_ */

