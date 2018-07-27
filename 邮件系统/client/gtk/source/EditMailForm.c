
#include <gtk/gtk.h>
#include "../include/EditMailForm_Widget.h"
#include "../include/EditMailForm_Func.h"


/////*************************************************************************
/////	函数名称	：	int main ( int argc, char *argv[])
/////
/////	函数功能	：	创建顶级窗口并调用邮箱编辑主界面 的函数 edit_window()  
/////
/////   返 回 值	：	空
/////*************************************************************************
int edit_mainwindow(gpointer data)
{
	GtkWidget *window;
	GtkWidget *table;
	
	
	gtk_init(0,0);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "邮箱编辑界面");//////////////////标题名称
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);////窗口默认位置
	gtk_window_set_default_size(GTK_WINDOW(window), 800,550);///////////尺寸
	//gtk_container_set_border_width(GTK_CONTAINER(window),20);
	
	gtk_signal_connect( GTK_OBJECT (window), "destroy", 
		                   GTK_SIGNAL_FUNC (closeApp), NULL);

	table = edit_func(data);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
