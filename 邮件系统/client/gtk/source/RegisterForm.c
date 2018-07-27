#include <gtk/gtk.h>
#include "../../function/include/DataStruct.h"
#include "../include/WidgetStruct.h"
#include "../../function/include/RegisterModule.h"
#include<string.h>


	GtkWidget *window1;////////////////////////////注册界面窗口
//////****************************************************
//// 注册里面      提交   按钮的接口函数 
//////****************************************************

void resultsucc_dialog()
{
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "注册成功！");
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "注册提示");
	gtk_widget_show(dialog);
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

void faile_dialog()
{
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "注册失败，请重新注册！");
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "注册提示");
	gtk_widget_show(dialog);
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}
void refer_button_clicked (GtkWidget *button, UserWidget* data)
{

	stUser userInfo;
	int checkResult;
	int sendResult;
	int recvResult;
	const gchar* a =	gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->UserName));
	const gchar* b =	gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->NickName));
	const gchar* c =	gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->Passwd));
	const gchar* d =	gtk_entry_get_text(GTK_ENTRY((GtkWidget *)data->Telephone));

	strcpy(userInfo.acUserID,a);
	strcpy(userInfo.acNickName, b);
	strcpy(userInfo.acPasswd,c);
	strcpy(userInfo.acTelephone,d);
	userInfo.iState = 0;
	checkResult = CheckRegisterInput(&userInfo);
	g_print("输入错误错误码：%d！", checkResult);
	if (checkResult == 0)
	{
		sendResult = SendUserInfoToServer(&userInfo);
		if (sendResult == 0)
		{
			g_print("发送成功！");
			recvResult = RecvRegisterResFromServer();
			if (recvResult == 0)
			{			
				resultsucc_dialog();
				gtk_widget_destroy(window1);
				g_print("注册成功");
			}
			else
			{
				faile_dialog();
				g_print("注册失败！");
			}
		}
		else
		{
			g_print("发送失败！");
		}
	}
	else
	{
		faile_dialog();
		g_print("输入有误！");
	}
 //g_print("%s\n",g_RigisterInfo.PasswdConfirm);
}

//////******************************************************
//// 注册里面     重置     按钮的接口函数 
//////******************************************************
void reset_button_clicked (GtkWidget *button, UserWidget* data)
{
 	gtk_entry_set_text(GTK_ENTRY((GtkWidget *)data->UserName),"");
	gtk_entry_set_text(GTK_ENTRY((GtkWidget *)data->NickName),"");
	gtk_entry_set_text(GTK_ENTRY((GtkWidget *)data->Passwd),"");
	gtk_entry_set_text(GTK_ENTRY((GtkWidget *)data->Telephone),"");
}


