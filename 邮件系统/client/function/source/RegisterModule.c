#include "../include/RegisterModule.h"
#include "../include/DataStruct.h"
#include "../include/mysocket.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
int CheckRegisterInput(stUser *userInfo)
{
	int i;
	int iUserIDLength = strlen(userInfo->acUserID);
	int iNickNameLength = strlen(userInfo->acNickName);
	int iPasswdLength = strlen(userInfo->acPasswd);
	int iTeleLength = strlen(userInfo->acTelephone);
	printf("用户长度%d,昵称长度%d,密码长度：%d\n",iUserIDLength,iNickNameLength,iPasswdLength);
	for (i = 0;i < iUserIDLength; ++i)
	{
		if((userInfo->acUserID[i] >= '0' && userInfo->acUserID[i] <= '9') || (userInfo->acUserID[i] >= 'a' && userInfo->acUserID[i] <= 'z') || (userInfo->acUserID[i] >= 'A' && userInfo->acUserID[i] <= 'Z'))
		{
			continue;	
		}
		else
		{
			return 1;//返回1，帐户有非法字符
		}
	}

	if (iNickNameLength < 1)
	{
		strcpy(userInfo->acNickName, "NewUser");
	}

	if (iPasswdLength < 6)
	{
		return 2;//返回2，密码长度不足6位
	}
	else if (iPasswdLength > 16)
	{
		return 3;//返回3，密码长度大于16位
	}
	if (iTeleLength > 16)
	{
		return 5;//电话号码过长
	}
	for (i = 0; i < iTeleLength; ++i)
	{
		if (userInfo->acTelephone[i] >= '0' && userInfo->acTelephone[i] <= '9')
		{
			continue;
		}
		else 
			return 6;//含有非数字字符
	}
	return 0;//正确，可提交注册
}

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
int SendUserIdToServer(char *pcUserID)
{
	close_socket(iresocketfd);
	int sendResult = -1;
	char *pUserId = pcUserID;
	int i;
	char event[10] = "sqlselect";
	char buffer[BUFLENGTH];
	memset(buffer, '\0', BUFLENGTH);

	while (*pUserId != '\0')
	{
		if((*pUserId >= '0' && *pUserId <= '9') || (*pUserId >= 'a' && *pUserId <= 'z') || (*pUserId >= 'A' && *pUserId <= 'Z'))
		{
			++pUserId;
		}
		else
			return 1;//含有非法字符
	}
	//调用socket函数传入
	sprintf(buffer, "%s|SELECT userId FROM UserTable WHERE userId = '%s'", event, pcUserID);
	iresocketfd = connect_socket(SERVERIP, SERVERPORT);
	sendResult = send_msg(iresocketfd, buffer, BUFLENGTH);	
	
	if (sendResult == -1)
	{
		return 1;//调用socket失败
	}
	
	return 0;//发送成功
}

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
int RecvUserIdResFromServer()
{
	int iItemNum = 0;   //记录条目数
	char cUserId[26];   //存储用户Id
	int i = 0;
	int j = 0;
	char receBuffer[BUFLENGTH];
	memset(cUserId, '\0', 26);
	memset(receBuffer, '\0', BUFLENGTH);

	recv_msg(iresocketfd,receBuffer,BUFLENGTH);
	//字符流形式'|'一个字段的结束 '~'一条记录的结束   1355|~
	while (receBuffer[i] != '\0')
	{
		//说明一个字段结束
		if (receBuffer[i] == '|' && receBuffer[i + 1] == '~')
		{
			break;		
		}	
		else
		{
			cUserId[j] = receBuffer[i];
			++j;	
		}
		++i;
	}
	close_socket(iresocketfd);
	if ((i = strcmp(cUserId, "0")) == 0)
	{		
		return 0; //说明可以注册
	}
	else
	{
		return 1;//说明不能注册
	}
}

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
int SendUserInfoToServer(stUser *userInfo)
{
	int sendResult;
	char buffer[BUFFERMAX];
	memset(buffer, '\0', BUFFERMAX);
	
	sprintf(buffer, "sqlinsert|INSERT INTO UserTable(userId, nickName, passwd, state, telephone) VALUES('%s','%s','%s',%d, '%s')", userInfo->acUserID, userInfo->acNickName, userInfo->acPasswd, userInfo->iState, userInfo->acTelephone);
	iresocketfd = connect_socket(SERVERIP, SERVERPORT);
	sendResult = send_msg(iresocketfd, buffer, BUFFERMAX);
	if (sendResult == -1)
	{
		return 1;//调用socket失败
	}
	else
	{
		return 0;
	}
}

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
int RecvRegisterResFromServer()
{
	char cregResult[10];
	char receBuffer[BUFFERMAX];
	int i = 0;
	int j = 0;
	memset(cregResult, '\0', 10);
	memset(receBuffer, '\0', 10);

	recv_msg(iresocketfd,receBuffer,BUFFERMAX);
	//字符流形式'|'一个字段的结束 '~'一条记录的结束   1355|~
	close_socket(iresocketfd);
	if ((i = strcmp(receBuffer, "1")) == 0)
	{		

		return 0; //说明注册成功
	}
	else
	{
		return 1;//说明注册失败
	}
}

















