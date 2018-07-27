#include "stdafx.h"
#include "StuInfo.h"


CStuInfo::CStuInfo()
{
}


CStuInfo::~CStuInfo()
{
}

/******************************************************************
Function	: SendStuInfoReq
Date		: 2015-05-12 09:32:46
Author		: xiaoheike
Parameter	: socket--����ѧ����SOCKET 
Return		: void
Desc		: ������ѧ������¼��Ϣ
******************************************************************/
void CStuInfo::SendStuInfoReq(SOCKET socket)
{
	CSocketCenter mySocket;
	mySocket.SendReadyInfo(socket, STUDENTINFO);
}

/******************************************************************
Function	: GetStuInfo
Date		: 2015-05-12 09:34:07
Author		: xiaoheike
Parameter	: socket--����ѧ����SOCKET
Return		: void
Desc		: ���ѧ���ĵ�¼��Ϣ
******************************************************************/
void CStuInfo::GetStuInfo(SOCKET socket)
{
	STUINFODATA stuInfoData;
	memset(&stuInfoData, 0, sizeof(STUINFODATA));
	CSocketCenter mySocket;
	mySocket.RecvDataTCP(socket, (char*)&stuInfoData, sizeof(STUINFODATA));

	m_stuName.Format(_T("%s"), stuInfoData.m_stuName);

	m_stuID.Format(_T("%s"), stuInfoData.m_stuID);
	m_stuMajor.Format(_T("%s"), stuInfoData.m_stuMajor);
	m_stuClass.Format(_T("%s"), stuInfoData.m_stuClass);
}

/******************************************************************
Function	: GetStuName
Date		: 2015-05-12 09:34:29
Author		: xiaoheike
Parameter	: ��
Return		: CString
Desc		: ���ѧ���ĵ�¼����
******************************************************************/
CString CStuInfo::GetStuName() const
{
	return m_stuName;
}


/******************************************************************
Function	: GetStuID
Date		: 2015-05-12 09:34:55
Author		: xiaoheike
Parameter	: ��
Return		: CString
Desc		: ���ѧ���ĵ�¼ID
******************************************************************/
CString CStuInfo::GetStuID() const
{
	return m_stuID;
}


/******************************************************************
Function	: GetStuMajor
Date		: 2015-05-12 09:35:09
Author		: xiaoheike
Parameter	: ��
Return		: CString
Desc		: ���ѧ���ĵ�¼רҵ
******************************************************************/
CString CStuInfo::GetStuMajor() const
{
	return m_stuMajor;
}


/******************************************************************
Function	: GetStuClass
Date		: 2015-05-12 09:35:20
Author		: xiaoheike
Parameter	: ��
Return		: CString
Desc		: ���ѧ���ĵ�¼�༶
******************************************************************/
CString CStuInfo::GetStuClass() const
{
	return m_stuClass;
}
