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
#include "Socket.h"
#include "Quoridor.h"
#include "GameLogic.h"

field pola[FIELD_SIZE][FIELD_SIZE];
point PLAYER;
point OPPONENT;
int isClient;

int main(int argc, char *argv[])
{
    initSocket();

    initPlayers();

    gtk_init(&argc, &argv);

    init();

    gtk_main();

    init(argc, argv);

    puts("exit");

    return EXIT_SUCCESS;
}

void initPlayers()
{
    PLAYER = malloc(sizeof(point*));
    PLAYER->x = FIELD_SIZE-1;
    PLAYER->y = FIELD_SIZE-1;

    OPPONENT = malloc(sizeof(point*));
    OPPONENT->x = 0;
    OPPONENT->y = 0;
}

void exitGame()
{
    char m[] = {25,'e'};
    sendMessage(m,isClient);
    endConnection();
    gtk_main_quit();
    exit(0);
}


void imReady()
{
    char msg[] = {"start"};
    puts("Ready");
    sendMessage(msg,1);
    startClient_label();
    g_timeout_add(100, waitForStart,NULL);
}

int  isOpponentConnected()
{
    if(waitForConnection())
        return 1;

    start_label();
    return 0;
}

int canStartGame()
{
    char mes[MAX_MES_LN];
    char check[]= {"start"};
    if(reciveData(mes, 0))return 1;
    for(int i = 0; i<5; i++)
    {
        if(mes[i]!=check[i])
            return 1;
    }
    update_start_label();
    start_button();
    return 0;
}

int waitForStart()
{
    client_button();
    char mes[MAX_MES_LN];
    char check[]= {"start"};

    if(reciveData(mes, 1))return 1;
    for(int i = 0; i<5; i++)
    {
        if(mes[i]!=check[i])
            return 1;
    }

    puts("Recived data");

    StartGameClient();
    return 0;
}

void StartGameServer()
{
    puts("Initialization game...");
    char nick[30];
    getNick(nick);
    sendMessage("start",0);
    while(reciveData(opponentNick,0));
    sendMessage(nick,0);
    isPlayerMove = 1;
    isClient=0;
    build_game();
    puts(" done.");
    g_timeout_add(300, analyzeMessenge,NULL);
}

void StartGameClient()
{
    puts("Initialization game...");
    char nick[30];
    getNick(nick);
    sendMessage(nick,1);
    while(reciveData(opponentNick,1));
    isClient=1;
    build_game();
    puts(" done.");
    g_timeout_add(300, analyzeMessenge,NULL);

}

int analyzeMessenge()
{
    gchar wejscie[MAX_MES_LN];

    if(reciveData(wejscie,isClient))return 1;

    if(wejscie[0]==25 && wejscie[1]== 'e')
    {
        updateBuforIn("--->Opponent has disconnected!<---");
        return 0;
    }

    char print[MAX_MES_LN+5];
    strcpy(print,opponentNick);
    strcat(print, ": ");
    strcat(print,wejscie);
    strcat(print,"\n");
    updateBuforIn(print);
    return 1;
}

void prepareToSendMessenge(GtkWidget *w, GtkWidget *text)
{
    (void)w;
    char wejscie[MAX_MES_LN+5];

    sendMessage((char *)gtk_entry_get_text (GTK_ENTRY (text)),isClient);

    getNick(wejscie);
    strcat(wejscie, ": ");
    strcpy(wejscie+strlen(wejscie),gtk_entry_get_text (GTK_ENTRY (text)));
    strcat(wejscie,"\n");
    updateBuforOut(wejscie);
}

