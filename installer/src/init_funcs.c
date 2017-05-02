#include "init_funcs.h"
#include "simple_parted/sparted.h"
#include <string.h>
#include "partition_funcs.c"

void init_user_info(installer* inst){

	if(inst==NULL) return;

	inst->uinfo.username=malloc(32*sizeof(char));
	inst->uinfo.hostname=malloc(21*sizeof(char));
	inst->uinfo.password=malloc(21*sizeof(char));
	inst->uinfo.auto_login=1;

	return;
}

void init_zone_combo(GtkWidget* w, gpointer data){

	installer* inst = (installer*) data;
	gtk_combo_box_text_remove_all(inst->linfo.zone_combo);
	gchar* tmp=malloc(40);
	gchar* active_region = gtk_combo_box_text_get_active_text(inst->linfo.region_combo);
	gchar* tmp_region = malloc(20);
	gchar* tmp_zone = malloc(20);
	inst->linfo.timezone_file=fopen("res/timezone.txt","r");
	while (!feof(inst->linfo.timezone_file)){
  		fscanf(inst->linfo.timezone_file,"%s\n",tmp);
			tmp_region=strtok(tmp,"/");
			tmp_zone=strtok(NULL,"/");
			if (!strcmp(tmp_region,active_region))
					gtk_combo_box_text_append_text(inst->linfo.zone_combo,tmp_zone);;
	}
	fclose(inst->linfo.timezone_file);

}

void init_disk_list(installer* inst){
	ped_device_free_all ();
	ped_device_probe_all();
 	PedDevice* pdv = NULL;
	while ((pdv=ped_device_get_next(pdv)) != NULL)
			gtk_combo_box_text_append_text(inst->pinfo.disk_list,pdv->path);
}

void init_partition(GtkWidget *w, gpointer ins){
		installer* inst = (installer*) ins;
		GtkToggleButton* first_radio = get_radio_button_at_index(inst,0);
		gchar* selected_disk=gtk_combo_box_text_get_active_text(inst->pinfo.disk_list);
        SPedDisk disk;
		SPedDiskInfo dinfo;
		PedSector disksize;
		if(strcmp(selected_disk,"Selectionnez un disque ..."))
		{
			  gtk_toggle_button_set_active(first_radio,TRUE);
			  gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
				inst->pinfo.selected_disk=selected_disk;
				disk=sped_get_disk_path(selected_disk);
				dinfo=sped_get_disk_info(disk.disk[0]);
				disksize = sped_get_device_path(selected_disk).device[0]->length*512;
				gtk_label_set_text(inst->pinfo.disk_size,get_human_size(disksize)); // set the chosen disk size
				gtk_label_set_text(inst->pinfo.disk_name,selected_disk);  // Set the chosen disk name
				fill_partition_grid(inst->pinfo.partition_grid,dinfo); // Fill the tables with the detected partitions
		}
		else{
			  gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),FALSE);
				clear_grid(inst->pinfo.partition_grid,0);
				gtk_label_set_text(inst->pinfo.disk_name,"");
				gtk_label_set_text(inst->pinfo.disk_size,"");
		}


}

void init_partition_structure(installer* inst){
	inst->pinfo.partition_grid=GTK_GRID(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"part_list"));
	inst->pinfo.disk_list= GTK_COMBO_BOX_TEXT(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"disk_list"));
	inst->pinfo.disk_name= GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"disk_name"));
	inst->pinfo.disk_size= GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"disk_size"));
}

void init_time_lang(installer* inst){

    if (inst == NULL) return;

    inst->linfo.keyboard=malloc(32*sizeof(char));
    inst->linfo.language=malloc(32*sizeof(char));
    inst->linfo.region=malloc(20*sizeof(char));
    inst->linfo.zone=malloc(20*sizeof(char));
    inst->linfo.timezone=malloc(40*sizeof(char));

    inst->linfo.region_combo=GTK_COMBO_BOX_TEXT(gtk_builder_get_object(inst->builders[3],"region_combo"));
	inst->linfo.zone_combo  =GTK_COMBO_BOX_TEXT(gtk_builder_get_object(inst->builders[3],"zone_combo"));

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


void init_installer(installer* inst,GtkBuilder *builder){
	
	init_builders(inst);
  	init_layout(inst);

	gtk_builder_add_from_file(builder, "res/installer.ui", NULL);
  	inst->pos=0;
  	inst->listbox = GTK_LIST_BOX(gtk_builder_get_object(builder,"listbox"));
  	inst->main_fixed = GTK_FIXED(gtk_builder_get_object(builder,"main_fixed"));
  	inst->buttons[0] = GTK_BUTTON(gtk_builder_get_object(builder,"button_back"));
  	inst->buttons[1] = GTK_BUTTON(gtk_builder_get_object(builder,"button_next"));
  	inst->buttons[2] = GTK_BUTTON(gtk_builder_get_object(builder,"button_finish"));
  	inst->buttons[3] = GTK_BUTTON(gtk_builder_get_object(inst->builders[2],"butt_gparted"));
	inst->buttons[4] = GTK_BUTTON(gtk_builder_get_object(inst->builders[2],"refresh_button"));
  	inst->window = GTK_WINDOW(gtk_builder_get_object (builder, "window"));
	gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[2]),FALSE);
  	gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),FALSE);
  	init_partition_structure(inst);
  	init_disk_list(inst);
  	init_listbox(inst->listbox);
  	init_user_info(inst);
  	init_time_lang(inst);

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

/*
//Old one
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

}*/
