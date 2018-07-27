#ifndef REGISTERMODULE_H
#define REGISTERMODULE_H

#include "DataStruct.h"


int iresocketfd; //描述注册socket号的全局变量

/***************************************************************************************
* Function Name :  CheckRegisterInput
* 
* Description   :  1、检查注册信息
*
* Date          :  2012－9－10
*
* Parameter     :  stUser *userInfo
*
* Return Code   :  0 提交成功
*				   1 - 6 各类提交错误
* 
* Author        :  张文跃 
****************************************************************************************/
int CheckRegisterInput(stUser *userInfo);

/***************************************************************************************
* Function Name :  SendUserIdToServer
* 
* Description   :  1、将用户名拼接成符合SQL语句的字符串
*				   2、发送用户id到服务端
*					
* Date          :  2012－9－8
*
* Parameter     :  char *userId
*
* Return Code   :  0 提交成功
*				   1 提交失败
* 
* Author        :  张文跃 
****************************************************************************************/
int SendUserIdToServer(char *pcUserId);

/***************************************************************************************
* Function Name :  RecvUserIdResFromServer
* 
* Description   :  1、接受从服务断返回的字符串
*				   2、对接受的字符串解析，判断结果
*					
* Date          :  2012－9－10
*
* Parameter     :  void
*
* Return Code   :  0 提交成功
*				   1 提交失败
* 
* Author        :  张文跃 
****************************************************************************************/
int RecvUserIdResFromServer();

/***************************************************************************************
* Function Name :  SendUserIdToServer
* 
* Description   :  1、将用户信息拼接成符合SQL语句的字符串
*				   2、发送用户id到服务端
*					
* Date          :  2012－9－8
*
* Parameter     :  char *userId
*
* Return Code   :  0 提交成功
*				   1 提交失败
* 
* Author        :  张文跃 
****************************************************************************************/
int SendUserInfoToServer(stUser *userInfo);

/***************************************************************************************
* Function Name :  RecvUserIdResFromServer
* 
* Description   :  1、接受从服务断返回的字符串
*				   2、对接受的字符串解析，判断结果
*					
* Date          :  2012－9－10
*
* Parameter     :  void
*
* Return Code   :  0 提交成功
*				   1 提交失败
* 
* Author        :  张文跃 
****************************************************************************************/
int RecvRegisterResFromServer();
#endif
