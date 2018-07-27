#include<gtk/gtk.h>
/*****************************************************
功能：创建菜单栏并为菜单栏添加菜单项
返回值：GtkWidget* menubar 菜单栏构件
参数：void
日期：2012－9－8
作者：张建涛
*****************************************************/
GtkWidget* menu(void)
{
	GtkWidget* menubar;
	GtkWidget* filemenu;
	GtkWidget* editmenu;
	GtkWidget *file;
	GtkWidget *edit;
	GtkWidget *quit;
	GtkWidget *helpmenu;
	GtkWidget *help;
	GtkWidget *about;

	menubar=gtk_menu_bar_new();
	filemenu=gtk_menu_new();
	editmenu=gtk_menu_new();
	helpmenu=gtk_menu_new();

	file=gtk_menu_item_new_with_label("文件");
	edit=gtk_menu_item_new_with_label("编辑");	
	quit=gtk_menu_item_new_with_label("退出");
	help=gtk_menu_item_new_with_label("帮助");
	about=gtk_menu_item_new_with_label("关于");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file),filemenu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit),editmenu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help),helpmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu),quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu),about);

	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),file);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),edit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),help);

	g_signal_connect(G_OBJECT(quit),"activate",G_CALLBACK(gtk_main_quit),NULL);	

	return menubar;
}
