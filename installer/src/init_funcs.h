#ifndef DEF_INIT_FUNCS
#define DEF_INIT_FUNCS

void init_user_info(installer* inst);
void init_partition(installer* inst);
void init_builders(installer* inst);
void init_structure(installer* inst,GtkBuilder *builder);
void init_listbox(GtkListBox* listbox);
void replace_layout(GtkFixed* container , GtkWidget* layout_to_destroy , GtkWidget* layout_to_put);
void init_layout(installer* inst);

#endif
