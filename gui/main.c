#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

void gquit(GtkWidget* widget,gpointer data){
	g_print("Quit\n");
	gtk_main_quit();
}

int main(int argc,char** argv){

	gtk_init(&argc,&argv);
	
	GtkAssistant *assistant=NULL;

	//building gui from xml file
	GtkBuilder *builder=gtk_builder_new();
	gtk_builder_add_from_file(builder,"firstWindow.glade",NULL);
	
	//getting gui object
	assistant=GTK_ASSISTANT(gtk_builder_get_object(builder,"main"));
	
	//event management
	g_signal_connect(G_OBJECT(assistant),"cancel",G_CALLBACK(gquit),NULL);
	g_signal_connect(G_OBJECT(assistant),"close",G_CALLBACK(gquit),NULL);

	gtk_widget_show_all(GTK_WIDGET(assistant));	
	gtk_main();

	return EXIT_SUCCESS;
}
