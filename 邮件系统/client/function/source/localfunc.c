#include <stdio.h>
#include "../include/DataStruct.h"
#include <stdlib.h>

  //从字符串src中查找target串，如果找到就返回第一个匹配的位置(从0开始)，否则返回-1
int StringMatch(char * src,char * target)
{
	int srcLen=0;
	int targetLen=0;
	int i=0;
	int j=0;
	int index=0;
	if(src==NULL&&target==NULL)
	{
		return -1;
	}
	srcLen=strlen(src);
	targetLen=strlen(target);
	while((i<srcLen) && (j<targetLen))
	{
		if(src[i]==target[j])
		{
			i++;
			j++;
		}
		else
		{
			i=i-j+1;
			j=0;
		}
	}
	if(j==targetLen)
	{
		index=i-targetLen;
	}
	else
	{
		index=-1;
	}
	return index;
}

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
char* GetContentBefChar(char* head,char chr,char* out)
{
   char* phead=head;
   char* plast=head;
   plast=strchr(phead,chr);
   strncpy(out,phead,plast-phead);
   out[plast-phead]='\0';
   return plast+1;
}




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
int GetLocalMail (stMail *pstMail,char* pcMailPath)
{
   char buffer[200][BUFFERSIZE];
   sprintf(buffer,"ls %s | head > tmp.dat",pcMailPath);
   system(buffer);
   
   FILE* fp=NULL;
   fp=fopen("./tmp.dat","r");
   if(fp==NULL)
   {
      printf("open erro\n");
      return -1;
   }
  
   int i=0; 

    char tmp[2];
   while( !feof(fp) )
   {
     int readlength=0;
    
	 buffer[i][0]='\0';
     while ((readlength =fread (tmp, sizeof (char), 1, fp)) > 0)
     {
		tmp[readlength]='\0';
	    //g_print("tmp=%s",tmp);
        if(tmp[0]=='\n'||tmp[0]==EOF)
        {
			break;
		}
		strcat(buffer[i],tmp);
	 }
    // printf("%s\n",buffer[i]);
     if(strlen(buffer[i])<2)
       break;
    // fscanf(fp,"%s ",buffer[i]);
     char* head=buffer[i];
	 head=GetContentBefChar(head,'|',pstMail[i].acEmailID);
	 head=GetContentBefChar(head,'|',pstMail[i].acTitle);
	 char str[20];
    // head=GetContentBefChar(head,'|',str);
	// pstMail[i].iType=atoi(str);
     head=GetContentBefChar(head,'|',pstMail[i].acSendTargetID);
	 head=GetContentBefChar(head,'|',pstMail[i].acEmailTime);
	 i++;
   }
   
   return i;
}

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
int SearchMail (char* pcKey,char* pcCondition,char* path,stMail* out)
{
    stMail pstMail[50];
	int num=GetLocalMail(pstMail,path);
    //printf("%d\n",num);
	int i=0;
    int j=0;
    while(i<num)
    { 
     if(StringMatch(pstMail[i].acTitle,pcKey)!=-1)
     { 
			strcpy(out[j].acEmailID,pstMail[i].acEmailID);
			strcpy(out[j].acTitle,pstMail[i].acTitle);
			strcpy(out[j].acSendTargetID,pstMail[i].acSendTargetID);
			strcpy(out[j].acEmailTime,pstMail[i].acEmailTime);
		    j++;
	 }
     i++;
    }
  return j;
}


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
int DeleteLocalMail (char* pcMailPath)
{
      //删除本地邮件
      
      int i=remove(pcMailPath);
      return i;
}




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
int ViewSingleMail (char*  pcMailPath, char * pcMailContent)
{
	FILE *fp;
	char c;
	int i = 0;
	if ((fp = fopen(pcMailPath, "r")) == NULL)
	{
		return 1;//打开文件失败
	}
	else
	{
		while ((c = fgetc(fp)) != EOF)
		{
			pcMailContent[i] = c;
			++i;
		}
		pcMailContent[i] = '\0';
	}
	fclose(fp);
	return 0;
}

