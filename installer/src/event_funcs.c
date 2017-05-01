#include "structs.h"
#include "event_funcs.h"
#include "init_funcs.c"
#include <string.h>
#include <ctype.h>
#include "script_ctrl.h"

#define INST_SCRIPT "./install.sh"

/*Old one
void disk_choosed(installer* inst){

	GtkComboBoxText* disk_list=GTK_COMBO_BOX_TEXT(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"disk_list"));
	char* n_disk=(char*) gtk_combo_box_text_get_active_text(disk_list);

	if(n_disk==NULL) return;

	GtkLabel* disk_name=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"disk_name"));
	//set the name of the choosed disk
	gtk_label_set_text(disk_name,n_disk);
	//getting disk info
	SPedDiskInfo dinfo=sped_get_disk_info(sped_get_disk_by_name(n_disk));
	
	//set the size of the choosed disk
	char* s_disk=malloc(sizeof(char)*16);

	if((dinfo.size/1024/1024)<1024)
		sprintf(s_disk,"%lldMb",dinfo.size/1024/1024);	
	else 
		sprintf(s_disk,"%lldGb",dinfo.size/1024/1024/1024);

	GtkLabel* disk_size=GTK_LABEL(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"disk_size"));
	gtk_label_set_text(disk_size,s_disk);

	g_free(n_disk);
	free(s_disk);

}*/

void install(GtkWidget* w,gpointer data){

	installer* inst=(installer*)data;

	if(inst->pos!=6) return;

	char* arg[]={INST_SCRIPT,(char*)inst->pinfo.selected_partition,inst->uinfo.username,inst->uinfo.password,inst->uinfo.password,"en_US.UTF-8","fr",inst->uinfo.hostname,NULL};

	ps_info psinfo=script_ctrl(INST_SCRIPT,arg);

	
	//test party
	FILE *script_stream = fdopen(psinfo.stdout_fd, "r");
		if (script_stream == NULL){
			perror("can't open file descriptor");
			return ;
		}

		char buffer[4096];

		//int catch_it=0;
		while ( fgets(buffer, sizeof(buffer), script_stream) ) {
			//if (catch_it)
				printf("%s",buffer);
			//catch_it = (strncmp(buffer, "[#@", 3) == 0);
		}

		// closing the pipes read ends
		fclose(script_stream);

		int pid_status;
		//waiting for the child process to terminate
		if ( waitpid(psinfo.pid,&pid_status,0) == -1 ){
			perror("ERROR while waiting for child process");
			return ;
		}
		//checking the exit status of the child process
		if (WEXITSTATUS(pid_status)){
			perror("ERROR exit status of the child process is diffrent from zero");
			return ;
    }

    gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
	
}

void open_gparted(GtkWidget* w,gpointer data){
	system("gparted");
}

void refresh_disk_list(GtkWidget* w , gpointer data){
	installer* inst = (installer*) data;
	g_signal_handlers_disconnect_by_func(inst->pinfo.disk_list, init_partition, inst);
	gtk_combo_box_text_remove_all(inst->pinfo.disk_list);
	gtk_combo_box_text_append_text(inst->pinfo.disk_list,"Selectionnez un disque ...");
	init_disk_list(inst);
	gtk_combo_box_set_active(GTK_COMBO_BOX(inst->pinfo.disk_list),0);
	clear_grid(inst->pinfo.partition_grid,0);
  	gtk_label_set_text(inst->pinfo.disk_size,"");
	gtk_label_set_text(inst->pinfo.disk_name,"");
	g_signal_connect(G_OBJECT(inst->pinfo.disk_list),"changed",G_CALLBACK(init_partition),inst);

}

int isValidName(char* name){

	for(int i=0;i<strlen(name);i++){
		if(!isalnum(name[i]) && name[i]!='-')
			return 0;
	}
	return 1;
}

