/*
 ============================================================================
 Name        : Quoridor.c
 Author      : Daniel Dubiel
 Version     : 4.2
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
#include "FileMenager.h"

fields pola[FIELD_SIZE][FIELD_SIZE];
point PLAYER;
point OPPONENT;
int isClient;

int main(int argc, char *argv[])
{
    initSocket();

    initPlayers();

    initGTK(argc, argv);

    gtk_main();

    puts("exit");

    return EXIT_SUCCESS;
}
//Funkcja rozpoczynajaca grê
void imReady()
{
    char msg[] = {"start"};
    puts("Ready");
    sendMessage(msg,1);
    startClient_label();
    g_timeout_add(100, waitForStart,NULL);
}
//Inicjalizuje pozycje graczy
void initPlayers()
{
    PLAYER = malloc(sizeof(point*));
    PLAYER->x = FIELD_SIZE/2;
    PLAYER->y = FIELD_SIZE-1;

    OPPONENT = malloc(sizeof(point*));
    OPPONENT->x = FIELD_SIZE/2;
    OPPONENT->y = 0;
}
//Wychodzi z gry
void exitGame()
{
    char m[] = {'e'};
    sendGameData(m,isClient);
    endConnection();
    gtk_main_quit();
    exit(0);
}
//Zapisuje stan  gry do pliku
void saveGame()
{
    puts("test");
    if(!isPlayerMove)
    {
        updateBuforIn("--->You can save game only when is your tour<---");
        return;
    }
    time_t czas;
    struct tm * ptr;
    time( & czas );
    ptr = localtime( & czas );
    char data[100];
    strftime( data, 80, "%H%M%S.quoridor", ptr );
    updateBuforIn("----->Game saved to file: <------");
    updateBuforIn(data);
    updateBuforIn("\n");
    saveMap(data);
}
//Wczytuje stan gry z pliku
void loadGame()
{
    char map[FIELD_SIZE*FIELD_SIZE+20];
    memset(map,0,FIELD_SIZE*FIELD_SIZE+20);
    map[0] = 'f';
    map[1] = playerWalls+48;
    map[2] = opponentWalls+48;
    map[3] = PLAYER->x+23;
    map[4] = PLAYER->y+23;
    map[5] = OPPONENT->x+23;
    map[6] = OPPONENT->y+23;
    int help = 7;
    for(int i = 0; i<FIELD_SIZE; i++)
    {
        if(i%2==0)
            for(int j = 0; j<FIELD_SIZE; j++)
            {
                map[help] = pola[i][j]->i+33 ;
                help++;

                if(pola[i][j]->i==1)
                {
                    point p = malloc(sizeof(point));
                    p->x = j;
                    p->y = i;
                    updateMap(p);
                    free(p);
                }
            }
        else
        {
            for(int j = 0; j<FIELD_SIZE; j+=2)
            {
                map[help] = pola[i][j]->i+33 ;
                help++;

                if(pola[i][j]->i==1)
                {
                    point p = malloc(sizeof(point));
                    p->x = j;
                    p->y = i;
                    updateMap(p);
                    free(p);

                }
            }
        }
    }
    sendGameData(map,isClient);
}
//Sprawdza czy mozna postawic scianê
void tryPlaceWall(GtkWidget *button, point p)
{
    point p2 = malloc(sizeof(point*));
    if(p->x == 0)
    {
        p2->y = p->y;
        p2->x = p->x+2;
    }
    else if(p->x == FIELD_SIZE-1)
    {
        p2->y = p->y;
        p2->x = p->x-2;
    }
    else if(p->y == 0)
    {
        p2->x = p->x;
        p2->y = p->y+2;
    }
    else if(p->y == FIELD_SIZE-1)
    {
        p2->x = p->x;
        p2->y = p->y-2;
    }
    else if(button == pola[p->y][p->x]->button)
    {
        if(p->x%2==0)
        {
            p2->x = p->x-2;
            p2->y =  p->y;
        }
        else
        {
            p2->x = p->x;
            p2->y =  p->y-2;
        }
    }
    else
    {
        if(p->x%2==0)
        {
            p2->x = p->x+2;
            p2->y =  p->y;
        }
        else
        {
            p2->x = p->x;
            p2->y =  p->y+2;
        }

    }

    if(!isCorrectWall(p, p2))return;

    pola[p->y][p->x]->i=1;
    pola[p2->y][p2->x]->i=1;

    if(p->x == 0 || p->x == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"legolas");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"bilbo");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"bilbo");

    }
    else if (p->y == 0 || p->y == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"saruman");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"sauron");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"sauron");

    }
    else if(p2->x == 0 || p2->x == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"legolas");
        gtk_widget_set_name(pola[p->y][p->x]->button,"bilbo");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"bilbo");

    }
    else if (p2->y == 0 || p2->y == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"saruman");
        gtk_widget_set_name(pola[p->y][p->x]->button,"sauron");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"sauron");

    }
    else if(p->x%2==0)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"bilbo");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"bilbo");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"bilbo");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"bilbo");
    }
    else
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"sauron");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"sauron");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"sauron");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"sauron");
    }

    isPlayerMove=0;
    playerWalls--;
    char toPlayer[MAX_MES_LN] = {"------>You have "};
    char num[2];
    sprintf(num, "%d", playerWalls);
    strcat(toPlayer, num);
    strcat(toPlayer, " walls left<------\n");
    updateBuforIn(toPlayer);
    char mes[6];
    mes[0]='w';
    mes[1]=(FIELD_SIZE-1)-p->x+33;
    mes[2]=(FIELD_SIZE-1)-p->y+33;
    mes[3]=(FIELD_SIZE-1)-p2->x+33;
    mes[4]=(FIELD_SIZE-1)-p2->y+33;
    mes[5]='\0';

    sendGameData(mes,isClient);
    free(p2);
}
//Koloruje sciany po wczytaniu mapy
void updateMap(point p)
{
    if(p->x == 0 || p->x == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"legolas");

    }
    else if (p->y == 0 || p->y == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"saruman");

    }
    else if(p->x%2==0)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"bilbo");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"bilbo");
    }
    else
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"sauron");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"sauron");

    }

}
//Koloruje sciany po ruchu przeciwnika
void updateWalls(point p,point p2)
{
    pola[p->y][p->x]->i=1;
    pola[p2->y][p2->x]->i=1;

    if(p->x == 0 || p->x == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"legolas");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"bilbo");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"bilbo");

    }
    else if (p->y == 0 || p->y == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"saruman");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"sauron");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"sauron");

    }
    else if(p2->x == 0 || p2->x == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"legolas");
        gtk_widget_set_name(pola[p->y][p->x]->button,"bilbo");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"bilbo");

    }
    else if (p2->y == 0 || p2->y == FIELD_SIZE -1)
    {
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"saruman");
        gtk_widget_set_name(pola[p->y][p->x]->button,"sauron");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"sauron");

    }
    else if(p->x%2==0)
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"bilbo");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"bilbo");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"bilbo");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"bilbo");
    }
    else
    {
        gtk_widget_set_name(pola[p->y][p->x]->button,"sauron");
        gtk_widget_set_name(pola[p->y][p->x]->wallButton,"sauron");
        gtk_widget_set_name(pola[p2->y][p2->x]->button,"sauron");
        gtk_widget_set_name(pola[p2->y][p2->x]->wallButton,"sauron");
    }
}
//Sprawdza czy mozna wykonac ruch
void tryMakeMove(GtkWidget *button, point p)
{
    (void)button;
    point tmp = malloc(sizeof(point*));
    tmp->x = p->x;
    tmp->y = p->y;
    if(!isCorrectMove(tmp))return;
    gtk_widget_set_name(pola[PLAYER->y][PLAYER->x]->button,"wladcaCieni");
    PLAYER->x=tmp->x;
    PLAYER->y=tmp->y;
    gtk_widget_set_name(pola[PLAYER->y][PLAYER->x]->button,"player");
    free(tmp);
    isPlayerMove=0;
    char mes[3];
    mes[0]='m';
    mes[1]=(FIELD_SIZE-1)-PLAYER->x+48;
    mes[2]=(FIELD_SIZE-1)-PLAYER->y+48;
    sendGameData(mes,isClient);
    if(isGameEnd())
    {
        sendGameData("g",isClient);
        updateBuforIn("---------->You win! Good job :D<---------\n");
    }
}
//Funkcja startowa
void startGameClient()
{
    puts("Initialization game...");
    char nick[30];
    getNick(nick);
    isClient=1;
    sendMessage(nick,isClient);
    while(reciveData(opponentNick,isClient));
    build_game();
    puts(" done.");
    g_timeout_add(300, analyzeMessenge,NULL);
}
//Funkcja startowa
void startGameServer()
{
    puts("Initialization game...");
    char nick[30];
    getNick(nick);
    isClient=0;
    sendMessage("start",isClient);
    while(reciveData(opponentNick,isClient));
    sendMessage(nick,isClient);
    isPlayerMove = 1;
    build_game();

    char filename[20];
    memset(filename,0,20);
    strcpy(filename,gtk_entry_get_text (GTK_ENTRY (fileEntry)));
    if(filename[0]!='F')
    {
        if(loadMap(filename))loadGame();
    }
    puts(" done.");
    g_timeout_add(300, analyzeMessenge,NULL);
}
//Przygotowuje program do wys³ania wiadomoœci
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
//Funkcja startowa
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

    startGameClient();
    return 0;
}
//Funkcja startowa
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
//Analizuje wszystkie dana jakie program odbiera od przeciwnika
int analyzeMessenge()
{
    gchar wejscie[MAX_MES_LN];
    memset(wejscie,0,MAX_MES_LN);
    int status = reciveData(wejscie,isClient);
    if(status==1)return 1;
    if(status == 2)
    {
        updateBuforIn("-------------->Connection lost!<--------------");
        return 0;
    }
    if(wejscie[0]==7)
        switch(wejscie[1])
        {
        case 'e':
            updateBuforIn("---->Opponent has disconnected!<----");
            return 0;
            break;
        case 'm':
            gtk_widget_set_name(pola[OPPONENT->y][OPPONENT->x]->button,"wladcaCieni");
            OPPONENT->x=wejscie[2]-48;
            OPPONENT->y=wejscie[3]-48;
            gtk_widget_set_name(pola[OPPONENT->y][OPPONENT->x]->button,"enemy");
            isPlayerMove=1;
            break;
        case 'w':
            ;
            point p1 = malloc(sizeof(point));
            point p2 = malloc(sizeof(point));
            p1->x = wejscie[2]-33;
            p1->y = wejscie[3]-33;
            p2->x = wejscie[4]-33;
            p2->y = wejscie[5]-33;
            updateWalls(p1,p2);
            opponentWalls--;
            isPlayerMove=1;
            break;
        case 'g':
            isPlayerMove=0;
            updateBuforIn("-------------->You lose :(<--------------");
            break;
        case 'f':
                printf("t\n");
            playerWalls=wejscie[2]-48;
            opponentWalls=wejscie[3]-48;
        printf("t\n");
            gtk_widget_set_name(pola[PLAYER->y][PLAYER->x]->button,"wladcaCieni");
            PLAYER->x=FIELD_SIZE -1-(wejscie[6]-23);
            PLAYER->y=FIELD_SIZE -1-(wejscie[7]-23);
            gtk_widget_set_name(pola[PLAYER->y][PLAYER->x]->button,"player");
        printf("t\n");
            gtk_widget_set_name(pola[OPPONENT->y][OPPONENT->x]->button,"wladcaCieni");
            OPPONENT->x=FIELD_SIZE-1 -(wejscie[4]-23);
            OPPONENT->y=FIELD_SIZE -1-(wejscie[5]-23);
            gtk_widget_set_name(pola[OPPONENT->y][OPPONENT->x]->button,"enemy");
        printf("t\n");
            int help = 8;
            for(int i = 0; i<FIELD_SIZE; i++)
            {
                printf("%d",help);
                if(i%2==0)
                    for(int j = 0; j<FIELD_SIZE; j++)
                    {
                        if(wejscie[help]-33==1)
                        {
                                printf("%d",help);
                            point p = malloc(sizeof(point));
                            p->x =FIELD_SIZE-1- j;
                            p->y =FIELD_SIZE-1- i;
                            pola[i][j]->i=1;
                            updateMap(p);
                            free(p);
                            printf("ok\n");
                        }
                        help++;

                    }
                else
                {
                    for(int j = 0; j<FIELD_SIZE; j+=2)
                    {

                        if(wejscie[help]-33==1)
                        {
                                printf("%d",help);
                            point p = malloc(sizeof(point));
                            p->x = FIELD_SIZE-1- j;
                            p->y = FIELD_SIZE-1- i;
                            pola[i][j]->i=1;
                            updateMap(p);
                            free(p);
                            printf("ok\n");
                        }
                        help++;
                    }
                }
            }

            break;
        }
    else
    {
        char print[MAX_MES_LN+10];
        strcpy(print,opponentNick);
        strcat(print, ": ");
        strcat(print,wejscie);
        strcat(print,"\n");
        updateBuforIn(print);
    }
    return 1;
}
//Sprawdza czy przeciwnik jest ju¿ po³aczony
int  isOpponentConnected()
{
    if(waitForConnection())
        return 1;

    start_label();
    return 0;
}
