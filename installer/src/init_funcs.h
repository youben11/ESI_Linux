#ifndef DEF_INIT_FUNCS
#define DEF_INIT_FUNCS

#include "simple_parted/sparted.h"
#include <string.h>
#include "partition_funcs.h"
#include "event_funcs.h"

void init_user_info(installer* inst);
void init_partition(GtkWidget *w,gpointer ins);
void init_builders(installer* inst);
void init_installer(installer* inst,GtkBuilder *builder);
void init_partition_structure(installer* inst);
void init_listbox(GtkListBox* listbox);
void replace_layout(GtkFixed* container , GtkWidget* layout_to_destroy , GtkWidget* layout_to_put);
void init_layout(installer* inst);
void init_time_lang(installer* inst);
void init_zone_combo(GtkWidget* w, gpointer data);
void init_summary(installer* inst);
void init_disk_list(installer* inst);

#endif
