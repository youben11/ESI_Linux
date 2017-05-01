#ifndef DEF_EVENT_FUNCS
#define DEF_EVENT_FUNCS

void disk_choosed(installer* inst);
void alert_dialog(GtkWindow* window,char* message);
int isValidName(char* name);
GtkWidget* get_child_by_name(GtkContainer* c,char* name);
void save_user_info(installer* inst);
int check_user_info(installer* inst);
void exit_finish(GtkWidget *w, gpointer userdata);
void layout_next(GtkFixed* main_fixed,GtkWidget* selected_layout,GtkWidget* wanted_layout,GtkListBox* listbox,gint selected, gint position);
void next_click(GtkApplication* app,gpointer data);
gboolean check_partition_info(installer* inst);
void open_gparted(GtkWidget* w,gpointer data);
void refresh_disk_list(GtkWidget* w , gpointer data);
void install(GtkWidget* w,gpointer data);

#endif
