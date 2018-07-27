#ifndef LOCALFUNC_H
#define LOCALFUNC_H
#include "./DataStruct.h"

//void CreateName(char* emailId,char* title,char* target,char* time,char* out);
//int SaveSendFile(char* text,char* userid,char* emailId,char* title,char* target,char* time,char *outputpath);

/***************************************************************************************
* Function Name :  GetContentBefChar
*
* Description   :  获得特定字符前的字符串
*
* Date          :  2012－9－9
*
* Parameter     : head:字符串的首地址;chr:特定字符;out:输出某个字符前的内容
*
* Return Code   :  char* 返回特定字符的后一位的地址
*
* Author        :  王明生
****************************************************************************************/
char* GetContentBefChar(char* head,char chr,char* out);




/***************************************************************************************
* Function Name :  GetLocalMail
*
* Description   :  获取邮件信息
*
* Date          :  2012－9－8
*
* Parameter     : pstMail:获得的邮件信息，是输出参数;pcMailPath:本地邮件;
*
* Return Code   :   返回文件个数
*				  
* Author        :  王明生
****************************************************************************************/
int GetLocalMail (stMail *pstMail,char* pcMailPath);


/***************************************************************************************
* Function Name :  SearchMail
*
* Description   :  查找邮件信息
*
* Date          :  2012－9－9
*
* Parameter     : out:获得的邮件信息，是输出参数;pcKey:查找的关键字;pcCondition:查找的条件;
*
* Return Code   :   返回查找到的文件个数
*				  
* Author        :  王明生
****************************************************************************************/
int SearchMail (char* pcKey,char* pcCondition,char* path,stMail* out);




/***************************************************************************************
* Function Name :  DeleteLocalMail
*
* Description   :  删除本地邮件
*
* Date          :  2012－9－9
*
* Parameter     : pcMailPath:删除的邮件路径;
*
* Return Code   :   ０代码删除成功
*				  
* Author        :  王明生
****************************************************************************************/
int DeleteLocalMail (char* pcMailPath);





/***************************************************************************************
* Function Name :  ViewSingleMail
* 
* Description   :  1、传入文件路径，读取文件内容
*
* Date          :  2012－9－10
*
* Parameter     :  char*  pcMailPath, char * pcMailContent
*
* Return Code   :  0 读取成功
*				   1 打开文件失败
* 
* Author        :  张文跃 
****************************************************************************************/
int ViewSingleMail (char*  pcMailPath, char * pcMailContent);

#endif

