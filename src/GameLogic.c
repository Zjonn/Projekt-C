/*
 * GameLogic.c
 *
 *  Created on: 29.01.2017
 *      Author: danie_000
 */

#include "GameLogic.h"
#include "Quoridor.h"
#include <math.h>
//1-Gracz 0-przeciwnik
int makeMove(int x, int y) {
	if (!isPlayerMove || (x = PLAYER->x && y == PLAYER->y))
		return 0;
	if ((abs(PLAYER->x - x) == 2 || abs(PLAYER->x - x) == 0)
			&& (abs(PLAYER->y - y) == 2 || abs(PLAYER->y - y) == 0)
			&& abs(PLAYER->x - x) != abs(PLAYER->y - y) && OPPONENT->x != x
			&& OPPONENT->y != y) {
		PLAYER->x = x;
		PLAYER->y = y;
		return 1;
	} else {
		if (x == PLAYER->x && y != PLAYER->y) {
			if (abs(PLAYER->y - y == 4))
				return 1;
		} else if (x != PLAYER->x && y == PLAYER->y) {
			if (abs(PLAYER->x - x == 4))
				return 1;
		}
		return 0;
	}
	return 0;
}

int placeWall(int x, int y) {
	if (!isPlayerMove)
		return 0;

}

int isCorrectWall(int x, int y) {

}

int isGameEnd() {

}
