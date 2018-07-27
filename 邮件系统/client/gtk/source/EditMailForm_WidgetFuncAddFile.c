#include <gtk/gtk.h>
#include "../include/EditMailForm_Func.h"
#include "../include/globlevalue.h"
#include "../include/WidgetStruct.h"

/////****************************************************************************
/////	函数名称	：	void file_ok_sel( GtkWidget *w,GtkFileSelection *fs )
/////	
/////	函数功能	：  获取到添加附件 选中的文件路径名称 （对已经选中的要添加的文件  点击“确定”后的操作）
/////   
/////   返 回 值	：	空
/////****************************************************************************

void file_ok_sel( GtkWidget *w,GtkFileSelection *fs )
//void file_ok_sel( GtkWidget *w,gchar *fs )
{
   	const gchar* file_name = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs));
    g_print ("%s\n", fs);
	//gtk_entry_set_text (GTK_ENTRY(addfile_entry),file_name);
	  gtk_entry_set_text(GTK_ENTRY(SendEntry.Filepath),file_name);

    
}

/////**********************************************************************
/////	函数名称	：	void add_file(GtkWidget *widget, gpointer label)
/////	
/////	函数功能	：  在 toolbar中创建  添加附件  
/////   
/////   返 回 值	：	空
/////**********************************************************************
void add_file(GtkWidget *widget, gpointer label)
{
	GtkWidget *filew;
    gtk_init (0, 0);
    
    /* 创建一个新的文件选择构件 */
    filew = gtk_file_selection_new ("添加文件");
    
    g_signal_connect (G_OBJECT (filew), "destroy",G_CALLBACK (gtk_main_quit), NULL);

	/* 为ok_button按钮设置回调函数，连接到file_ok_sel function函数 */
    g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),"clicked", 
                      G_CALLBACK (file_ok_sel), filew);
    
    /* 为cancel_button设置回调函数，销毁构件 */
    g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),"clicked",
							G_CALLBACK (gtk_widget_destroy), filew);
    
    /* 设置文件名，比如这个一个文件保存对话框，我们给了一个缺省文件名 */
    gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew),"123");
    
    gtk_widget_show (filew);
    gtk_main ();
    return ;
	
}

