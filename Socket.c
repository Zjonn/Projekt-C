/*
 * Socket.c
 *
 *  Created on: 17.01.2017
 *      Author: danie_000
 */

#include "Socket.h"
#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#include <sys/time.h>

WSADATA wsa;
SOCKET s, newSocket;

struct timeval timeout;

struct sockaddr_in server, client;
//Inicjuje soket
int initSocket()
{
    puts("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    puts("Initialised.");

    //Create a socket
    if((s = socket(AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    puts("Socket created.");

    return 0;
}
//inicjuje server
int initServer(int port)
{
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    server.sin_addr.s_addr = INADDR_ANY;

    puts("Binding...");
    //Bind

    if( bind(s,(struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d ", WSAGetLastError());
        return 1;
    }
    puts("Bind done");

    listen(s, 1);
    puts("Waiting for incoming connections...");
    return 0;
}
//Inicjujê socket jako clienta
int initClient(char ip[], int port)
{
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

    puts("Connected");
    return 0;
}
//Zamyka po³aczenie
void endConnection()
{
    puts("Closing connection....");
    closesocket(s);
    WSACleanup();
    puts("Closed.");
}
//Resetuje socket
void resetSocket()
{
    endConnection();
    initSocket();
}

//Wysy³a dane ktorych nie widzi u¿ytkownik
int sendGameData(char messenge[], int isClient)
{
    char sys[300];
    memset(sys,0,300);
    sys[0]= 7;
    strcat(sys, messenge);
    switch(isClient)
    {
    case 1:
        if( send(s, sys, strlen(sys)+1, 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        break;
    case 0:
        if( send(newSocket, sys, strlen(sys)+1, 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        break;
    }
    puts("GData Send");
    return 0;
}
//Odbiera dane
int reciveData(char messenge[], int isClient)
{
    fd_set readSet;
    int recv_size;
    FD_ZERO(&readSet);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    switch(isClient)
    {
    case 0:
        FD_SET(newSocket, &readSet);
        if(select(newSocket, &readSet, NULL, NULL,  &timeout) == 0) return 1;
        if((recv_size = recv(newSocket, messenge, MAX_MES_LN, 0)) == SOCKET_ERROR)
        {
            puts("recv failed");
            return 2;
        }
        break;
    case 1:
        FD_SET(s, &readSet);
        if(select(s, &readSet, NULL, NULL,  &timeout) == 0) return 1;
        if((recv_size = recv(s, messenge, MAX_MES_LN, 0)) == SOCKET_ERROR)
        {
            puts("recv failed");
            return 2;
        }
        break;
    }

    puts("Reply received");
    return 0;
}
//Wysy³a wiadomosci na czacie
int sendMessage(char messenge[], int isClient)
{
    switch(isClient)
    {
    case 1:
        if( send(s, messenge, strlen(messenge)+1, 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        break;
    case 0:
        if( send(newSocket, messenge, strlen(messenge)+1, 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        break;
    }
    puts("Data Send");
    return 0;
}
//Zwraca ip komputera na ktorym jestesmy
int myIP(char ip[])
{
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
    {
        printf( "Error %d when getting local host name.\n", WSAGetLastError());
        return 1;
    }
    struct hostent *phe = gethostbyname(ac);
    if (phe == 0)
    {
        puts( "Bad host lookup.");
        return 1;
    }

    for (int i = 0; phe->h_addr_list[i] != 0; ++i)
    {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf( "Address   %d: %s \n",i,inet_ntoa(addr));
        strcpy(ip,inet_ntoa(addr));
    }
    return 0;
}
//Czeka na polaczenie
int waitForConnection()
{
    int c;
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(s, &readSet);

    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    c = sizeof(struct sockaddr_in);
    if(select(s, &readSet, NULL, NULL,  &timeout) == 1)
    {
        newSocket = accept(s, (struct sockaddr *)&client, &c);
        if (newSocket == INVALID_SOCKET)
        {
            printf("accept failed with error code : %d", WSAGetLastError());
            return 1;
        }
        puts("Connection accepted");
        return 0;
    }
    return 1;
}

