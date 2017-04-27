#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "installer.h"

installer *i;

void alert_dialog(GtkWindow* window,char* message){
	GtkMessageDialog *dialog=GTK_MESSAGE_DIALOG(gtk_message_dialog_new(window,
																		GTK_DIALOG_DESTROY_WITH_PARENT,
																		GTK_MESSAGE_WARNING,
																		GTK_BUTTONS_NONE,
																		message));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void next_click(GtkApplication* app,gpointer data);

static void activate(GtkApplication* app,gpointer data);

int main(int argc,char** argv){
    
    int result;

    GtkApplication* app=gtk_application_new("esi.linux",G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(G_OBJECT(app),"activate",G_CALLBACK(activate),NULL);
    result=g_application_run(G_APPLICATION(app),argc,argv);
	g_object_unref(app);
    
    return result;
}

static void activate(GtkApplication* app,gpointer data){
    
	installer* i=malloc(sizeof(installer));
	i->pos=0;

	GtkWindow *window=NULL;
	GtkFixed *fixed=NULL;
	GtkLayout *partition_layout=NULL;
	GtkGrid *partition_grid=NULL;
	GtkFixed *installation_layout=NULL;
	GtkFixed *gpl_layout=NULL;
	GtkBox *box=NULL;
	GtkButton *next_button=NULL;

	//building gui from xml file
	GtkBuilder *builder=gtk_builder_new();
	GtkBuilder *partition_builder=gtk_builder_new();
	GtkBuilder *gpl_builder=gtk_builder_new();
	GtkBuilder *instalation_builder=gtk_builder_new();
	gtk_builder_add_from_file(builder,"ui/main_window.ui",NULL);
	gtk_builder_add_from_file(partition_builder,"ui/partition.ui",NULL);
	gtk_builder_add_from_file(gpl_builder,"ui/gpl.ui",NULL);
	gtk_builder_add_from_file(instalation_builder,"ui/installation.ui",NULL);
	
	//getting gui object
	window=GTK_WINDOW(gtk_builder_get_object(builder,"main_window"));
	fixed=GTK_FIXED(gtk_builder_get_object(builder,"fixed_layout"));
	partition_layout=GTK_LAYOUT(gtk_builder_get_object(partition_builder,"partition_layout"));
	partition_grid=GTK_GRID(gtk_builder_get_object(partition_builder,"partition_grid"));
	gpl_layout=GTK_FIXED(gtk_builder_get_object(gpl_builder,"gpl_layout"));
	installation_layout=GTK_FIXED(gtk_builder_get_object(instalation_builder,"installation_layout"));
	box=GTK_BOX(gtk_builder_get_object(partition_builder,"button_box"));
	next_button=GTK_BUTTON(gtk_builder_get_object(builder,"next_button"));

	//signals
	g_signal_connect(G_OBJECT(next_button),"pressed",G_CALLBACK(next_click),i);

	gtk_grid_insert_row(partition_grid,3);
	gtk_grid_attach(partition_grid,gtk_label_new("ayoub"),0,3,1,1);
	gtk_fixed_put(fixed,GTK_WIDGET(installation_layout),0,0);
	i->layouts[0]=(GtkWidget*)installation_layout;

	//gtk_widget_destroy(GTK_WIDGET(partition_layout));
	//gtk_fixed_put(fixed,GTK_WIDGET(gtk_label_new("test")),0,0);

	//window customization
	gtk_window_set_default_size(GTK_WINDOW(window),1000,600);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

	//event management
	//g_signal_connect(G_OBJECT(window),"delete",G_CALLBACK(gtk_widget_destroy),window);

	//starting gui
    gtk_application_add_window(app,GTK_WINDOW(window));
	gtk_widget_show_all(GTK_WIDGET(window));

}

void next_click(GtkApplication* app,gpointer data){
	
	if(data==NULL) return;

	installer* i=(installer*)data;

	switch(i->pos){

		case 0:
			i->layouts[1]=gtk_builder_get_object(builder_license,"");
		break;

		case 1:

		break;

		case 2:

		break;

		case 3:

		break;

		case 4:

		break;

		case 5:

		break;

		case 6:

		break;

		case 7:

		break;

		case 8:

		break;

	}
	//layout_next(i->main_fixed,*(i->layouts[i->pos]),*(i->layouts[i->pos+1]));
	gtk_widget_destroy((GtkWidget*)(i->layouts[i->pos]));
	i->pos++;
	
}