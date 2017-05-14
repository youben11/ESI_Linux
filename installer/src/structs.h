#ifndef DEF_STRUCTS
#define DEF_STRUCTS

#include <gtk/gtk.h>
#include <stdlib.h>

typedef struct partition_info{
		GtkGrid *partition_grid;
		GtkComboBoxText *disk_list;
		GtkLabel *disk_name;
		GtkLabel *disk_size;
		long long spartition_size;
		gchar* selected_partition;
		gchar* selected_disk;

} partition_info;

typedef struct user_info{

	char* username;
	char* password;
	char* hostname;
	int auto_login;
	GtkButton* showpass_button;
	gboolean visible;
	
}user_info;

typedef struct lang_time_info{

	char* keyboard;
	char* language;
	char* region;
	char* zone;
	char* timezone;
	FILE* timezone_file;
	GtkComboBoxText* region_combo;
	GtkComboBoxText* zone_combo;

}lang_time_info;

typedef struct installer{

	GtkApplication* app;

	GtkBuilder* builders[8];
	GtkWindow* window;
	GtkButton* buttons[5];
	GtkFixed* main_fixed;
	GtkWidget* layouts[8];
	GtkListBox* listbox;
	GtkGrid* partition_grid;
	GtkSpinner* spinner[6];
	GtkImage* image[6];

	user_info uinfo;
	lang_time_info linfo;
	partition_info pinfo;
	int pos;
}installer;

#endif