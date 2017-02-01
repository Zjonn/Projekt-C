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

GtkWidget *mainMenuBox, *gameBox;

GtkWidget *clientBox, *serverBox;

void build_mainMenu();
void build_game();



void init(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *menuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_window_set_title(GTK_WINDOW(window), "Quoridor");
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
			NULL);

	build_mainMenu();
	build_server();
	build_client();
	build_game();



	gtk_box_pack_start(GTK_BOX(menuBox), mainMenuBox, false, true, true);
	gtk_box_pack_start(GTK_BOX(menuBox), serverBox, false, true, true);
	gtk_box_pack_start(GTK_BOX(menuBox), clientBox, false, true, true);
	gtk_box_pack_start(GTK_BOX(menuBox), gameBox, false, false, false);
	printf("1");

	gtk_box_pack_start(GTK_BOX(menuBox), mainMenuBox, false, true, true);
	gtk_box_pack_start(GTK_BOX(menuBox), gameBox, false, true, true);
	gtk_box_pack_start(GTK_BOX(menuBox), serverBox, false, true, true);
	gtk_box_pack_start(GTK_BOX(menuBox), clientBox, false, true, true);

	gtk_widget_set_halign(menuBox, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(menuBox, GTK_ALIGN_CENTER);
	gtk_container_add(GTK_CONTAINER(window), menuBox);
	printf("1");

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

	menu_show();
	gtk_widget_show(menuBox);
	gtk_widget_show(window);


	gtk_main();

}

void menu_hide() {
	if (gtk_widget_get_visible(mainMenuBox))
		gtk_widget_hide(mainMenuBox);
}

void game_hide() {
	if (gtk_widget_get_visible(gameBox))
		gtk_widget_hide(gameBox);
}

void server_hide() {
	if (gtk_widget_get_visible(serverBox))
		gtk_widget_hide(serverBox);
}

void client_hide() {
	if (gtk_widget_get_visible(clientBox))
		gtk_widget_hide(clientBox);
}

void menu_show() {
	if (!gtk_widget_get_visible(mainMenuBox))
		gtk_widget_show_all(mainMenuBox);
}

void game_show() {
	if (!gtk_widget_get_visible(gameBox))
		gtk_widget_show(gameBox);
}

void server_show() {
	if (!gtk_widget_get_visible(gameBox))
		gtk_widget_show(serverBox);
}

void client_show() {
	if (!gtk_widget_get_visible(gameBox))
		gtk_widget_show(clientBox);
}

void hide_all() {
	menu_hide();
	game_hide();
	server_hide();
	client_hide();
}
int is_menu_visiable() {
	if (gtk_widget_get_visible(mainMenuBox) == true)
		return 1;
	return 0;
}

int is_game_visiable() {
	if (gtk_widget_get_visible(gameBox) == true)
		return 1;
	return 0;
}

