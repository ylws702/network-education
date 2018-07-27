#include <gtk/gtk.h>
#include <string.h>
#include"../include/MainForm.h"
#include"../include/WidgetStruct.h"
#include"../include/DataStruct.h"
///////********************************
///////close the window
///////********************************
void closeApp3( GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}
////////*********************************
////////    login failed：弹出一个对话框 
////////*********************************

void login_failed()
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,
									GTK_BUTTONS_OK,
									"登  录  失  败");
	gtk_window_set_title(GTK_WINDOW(dialog), "错　误");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

}

///////*************************************************************************************
///////响应主窗口中的   login 按钮 ： 登录成功：调用mail_window()函数  , 登录失败：调用 login_failed()函数
///////*************************************************************************************
void afterlogin_window (GtkWidget *button, UserWidget* data)
{
	stUser LogInfo;
	int checkResult;
	int sendResult;
	int recvResult;
	extern char g_userId[25];
	const char *a = gtk_entry_get_text(GTK_ENTRY((GtkWidget *) data->UserName));
	const char *b = gtk_entry_get_text(GTK_ENTRY((GtkWidget *) data->Passwd));
	strcpy(LogInfo.acUserID,a);
	strcpy(LogInfo.acPasswd,b);
	//检查密码输入和帐户输入是否符合规则，注意密码长度6与16之间含6，16
	checkResult = CheckLoginInput(LogInfo.acUserID, LogInfo.acPasswd);
	if (checkResult == 1)
	{
		g_print("用户名含有非法字符，请重新输入\n");
		login_failed();
	}
	else if (checkResult == 2)
	{
		g_print("密码长度不妥，请重新输入\n");
		login_failed();		
	}
	else
	{
		sendResult = SendLoginInfoToServer(LogInfo.acUserID, LogInfo.acPasswd);
		if (sendResult == -1)
		{	
			g_print("调用socket失败!\n");	
			login_failed();				
		}
		else
		{
			recvResult = RecvLoginResFromServer();
			g_print("denglujieguo%d", recvResult);
			if (recvResult == 1)
			{
				g_print("用户名或者密码错误，请确认后输入!\n");
				login_failed();
			}
			else if (recvResult == 2)
			{
				g_print("您已经登录!\n");
				login_failed();;

			}
			else
			{
				g_print("登录成功!\n");
			
				CreateUserFolder(LogInfo.acUserID);//创建文件夹
				strcpy(g_userId, LogInfo.acUserID);//全部变量
				gtk_widget_destroy((GtkWidget *)data->window);
				main_window(); 
			
			}
		}
	}
	

}
