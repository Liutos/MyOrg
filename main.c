/*
 * main.c
 *
 *
 *
 * Copyright (C) 2012-04-29 liutos
 */
#include "model.h"
#include "types.h"
#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>

GtkWidget *id_text, *brief_text, *detail_text;
GtkTextBuffer *id_buffer, *brief_buffer, *detail_buffer;

GtkWidget *init_button_vbox(void);
GtkWidget *init_todo_list_view(void);
GtkWidget *init_todo_info_table(void);
void quit_myorg(GtkWidget *, gpointer);

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *hbox;
    GtkWidget *scroll_window;
    GtkWidget *button_vbox, *vbox;
    GtkWidget *todo_list_view;
    GtkWidget *todo_info_table;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    hbox = gtk_hbox_new(FALSE, 5);
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    button_vbox = init_button_vbox();
    todo_list_view = init_todo_list_view();
    vbox = gtk_vbox_new(FALSE, 5);
    todo_info_table = init_todo_info_table();

    gtk_container_add(GTK_CONTAINER(scroll_window), todo_list_view);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), todo_info_table, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_vbox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(quit_myorg), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

void quit_myorg(GtkWidget *button, gpointer data)
{
    close_db();
    gtk_main_quit();
}

GtkWidget *init_button_vbox(void)
{
    GtkWidget *quit_btn;
    GtkWidget *vbox;
    GtkWidget *lift_todo_btn;
    GtkWidget *mark_todo_btn;

    vbox = gtk_vbox_new(TRUE, 5);
    quit_btn = gtk_button_new_with_mnemonic("_Quit");
    lift_todo_btn = gtk_button_new_with_label("Lift TODO");
    mark_todo_btn = gtk_button_new_with_label("Mark TODO");

    gtk_box_pack_start(GTK_BOX(vbox), lift_todo_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), mark_todo_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), quit_btn, FALSE, FALSE, 0);

    g_signal_connect(GTK_OBJECT(quit_btn), "clicked",
		     G_CALLBACK(quit_myorg), NULL);

    return vbox;
}

void insert_text_column(GtkWidget *tree_view, gint position, const char *column_title)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(column_title,
						      renderer,
						      "text",
						      position,
						      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
}

GtkWidget *init_todo_list_view(void)
{
    GtkWidget *todo_list_view;

    todo_list_view = gtk_tree_view_new();

    gtk_tree_view_set_model(GTK_TREE_VIEW(todo_list_view),
			    GTK_TREE_MODEL(init_todo_list_model()));
    insert_text_column(todo_list_view, ID, "TODO Id");
    insert_text_column(todo_list_view, BRIEF, "TODO Brief");
    insert_text_column(todo_list_view, DETAIL, "TODO Detail");

    return todo_list_view;
}

char *get_text_buffer_text(GtkTextBuffer *buf)
{
    GtkTextIter start, end;

    gtk_text_buffer_get_start_iter(buf, &start);
    gtk_text_buffer_get_end_iter(buf, &end);

    return gtk_text_buffer_get_text(buf, &start, &end, FALSE);
}

void add_todo_callback(GtkWidget *button, gpointer data)
{
    char *id, *brief, *detail;

    id = get_text_buffer_text(id_buffer);
    brief = get_text_buffer_text(brief_buffer);
    detail = get_text_buffer_text(detail_buffer);

    add_todo(atoi(id), brief, detail);
}

GtkWidget *init_todo_info_table(void)
{
    GtkWidget *table;
    GtkWidget *id_label, *brief_label, *detail_label;
    GtkWidget *id_frame, *brief_frame, *detail_frame;
    GtkWidget *add_todo_btn;

    table = gtk_table_new(4, 3, TRUE);
    id_label = gtk_label_new("TODO Id");
    brief_label = gtk_label_new("TODO Brief");
    detail_label = gtk_label_new("TODO Detail");
    id_text = gtk_text_view_new();
    brief_text = gtk_text_view_new();
    detail_text = gtk_text_view_new();
    add_todo_btn = gtk_button_new_with_label("Add TODO");
    id_frame = gtk_frame_new(NULL);
    brief_frame = gtk_frame_new(NULL);
    detail_frame = gtk_frame_new(NULL);

    id_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(id_text));
    brief_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(brief_text));
    detail_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(detail_text));

    gtk_frame_set_shadow_type(GTK_FRAME(id_frame), GTK_SHADOW_IN);
    gtk_frame_set_shadow_type(GTK_FRAME(brief_frame), GTK_SHADOW_IN);
    gtk_frame_set_shadow_type(GTK_FRAME(detail_frame), GTK_SHADOW_IN);

    gtk_container_add(GTK_CONTAINER(id_frame), id_text);
    gtk_container_add(GTK_CONTAINER(brief_frame), brief_text);
    gtk_container_add(GTK_CONTAINER(detail_frame), detail_text);
    gtk_table_attach_defaults(GTK_TABLE(table), id_label, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), brief_label, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), detail_label, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), id_frame, 1, 3, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), brief_frame, 1, 3, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), detail_frame, 1, 3, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), add_todo_btn, 2, 3, 3, 4);

    g_signal_connect(GTK_OBJECT(add_todo_btn), "clicked",
		     G_CALLBACK(add_todo_callback), NULL);

    return table;
}
