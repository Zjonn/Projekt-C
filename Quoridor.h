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

typedef struct fields *fields;

typedef struct point *point;

struct fields
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

extern fields pola[FIELD_SIZE][FIELD_SIZE];

extern int isClient;

char opponentNick[50];

void imReady();

void initPlayers();

void saveGame();

void loadGame();

void exitGame();

void updateMap(point p);

void updateWalls(point p,point p2);

void tryPlaceWall(GtkWidget *button, point p);

void tryMakeMove(GtkWidget *button, point p);

void startGameClient();

void startGameServer();

void opponentDisconnect();

void prepareToSendMessenge(GtkWidget *w, GtkWidget *text);

int waitForStart();

int canStartGame();

int analyzeMessenge();

int isOpponentConnected();

#endif /* QUORIDOR_H_ */

