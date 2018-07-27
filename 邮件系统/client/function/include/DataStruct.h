#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#define BUFFER_SIZE 1024
#define BUFFERSIZE 1024
#define FILEBUFFERSIZE 1024*10
#define SERVERIP "10.19.46.217"
#define SERVERPORT 1234
#define RES_LENGTH 1024
#define FILE_BUFFER_SIZE 1024*10
#define UserIDPasswdLen 250
#define BUFLENGTH 250
#define BUFFERMAX 1024
#define SQL_MAX   300


typedef struct tagUSER_t
{
	char acUserID[25];		//用户ID
	char acNickName[50];	//昵称,默认为字符串（“新用户”）
	char acPasswd[16];		//密码
	char acTelephone[11];	//电话号码
	int  iState;		//登录状态
}stUser;

typedef struct tagMAIL_t
{
	char acEmailID[25];			//邮件ID
	char acTitle[50];			//邮件标题
	char acContent[50];			//邮件内容
	int  iType;					//邮件类型reserved/send/receive/copy/draft/…
	int  iState;				//0保留/1已经读过/2未读过/3已经回复/4未回复/…
	char acSendTargetID[500];	//多个目标用户ID串，以分号作为分隔符
	char acCopyTargetID[500];	//多个目标用户ID串，以分号作为分隔符
	char acEmailTime[30];		//邮件发送时间/接收时间/草稿保存时间/邮件回复时间
	int  iEmailSize;			//邮件内容大小;注意取值范围0-1MB
	char acAttachedFilePath[50];//多个附件存放路径字符串，以分号作为分隔符；
	char acUserID[25];			//邮件所属用户ID(邮件撰写者)
}stMail;

typedef struct
{ 
   char* emailId;
   char* title;
   char* target;
   char* time;
}FileName;
#endif
