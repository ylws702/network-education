#include<gtk/gtk.h>
//#include<menu.h>
//#include<tool.h>
//#include<paned.h>

int main_window()
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *toolbar;
	GtkWidget *hpaned;
	GtkWidget *statusbar;
	

//窗口创建
	gtk_init(0,0);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),800,600);
	gtk_window_set_title(GTK_WINDOW(window),"菊花邮件系统");
	g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//gtk_window_set_icon(GTK_WINDOW(window),create_pixbuf("web.png"));//设置窗口图标
//功能添加	
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);

	menubar=menu();
	gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,FALSE,0);
	toolbar=tool();
	gtk_box_pack_start(GTK_BOX(vbox),toolbar,FALSE,FALSE,0);

	hpaned=paned();
	gtk_box_pack_start(GTK_BOX(vbox),hpaned,TRUE,TRUE,0);
//状态栏
	statusbar = gtk_statusbar_new();
	gtk_box_pack_end(GTK_BOX(vbox), statusbar, FALSE, FALSE, 1);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

