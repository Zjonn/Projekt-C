/*
 * GUI.c
 *
 *  Created on: 12.01.2017
 *      Author: danie_000
 */

#include <stdbool.h>
#include <gtk/gtk.h>
#include "GUI.h"

GtkWidget *menuBox, *gameBox;
int isMenuVisiable = 0, isGameVisiable = 0;

void build_menu();
void build_game();

void init(int argc, char *argv[]){
	gtk_init(&argc, &argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Quoridor");
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
			NULL);

	build_menu();
	build_game();

	gtk_container_add(GTK_CONTAINER(window), menuBox);

	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;

	//GtkWidget *image = gtk_image_new_from_file("539044.jpg");
	provider = gtk_css_provider_new ();
	display = gdk_display_get_default ();
	screen = gdk_display_get_default_screen (display);
	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	gtk_css_provider_load_from_file(provider, g_file_new_for_path("t.css"), NULL);
	g_object_unref (provider);


	gtk_widget_show_all(window);
	gtk_main();
}

void menu_set_visiable(int isVisiable) {
	if (isVisiable == 1)
		gtk_widget_show(menuBox);
	else
		gtk_widget_hide(menuBox);
}

void game_set_visiable(int isVisiable) {
	if (isVisiable == 1)
		gtk_widget_show(gameBox);
	else
		gtk_widget_hide(gameBox);
}

int is_menu_visiable() {
	return isMenuVisiable;
}

int is_game_visiable() {
	return isGameVisiable;
}

void build_menu() {
	menuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	//gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
	//gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

	GtkWidget *client, *server, *entry;
	client = gtk_button_new_with_label("Client");
	server = gtk_button_new_with_label("Server");
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry), "Enter your nick");

	gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 20, 1);
	gtk_grid_attach(GTK_GRID(grid), client, 1, 2, 20, 1);
	gtk_grid_attach(GTK_GRID(grid), server, 1, 3, 20, 1);

	gtk_box_pack_start(GTK_BOX(menuBox), grid, false, true, true);
	gtk_widget_set_halign(menuBox, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(menuBox, GTK_ALIGN_CENTER);

	gtk_widget_show(menuBox);
	isMenuVisiable = 1;

}
void build_game() {
	gameBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
}

