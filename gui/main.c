#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

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
    
	GtkAssistant *assistant=NULL;

	//building gui from xml file
	GtkBuilder *builder=gtk_builder_new();
	gtk_builder_add_from_file(builder,"installer1.glade",NULL);
	
	//getting gui object
	assistant=GTK_ASSISTANT(gtk_builder_get_object(builder,"main"));
    
	//assistant customization
	gtk_window_set_default_size(GTK_WINDOW(assistant),800,600);
	gtk_window_set_position(GTK_WINDOW(assistant),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(assistant),FALSE);
	
	//event management
	g_signal_connect(G_OBJECT(assistant),"cancel",G_CALLBACK(gtk_widget_destroy),assistant);
	g_signal_connect(G_OBJECT(assistant),"close",G_CALLBACK(gtk_widget_destroy),assistant);

	//starting gui
	//GTimeZone* tz=g_time_zone_new(NULL);
	//gtk_application_add_window(app,GTK_WINDOW(tz));
    gtk_application_add_window(app,GTK_WINDOW(assistant));
	gtk_widget_show_all(GTK_WIDGET(assistant));
    
}