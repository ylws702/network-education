#include<gtk/gtk.h>
#include "../include/EditMailForm_WidgetFunc.h"
#include "../include/EditMailForm_WidgetTree.h"
#include "../include/EditMailForm_Func.h"
#include "../include/globlevalue.h"
//#include "../include/DataStruct.h"
#include "../include/WidgetStruct.h"
#include "../../function/include/DataStruct.h"
extern FileName filename;
extern char maileTextbuffer[1024];
/////**********************************************************************
/////	函数名称	：	GtkWidget *build_scrolled_win()
/////	
/////	函数功能	：	创  建  scrolled滚动窗口及文本框    
/////									
/////   返 回 值	：	Send_scrolled_win
/////**********************************************************************
GtkWidget *build_scrolled_win()
{
	//GtkWidget *text;
	GtkWidget *scrolled;

	Send_scrolled_win = gtk_scrolled_window_new(NULL,NULL);
	Send_textview = gtk_text_view_new();                                          
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(Send_textview),GTK_WRAP_WORD);	  
	gtk_text_view_set_justification(GTK_TEXT_VIEW(Send_textview),GTK_JUSTIFY_LEFT);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(Send_textview),TRUE);					
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Send_textview),TRUE);			
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(Send_textview),5);			
	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(Send_textview),5);			
	gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(Send_textview),5);			
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(Send_textview),10);					
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(Send_textview),10);				
	Send_buffer =  gtk_text_view_get_buffer(GTK_TEXT_VIEW(Send_textview));	

	//////////**********************************************************************************************字体、颜色 的消息循环
	g_signal_connect(G_OBJECT(font), "clicked", G_CALLBACK(select_font),(gpointer)Send_textview);
	g_signal_connect(G_OBJECT(font_color), "clicked", G_CALLBACK(select_color),(gpointer)Send_textview);//将color对象与clicked相关联，回调函数为select_font
	gtk_container_add(GTK_CONTAINER(Send_scrolled_win),Send_textview);

	return Send_scrolled_win;
}



/////******************************************＊＊*****************************＊＊
/////	函数名称	：	GtkWidget *edit_window (void)
/////	
/////	函数功能	：	创 建  邮 件 编 辑 界 面 的 表 （30，30）
/////
/////   返 回 值	：	table
/////******************************************＊＊*****************************＊＊
GtkWidget *edit_func (gpointer data)
{
	GtkWidget *table;
	GtkWidget *text_view;
	//GtkWidget *vscrollbar,*hscrollbar;
	GtkWidget *menubar;
	GtkWidget *toolbar;
	GtkWidget *recevie_label;//////收件人标签
	GtkWidget *theme_label;///////////主题标签
	GtkWidget *chaos_label;/////////抄送label
	GtkWidget *mis_label;/////////密送label
	GtkWidget *addfile_label;/////////添加附件label
	GtkWidget *recevie_entry;//////收件人输入框
	GtkWidget *theme_entry;///////////主题输入框
	GtkWidget *chaos_entry;//////////抄送entry
	GtkWidget *mis_entry;/////////密送entry
	GtkWidget *addfile_entry;
	GtkWidget *statusbar;
	GtkWidget *tree;
	
	/*创建一个table构件*/
	table = gtk_table_new (40, 40, FALSE);

	/*	调用函数		menu()		创建一个菜单栏构件，并在表内分配空间*/
	menubar = menu1();
	gtk_table_attach (GTK_TABLE (table), menubar, 0, 40, 0, 1,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	//stMail stEmailInfo;
   // stEmailInfo->
	/*	调用函数		tool()		创建一个菜单栏构件，并在表内分配空间*/
	toolbar = tool1();
	gtk_table_attach (GTK_TABLE (table), toolbar, 0, 40,1, 2,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);

	////*************创建	收件人  的label 及entry ***************///////
	recevie_label = gtk_label_new("收件人（T）:");/////用户名标签
	gtk_table_attach (GTK_TABLE (table), recevie_label, 0, 2, 2, 3,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	//recevie_entry = gtk_entry_new();
	SendEntry.Address=gtk_entry_new();
	gtk_table_attach (GTK_TABLE (table), SendEntry.Address, 2, 39, 2, 3,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);

	////*************创建	抄 送  的label 及entry ***************///////
	chaos_label = gtk_label_new("抄　送 (C) :");/////抄送标签
	gtk_table_attach (GTK_TABLE (table), chaos_label, 0, 2, 3, 4,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	
	//chaos_entry = gtk_entry_new();
	SendEntry.CopyAddress=gtk_entry_new();	
	gtk_table_attach (GTK_TABLE (table), SendEntry.CopyAddress, 2, 39, 3, 4,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);


	////*************创建	密 送  的label 及entry ***************///////
	mis_label = gtk_label_new("密　送 (M) :");/////密送标签
	gtk_table_attach (GTK_TABLE (table), mis_label, 0, 2, 4, 5,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	//mis_entry = gtk_entry_new();
	SendEntry.SecretAddress=gtk_entry_new();
	gtk_table_attach (GTK_TABLE (table), SendEntry.SecretAddress, 2, 39, 4, 5,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);	

	////*************创建	主  题  的label 及entry ***************///////
	theme_label = gtk_label_new("主　题（U）:");//////////主题标签
	gtk_table_attach (GTK_TABLE (table), theme_label, 0, 2, 5, 6,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	//theme_entry = gtk_entry_new();
	SendEntry.Theme=gtk_entry_new();
	gtk_table_attach (GTK_TABLE (table), SendEntry.Theme, 2, 39, 5, 6,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);

	////*************创建	附 件  的label 及entry ***************///////
	addfile_label = gtk_label_new("附　件（F）:");//////////
	gtk_table_attach (GTK_TABLE (table), addfile_label, 0, 2, 6, 7,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	//addfile_entry = gtk_entry_new();
	SendEntry.Filepath=gtk_entry_new();
	gtk_table_attach (GTK_TABLE (table), SendEntry.Filepath, 2, 39, 6, 7,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);

	/*	调用函数	build_scrolled_win()	创建一个  scrolled滚动窗口及文本框  构件，并在表内分配空间*/
	text_view = build_scrolled_win();
	gtk_table_attach (GTK_TABLE (table), text_view, 0, 38, 7, 39,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);

	/*	调用函数	create_view_and_model()	创建一个树视图，并在表内分配空间*/
	tree = create_view_and_model1();
	gtk_table_attach (GTK_TABLE (table), tree, 38, 40, 7, 39,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);

	/*创建一个状态栏构件，并在表内分配空间*/
	statusbar = gtk_statusbar_new();
	gtk_table_attach (GTK_TABLE (table), statusbar, 0, 40, 39, 40,
	GTK_FILL | GTK_EXPAND,
	GTK_FILL | GTK_EXPAND | GTK_SHRINK, 0, 0);
	if(0==strcmp("respond",data))
	{
		gtk_entry_set_text(GTK_ENTRY(SendEntry.Address),filename.target);
		//gtk_entry_set_text(GTK_ENTRY(SendEntry.Theme),filename.title);
		
	}
	
	if(0==strcmp("transmit",data))
	{	
       
		  Send_buffer =  gtk_text_view_get_buffer(GTK_TEXT_VIEW(Send_textview));
 g_print("adadhufgbkjk%s\n",Send_buffer);
		  gtk_text_buffer_set_text(Send_buffer,Send_buffer,-1);
		gtk_entry_set_text(GTK_ENTRY(SendEntry.Theme),filename.title);
	}
	return table;
}

