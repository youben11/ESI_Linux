#include <gtk/gtk.h>
#include <stdlib.h>
#include "event_funcs.c"



static void activate (GtkApplication *app,gpointer  user_data)
{
  // Init installer struct
  installer* inst = malloc(sizeof(*inst));
  GtkBuilder *builder = gtk_builder_new();
  init_builders(inst);
  init_layout(inst);
  init_structure(inst,builder);
  init_listbox(inst->listbox);
  init_user_info(inst);
  init_time_lang(inst);

  gtk_fixed_put(inst->main_fixed,inst->layouts[0],0,0);

  //signals
  g_signal_connect(G_OBJECT(inst->buttons[2]),"clicked",G_CALLBACK(exit_finish),app);
  g_signal_connect(G_OBJECT(inst->buttons[1]),"clicked",G_CALLBACK(next_click),inst);
  g_signal_connect(G_OBJECT(inst->buttons[0]),"clicked",G_CALLBACK(back_click),inst);
  //g_signal_connect(G_OBJECT(inst->window),"destroy",G_CALLBACK(g_application_quit),app);

  // Start the app
  gtk_application_add_window(app,inst->window);
  gtk_widget_show_all(GTK_WIDGET(inst->window));
  g_object_unref(app);
}

int main(int   argc,  char *argv[]){

  int status;

  g_autoptr(GtkApplication) app = NULL;
  
  app = gtk_application_new ("org.esilinux.installer", G_APPLICATION_FLAGS_NONE);
  
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  
  status = g_application_run (G_APPLICATION (app), argc, argv);
  
  return status;
}
