#include<gtk/gtk.h>
#include"../include/globlevalue.h"
#include"../include/DataStruct.h"
#include"../include/WidgetStruct.h"
#include "../../function/include/mysocket.h"
#include "../../function/include/localfunc.h"
#include <time.h>

/////**********************************************************************
/////	函数名称	：	void closeApp( GtkWidget *window, gpointer data)
/////	
/////	函数功能	：	关 闭 窗 口    
/////									
/////   返 回 值	：	空
/////**********************************************************************
void closeApp( GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

/////**********************************************************************
/////	函数名称	：	void help_dialog()
/////	
/////	函数功能	：	点击帮助 选项后 弹跳出一个help 的  dialog对话框    
/////									
/////   返 回 值	：	空
/////**********************************************************************
void help_dialog()
{
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "本产品最终解释权属于team5小组.");
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "信息");
	gtk_widget_show(dialog);

	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);

}
/////**********************************************************************
/////	函数名称	：	void help_dialog()
/////	
/////	函数功能	：	点击帮助 选项后 弹跳出一个message 的  dialog对话框    
/////									
/////   返 回 值	：	空
/////**********************************************************************
void message_dialog(gchar* title,gchar* content)
{
	GtkWidget *dialog = gtk_about_dialog_new();
	  
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), content);
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), title);
	gtk_widget_show(dialog);

	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

/////**********************************************************************
/////	函数名称	：	void save_file()
/////	
/////	函数功能	：	文件菜单中的保存功能的实现    
/////									
/////   返 回 值	：	空
/////**********************************************************************
void save_file()
{
	GtkTextIter start,end;
	
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer),&start,&end);
	const GtkTextIter s = start,e = end;
	text_info = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(Send_buffer),&s,&e,FALSE);    
	g_print("%s\n",text_info);
     int se=SendMailBy(4,2);
    if(se==0)
    {
        message_dialog("信息","保存成功");
    }else if(se==-5)
    {
		message_dialog("信息","保存附件失败");
    }
    else
    {
		message_dialog("信息","保存邮件失败");
    } 
}

/////**********************************************************************
/////	函数名称	：	void save_func()
/////	
/////	函数功能	：	  点击toolbar中的 保存选项  实现保存功能
/////									
/////   返 回 值	：	空
/////**********************************************************************
void save_func()
{
	GtkTextIter start,end;

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer),&start,&end);
	const GtkTextIter s = start,e = end;
	text_info = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(Send_buffer),&s,&e,FALSE);

   	g_print("%s\n",text_info);
    int se=SendMailBy(4,2);
    if(se==0)
    {
        message_dialog("信息","保存成功");
    }else if(se==-5)
    {
		message_dialog("信息","保存附件失败");
    }
    else
    {
		message_dialog("信息","保存邮件失败");
    }   
}

/////**********************************************************************
/////	函数名称	：	void send_func()
/////	
/////	函数功能	：	  点击toolbar中的 发送选项  实现发送功能
/////									
/////   返 回 值	：	空
/////**********************************************************************
void send_func(GtkWidget *widget, gpointer* data2)
{
    //MailWidget * data=(MailWidget * )data2;
    int se=SendMailBy(1,2);//发送给服务器，并保存到发件箱
    if(se==0)
    {
        message_dialog("信息","发送成功");
    }else if(se==-5)
    {
		message_dialog("信息","发送附件失败");
    }
    else
    {
		message_dialog("信息","发送邮件失败");
    }
}

void save_emailtext()
{
	GtkTextIter start,end;
	
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer),&start,&end);
	const GtkTextIter s = start,e = end;
	text_info = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(Send_buffer),&s,&e,FALSE);
     
	g_print("%s\n",text_info);
    
}