///////********************************
///////close the window
///////********************************
void closeApp2( GtkWidget *window, gpointer data)
{
//显示登录界面
	gtk_widget_show(GTK_WIDGET(data));
  gtk_main_quit();
}
//////////*************************************************
/* 回调函数*////////
///////////************************************************
void register_window(GtkWidget *widget,gpointer data)
{	
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *separator2;/////////////////////////frame与重置提交之间的分割线

	GtkWidget *mailaddr_label, *passwd_label;//////邮箱地址标签 、密码标签
 	GtkWidget *pass_label, *tel_label;/////////////确认密码标签 、手机号码标签
	GtkWidget *add1_label, *add2_label;////////////邮箱地址限制条件标签、 密码限制条件标签
	GtkWidget *add3_label, *add4_label;////////////再次输入密码标签 、 短信找回密码标签
	GtkWidget *mailaddr_entry, *passwd_entry;//////邮箱地址输入框 、 密码输入框
	GtkWidget *pass_entry,*tel_entry;//////////////再次输入密码框 、 手机号码输入框
	GtkWidget *hbox1, *hbox2,*hbox3;///////////////hbox1(第一行)、hbox2(第二行)、hbox3(第三行)、
	GtkWidget *hbox4,*hbox5;///////////////////////hbox4(第四行)、  hbox5(整合注册、提交为一行)、
	GtkWidget *vbox1,*vbox2,*vbox3;/////////////////vbox1(整合hbox1、2、3、4)、   box2(整合hbox5)、
	GtkWidget *refer_button,*reset_button;/////////提交按钮       重置按钮
	GtkWidget *frame_label;////////////////////////菊花邮箱  的 框架
//隐藏登录窗口
gtk_widget_hide(GTK_WIDGET(data));
	UserWidget ris;	

	  gtk_init(0,0);

	  window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);///////////////////////创建整个注册窗口
	  gtk_window_set_title(GTK_WINDOW(window1), "用户注册");////////////////标题名称
	  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);////窗口默认位置为中央
	  gtk_window_set_default_size(GTK_WINDOW(window1), 400, 500);////尺寸
	  gtk_window_set_policy(GTK_WINDOW(window1),FALSE,FALSE,TRUE);/////////杀死最大化
	  gtk_container_set_border_width(GTK_CONTAINER(window1),50);///////////设置窗口边界
	
	  gtk_signal_connect( GTK_OBJECT(window1), "destroy", ////////////////destroy  调用closeApp
                       GTK_SIGNAL_FUNC(closeApp2), data);

	  hbox1 = gtk_hbox_new ( FALSE, 1 );/////创建组装盒
	  hbox2 = gtk_hbox_new ( FALSE, 1 );/////
	  hbox3 = gtk_hbox_new ( FALSE, 1 );/////创建组装盒
	  hbox4 = gtk_hbox_new ( FALSE, 1 );/////
	  hbox5 = gtk_hbox_new ( FALSE, 20);/////创建组装盒
	  vbox1 = gtk_vbox_new ( FALSE, 5);///////
	  vbox2 = gtk_vbox_new ( FALSE, 5);//////创建组装盒
	  vbox3 = gtk_vbox_new ( FALSE, 5);//////创建组装盒

	separator2 = gtk_hseparator_new();/////创建一条分割线
	  
	  frame_label = gtk_frame_new("欢迎使用菊花邮箱");/////////////////////////框架的创建
	  gtk_container_set_border_width(GTK_CONTAINER(frame_label),20);////////设置框架边界	

	  mailaddr_label = gtk_label_new("＊邮箱地址:");/////创建用户名标签
	  passwd_label = gtk_label_new("＊昵　　称:");///////创建密码输入标签
	  pass_label = gtk_label_new("＊密　　码:");/////////创建
	  tel_label = gtk_label_new("＊手机号码:");//////////创建

	  add1_label = gtk_label_new("6~16个字符，包括字母、数字、下划线。以字母开头。");
 	  add2_label = gtk_label_new("1~50个字符，包括字母、数字、特殊符号。区分大小写。");
	  add3_label = gtk_label_new("6~16个字符，包括字母、数字、特殊符号。区分大小写。");
	  add4_label = gtk_label_new("密码遗忘或被盗时，可通过手机短信取回密码。");

	  refer_button = gtk_button_new_with_label("提    交");////////////.//“ 提  交 “ 按钮
	  reset_button = gtk_button_new_with_label("重    置");///////////////“ 重  置 “ 按钮
		
		gtk_signal_connect (GTK_OBJECT (refer_button), "clicked",/////////提交 按钮的接口函数
		                  G_CALLBACK(refer_button_clicked),&ris);
		gtk_signal_connect (GTK_OBJECT (reset_button), "clicked", ////////重置按钮的接口函数
		                  GTK_SIGNAL_FUNC(reset_button_clicked),&ris);

	ris.UserName = gtk_entry_new();
	ris.NickName = gtk_entry_new();
	ris.Passwd = gtk_entry_new();
	ris.Telephone = gtk_entry_new();

	gtk_entry_set_visibility(GTK_ENTRY(ris.Passwd), FALSE);;
	//mailaddr_entry = gtk_entry_new();///// 创建
	// passwd_entry = gtk_entry_new();/////// 各个
	//pass_entry = gtk_entry_new();///////// 输入框
	tel_entry = gtk_entry_new();//////////
	//gtk_entry_set_visibility(GTK_ENTRY (passwd_entry), FALSE);
	// gtk_entry_set_visibility(GTK_ENTRY (pass_entry), FALSE);	



	  gtk_box_pack_start(GTK_BOX(hbox1), mailaddr_label, FALSE, FALSE, 5);/////  以下是 打包 处理
	 // gtk_box_pack_start(GTK_BOX(hbox1), mailaddr_entry, FALSE, FALSE, 5);/////
 gtk_box_pack_start(GTK_BOX(hbox1), ris.UserName, FALSE, FALSE, 5);/////
	  gtk_box_pack_start(GTK_BOX(hbox1), add1_label, FALSE, FALSE,5);//////////

	  gtk_box_pack_start(GTK_BOX(hbox2), passwd_label, FALSE, FALSE, 5);///////
	  //gtk_box_pack_start(GTK_BOX(hbox2), passwd_entry, FALSE, FALSE, 5);//////