void build_mainMenu() {
	if (mainMenuBox == NULL) {
		mainMenuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

		GtkWidget *client, *server, *entry, *grid = gtk_grid_new();

		client = gtk_button_new_with_label("Client");
		server = gtk_button_new_with_label("Server");
		entry = gtk_entry_new();

		gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
		gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
		gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 20, 1);
		gtk_grid_attach(GTK_GRID(grid), client, 1, 2, 20, 1);
		gtk_grid_attach(GTK_GRID(grid), server, 1, 3, 20, 1);


//		gtk_widget_set_halign(mainMenuBox, GTK_ALIGN_CENTER);
//		gtk_widget_set_valign(mainMenuBox, GTK_ALIGN_CENTER);

		gtk_widget_set_halign(mainMenuBox, GTK_ALIGN_CENTER);
		gtk_widget_set_valign(mainMenuBox, GTK_ALIGN_CENTER);

		gtk_entry_set_text(GTK_ENTRY(entry), "Enter your nick");
		gtk_box_pack_start(GTK_BOX(mainMenuBox), grid, false, true, true);

		g_signal_connect(G_OBJECT(server), "clicked", G_CALLBACK(build_server),
				NULL);
		g_signal_connect(G_OBJECT(client), "clicked", G_CALLBACK(build_client),
				NULL);

		gtk_widget_hide(mainMenuBox);
	} else {
		hide_all();
		gtk_widget_show_all(mainMenuBox);
		printf("Main\n");
	}

}
void build_game() {
	if (gameBox == NULL) {
		gameBox = gtk_grid_new();
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				printf("%d %d ", i, j);
				Field p = malloc(sizeof(Field*));

				p->button = gtk_button_new();
				gtk_widget_set_name(p->button,"wladcaCieni");

				p->i = (i + j) % 2;
				pola[i][j] = p;
				printf("%d \n", pola[i][j]->i);
				gtk_grid_attach(GTK_GRID(gameBox), p->button, i*5, j*5, 5, 5);
			}
		}
		gtk_widget_hide(gameBox);

	} else {
		hide_all();
		gtk_widget_show_all(gameBox);
		printf("Game\n");
	}
}

void build_server() {
	if (serverBox == NULL) {
		serverBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

		GtkWidget *back, *host, *ipLabel, *grid = gtk_grid_new();

		back = gtk_button_new_with_label("Back");
		host = gtk_button_new_with_label("Host");
		ipLabel = gtk_label_new("Send this ip to your\n opponent:");
		gtk_label_set_justify(GTK_LABEL(ipLabel), GTK_JUSTIFY_CENTER);
		gtk_widget_set_name(ipLabel, "myipLabel");

		gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
		gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

		gtk_grid_attach(GTK_GRID(grid), ipLabel, 1, 1, 20, 1);
		gtk_grid_attach(GTK_GRID(grid), back, 1, 3, 20, 1);
		gtk_grid_attach(GTK_GRID(grid), host, 1, 2, 20, 1);

//		gtk_widget_set_halign(serverBox, GTK_ALIGN_CENTER);
//		gtk_widget_set_valign(serverBox, GTK_ALIGN_CENTER);
		gtk_box_pack_start(GTK_BOX(serverBox), grid, false, true, true);

		g_signal_connect(G_OBJECT(host), "clicked", G_CALLBACK(build_game),
				NULL);
		g_signal_connect(G_OBJECT(back), "clicked", G_CALLBACK(build_mainMenu),
				NULL);

		gtk_widget_hide(serverBox);
	} else {
		hide_all();
		gtk_widget_show_all(serverBox);
		printf("Server\n");
	}

}
void build_client() {
	if (clientBox == NULL) {
		clientBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
		GtkWidget *back, *connect, *ip, *grid = gtk_grid_new();

		back = gtk_button_new_with_label("Back");
		connect = gtk_button_new_with_label("Connect");
		ip = gtk_entry_new();

		gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
		gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
		gtk_grid_attach(GTK_GRID(grid), ip, 1, 1, 20, 1);
		gtk_grid_attach(GTK_GRID(grid), back, 1, 3, 20, 1);
		gtk_grid_attach(GTK_GRID(grid), connect, 1, 2, 20, 1);

//		gtk_widget_set_halign(clientBox, GTK_ALIGN_CENTER);
//		gtk_widget_set_valign(clientBox, GTK_ALIGN_CENTER);
		gtk_entry_set_text(GTK_ENTRY(ip), "Enter ip");
		gtk_box_pack_start(GTK_BOX(clientBox), grid, false, true, true);

		//g_signal_connect(G_OBJECT(connect), "clicked", G_CALLBACK(build_server),
		//		NULL);
		g_signal_connect(G_OBJECT(back), "clicked", G_CALLBACK(build_mainMenu),
				NULL);

		gtk_widget_hide(clientBox);

	} else {
		hide_all();
		gtk_widget_show_all(clientBox);
		printf("Client\n");
	}

}



