#include "types.h"
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <string.h>

GtkTreeIter iter;
sqlite3 *db;

int fill_store_callback(void *data, int cols, char *col_values[], char *col_names[])
{
    GtkListStore *store;

    store = (GtkListStore *)data;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
		       ID, atoi(col_values[ID]),
		       BRIEF, col_values[BRIEF],
		       DETAIL, col_values[DETAIL],
		       -1);

    return 0;
}

GtkListStore *init_todo_list_model(void)
{
    GtkListStore *todo_list_model;
    char *errmsg;

    if (sqlite3_open("todos.db", &db) != SQLITE_OK) {
	fprintf(stderr, "Can not open database file `todos.db' in current directory\n");
	exit(1);
    }
    todo_list_model = gtk_list_store_new(COLUMNS,
					 G_TYPE_INT,
					 G_TYPE_STRING,
					 G_TYPE_STRING);
    g_printf("Reading TODOs' information from Sqlite database...\n");
    sqlite3_exec(db, "SELECT TodoId, TodoBrief, TodoDetail FROM Todo",
		 &fill_store_callback, todo_list_model, &errmsg);
    g_printf("Finished!\n");
    if (errmsg != NULL) {
	g_fprintf(stderr, "%s\n", errmsg);
	sqlite3_free(errmsg);
	exit(2);
    }

    return todo_list_model;
}

int add_todo(int todo_id, char *todo_brief, char *todo_detail)
{
    char *sql = malloc((34 + 3 + 20 + 100 + 1) * sizeof(char));
    char *brief, *detail;
    char *errmsg;

    brief = strndup(todo_brief, 20);
    detail = strndup(todo_detail, 100);
    sprintf(sql, "INSERT INTO Todo VALUES (%3d, '%s', '%s')",
	    todo_id, brief, detail);
    sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (errmsg != NULL) {
	g_fprintf(stderr, "%s\n", errmsg);
	return ERROR;
    } else
	return OK;
}

void close_db(void)
{
    g_printf("Closing Sqlite database...\n");
    sqlite3_close(db);
}
