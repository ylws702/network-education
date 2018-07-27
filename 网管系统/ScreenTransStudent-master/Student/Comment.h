#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
//////////////////////////////////////////////////////////////////////////
// �ڴ�й¶ͷ�ļ�
// #define CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// visual Leak dealtector �ڴ�й¶��Ҫ��ӵ�ͷ�ļ� 
//#include <vld.h>
//////////////////////////////////////////////////////////////////////////


#define SCREEN_TRANS_SIZE (20*1024)
#define MULTICAST_TRANS_SIZE (50*1024)
//#define FILE_TRANS_SIZE 512 
#define CONNECT_IP "127.0.0.1"
//efine CONNECT_IP "192.168.1.104"
#define CONNECT_PORT 8000
//#define CONNECT_IP "192.168.1.30"
#define MULTICAST_IP "224.0.0.210"
#define MULTICAST_TRANS_PORT 8030
#define SCREEN_TRANS_PORT 8010
//#define DISKINFO_TRANS_PORT 8020
//#define BLOCKNUM 4

//#define KILOBYTES		1024

#define STUDENTINFO  0x01
#define BEGINSCREENMONITOR 0x0E
//#define SCREENINFO 0x10
#define ENDSCREENMONITOR 0x11
#define BEGINMULTICAST 0x20
//#define MULTICASTINFO 0x21
#define ENDMULTICAST 0x22
//#define MULTICAST 0x23


typedef struct tagMSGTYPE
{
	int msgID;	// ��Ϣ��ID
//	bool flag; // ��־λ�����ڽ�����ʼ���ж�
}MSGTYPE;

typedef struct tagSTUINFO
{
	// ѧ������
	TCHAR m_stuName[20];
	// ѧ���İ༶
	TCHAR m_stuClass[20];
	// ѧ����ѧ��
	TCHAR m_stuID[20];
	// ѧ����רҵ
	TCHAR m_stuMajor[20];
}STUINFODATA;

typedef struct tagSYSTEMINFO
{
	int os;
//	bool Cam; //����ͷ
	double ver;
}SYSTEMINFO;

typedef struct tagBMPHEADINFO
{
	unsigned long bmpCompressSize;
	BITMAPINFO bmpHeadInfo;
}BMPINFO;

 typedef struct tagBMPDATA
 {
	 bool isShow; // �Ƿ������ʾͼ��
	 int infoType;
	 UINT beginPos;
//	 int ID;
	 unsigned long bmpCompressSize;
	 BITMAPINFO bmpHeadInfo;
	 BYTE screenBmpData[SCREEN_TRANS_SIZE];
 }BMPDATA;

 typedef struct tagMULTICAST
 {
	 bool isShow; // �Ƿ������ʾͼ��
	 int infoType;    
	 UINT beginPos;
	 int ID;
	 unsigned long bmpCompressSize;
	 BITMAPINFO bmpHeadInfo;
	 BYTE transData[MULTICAST_TRANS_SIZE];
 }MULTICASTDATA;
