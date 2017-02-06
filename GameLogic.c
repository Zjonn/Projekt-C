/*
 * GameLogic.c
 *
 *  Created on: 29.01.2017
 *      Author: danie_000
 */

#include "GameLogic.h"
#include <math.h>
//1-Gracz 0-przeciwnik

int isPlayerMove = 0;
int isEnd = 0;

int isCorrectMove(int x, int y)
{
    if (!isPlayerMove || (x = PLAYER->x && y == PLAYER->y))
        return 0;
    if (OPPONENT->x != x && OPPONENT->y != y)
    {
        if ((PLAYER->x - x == 2 && PLAYER->y == y && pola[x - 1][y]->i == 0)
                || (PLAYER->x - x == -2 && PLAYER->y == y
                    && pola[x + 1][y]->i == 0)
                || (PLAYER->x == x && PLAYER->y - y == 2
                    && pola[x][y - 1]->i == 0)
                || (PLAYER->x == x && PLAYER->y - y == -2
                    && pola[x][y + 1]->i == 0))
            return 1;
    }
    else
    {
        if (x == PLAYER->x && y != PLAYER->y)
        {
            if (abs(PLAYER->y - y == 4))
                return 1;
        }
        else if (x != PLAYER->x && y == PLAYER->y)
        {
            if (abs(PLAYER->x - x == 4))
                return 1;
        }
    }
    return 0;
}

int isCorrectWall(point P1, point P2)
{
    if (!isPlayerMove || pola[P1->x][P1->y]->i == 1
            || pola[P2->x][P2->y]->i == 1)
        return 0;

    if ((abs(P1->x - P2->x) == 2 && P1->y == P2->y)
            || (abs(P1->y - P2->y) == 2 && P1->x == P2->x))
        return isGameCanEnd(P1, P2);
    return 0;
}

int isGameCanEnd(point P1, point P2)
{
    pola[P1->x][P1->y]->i = 1;
    pola[P2->x][P2->y]->i = 1;

    edge theLastGuardian = buildGraph(PLAYER);
    int result = checkGraph(theLastGuardian);
    destroyGraph(theLastGuardian);
    pola[P1->x][P1->y]->i = 0;
    pola[P2->x][P2->y]->i = 0;
    return result;
}

int isGameEnd()
{
    if (PLAYER->y == 0 || isEnd)
        return 1;
    return 0;
}

int checkGraph(edge e)
{
    if (e == NULL)
        return 0;
    else if (e->point->y == 0)
        return 1;
    if (checkGraph(e->nodes[LEFT]) || checkGraph(e->nodes[RIGHT])
            || checkGraph(e->nodes[UP]) || checkGraph(e->nodes[DOWN]))
        return 1;
    return 0;
}

edge buildGraph(point p)
{
    if (p->x < 0 || p->x >= FIELD_SIZE || p->y < 0 || p->y >= FIELD_SIZE
            || pola[p->x][p->y]->i == 1)
        return NULL;
    pola[p->x][p->y]->i = 1;
    edge e = malloc(sizeof(edge*));
    e->isVisited = 1;
    e->point = p;

    if (p->x - 2 > 0 && pola[p->x - 1][p->y]->i == 0)
    {
        point t = malloc(sizeof(point*));
        t->x = p->x - 2;
        t->y = p->y;
        e->nodes[LEFT] = buildGraph(t);
    }
    if (p->x + 2 > 0 && pola[p->x + 1][p->y]->i == 0)
    {
        point t = malloc(sizeof(point*));
        t->x = p->x + 2;
        t->y = p->y;
        e->nodes[RIGHT] = buildGraph(t);
    }
    if (p->y - 2 > 0 && pola[p->x][p->y - 1]->i == 0)
    {
        point t = malloc(sizeof(point*));
        t->x = p->x;
        t->y = p->y - 2;
        e->nodes[UP] = buildGraph(t);
    }
    if (p->y + 2 > 0 && pola[p->x][p->y + 1]->i == 0)
    {
        point t = malloc(sizeof(point*));
        t->x = p->x;
        t->y = p->y + 2;
        e->nodes[DOWN] = buildGraph(t);
    }
    return e;
}

edge destroyGraph(edge e)
{
    if (e == NULL)
    {
        return NULL;
    }
    destroyGraph(e->nodes[LEFT]);
    destroyGraph(e->nodes[RIGHT]);
    destroyGraph(e->nodes[UP]);
    destroyGraph(e->nodes[DOWN]);

    pola[e->point->x][e->point->y]->i = 0;
    free(e->point);
    free(e);

    return NULL;
}
