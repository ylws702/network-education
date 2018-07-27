#ifndef LOGINMODULE_H
#define LOGINMODULE_H
/**************************************************************************************/
/*      著作权所有者     ： 张文跃         								                                                   */
/*      文件名               ： LoginModule.c                                                  */
/*      内容                  ： 登录模块的函数     								                                   */
/*      作成日期  作者   ：                                                         					         			   */
/*      修正日期  作者   ：                                                         									   */
/**************************************************************************************/




int isocketfd; //描述socket号的全局变量
/***************************************************************************************
* Function Name :  CheckLoginInput
*
* Description   :  分析用户输入的帐号密码是否合法
*
* Date          :  2012－9－6
*
* Parameter     :  char *pcUserID,char* pcPasswd
*
* Return Code   :  0 成功
*				   1 帐户密码有非法字符  
*				   2 密码有非法字符
*				   3 密码太短
*
* Author        :  张文跃 
****************************************************************************************/
int CheckLoginInput(char *pcUserID, char* pcPasswd);

/***************************************************************************************
* Function Name :  SendLoginInfoToServer
*
* Description   :  1、将命令类别，帐号，密码连接成数据库字符串
*				   2、调用socket函数发送字符串到服务器
*
* Date          :  2012－9－7
*
* Parameter     :  char *pcUserID,char* pcPasswd
*
* Return Code   :  0 发送成功
*				   1 调用socket失败
*
* Author        :  张文跃 
****************************************************************************************/
int SendLoginInfoToServer(char *pcUserID, char* pcPasswd);

/***************************************************************************************
* Function Name :  RecvLoginResFromServer
*
* Description   :  1、接受服务器处理后返回的字符串
*				   2、对字符串作处理判断登录结果并返回
*
* Date          :  2012－9－7
*
* Parameter     : void
*
* Return Code   :  0 登录成功
*				   1 登录名或者密码错误
*				   2 已经登录过
*
* Author        :  张文跃 
****************************************************************************************/
int RecvLoginResFromServer();

/***************************************************************************************
* Function Name :  CreateUserFolder
* 
* Description   :  1、登录成功后创建接收文件夹(recv),发送文件夹(send),草稿箱(draft)
*
* Date          :  2012－9－9
*
* Parameter     :  char *userId
*
* Return Code   :  0 创建成功
*				   1 创建失败
* 
* Author        :  张文跃 
****************************************************************************************/
int CreateUserFolder(char *userId);

int ViewSingleMail (char*  pcMailPath, char * pcMailContent);
#endif
