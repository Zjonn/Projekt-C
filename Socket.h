/*
 * Socket.h
 *
 *  Created on: 17.01.2017
 *      Author: danie_000
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#define MAX_MES_LN 1000

#include<stdio.h>
#include<winsock2.h>
#include "Quoridor.h"



int initSocket();
int initServer(int port);
int initClient(char ip[], int port);
int sendGameData(char messenge[], int isClient);
int reciveData(char messenge[], int isClient);
int sendMessage(char messenge[], int isClient);
int myIP(char ip[]);
int waitForConnection();

void endConnection();
void resetSocket();



#endif /* SOCKET_H_ */
