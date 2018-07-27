#include <gtk/gtk.h>
#include"../include/WidgetStruct.h"
void register_window(GtkWidget *,gpointer);
void afterlogin_window (GtkWidget *, gpointer);

const char *password = "123";
char g_userId[25];

///////*****************************************
///////close the window
///////*****************************************
void closeApp1( GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}


///////////**************************************
/////////                main    函   数
///////////**************************************
int main (int argc, char *argv[])
{
	  //GtkWidget *window;/////////////////////////////////主窗口
	  GtkWidget *username_label, *password_label;////////用户名、输入密码标签
	  GtkWidget *username_entry, *password_entry;////////用户名、输入 密码 输入框 
	  GtkWidget *login_button;///////////////////login按钮
	  GtkWidget *register_button;//////////////// 注 册按钮
	  GtkWidget *hbox1, *hbox2, *hbox3;
	  GtkWidget *vbox;
	  GtkWidget	*separator;
	  UserWidget acLogin;//获取登录信息相关
	  gtk_init(&argc, &argv);

	 acLogin.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);///////////////////////创建主窗口
	  gtk_window_set_title(GTK_WINDOW( acLogin.window), "Login");//////////////////标题名称
	  gtk_window_set_position(GTK_WINDOW(acLogin.window), GTK_WIN_POS_CENTER);////窗口默认位置
	  //gtk_window_set_default_size(GTK_WINDOW(window), 300,200);/////////尺寸
	  gtk_window_set_policy(GTK_WINDOW( acLogin.window),FALSE,FALSE,TRUE);/////////杀死最大化
	  gtk_container_set_border_width(GTK_CONTAINER(acLogin.window),20);

	  gtk_signal_connect( GTK_OBJECT ( acLogin.window), "destroy", 
		                   GTK_SIGNAL_FUNC (closeApp1), NULL);

	  hbox1 = gtk_hbox_new ( TRUE, 0 );//////////////////为标签创建组装盒
	  hbox2 = gtk_hbox_new ( TRUE, 0 );
	  hbox3 = gtk_hbox_new ( TRUE, 2 );

	  vbox = gtk_vbox_new ( FALSE, 10);

	  username_label = gtk_label_new("用户名:");/////用户名标签
	  password_label = gtk_label_new("密　码:");/////密码输入标签
//
	  acLogin.UserName = gtk_entry_new();
	  acLogin.Passwd = gtk_entry_new();
	  gtk_entry_set_visibility(GTK_ENTRY (acLogin.Passwd), FALSE);

	  separator = gtk_hseparator_new();///////////////////////////////////创建一条分割线
	  gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, TRUE, 5);/////组装分割线到盒子里
	  gtk_widget_show (separator);	////////////////////////////////////显示分割线

	  login_button = gtk_button_new_with_label(" 登　录 ");///////////////登录按钮
	  register_button = gtk_button_new_with_label(" 注　册 ");////////////register按钮

	  gtk_signal_connect (GTK_OBJECT (login_button), "clicked", 
		                   GTK_SIGNAL_FUNC(afterlogin_window),&acLogin.window);/////login 的点击事件触发


	 // gtk_widget_destroy(window);



	  gtk_box_pack_start(GTK_BOX(hbox1), username_label, FALSE, FALSE, 0);
	  gtk_box_pack_start(GTK_BOX(hbox1),  acLogin.UserName, TRUE,TRUE, 0);

	  gtk_box_pack_start(GTK_BOX(hbox2), password_label, FALSE, FALSE, 0);
	  gtk_box_pack_start(GTK_BOX(hbox2), acLogin.Passwd, TRUE, TRUE, 0);
	  gtk_box_pack_start(GTK_BOX(hbox3), register_button, TRUE, FALSE, 5);
	  gtk_box_pack_start(GTK_BOX(hbox3), login_button, TRUE, FALSE, 5);

	  gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 5);
	  gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
	  gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 5);


	  gtk_signal_connect (GTK_OBJECT (register_button), "clicked", ////////对register按钮的响应函数 
	 					 GTK_SIGNAL_FUNC (register_window),/////调用将按钮的" c l i c k e d "信号
	 					 acLogin.window);///连接到前面创建的回调函数上

	  //gtk_signal_connect(GTK_OBJECT(register_button),"delete_event",G_CALLBACK(delete_event),NULL);
	 
	  gtk_container_add(GTK_CONTAINER( acLogin.window), vbox);

	  gtk_widget_show_all( acLogin.window);
	  gtk_main ();

	  return 0;
}


