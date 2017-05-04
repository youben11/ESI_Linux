#ifndef DEF_PART_FUNCS
#define DEF_PART_FUNCS

#include "simple_parted/sparted.h"
#include <gtk/gtk.h>
#include "structs.h"
#include "event_funcs.h"

gchar* fs_filter(gchar* filesystem);
void fill_grid_row(GtkGrid* row,gchar* partition,gchar* name,gchar* filesystem,gchar* type,gchar* size);
gchar* get_human_size(long long size);
void clear_grid(GtkGrid* glb, gint pos_start);
void fill_partition_grid(GtkGrid* grid,SPedDiskInfo dinfo);
gchar* get_partition_path_at_index(installer* inst,gint i);
PedSector get_partition_size_at_index(installer* inst,gint in);
int get_active_radio_button(installer* inst);
GtkToggleButton* get_radio_button_at_index(installer* inst,gint i);

#endif
