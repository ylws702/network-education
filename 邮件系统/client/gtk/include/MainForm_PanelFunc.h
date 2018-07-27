//所有在MainForm_PanelFunc中定义的函数的声明

//#include"struct1.h"
void selection_made( GtkWidget *clist,gint row,gint column,GdkEventButton *event,gpointer data );

void show_receive( gpointer data );

void show_send( gpointer data );

void show_draft( gpointer data );

void on_changed(GtkWidget *widget, gpointer data) ;

void add_button_clicked(GtkWidget *widget, gpointer data) ;


int add_linkman(GtkWidget *widget,gpointer data);

void search_email(GtkWidget *search_button, gpointer data);
int update_linkman(GtkWidget *widget,gpointer data);

void selection_linkman(GtkWidget *clist1,gint row,gint column,GdkEventButton *event, gchar* data);

int delete_linkman();