gtk_box_pack_start(GTK_BOX(hbox2), ris.NickName, FALSE, FALSE, 5);///////
	  gtk_box_pack_start(GTK_BOX(hbox2), add2_label, FALSE, FALSE, 5);/////////
	
	  gtk_box_pack_start(GTK_BOX(hbox3), pass_label, FALSE, FALSE, 5);/////////
	  //gtk_box_pack_start(GTK_BOX(hbox3), pass_entry, FALSE, FALSE, 5);/////////
gtk_box_pack_start(GTK_BOX(hbox3), ris.Passwd, FALSE, FALSE, 5);///////
	  gtk_box_pack_start(GTK_BOX(hbox3), add3_label, FALSE, FALSE, 5);/////////
	
	  gtk_box_pack_start(GTK_BOX(hbox4), tel_label, FALSE, FALSE, 5);//////////
	  gtk_box_pack_start(GTK_BOX(hbox4), ris.Telephone, FALSE, FALSE, 5);//////////
	  gtk_box_pack_start(GTK_BOX(hbox4), add4_label, FALSE, FALSE,5);//////////

	  gtk_box_pack_start(GTK_BOX(hbox5), refer_button, TRUE, FALSE, 5);////////
	  gtk_box_pack_start(GTK_BOX(hbox5), reset_button, TRUE, FALSE,5);/////////

	  gtk_box_pack_start(GTK_BOX(vbox1), hbox1, FALSE, FALSE, 10);/////////////
	  gtk_box_pack_start(GTK_BOX(vbox1), hbox2, FALSE, FALSE, 10);/////////////
	  gtk_box_pack_start(GTK_BOX(vbox1), hbox3, FALSE, FALSE, 10);/////////////
	  gtk_box_pack_start(GTK_BOX(vbox1), hbox4, FALSE, FALSE, 10);/////////////
	  gtk_box_pack_start(GTK_BOX(vbox2), hbox5, FALSE, FALSE, 10);/////////////
	 

	gtk_container_add(GTK_CONTAINER(frame_label),vbox1);
	gtk_container_add(GTK_CONTAINER(vbox3),frame_label);
	
	
	gtk_box_pack_start (GTK_BOX (vbox3), separator2, FALSE, TRUE, 5);/////组装分割线到盒子里
	gtk_widget_show (separator2);	/////显示分割线*/	

	gtk_container_add(GTK_CONTAINER(vbox3),vbox2);
	gtk_container_add(GTK_CONTAINER(window1),vbox3);
	
	
	gtk_widget_show_all(window1);////////显示所有主件
	gtk_main();
	
}



