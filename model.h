#ifndef MODEL_H
#define MODEL_H

#include <gtk/gtk.h>

extern GtkListStore *init_todo_list_model(void);
extern int add_todo(int, char *, char *);
extern void close_db(void);

#endif
