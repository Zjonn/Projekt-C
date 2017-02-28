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
int playerWalls = 5, opponentWalls = 5;
int isEnd = 0;
//Sprawdza poprawnosc ruchu
int isCorrectMove(point p)
{
    int x = p->y;
    int y = p->x;
    if (!isPlayerMove || (x == PLAYER->y && y == PLAYER->x))
        return 0;
    if (OPPONENT->y != x || OPPONENT->x != y)
    {
        if ((PLAYER->y - x == 2 && PLAYER->x == y && pola[x + 1][y]->i == 0)
                || (PLAYER->y - x == -2 && PLAYER->x == y
                    && pola[x - 1][y]->i == 0)
                || (PLAYER->y == x && PLAYER->x - y == 2
                    && pola[x][y + 1]->i == 0)
                || (PLAYER->y == x && PLAYER->x - y == -2
                    && pola[x][y - 1]->i == 0))
            return 1;

    }
    else if(OPPONENT->y == x && OPPONENT->x == y)
    {
        if (PLAYER->y - x == 2 && PLAYER->x == y && pola[x + 1][y]->i == 0 && PLAYER ->y - 4>=0)
        {
            p->y = PLAYER->y-4;
            return 1;
        }
        else if (PLAYER->y - x == -2 && PLAYER->x == y
                 && pola[x - 1][y]->i == 0 && PLAYER ->y + 4<FIELD_SIZE)
        {
            p->y = PLAYER->y+4;
            return 1;
        }
        else if (PLAYER->y == x && PLAYER->x - y == 2
                 && pola[x][y + 1]->i == 0 && PLAYER ->x - 4>=0)
        {
            p->x = PLAYER->x-4;
            return 1;
        }
        else if (PLAYER->y == x && PLAYER->x - y == -2
                 &&  pola[x][y - 1]->i == 0 && PLAYER ->x + 4<FIELD_SIZE)
        {
            p->x = PLAYER->x+4;
            return 1;
        }
    }
    return 0;
}
//Sprawdza poprawnosc stawianej sciany
int isCorrectWall(point P1, point P2)
{
    int wallsInLine = 0;
    if (!isPlayerMove|| playerWalls==0 || pola[P1->y][P1->x]->i == 1
            || pola[P2->y][P2->x]->i == 1)
        return 0;
    if (abs(P1->x - P2->x) == 2 && P1->y == P2->y)
    {
        if(P1->x - P2->x>0)
            for(int i = P1->y+1; i<FIELD_SIZE;  i+=2)
            {
                if(pola[i][P1->x-1]->i==1)
                    wallsInLine++;
            }
        if(P1->x - P2->x<0)
            for(int i = P1->y+1; i<FIELD_SIZE; i+=2)
            {
                if(pola[i][P1->x+1]->i==1)
                    wallsInLine++;
            }
    }
    else if(abs(P1->y - P2->y) == 2 && P1->x == P2->x)
    {
        if(P1->y - P2->y>0)
            for(int i = P1->x+1; i<FIELD_SIZE;  i+=2)
            {
                if(pola[P1->y-1][i]->i==1)
                    wallsInLine++;
            }
        if(P1->y - P2->y<0)
            for(int i = P1->x+1; i<FIELD_SIZE;  i+=2)
            {
                if(pola[P1->y+1][i]->i==1)
                    wallsInLine++;
            }
    }
    if(wallsInLine%2==0)
        return isGameCanEnd(P1, P2);
    return 0;
}
//Sprawdza czy po postawieniu sciany da sie skonczyc gre
int isGameCanEnd(point P1, point P2)
{
    pola[P1->y][P1->x]->i = 1;
    pola[P2->y][P2->x]->i = 1;
    puts("Finding path....");
    clearPlayer();
    int result = isReachable(PLAYER,0);
    clearPlayer();
    if(! isReachable(OPPONENT,FIELD_SIZE-1))
        result = 0 ;
    pola[P1->y][P1->x]->i = 0;
    pola[P2->y][P2->x]->i = 0;
    puts(" done.");
    clearPlayer();
    return result;
}
//Sprawdza czy gra sie juz nie skonczyla
int isGameEnd()
{
    if (PLAYER->y == 0 || isEnd)
        return 1;
    return 0;
}
//Pomoga w szukaniu drogi
void clearPlayer()
{
    for(int i = 0; i<FIELD_SIZE; i+=2)
    {
        for(int j = 0; j<FIELD_SIZE; j+=2)
        {
            pola[i][j]->i=0;
        }
    }
}
//Sprawdza czy istnieje trasa z pozycji gracza do funktu finish
int isReachable(point p,int finish)
{
    if(pola[p->y][p->x]->i==1||p->x>=FIELD_SIZE || p->y>=FIELD_SIZE || p->x<0|| p->y<0)
    {
        return 0;
    }
    if(p->y== finish)return 1;

    point tmp = malloc(sizeof(point*));;
    tmp->x = p->x;
    tmp->y = p->y;

    pola[p->y][p->x]->i=1;

    if(p->y-2>=0 &&           pola[p->y-1][p->x]->i==0)
    {
        tmp->y = p->y-2;
        if(isReachable(tmp, finish))return 1;
    }
    if(p->y+2<FIELD_SIZE &&          pola[p->y+1][p->x]->i==0)
    {
        tmp->y = p->y+2;
        if(isReachable(tmp, finish))return 1;
    }
    if(p->x-2>=0 &&         pola[p->y][p->x-1]->i==0)
    {
        tmp->x = p->x-2;
        if( isReachable(tmp, finish))return 1;
    }
    if(p->x+2<FIELD_SIZE &&         pola[p->y][p->x+1]->i==0)
    {
        tmp->x = p->x+2;
        if(isReachable(tmp, finish))return 1;
    }
    free(tmp);
    return 0;
}