//
void SendMailBy(int iType,int iState)
{
	const char* a =gtk_entry_get_text (GTK_ENTRY((GtkWidget*)SendEntry.Address));///////获取发件人信息到
	g_print("%s\n",a);
	const char* b =gtk_entry_get_text (GTK_ENTRY((GtkWidget*)SendEntry.CopyAddress));///////获取收件人信息到
	g_print("%s\n",b);
	const char* c =gtk_entry_get_text (GTK_ENTRY((GtkWidget*)SendEntry.SecretAddress));///////获取收件人信息到
	g_print("%s\n",c);
	const char* d =gtk_entry_get_text (GTK_ENTRY((GtkWidget*)SendEntry.Theme));///////获取收件人信息到
	g_print("%s\n",d);
    char* e="NULL";
	e =gtk_entry_get_text (GTK_ENTRY((GtkWidget*)SendEntry.Filepath));///////获取收件人信息到
	g_print("%s\n",e);
    stMail email;
    strcpy(email.acSendTargetID,a);//发送对象
    strcpy(email.acCopyTargetID,b);//密送对象
    //strcpy(email.acCopyTargetID,c);
    strcpy(email.acTitle,d);
    if(strcmp(e,"NULL"))
    {
		strcpy(email.acAttachedFilePath,"NULL");//没有附件
        g_print("email->acEmailID:%s\n");
    }
    else
    {
         strcpy(email.acAttachedFilePath,e);//有附件
    }
    g_print("email->acEmailID:%s\n",email.acSendTargetID);
////在此
    save_emailtext();
    strcpy(email.acContent,"../include/1");  
    extern char g_userId[25];
    strcpy(email.acUserID,g_userId);

    struct tm * ptr;
    time_t lt;
    lt=time(NULL);
    ptr=localtime(&lt);
    char acTime[30]={0};
    sprintf(acTime,"%d-%d-%d-%d-%d-%d",ptr->tm_year+1900,ptr->tm_mon+1,ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec);
    strcpy(email.acEmailTime,acTime);
    email.iEmailSize=1000;
    email.iType=iType;
    email.iState=iState;
    int se=SendEmail(&email,text_info);//向服务器发送邮件，同时保存到本地
    return se;
    /*printf("se:%d\n",se);
    if(se==0)
    {
        message_dialog("信息","发送成功");
    }else if(se==-5)
    {
		message_dialog("信息","发送附件失败");
    }
    else
    {
		message_dialog("信息","发送邮件失败");
    } */
}



/////**********************************************************************
/////	函数名称	：	void cut_func()
/////	
/////	函数功能	：	  点击toolbar中的 剪切选项  实现剪切功能
/////									
/////   返 回 值	：	空
/////**********************************************************************
void cut_func()
{
	g_print("cut_func");
}

/////**********************************************************************
/////	函数名称	：	void copy_func()
/////	
/////	函数功能	：	  点击toolbar中的 复制选项  实现复制功能
/////									
/////   返 回 值	：	空
/////**********************************************************************
void copy_func()
{
	g_print("copy_func");
}

/////**********************************************************************
/////	函数名称	：	 void select_font(GtkWidget *widget, gpointer label)
/////	
/////	函数功能	：  在 toolbar中创建  字体选择 选项 
/////   
/////   返 回 值	：	空
/////**********************************************************************
 void select_font(GtkWidget *widget, gpointer label)
     {
     GtkResponseType result;
     GtkWidget *dialog = gtk_font_selection_dialog_new("Select Font");//字体选择对话框的创建 Select Font是对话框标题
     result = gtk_dialog_run(GTK_DIALOG(dialog));//运行对话框
     if (result == GTK_RESPONSE_OK || result == GTK_RESPONSE_APPLY)//判断对话框是否打开或运用
      {
     PangoFontDescription *font_desc;
     gchar *fontname = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(dialog));//fontname为所选字体 样式 大小的字符串集
     font_desc = pango_font_description_from_string(fontname);
     gtk_widget_modify_font(GTK_WIDGET(label), font_desc);
    
     g_free(fontname);
     }
     gtk_widget_destroy(dialog);
}


/////**********************************************************************
/////	函数名称	：	void select_color(GtkWidget *widget, gpointer m_color)
/////	
/////	函数功能	：  在 toolbar中创建  颜色选择  选项
/////   
/////   返 回 值	：	空
/////**********************************************************************
void select_color(GtkWidget *widget, gpointer m_color)
{
     GtkResponseType result;
     GtkColorSelection *colorsel;
     GdkColor color;
     GtkWidget *dialog = gtk_color_selection_dialog_new("Font Color");//创建副对话框标题为Font Color
     result = gtk_dialog_run(GTK_DIALOG(dialog));
     if (result == GTK_RESPONSE_OK)//若对话框响应事件 
     {
     colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(dialog)->colorsel);

     gtk_color_selection_get_current_color(colorsel, &color);//获取当前颜色
     gtk_widget_modify_text(GTK_WIDGET(m_color),GTK_STATE_NORMAL,&color);//改变标签的字体颜色
     }
     gtk_widget_destroy(dialog);
}



/////**********************************************************************
/////	函数名称	：	 void linkman_connect_entry()
/////	
/////	函数功能	：  点击联系人 将联系人的邮箱地址出现在 收件人、  
/////									抄送人	密送人 的entry 中
/////   返 回 值	：	空
/////**********************************************************************
void linkman_connect_entry()
{

g_print("lingman");

}






