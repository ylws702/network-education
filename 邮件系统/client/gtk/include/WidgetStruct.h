//结构体用于存放注册信息
//登录注册相关文本entry构件结构体
#ifndef DATASTRUCT_H1
#define DATASTRUCT_H1
typedef struct 
{
	GtkWidget* window;
	GtkWidget *UserName;//用户名
	GtkWidget *NickName;//密码
	GtkWidget *Passwd;//密码确认
	GtkWidget *Telephone;
}UserWidget;

//添加／修改联系人 相关构件
typedef struct 
{
	GtkWidget *LinkName;//用户名
	GtkWidget *LinkId;//密码
}LinkmanWidget;

typedef struct
{
GtkWidget* Address;
GtkWidget* CopyAddress;
GtkWidget* SecretAddress;
GtkWidget* Theme;
GtkWidget* Filepath;
}MailWidget;

MailWidget SendEntry;
#endif
