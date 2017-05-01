#include "partition_funcs.h"

gchar* fs_filter(gchar* filesystem){
	if (strstr(filesystem,"swap")) return "swap";
	return filesystem;
}

void fill_grid_row(GtkGrid* row,gchar* partition,gchar* name,gchar* filesystem,gchar* type,gchar* size){
	GtkLabel *lpartition,*lname,*lfilesystem,*ltype,*lsize;
	lpartition=GTK_LABEL(gtk_grid_get_child_at(row,1,0));
	lname=GTK_LABEL(gtk_grid_get_child_at(row,2,0));
	lfilesystem=GTK_LABEL(gtk_grid_get_child_at(row,3,0));
	ltype=GTK_LABEL(gtk_grid_get_child_at(row,4,0));
	lsize=GTK_LABEL(gtk_grid_get_child_at(row,5,0));
	gtk_label_set_text(lpartition,partition);
	gtk_label_set_text(lname,name);
	gtk_label_set_text(lfilesystem,filesystem);
	gtk_label_set_text(ltype,type);
	gtk_label_set_text(lsize,size);
	gtk_widget_set_visible(GTK_WIDGET(row),TRUE);
}

gchar* get_human_size(long long size){
	long double s=size/1048576;
	gchar* st = malloc(10);
	sprintf(st,"%.2llf MiB",s);
	if (s > 1024)
		sprintf(st,"%.2llf GiB",s/1024);
	else if (s/1024 > 1024)
		sprintf(st,"%.2llf TiB",s/1024/1024);
	return st;
}

void clear_grid(GtkGrid* glb, gint pos_start){
	GtkGrid* row;
	for (int i=pos_start; i<10; i++){
		row=GTK_GRID(gtk_grid_get_child_at(glb,0,i));
		gtk_widget_set_visible(GTK_WIDGET(row),FALSE);
	}
}

void fill_partition_grid(GtkGrid* grid,SPedDiskInfo dinfo){
	GtkGrid* row;
	SPedPartitionInfo pi;
	for(int i=0;i<dinfo.partition_info_count;i++){
			pi=dinfo.partition_info[i];
			row=GTK_GRID(gtk_grid_get_child_at(grid,0,i));
			fill_grid_row(row,pi.path,pi.name,fs_filter(pi.fs),pi.type,get_human_size(pi.size));
	}
	clear_grid(grid,dinfo.partition_info_count);
}

gchar* get_partition_path_at_index(installer* inst,gint i){
	GtkGrid* row=GTK_GRID(gtk_grid_get_child_at(inst->pinfo.partition_grid,0,i));
	GtkLabel* l= GTK_LABEL(get_child_by_name(GTK_CONTAINER(row),"part_label"));
	return (gchar*)gtk_label_get_text(l);

}

PedSector get_partition_size_at_index(installer* inst,gint in){
	SPedDisk disk=sped_get_disk_path(inst->pinfo.selected_disk);
	SPedDiskInfo dinfo=sped_get_disk_info(disk.disk[0]);
	for(int i=0;i<dinfo.partition_info_count;i++)
			if (i==in)
				return dinfo.partition_info[i].size;
}

int get_active_radio_button(installer* inst){
		GtkRadioButton* radio_button = GTK_RADIO_BUTTON(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"radio1"));
		GSList* radio_list = gtk_radio_button_get_group(radio_button);
		GtkToggleButton *selected_radio = NULL;
		int count=10;
		while (radio_list && count-- >0)
		{
  			selected_radio = radio_list->data;
  			radio_list =g_slist_next(radio_list);
  			if (gtk_toggle_button_get_active(selected_radio))	break;
		}
		return count;
}

GtkToggleButton* get_radio_button_at_index(installer* inst,gint i){
		GtkRadioButton* radio_button = GTK_RADIO_BUTTON(get_child_by_name(GTK_CONTAINER(inst->layouts[2]),"radio1"));
		GSList* radio_list = gtk_radio_button_get_group(radio_button);
		GtkToggleButton *selected_radio = NULL;
		int count=9;
		while (radio_list && count != i){
			selected_radio=radio_list->data;
			radio_list=g_slist_next(radio_list);
		}
		return selected_radio;
}
