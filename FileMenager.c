/*
 * FileMenager.c
 *
 *  Created on: 29.01.2017
 *      Author: danie_000
 */

#include <stdlib.h>
#include <stdio.h>

#include "Quoridor.h"
#include "FileMenager.h"
#include "GameLogic.h"

int printError(FILE *f, char gameName[]);
//Zapisuje mape/plansze?
int saveMap(char gameName[])
{
    FILE *f = fopen(gameName, "w");

    if (!printError(f, gameName))
    {
        return 0;
    }

    fprintf(f,"%d\n",playerWalls);
    fprintf(f,"%d\n",opponentWalls);
    fprintf(f,"%d\n",PLAYER->x);
    fprintf(f,"%d\n",PLAYER->y);
    fprintf(f,"%d\n",OPPONENT->x);
    fprintf(f,"%d\n",OPPONENT->y);

    for (int i = 0; i < FIELD_SIZE; i++)
    {
        if(i%2==0)
            for (int j = 0; j < FIELD_SIZE; j++)
            {
                fprintf(f, "%d\n", pola[i][j]->i);
            }
        else
            for (int j = 0; j <9; j++)
            {
                fprintf(f, "%d\n", pola[i][2*j]->i);
            }

    }
    fclose(f);
    return 1;

}
//Wczytuje mape
int loadMap(char gameName[])
{
    FILE *f = fopen(gameName, "r");

    if (!printError(f, gameName))
    {
        return 0;
    }

    fscanf(f,"%d",&playerWalls);
    fscanf(f,"%d",&opponentWalls);

    gtk_widget_set_name(pola[PLAYER->y][PLAYER->x]->button,"wladcaCieni");

    fscanf(f,"%d",&PLAYER->x);
    fscanf(f,"%d",&PLAYER->y);

    gtk_widget_set_name(pola[PLAYER->y][PLAYER->x]->button,"player");

    gtk_widget_set_name(pola[OPPONENT->y][OPPONENT->x]->button,"wladcaCieni");

    fscanf(f,"%d",&OPPONENT->x);
    fscanf(f,"%d",&OPPONENT->y);

    gtk_widget_set_name(pola[OPPONENT->y][OPPONENT->x]->button,"enemy");

    for (int i = 0; i <FIELD_SIZE; i++)
    {
            if(i%2==0)
                for (int j = 0; j < FIELD_SIZE; j++)
                {
                     fscanf(f,"%d",&pola[i][j]->i);
                }
            else
                for (int j = 0; j <9; j++)
                {
                     fscanf(f,"%d",&pola[i][2*j]->i);
                }
    }
     puts("test");
    fclose(f);
    return 1;
}
//Wypisuje bledy
int printError(FILE *f, char gameName[])
{
    if (!f)
    {
        printf("Nie moge uzyskac dostepu do pliku: %s", gameName);
        return 0;
    }
    printf("Otworzono plik: %s", gameName);
    return 1;
}
