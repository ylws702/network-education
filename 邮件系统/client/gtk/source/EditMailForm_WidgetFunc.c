#include <gtk/gtk.h>
#include "../include/EditMailForm_Func.h"
#include "../include/EditMailForm_WidgetFuncAddFile.h"
#include"../include/globlevalue.h"
#include"../include/WidgetStruct.h"

/////**********************************************************************
/////	函数名称	：	GtkWidget* menu(void)
/////	
/////	函数功能	：  在 table中创建menubar  菜单栏
/////   
/////   返 回 值	：	menubar
/////**********************************************************************

GtkWidget* menu1(void)
{
	GtkWidget *menubar;
	GtkWidget *filemenu;
	GtkWidget *editmenu;
	GtkWidget *helpmenu;
	GtkWidget *file;
	GtkWidget *help;
	GtkWidget *helps;
	GtkWidget *new;
	GtkWidget *save;
	GtkWidget *quit;

	/********创建 menubar 菜单栏*****/
	menubar=gtk_menu_bar_new();
	/********创建 menubar 菜单栏下的   子菜单栏    选项*****/
	filemenu=gtk_menu_new();
	//editmenu=gtk_menu_new();
	helpmenu=gtk_menu_new();

	file=gtk_menu_item_new_with_label("文件");
	save=gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);	/////带有图标和快捷键
	quit=gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);

	help =gtk_menu_item_new_with_label("帮助");
	helps=gtk_menu_item_new_with_label("help");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file),filemenu);
	//gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit),editmenu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help),helpmenu);
	/* ＊＊        文件下的选项            ＊＊ */
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu),save);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu),quit);

	gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu),helps);
	
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),file);
	//gtk_menu_shell_append(GTK_MENU_SHELL(menubar),edit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),help);
//////////////////////////////////////////////////////////////////////////////////////菜 单 栏 中 的消息循环
	g_signal_connect(G_OBJECT(save), "activate", G_CALLBACK(save_file),NULL);
	g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(closeApp),NULL);
	g_signal_connect(G_OBJECT(helps), "activate", G_CALLBACK(help_dialog),NULL);	

	return menubar;
}

/////**********************************************************************
/////	函数名称	：	GtkWidget* tool(void)
/////	
/////	函数功能	：  在 table中创建toolbar  工具栏
/////   
/////   返 回 值	：	toolbar
/////**********************************************************************
GtkWidget* tool1(void)
{
	GtkWidget *toolbar;
	GtkToolItem *send;

	GtkToolItem *save;
	GtkToolItem *cut;
	GtkToolItem *copy;
	GtkToolItem *addfile;//////添加附件
	GtkToolItem *sep1,*sep2;////竖直分割线
	//GtkToolItem *font,*font_color;
	
	
	/********创建 toobar 工具栏*****/
	toolbar=gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);///////使得产生的按钮都是图表模式 不是文字模式

	/********创建 toobar 工具栏  中 的   发  送  选  项*****/
	send = gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(send),"发送");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),send,-1);

	/********创建 toobar 工具栏  中 的   save 选  项*****/
	save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),save,-1);

	/********创建 toobar 工具栏  中 的   分割线  1 选  项*****/
	sep1 = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep1, -1);

	/********创建 toobar 工具栏  中 的   Cut 选  项*****/
	cut = gtk_tool_button_new_from_stock(GTK_STOCK_CUT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),cut,-1);

	/********创建 toobar 工具栏  中 的   Copy 选  项*****/
	copy = gtk_tool_button_new_from_stock(GTK_STOCK_COPY);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),copy,-1);

	/********创建 toobar 工具栏  中 的   分割线  2 选  项*****/
	sep2 = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep2, -1);

	/********创建 toobar 工具栏  中 的   字体 选  项*****/
	font = gtk_tool_button_new_from_stock(GTK_STOCK_SELECT_FONT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), font, -1);

	/********创建 toobar 工具栏  中 的   字体颜色 选  项*****/
	font_color = gtk_tool_button_new_from_stock(GTK_STOCK_SELECT_COLOR);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), font_color, -1);

	/********创建 toobar 工具栏  中 的   添加附加  选  项*****/
	addfile = gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(addfile),"附件");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), addfile, -1);
	

   
/////////************************************************************************** toolbar中的发送 、保存 、addfile   的消息循环
	g_signal_connect(G_OBJECT(send), "clicked", G_CALLBACK(send_func),NULL);//////send
	g_signal_connect(G_OBJECT(save), "clicked", G_CALLBACK(save_func),NULL);//////save
	g_signal_connect(G_OBJECT(addfile), "clicked", G_CALLBACK(add_file),NULL);////////cut
	g_signal_connect(G_OBJECT(cut), "clicked", G_CALLBACK(cut_func),NULL);////////cut
	g_signal_connect(G_OBJECT(copy), "clicked", G_CALLBACK(copy_func),NULL);//////copy

////************************备注	:字体、颜色的消息循环 放在了build_scrolled_win()函数中， 添加附件的消息循环放在了 add_file.c文件下的add_file()函数中

	return toolbar;
}

