/*
 * GameLogic.h
 *
 *  Created on: 29.01.2017
 *      Author: danie_000
 */

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include "Quoridor.h"

int isPlayerMove = 0;

int makeMove(int x,int y);

int placeWall(int x,int y);

int isCorrectWall(int x,int y);

int enemyMoveUpdate(int x,int y);

int isGameEnd();



#endif /* GAMELOGIC_H_ */
