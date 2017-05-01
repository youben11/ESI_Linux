#include "init_funcs.h"
#include "simple_parted/sparted.h"
#include <string.h>

void init_user_info(installer* inst){

	if(inst==NULL) return;

	inst->uinfo.username=malloc(32*sizeof(char));
	inst->uinfo.hostname=malloc(21*sizeof(char));
	inst->uinfo.password=malloc(21*sizeof(char));
	inst->uinfo.auto_login=1;

	return;
}

//NOT COMPLETED
void init_partition(installer* inst){

	SPedDisk disks=sped_get_disk();
	char** disks_path=malloc(sizeof(char*)*disks.disk_count);
	for(int i=0;i<disks.disk_count;i++){
		disks_path[i]=malloc(sizeof(char)*(strlen(disks.disk[i]->dev->path)+1));
		strcpy(disks_path[i],disks.disk[i]->dev->path);
	}

	GtkComboBoxText* disk_list=GTK_COMBO_BOX_TEXT(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"disk_list"));
	
	if(disk_list==NULL){
		g_print("error in getting object disk_list\n");
		return;
	}
	gtk_combo_box_text_remove_all(disk_list);

	for(int i=0;i<disks.disk_count;i++){
		gtk_combo_box_text_append(disk_list,NULL,disks_path[i]);
	}

}

void init_time_lang(installer* inst){

    if (inst == NULL) return;

    inst->linfo.keyboard=malloc(32*sizeof(char));
    inst->linfo.language=malloc(32*sizeof(char));
    inst->linfo.time_zone=malloc(40*sizeof(char));

    return;
}

void init_builders(installer* inst){
	for (int i=0; i<8;i++)	inst->builders[i]=gtk_builder_new();
	gtk_builder_add_from_file(inst->builders[0],"res/welcome.ui", NULL);
	gtk_builder_add_from_file(inst->builders[1],"res/license.ui", NULL);
	gtk_builder_add_from_file(inst->builders[2],"res/partition.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[3],"res/langtime.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[4],"res/user.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[5],"res/summary.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[6],"res/installation.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[7],"res/end.ui", NULL);
}

void init_listbox(GtkListBox* listbox){
	GtkWidget* label =NULL;
	gint selected=0;
	GtkListBoxRow* row;
  	gchar* labels[] = {"Bienvenue","License","Partitionement",
	"Langue/Heure","Utilisateur","Résumé","Installation","Fin"
  	};
	// Creation des Labels et modifications des etats des boutons
	for (gint i=0; i <8; i++){
		label = gtk_label_new(labels[i]);
		gtk_widget_set_size_request(label,100,40);
		gtk_list_box_insert(listbox,label,i);
		row = gtk_list_box_get_row_at_index(listbox,i);
		gtk_list_box_row_set_selectable(row,FALSE);
		if (i==selected) gtk_list_box_row_set_selectable(row,TRUE);
	  }
	gtk_list_box_set_selection_mode(listbox,GTK_SELECTION_SINGLE);
  	gtk_list_box_select_row(listbox,gtk_list_box_get_row_at_index(listbox,selected));
}


void init_structure(installer* inst,GtkBuilder *builder){
	gtk_builder_add_from_file(builder, "res/installer.ui", NULL);
  	inst->pos=0;
  	inst->listbox = GTK_LIST_BOX(gtk_builder_get_object(builder,"listbox"));
  	inst->main_fixed = GTK_FIXED(gtk_builder_get_object(builder,"main_fixed"));
  	inst->buttons[0] = GTK_BUTTON(gtk_builder_get_object(builder,"button_back"));
  	inst->buttons[1] = GTK_BUTTON(gtk_builder_get_object(builder,"button_next"));
  	inst->buttons[2] = GTK_BUTTON(gtk_builder_get_object(builder,"button_finish"));
  	inst->window = GTK_WINDOW(gtk_builder_get_object (builder, "window"));
	gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[2]),FALSE);
  	gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),FALSE);
}

void init_layout(installer* inst){
	inst->layouts[0] = GTK_WIDGET(gtk_builder_get_object(inst->builders[0],"welcome_layout"));
	inst->layouts[1]=(GtkWidget*)gtk_builder_get_object(inst->builders[1],"license_layout");
	inst->layouts[2]=(GtkWidget*)gtk_builder_get_object(inst->builders[2],"partition_layout");
	inst->layouts[3]=(GtkWidget*)gtk_builder_get_object(inst->builders[3],"langtime_layout");
	inst->layouts[4]=(GtkWidget*)gtk_builder_get_object(inst->builders[4],"user_layout");
	inst->layouts[5]=(GtkWidget*)gtk_builder_get_object(inst->builders[5],"summary_layout");
	inst->layouts[6]=(GtkWidget*)gtk_builder_get_object(inst->builders[6],"installation_layout");
	inst->layouts[7]=(GtkWidget*)gtk_builder_get_object(inst->builders[7],"end_layout");

}

void replace_layout(GtkFixed* container , GtkWidget* layout_to_destroy , GtkWidget* layout_to_put){
	g_object_ref(layout_to_destroy);
	gtk_container_remove(GTK_CONTAINER(container),layout_to_destroy);
	gtk_fixed_put(container,GTK_WIDGET(layout_to_put),0,0);
}


