#include "init_funcs.h"

void init_images(installer* inst, GtkBuilder* builder){
	GtkImage *logo_img,*welcome_img,*disk_img,*refresh_img,*lang_img,*time_img,*user_img,*password_img,*showpass_img,*end_img;

	logo_img = GTK_IMAGE(gtk_builder_get_object(builder,"logo_image"));
	welcome_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[0],"welcome_image"));
	disk_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[2],"disk_image"));
	refresh_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[2],"refresh_image"));
	lang_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[3],"lang_image"));
	time_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[3],"time_image"));
	user_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[4],"user_image"));
	password_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[4],"password_image"));
	showpass_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[4],"showpass_image"));
	end_img = GTK_IMAGE(gtk_builder_get_object(inst->builders[7],"end_image"));

	gtk_image_set_from_file(logo_img,"img/logo.png");
	gtk_image_set_from_file(welcome_img,"img/welcome.png");
	gtk_image_set_from_file(disk_img,"img/harddisk.png");
	gtk_image_set_from_file(refresh_img,"img/refresh.png");
	gtk_image_set_from_file(lang_img,"img/lang.png");
	gtk_image_set_from_file(time_img,"img/time.png");
	gtk_image_set_from_file(user_img,"img/username.png");
	gtk_image_set_from_file(password_img,"img/password.png");
	gtk_image_set_from_file(showpass_img,"img/show_pass.png");
	gtk_image_set_from_file(end_img,"img/end.png");
}

void init_user_info(installer* inst){

	if(inst==NULL) return;

	inst->uinfo.username=malloc(32*sizeof(char));
	inst->uinfo.hostname=malloc(21*sizeof(char));
	inst->uinfo.password=malloc(21*sizeof(char));
	inst->uinfo.auto_login=1;
	inst->uinfo.visible=FALSE;
	inst->uinfo.showpass_button=GTK_BUTTON(gtk_builder_get_object(inst->builders[4],"showpass_button"));

	return;
}

void init_zone_combo(GtkWidget* w, gpointer data){

	installer* inst = (installer*) data;
	gtk_combo_box_text_remove_all(inst->linfo.zone_combo);
	gchar* tmp=malloc(40);
	gchar* active_region = gtk_combo_box_text_get_active_text(inst->linfo.region_combo);
	gchar* tmp_region = malloc(20);
	gchar* tmp_zone = malloc(20);
	inst->linfo.timezone_file=fopen("ui/timezone.txt","r");
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

	SPedDevice devs=sped_get_device();
	for(int i=0;i<devs.device_count;i++)
			gtk_combo_box_text_append_text(inst->pinfo.disk_list,devs.device[i]->path);
}

void init_installation(installer* inst){

	
	inst->spinner[0]=GTK_SPINNER(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"cp_file_spinner"));
	inst->spinner[1]=GTK_SPINNER(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"cp_kernel_spinner"));
	inst->spinner[2]=GTK_SPINNER(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"system_conf_spinner"));
	inst->spinner[3]=GTK_SPINNER(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"user_conf_spinner"));
	inst->spinner[4]=GTK_SPINNER(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"ramdisk_spinner"));
	inst->spinner[5]=GTK_SPINNER(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"bootloader_spinner"));

	inst->image[0]=GTK_IMAGE(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"cp_file_image"));
	inst->image[1]=GTK_IMAGE(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"cp_kernel_image"));
	inst->image[2]=GTK_IMAGE(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"system_conf_image"));
	inst->image[3]=GTK_IMAGE(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"user_conf_image"));
	inst->image[4]=GTK_IMAGE(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"ramdisk_image"));
	inst->image[5]=GTK_IMAGE(get_child_by_name(GTK_CONTAINER(inst->layouts[6]),"bootloader_image"));

	gtk_spinner_start(inst->spinner[0]);
	

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

void init_summary(installer* inst){
  GtkLabel *lpartition,*ltimezone,*lkeyboard,*llanguage,*lusername,*lhostname,*lautologin,*lsize,*lbootloader;
	lpartition=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"partition_summary"));
	ltimezone=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"timezone_summary"));
	lkeyboard=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"keyboard_summary"));
	llanguage=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"language_summary"));
	lusername=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"username_summary"));
	lhostname=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"hostname_summary"));
	lautologin=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"autologin_summary"));
	lbootloader=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"bootloader_summary"));
  	lsize=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[5]),"size_summary"));

	gtk_label_set_text(lpartition,inst->pinfo.selected_partition);
	gtk_label_set_text(ltimezone,inst->linfo.timezone);


	if (!strcmp(inst->linfo.language,"fr_FR.UTF-8"))
			gtk_label_set_text(llanguage,"français (France)");
	else
	    gtk_label_set_text(llanguage,"english (United States)");


	if (!strcmp(inst->linfo.keyboard,"fr"))
			gtk_label_set_text(lkeyboard,"Français (AZERTY)");
	else
			gtk_label_set_text(lkeyboard,"Anglais (QWERTY)");
		
	gtk_label_set_text(lusername,inst->uinfo.username);
	gtk_label_set_text(lhostname,inst->uinfo.hostname);
	gtk_label_set_text(lsize,get_human_size(inst->pinfo.spartition_size));
	if (inst->uinfo.auto_login==1)
			gtk_label_set_text(lautologin,"Oui");
	else
			gtk_label_set_text(lautologin,"Non");
	gtk_label_set_text(lbootloader,inst->pinfo.selected_disk);
}

void init_builders(installer* inst){
	for (int i=0; i<8;i++)	inst->builders[i]=gtk_builder_new();
	gtk_builder_add_from_file(inst->builders[0],"ui/welcome.ui", NULL);
	gtk_builder_add_from_file(inst->builders[1],"ui/license.ui", NULL);
	gtk_builder_add_from_file(inst->builders[2],"ui/partition.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[3],"ui/langtime.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[4],"ui/user.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[5],"ui/summary.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[6],"ui/installation.ui", NULL);
  	gtk_builder_add_from_file(inst->builders[7],"ui/end.ui", NULL);
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

	gtk_builder_add_from_file(builder, "ui/installer.ui", NULL);
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
  	init_installation(inst);
  	init_images(inst,builder);

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