void alert_dialog(GtkWindow* window,char* message){
	GtkMessageDialog *dialog=GTK_MESSAGE_DIALOG(gtk_message_dialog_new(window,
																		GTK_DIALOG_DESTROY_WITH_PARENT,
																		GTK_MESSAGE_INFO,
																		GTK_BUTTONS_OK,
																		message));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

GtkWidget* get_child_by_name(GtkContainer* c,char* name){
	
	const gchar* wname=gtk_widget_get_name(GTK_WIDGET(c));
    
    if(!strcmp(name,wname))
        return GTK_WIDGET(c);
      	
    if (GTK_IS_CONTAINER(c)){

        GList* l=gtk_container_get_children(GTK_CONTAINER(c));
        GtkWidget* w;
        char* wname;
            
        while(l!=NULL){
            w = get_child_by_name(l->data,name);
                    
            if(w) return w;
                    
            l=g_list_next(l);

        }
    }

    return NULL;

}

void save_user_info(installer* inst){

	if(inst==NULL) return;

	GtkEntry *username,*hostname,*password;
	GtkCheckButton* auto_login;

	//getting child widget
	username=GTK_ENTRY(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_username"));
	hostname=GTK_ENTRY(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_hostname"));
	password=GTK_ENTRY(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_password"));
	auto_login=GTK_CHECK_BUTTON(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_auto_login"));

	//getting user info
	strcpy(inst->uinfo.username,gtk_entry_get_text(username));
	strcpy(inst->uinfo.hostname,gtk_entry_get_text(hostname));
	strcpy(inst->uinfo.password,gtk_entry_get_text(password));

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(auto_login))==TRUE)
		inst->uinfo.auto_login=1;
	else inst->uinfo.auto_login=0;

}

void save_time_lang(installer* inst){

    if(inst==NULL) return;

    GtkComboBoxText *keyboard,*language,*time_zone;

    //getting child widget
    time_zone=(GtkComboBoxText*)get_child_by_name(GTK_CONTAINER(inst->layouts[3]),"langtime_timezone");
    keyboard=(GtkComboBoxText*)get_child_by_name(GTK_CONTAINER(inst->layouts[3]),"langtime_keyboard");
    language=(GtkComboBoxText*)get_child_by_name(GTK_CONTAINER(inst->layouts[3]),"langtime_lang");

    //getting time_zone info
    strcpy(inst->linfo.time_zone,gtk_combo_box_text_get_active_text(time_zone));
    strcpy(inst->linfo.keyboard,gtk_combo_box_text_get_active_text(keyboard));
    strcpy(inst->linfo.language,gtk_combo_box_text_get_active_text(language));

}

int check_partition_info(installer* inst){
	inst->pinfo.spartition_size=get_partition_size_at_index(inst,get_active_radio_button(inst));
	if (inst->pinfo.spartition_size < 16106127360)
			return 0;
	return 1;
}

int check_user_info(installer* inst){

	if(inst==NULL) return 0;

	GtkEntry *username,*hostname,*password,*password2;
	char *uname,*hname,*pass,*pass2;
	uname=malloc(sizeof(char)*32);
	hname=malloc(sizeof(char)*21);
	pass=malloc(sizeof(char)*21);
	pass2=malloc(sizeof(char)*21);

	//getting child widget
	username=GTK_ENTRY(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_username"));
	hostname=GTK_ENTRY(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_hostname"));
	password=GTK_ENTRY(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_password"));
	password2=GTK_ENTRY(get_child_by_name(GTK_CONTAINER(inst->layouts[4]),"user_password2"));

	//getting user info
	strcpy(uname,gtk_entry_get_text(username));
	strcpy(hname,gtk_entry_get_text(hostname));
	strcpy(pass,gtk_entry_get_text(password));
	strcpy(pass2,gtk_entry_get_text(password2));

	if(strlen(uname)==0){
		alert_dialog(inst->window,"Vous devez entrez un nom d'utilisateur");
		return 0;
	}

	if(!isalpha(*uname)){
		alert_dialog(inst->window,"Le nom d'utilisateur doit commencer par une lettre");
		return 0;	
	}

	if(!isValidName(uname)){
		alert_dialog(inst->window,"Le nom d'utilisateur doit contenir que des lettres et des nombre ou un tiret(-) ");
		return 0;
	}

	if(strlen(hname)==0){
		alert_dialog(inst->window,"Vous devez entrez un nom de machine");
		return 0;
	}

	if(!isalpha(*hname)){
		alert_dialog(inst->window,"Le nom de la machine doit commencer par une lettre");
		return 0;	
	}

	if(!isValidName(hname)){
		alert_dialog(inst->window,"Le nom de la machine doit contenir que des lettres et des nombre ou un tiret(-) ");
		return 0;
	}

	if(strlen(pass)==0){
		alert_dialog(inst->window,"Vous devez entrez un mot de passe");
		return 0;
	}

	if(strcmp(pass,pass2)){
		alert_dialog(inst->window,"Les deux mot de passe ne sont pas identique");
		return 0;
	}

	free(uname);
	free(hname);
	free(pass);
	free(pass2);

	return 1;
}

void exit_finish(GtkWidget *w, gpointer userdata){
	g_application_quit(G_APPLICATION(userdata));
}


void layout_next(GtkFixed* main_fixed,GtkWidget* selected_layout,GtkWidget* wanted_layout,GtkListBox* listbox,gint selected, gint position){
	replace_layout(main_fixed,selected_layout,wanted_layout);
	gtk_list_box_unselect_row(listbox,gtk_list_box_get_row_at_index(listbox,selected));
	gtk_list_box_row_set_selectable(gtk_list_box_get_row_at_index(listbox,selected),FALSE);
	gtk_list_box_row_set_selectable(gtk_list_box_get_row_at_index(listbox,position),TRUE);
	gtk_list_box_select_row(listbox,gtk_list_box_get_row_at_index(listbox,position));

}




void next_click(GtkApplication* app,gpointer data){

	if(data==NULL) return;

	installer* inst=(installer*)data;

	int checked=1;


	switch(inst->pos){

	  case 0: // welcome
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);

	  break;

  	  case 1: // license
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),FALSE);
		refresh_disk_list(GTK_WIDGET(inst->buttons[4]),inst);

	  break;

	  case 2: // partition
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
		if((checked=check_partition_info(inst))){
					inst->pinfo.selected_partition=get_partition_path_at_index(inst,get_active_radio_button(inst));
					gchar* message = malloc(54);
					sprintf(message,"ESI Linux va etre installe dans la partition %s",inst->pinfo.selected_partition);
					alert_dialog(inst->window,message);
									//g_print("Disk: %s\nPartition: %s\nSize: %s",inst->pinfo.selected_disk,inst->pinfo.selected_partition,get_human_size(inst->pinfo.spartition_size));
				}
                else
					alert_dialog(inst->window,"Il vous faut au moins 15 GiB d'espace disque");

	  break;

	  case 3: // lang time
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
		save_time_lang(inst);
		

	  break;

	  case 4: // user
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
		if(checked=check_user_info(inst)){
			save_user_info(inst);
			gtk_button_set_label(inst->buttons[1],"Installer");
		}

	  break;

	  case 5: // summary
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),FALSE);
		//g_print("username:%s\nhostname:%s\npassword:%s\nautologin:%d\nkeyobard:%s\nlanguage:%s\ntimezone:%s\npartition:%s\n",
		//		inst->uinfo.username,inst->uinfo.hostname,inst->uinfo.password,inst->uinfo.auto_login,inst->linfo.keyboard,inst->linfo.language,inst->linfo.time_zone,(char*)inst->pinfo.selected_partition);

	  break;

	  case 6: // installation
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[2]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),FALSE);
	  break;


	}

	//Goto the next layout
	if(checked){
		layout_next(inst->main_fixed,inst->layouts[inst->pos],inst->layouts[inst->pos+1],inst->listbox,inst->pos,inst->pos+1);
		inst->pos++;
	}

}


void back_click(GtkApplication* app,gpointer data){

	if(data==NULL) return;
	installer* inst=(installer*)data;
	switch(inst->pos){

  	  case 1: // License
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
	  break;

	  case 2:  // Partitionement
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
	  break;

	  case 3: // Lang
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
		//disk_choosed(inst);
	  break;

	  case 4: // user
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
	  break;

	  case 5: // summary
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[0]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(inst->buttons[1]),TRUE);
		gtk_button_set_label(inst->buttons[1],"Suivant");
	  break;

	  case 6: // installation

	  break;

	}

	layout_next(inst->main_fixed,inst->layouts[inst->pos],inst->layouts[inst->pos-1],inst->listbox,inst->pos,inst->pos-1);
	inst->pos--;

}
