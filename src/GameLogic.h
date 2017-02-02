/*
 * GameLogic.h
 *
 *  Created on: 29.01.2017
 *      Author: danie_000
 */

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include <stdlib.h>
#include "Quoridor.h"

typedef struct edge* edge;

typedef enum direction direction;

struct edge {
	int isVisited;
	point point;
	edge nodes[4];
};

enum direction {
	LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3
};

int isPlayerMove = 0;

int isEnd = 0;

int makeMove(int x, int y);

int isCorrectWall(point P1, point P2);

int isGameCanEnd(point P1, point P2);

int isGameEnd();

edge buildGraph(point p);

edge destroyGraph(edge e);

#endif /* GAMELOGIC_H_ */