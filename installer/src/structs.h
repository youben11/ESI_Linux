#ifndef DEF_STRUCTS
#define DEF_STRUCTS

typedef struct user_info{

	char* username;
	char* password;
	char* hostname;
	int auto_login;

}user_info;

typedef struct lang_time_info{

	char* keyboard;
	char* language;
	int time_zone;

}lang_time_info;

typedef struct used_partition{

	char* path;

}used_partition;

typedef struct installer{
	GtkBuilder* builders[8];
	GtkWindow* window;
	GtkButton* buttons[3];
	GtkFixed* main_fixed;
	GtkWidget* layouts[8];
	GtkListBox* listbox;
	user_info uinfo;
	lang_time_info linfo;
	used_partition partition;

	int pos;
}installer;

#endif