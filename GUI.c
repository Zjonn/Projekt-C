/*
 * GUI.c
 *
 *  Created on: 12.01.2017
 *      Author: danie_000
 */

#include <stdbool.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "GUI.h"
#include "Quoridor.h"
#include "Socket.h"

GtkWidget *menuBox;

GtkWidget *mainMenuBox, *gameBox;

GtkWidget *clientBox, *serverBox;

GtkWidget *clientStartBox, *serverStartBox;

GtkWidget *nickEntry,*clientLabel, *ipClient, *ipLabel,*startLabel, *startLabelClient;

GtkWidget *start, *clientPort,*serverPort, *bufor, *ready, *text, *fileEntry;

int isBuilt =0;
//Inicjuje gtk
void initGTK(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    menuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_title(GTK_WINDOW(window), "Quoridor");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 1024);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(exitGame),
                     NULL);

    build_mainMenu();
    build_server();
    build_client();
    build_game();

    build_clientStartBox();

    build_serverStartBox();

    isBuilt = 1;

    gtk_box_pack_start(GTK_BOX(menuBox), mainMenuBox, false, true, true);
    gtk_box_pack_start(GTK_BOX(menuBox), serverBox, false, true, true);
    gtk_box_pack_start(GTK_BOX(menuBox), clientBox, false, true, true);
    gtk_box_pack_start(GTK_BOX(menuBox), gameBox, false, false, false);
    gtk_box_pack_start(GTK_BOX(menuBox), clientStartBox, false, false, false);
    gtk_box_pack_start(GTK_BOX(menuBox), serverStartBox, false, false, false);

    gtk_widget_set_halign(menuBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(menuBox, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(window), menuBox);

    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen,
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_css_provider_load_from_file(provider, g_file_new_for_path("t.css"),
                                    NULL);

    g_object_unref(provider);

    gtk_widget_show_all(mainMenuBox);
    gtk_widget_show(menuBox);
    gtk_widget_show(window);

    gtk_main();

}
//Chowa wszystkie widgety
void hide_all()
{
    gtk_widget_hide(mainMenuBox);
    gtk_widget_hide(gameBox);
    gtk_widget_hide(serverBox);
    gtk_widget_hide(clientBox);
    if(serverStartBox!=NULL)
        gtk_widget_hide(serverStartBox);
    if(clientStartBox!=NULL)
        gtk_widget_hide(clientStartBox);
}
// Tworzy menu glowne
void build_mainMenu()
{
    if (mainMenuBox == NULL)
    {
        mainMenuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        GtkWidget *client, *server, *nickLabel,  *grid = gtk_grid_new();

        client = gtk_button_new_with_label("Client");
        server = gtk_button_new_with_label("Server");
        nickEntry = gtk_entry_new();
        nickLabel = gtk_label_new("Enter your nick:");

        gtk_widget_set_name(nickLabel, "myipLabel");

        gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

        gtk_grid_attach(GTK_GRID(grid), nickLabel, 1, 1, 2, 1);
        gtk_grid_attach(GTK_GRID(grid), nickEntry, 1, 2, 2, 1);
        gtk_grid_attach(GTK_GRID(grid), client, 1, 3, 2, 1);
        gtk_grid_attach(GTK_GRID(grid), server, 1, 4, 2, 1);

        gtk_widget_set_halign(mainMenuBox, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(mainMenuBox, GTK_ALIGN_CENTER);


        char nick[10];
        nick[9] = '\0';
        for(int i = 0; i<9; i++)
        {
            nick[i]='A' + (rand()+time(NULL))%57;
        }

        gtk_entry_set_text(GTK_ENTRY(nickEntry),nick);
        gtk_box_pack_start(GTK_BOX(mainMenuBox), grid, false, true, true);

        g_signal_connect(G_OBJECT(server), "clicked", G_CALLBACK(build_server),
                         NULL);
        g_signal_connect(G_OBJECT(client), "clicked", G_CALLBACK(build_client),
                         NULL);

        gtk_widget_hide(mainMenuBox);
    }
    else
    {
        hide_all();
        gtk_widget_show_all(mainMenuBox);
        puts("MainMenu");
    }

}
//Plansza + czat
void build_game()
{
    if (gameBox == NULL)
    {
        gameBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
        GtkWidget *verBox =  gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

        GtkWidget *horBox =  gtk_fixed_new();
        int a=0,b=0,r=81;
        for(int j = 0; j<FIELD_SIZE; j++)
        {
            if(j%2==0)
            {
                int x=0,y=0;
                for(int i = 0; i<  FIELD_SIZE; i++)
                {
                    GtkWidget *field_button = gtk_button_new(),*button1 = gtk_button_new();
                    point p = malloc(sizeof(point*));
                    fields field = malloc(sizeof(fields*));
                    pola[j][i] = field;
                    pola[j][i]->wallButton = NULL;
                    pola[j][i]->i=0;
                    p->x = i;
                    p->y = j;
                    if(i%2==0)
                    {
                        if(j==16&&i==8)
                        {
                            gtk_widget_set_name(field_button,"player");
                        }
                        else if(j==0&&i==8)
                        {
                            gtk_widget_set_name(field_button,"enemy");
                        }
                        else
                            gtk_widget_set_name(field_button,"wladcaCieni");
                        gtk_fixed_put(GTK_FIXED(horBox),field_button,x*r+y*17+20,a*r+b*20);
                        g_signal_connect(G_OBJECT(field_button), "clicked",G_CALLBACK(tryMakeMove), p);
                    }
                    else
                    {
                        if(j == 1 || j==15|| j==0 || j == 16)
                        {
                            gtk_widget_set_name(field_button,"mlodyBog");

                            gtk_fixed_put(GTK_FIXED(horBox),field_button,x*r+y*17+20,a*r+b*20);

                            g_signal_connect(G_OBJECT(field_button), "clicked",G_CALLBACK(tryPlaceWall), p);
                        }
                        else
                        {
                            gtk_widget_set_name(field_button,"boromir");
                            gtk_widget_set_name(button1,"boromir");

                            pola[j][i]->wallButton = button1;

                            gtk_fixed_put(GTK_FIXED(horBox),field_button,x*r+y*17+20,a*r+b*20);
                            gtk_fixed_put(GTK_FIXED(horBox),button1,x*r+y*17+20,a*r+b*20+r/2);

                            g_signal_connect(G_OBJECT(field_button), "clicked",G_CALLBACK(tryPlaceWall), p);
                            g_signal_connect(G_OBJECT(button1), "clicked",G_CALLBACK(tryPlaceWall), p);

                        }
                    }
                    pola[j][i]->button = field_button;
                    if(i%2==0)x++;
                    else y++;
                }

            }
            else
            {
                for(int i = 0; i<  9; i++)
                {
                    point p = malloc(sizeof(point*));
                    fields field = malloc(sizeof(fields*));
                    pola[j][2*i] = field;
                    pola[j][2*i]->wallButton = NULL;
                    pola[j][2*i]->i=0;
                    p->x = 2*i;
                    p->y = j;
                    GtkWidget *field_button = gtk_button_new(),*button1 = gtk_button_new();
                    if(i == 0 || i ==8)
                    {
                        gtk_widget_set_name(field_button,"gumdalf");
                        if(i==0)
                        {
                            gtk_fixed_put(GTK_FIXED(horBox),field_button,i*r+20,a*r+b*20);
                            pola[j][2*i]->button = field_button;
                        }

                        else
                        {
                            gtk_fixed_put(GTK_FIXED(horBox),field_button,i*r + 17*i+20,a*r+b*20);
                            pola[j][2*i]->button = field_button;
                        }
                        g_signal_connect(G_OBJECT(field_button), "clicked",G_CALLBACK(tryPlaceWall), p);
                    }
                    else
                    {
                        gtk_widget_set_name(field_button,"aragorn");
                        gtk_widget_set_name(button1,"aragorn");

                        gtk_fixed_put(GTK_FIXED(horBox),field_button,i*r + 17*i+20,a*r+b*20);
                        gtk_fixed_put(GTK_FIXED(horBox),button1,i*r+ 17*i +20+r/2,a*r+b*20);

                        pola[j][2*i]->button = field_button;
                        pola[j][2*i]->wallButton = button1;

                        g_signal_connect(G_OBJECT(field_button), "clicked",G_CALLBACK(tryPlaceWall), p);
                        g_signal_connect(G_OBJECT(button1), "clicked",G_CALLBACK(tryPlaceWall), p);
                    }
                }
            }
            if(j%2==0)a++;
            else b++;
        }
        gtk_box_pack_start(GTK_BOX(verBox),horBox,false,false,false);

        GtkWidget *grid = gtk_grid_new();
        gtk_grid_set_row_spacing(GTK_GRID(grid), 1);

        bufor = (GtkWidget *)gtk_text_buffer_new (NULL);
        GtkWidget *text_view = gtk_text_view_new_with_buffer (GTK_TEXT_BUFFER(bufor));
        gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

        GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC,
                                        GTK_POLICY_AUTOMATIC);
        gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
        gtk_container_set_border_width (GTK_CONTAINER(scrolled_window), 1);
        gtk_grid_attach(GTK_GRID(grid), scrolled_window, 1, 1, 1, 2);
        g_object_set (scrolled_window, "expand", TRUE, NULL);

        text = gtk_entry_new();
        gtk_entry_set_max_length(GTK_ENTRY(text), MAX_MES_LN);
        gtk_entry_set_text(GTK_ENTRY(text), "");
        g_signal_connect(G_OBJECT(text), "activate",G_CALLBACK(prepareToSendMessenge),(gpointer) text);
        gtk_grid_attach(GTK_GRID(grid), text, 1, 3, 1, 1);
        gtk_widget_set_name(text,"myipLabel");


        GtkWidget *button=gtk_button_new_with_label("Exit");
        g_signal_connect(G_OBJECT(button), "clicked",G_CALLBACK(exitGame), NULL);
        gtk_grid_attach(GTK_GRID(grid), button, 1, 5, 1, 1);

        GtkWidget *save=gtk_button_new_with_label("Save");
        g_signal_connect(G_OBJECT(save), "clicked",G_CALLBACK(saveGame), NULL);
        gtk_grid_attach(GTK_GRID(grid), save, 1, 4, 1, 1);

        gtk_box_pack_start(GTK_BOX(gameBox),verBox,false,false,false);
        gtk_box_pack_start(GTK_BOX(gameBox),grid,true,true,true);
        gtk_widget_hide(gameBox);

    }
    else
    {
        hide_all();
        gtk_widget_show_all(gameBox);
        puts("Game");
    }
}
//Glowne menu serwera
void build_server()
{
    if (serverBox == NULL)
    {
        serverBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        GtkWidget *back, *host, *grid = gtk_grid_new();

        back = gtk_button_new_with_label("Back");
        host = gtk_button_new_with_label("Host");
        serverPort = gtk_entry_new();

        gtk_entry_set_text(GTK_ENTRY(serverPort),"8888");
        char a[60]= {"Send this ip to your opponent:\n"};
        char b[16];
        myIP(b);
        strcat(a,b);
        strcat(a,"\n Enter port number:");
        ipLabel = gtk_label_new(a);
        gtk_label_set_justify(GTK_LABEL(ipLabel), GTK_JUSTIFY_CENTER);
        gtk_widget_set_name(ipLabel, "myipLabel");

        gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

        gtk_grid_attach(GTK_GRID(grid), ipLabel, 1, 1, 2, 1);
        gtk_grid_attach(GTK_GRID(grid),  serverPort, 1, 2, 2, 1);
        gtk_grid_attach(GTK_GRID(grid), host, 1, 3, 2, 1);
        gtk_grid_attach(GTK_GRID(grid), back, 1, 4, 2, 1);


        gtk_box_pack_start(GTK_BOX(serverBox), grid, true, true, true);

        g_signal_connect(G_OBJECT(host), "clicked", G_CALLBACK(build_serverStartBox),
                         NULL);
        g_signal_connect(G_OBJECT(back), "clicked", G_CALLBACK(build_mainMenu),
                         NULL);

        gtk_widget_hide(serverBox);
    }
    else
    {
        hide_all();
        gtk_widget_show_all(serverBox);
        puts("Server");
    }
}
//Glowne menu clienta
void build_client()
{
    if (clientBox == NULL)
    {
        clientBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        GtkWidget *back, *connect, *grid = gtk_grid_new();

        back = gtk_button_new_with_label("Back");
        connect = gtk_button_new_with_label("Connect");
        clientLabel = gtk_label_new("Enter  server ip and port number:");

        ipClient = gtk_entry_new();
        clientPort = gtk_entry_new();

        gtk_widget_set_name(clientLabel,"myipLabel");
        gtk_label_set_justify(GTK_LABEL(clientLabel), GTK_JUSTIFY_CENTER);

        gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

        gtk_grid_attach(GTK_GRID(grid), clientLabel, 1, 1, 2, 1);
        gtk_grid_attach(GTK_GRID(grid),  ipClient, 1, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), clientPort, 2, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), connect, 1, 3, 2, 1);
        gtk_grid_attach(GTK_GRID(grid), back, 1, 4, 2, 1);

        gtk_entry_set_text(GTK_ENTRY(ipClient), "IP");
        gtk_entry_set_text(GTK_ENTRY(clientPort), "Port number");
        gtk_box_pack_start(GTK_BOX(clientBox), grid, false, false, false);

        g_signal_connect(G_OBJECT(connect), "clicked", G_CALLBACK(build_clientStartBox),
                         NULL);
        g_signal_connect(G_OBJECT(back), "clicked", G_CALLBACK(build_mainMenu),
                         NULL);

        gtk_widget_hide(clientBox);

    }
    else
    {
        hide_all();
        gtk_widget_show_all(clientBox);
        puts("Client");
    }

}
//Tworzy menu oczekiwania dla clienta
void build_clientStartBox()
{
    if(isBuilt==1)
        resetSocket();
    if(isBuilt==1&&initClient((char *)gtk_entry_get_text(GTK_ENTRY(ipClient)), atoi((char *)gtk_entry_get_text(GTK_ENTRY(clientPort)))))
    {
        gtk_label_set_text(GTK_LABEL(clientLabel),"Ip or port number are incorrect!\n Please enter correct value.");
        endConnection();
        hide_all();
        build_client();
        return;
    }
    if(clientStartBox==NULL)
    {
        GtkWidget *back, *grid = gtk_grid_new();

        clientStartBox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

        startLabelClient = gtk_label_new("Successful connected.\n Press \"Ready\" if you are ready.");
        gtk_widget_set_name(startLabelClient,"myipLabel");
        gtk_label_set_justify(GTK_LABEL(startLabelClient),GTK_JUSTIFY_CENTER);

        back=gtk_button_new_with_label("Back");
        ready=gtk_button_new_with_label("Ready");

        gtk_grid_attach(GTK_GRID(grid), back, 1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), ready, 1, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), startLabelClient, 1, 1, 1, 1);

        gtk_box_pack_start(GTK_BOX(clientStartBox), grid, false, true, false);

        g_signal_connect(G_OBJECT(ready), "clicked", G_CALLBACK(imReady),
                         NULL);
        g_signal_connect(G_OBJECT(back), "clicked", G_CALLBACK(build_mainMenu),
                         NULL);
        gtk_widget_hide(clientStartBox);

    }
    else
    {
        hide_all();
        gtk_widget_show_all(clientStartBox);
        puts("clientStartBox");
    }

}
//Zmienia tresc napisu
void start_label()
{
    gtk_label_set_text(GTK_LABEL(startLabel),"Connection successful.\n Wait for your enemy will be ready!");
    g_timeout_add(100,canStartGame,NULL);
}
//Zmienia tresc napisu
void update_start_label()
{
    gtk_label_set_text(GTK_LABEL(startLabel),"Your opponent is ready.\n Let`s start the GAME!");
}
//Zmienia tresc napisu
void startClient_label()
{
    gtk_label_set_text(GTK_LABEL(startLabelClient),"Please wait until the game\n starts!");
}
//Zmienia ustawnia buttona
void start_button()
{
    gtk_widget_set_sensitive(start,TRUE);
}
//Zmienia ustawnia buttona
void client_button()
{
    gtk_widget_set_sensitive(ready,FALSE);
}
//Tworzy menu serwera podczas oczekiwania
void build_serverStartBox()
{
    if(isBuilt==1)
        resetSocket();
    if(isBuilt==1&&initServer(atoi((char *)gtk_entry_get_text(GTK_ENTRY(serverPort)))))
    {
        char messenge[50];
        strcpy(messenge,(char *)gtk_label_get_text(GTK_LABEL(ipLabel)));
        strcat(messenge,"\n Can`t bind port number!\n Please try enter another value");
        gtk_label_set_text(GTK_LABEL(ipLabel),messenge);
        hide_all();
        build_server();
        return;
    }
    if(serverStartBox==NULL)
    {
        serverStartBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        GtkWidget *grid = gtk_grid_new();
        gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);


        startLabel = gtk_label_new("Waiting for connection...");
        GtkWidget *back=gtk_button_new_with_label("Back");
        start=gtk_button_new_with_label("Start Game");


        GtkWidget *fileLabel = gtk_label_new("Type filename to load.");
        gtk_grid_attach (GTK_GRID (grid), fileLabel, 1, 4, 1, 1);
        gtk_widget_set_name(fileLabel, "myipLabel");

        fileEntry = gtk_entry_new();
        gtk_grid_attach (GTK_GRID (grid), fileEntry, 1, 5, 1, 1);
        gtk_entry_set_text(GTK_ENTRY(fileEntry),"F");

        gtk_widget_set_name(startLabel,"myipLabel");
        gtk_label_set_justify(GTK_LABEL(startLabel),GTK_JUSTIFY_CENTER);

        gtk_grid_attach(GTK_GRID(grid), back, 1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), start, 1, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), startLabel, 1, 1, 1, 1);

        gtk_box_pack_start(GTK_BOX(serverStartBox), grid, false, true, false);

        g_signal_connect(G_OBJECT(start), "clicked", G_CALLBACK(startGameServer),
                         NULL);
        g_signal_connect(G_OBJECT(back), "clicked", G_CALLBACK(build_mainMenu),
                         NULL);
        gtk_widget_hide(serverStartBox);
    }
    else
    {
        hide_all();
        gtk_widget_show_all(serverStartBox);
        gtk_widget_set_sensitive(start,FALSE);
        g_timeout_add(100, isOpponentConnected,NULL);
        puts("serverStartBox");
    }

}
//Zwraca nick gracza
void getNick(char nick[])
{
    strcpy(nick,gtk_entry_get_text(GTK_ENTRY(nickEntry)));
}
//Bufor wiadomosci wychodzacych
void updateBuforOut(char wejscie[])
{
    gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(bufor),wejscie,-1);
    gtk_entry_set_text(GTK_ENTRY(text), "");
}
//Bufor wiadomosci przychodzacych
void updateBuforIn(char wejscie[])
{
    gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(bufor),wejscie,-1);
}
