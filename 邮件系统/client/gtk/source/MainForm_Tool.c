#include<gtk/gtk.h>
#include "../../function/include/mysocket.h"
#include "../../function/include/DataStruct.h"
#include "../../function/include/localfunc.h"
extern char g_userId[25];
int edit_mainwindow(gpointer data);
extern GtkWidget *clist;
extern char *value;
/*****************************************************
以下6个函数均为消息响应函数
功能：工具按钮响应
参数：GtkWidget * ，gpointer
日期：2012－9－8
作者：张建涛
*****************************************************/
//生成EditMailForm窗口
void new_email(GtkWidget *new, gpointer data)
{
	edit_mainwindow("new");
	g_print("new\n");
}

void send_email(GtkWidget *send, gpointer data)
{
/////////////////////////////////write here ///////////////////////////////////
	g_print("send\n");

    //printf("%s\n",pstNewMail[1].acEmailID);
    //GetMailText(atoi(pstNewMail[1].acEmailID),"zhanglongde");
    //printf("target record number:%d\n",i);
    
}
/*
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
*/
void receive_email(GtkWidget *receive, gpointer data)
{
/////////////////////////////////write here ///////////////////////////////////
	g_print("receive\n");
    stMail astNewMail[50];
    //g_print("g_userId:%s\n",g_userId);
    int iGetEmail=GetMail(astNewMail,g_userId,1,2);//g_userId作为sendTarget,应为send1类型
    printf("iGetEmail:%d\n",iGetEmail);
    if(iGetEmail<1)
    {
		message_dialog("提示","您目前没有未读的邮件");
        return ;
    }
    int i=0;
    for(i=0;i<iGetEmail;i++)
    {
		printf("astNewMail[i].acSendTargetID:%s\n",astNewMail[i].acSendTargetID);
        printf("astNewMail[i].acEmailID:%s\n",astNewMail[i].acEmailID);
        printf("astNewMail[i].acTitle:%s\n",astNewMail[i].acTitle);
        printf("astNewMail[i].acContent:%s\n",astNewMail[i].acContent);
        printf("astNewMail[i].acUserID:%s\n",astNewMail[i].acUserID);
        //printf("astNewMail[i].acUserID:%s\n",astNewMail[i].acEmailID);
        char filename[100]={0};
    sprintf(filename,"./data/%s/recv/%s|%s|%s|%s|.txt",astNewMail[i].acSendTargetID,astNewMail[i].acEmailID,astNewMail[i].acTitle,astNewMail[i].acUserID,astNewMail[i].acEmailTime); 
       printf("filename:%s\n",filename);
        int iGeFlag=GetMailText(atoi(astNewMail[i].acEmailID),filename);
        
        if(iGeFlag!=0)
        {
			message_dialog(astNewMail[i].acTitle,"接收邮件失败");
        }
        else
        {
			message_dialog(astNewMail[i].acTitle,"接收邮件成功");
            ChangeEmailState(astNewMail[i].acEmailID,1);
        }
    }
    //sprintf(out,"%s|%s|%s|%s|.txt",emailId,title,target,time);    
}

void respond_email(GtkWidget *respond, gpointer data)
{
/////////////////////////////////write here ///////////////////////////////////
	g_print("respond\n");
	edit_mainwindow("respond");
    //int SendEmail(stMail* pstNewMail,char * text);
}

void transmit_email(GtkWidget *transmit, gpointer data)
{
/////////////////////////////////write here ///////////////////////////////////
	g_print("transmit\n");
	edit_mainwindow("transmit");
}

void delete_email(GtkWidget *delete, gpointer data)
{
/////////////////////////////////write here ///////////////////////////////////
	//g_print("delete\n");
    extern FileName filename;
    extern char *value;
    //extern char g_userId[20];
    char path[100];
    char strhead[100];
    sprintf(path,"%s|%s|%s|%s|.txt",filename.emailId,filename.title,filename.target,filename.time);
    if(0 == strcmp(value,"收件箱"))
	{
        sprintf(strhead,"./data/%s/recv/",g_userId);
	}
	else if(0 == strcmp(value,"发件箱"))
	{
		sprintf(strhead,"./data/%s/send/",g_userId);
	}
	else if(0 == strcmp(value,"草稿"))
	{
	 	sprintf(strhead,"./data/%s/draft/",g_userId);
	}
    strcat(strhead,path);
    //g_print("%s\n",path);
    DeleteLocalMail (strhead);
    DeleteServerEmail(filename.emailId);   
	
    //刷新显示控件
    stMail email[100];
	char str[50]; 

	if(0 == strcmp(value,"收件箱"))
	{
		sprintf(str,"./data/%s/recv/",g_userId);
	}
	else if(0 == strcmp(value,"发件箱"))
    {
	   sprintf(str,"./data/%s/send/",g_userId);
	}
	else if(0 == strcmp(value,"草稿"))
    {
	   sprintf(str,"./data/%s/draft/",g_userId);
	}
	int num=GetLocalMail (email,str);
	show_receive(email,num,clist);

}

/*****************************************************
功能：创建工具栏并为工具栏添加工具项
返回值：GtkWidget* toolbar 工具栏构件
参数：void
日期：2012－9－8
作者：张建涛
*****************************************************/
GtkWidget* tool(void)
{
	GtkWidget *toolbar;
	GtkToolItem *new;
	GtkToolItem *send;
	GtkToolItem *receive;
	GtkToolItem *respond;
	GtkToolItem *transmit;
	GtkToolItem *delete;	
	GtkToolItem *sep1;
	GtkToolItem *sep2;
	GtkToolItem *sep3;
	

	toolbar=gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_TEXT);
	
	sep1=gtk_separator_tool_item_new(); 
	sep2=gtk_separator_tool_item_new();
	sep3=gtk_separator_tool_item_new();  
	
	//gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_HORIZONTAL);
	//gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));
	//gtk_toolbar_remove_space(GTK_TOOLBAR(toolbar),-1);

	send=gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label((GtkToolButton *)send,"发送");

	receive=gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label((GtkToolButton *)receive,"接收");

	new=gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label((GtkToolButton *)new,"新建");

	respond=gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label((GtkToolButton *)respond,"回复");

	transmit=gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label((GtkToolButton *)transmit,"转发");

	delete=gtk_tool_button_new(NULL,NULL);
	gtk_tool_button_set_label((GtkToolButton *)delete,"删除");

	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),new,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),sep1,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),send,-1);
	

	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),receive,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),sep2,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),respond,-1);
	
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),transmit,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),sep3,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),delete,-1);

	g_signal_connect(G_OBJECT(new),"clicked",G_CALLBACK(new_email),NULL);
	g_signal_connect(G_OBJECT(send),"clicked",G_CALLBACK(send_email),NULL);
	g_signal_connect(G_OBJECT(receive),"clicked",G_CALLBACK(receive_email),NULL);
	g_signal_connect(G_OBJECT(respond),"clicked",G_CALLBACK(respond_email),NULL);
	g_signal_connect(G_OBJECT(transmit),"clicked",G_CALLBACK(transmit_email),NULL);
	g_signal_connect(G_OBJECT(delete),"clicked",G_CALLBACK(delete_email),NULL);

	return toolbar;
}

