/******************************************************************* 
FileName: StuInfo.cpp
Author	: xiaoheike
Date	: 2015-05-10 14:41:45
Desc	: ר�Ŵ���ѧ����Ϣ������ݵ���
*******************************************************************/ 
#include "stdafx.h"
#include "StuInfo.h"
#include "SocketCenter.h"


CStuInfo::CStuInfo()
{
}


CStuInfo::~CStuInfo()
{
}

/******************************************************************
Function	: SendStuInfo
Date		: 2015-05-10 14:41:35
Author		: xiaoheike
Parameter	: socket--���ӽ�ʦ����SOCKET�� stuInfo--ѧ����Ϣ�ṹ��
Return		: 
Desc		: 
******************************************************************/
void CStuInfo::SendStuInfo(SOCKET socket, STUINFODATA& stuInfo)
{
	CSocketCenter socketCenter;
	// ֪ͨ��ʦ��׼������ѧ����Ϣ
	socketCenter.SendReadyInfo(socket, STUDENTINFO);
	// ���͵�¼ѧ������Ϣ����ʦ��
	socketCenter.SendDataTCP(socket, (char*)&stuInfo, sizeof(STUINFODATA));
}
