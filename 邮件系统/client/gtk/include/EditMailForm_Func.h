#ifndef SIGNAL_H
#define SIGNAL_H
void SendMailBy(int iType,int iState);

void closeApp( GtkWidget *, gpointer);
void save_file();
void help_dialog();
void message_dialog(gchar* title,gchar* content);
//void send_func();
void send_func(GtkWidget *widget, gpointer* data);
void save_func();
void cut_func();
void copy_func();
void select_font(GtkWidget *, gpointer);
void select_color(GtkWidget *, gpointer);
//void file_ok_sel( GtkWidget *,GtkFileSelection * );
void linkman_connect_entry();



#endif
