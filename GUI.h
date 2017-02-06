/*
 * GUI.h
 *
 *  Created on: 12.01.2017
 *      Author: danie_000
 */

#ifndef GUI_H_
#define GUI_H_

void init();

void hide_all();

void build_mainMenu();

void build_game();

void build_server();

void build_client();

void build_clientStartBox();

void build_serverStartBox();

void start_label();

void update_start_label();

void start_button();

void client_button();

void startClient_label();

void getNick(char nick[]);

void updateBuforOut();

void updateBuforIn(char wejscie[]);

#endif /* GUI_H_ */
