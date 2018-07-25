/*******************************************************************
FileName: ControlCenter.cpp
Author	: xiaoheike
Date	: 2015-05-09 10:27:18
Desc	: �������Խ�ʦ������Ϣ�����з��䴦��
*******************************************************************/
#include "stdafx.h"
#include "ControlCenter.h"
#include "StuInfo.h"

/******************************************************************
Function	: CControlCenter
Date		: 2015-05-09 10:28:59
Author		: xiaoheike
Parameter	: stuInfo--�Զ���ṹ�壬ѧ������Ϣ
Return		: ��
Desc		: ���췽��
******************************************************************/
CControlCenter::CControlCenter(STUINFODATA stuInfo)
: m_port(0)
, m_socketMsg(INVALID_SOCKET)
, m_pScreenMonitor(NULL)
, m_stuInfo(stuInfo)
, m_pMulticastDlg(NULL)
{
}

CControlCenter::~CControlCenter()
{
	DeleteSocketMsg();

	DeletepScreenMonitor();

	DeleteSocketMulticast();

	DeletepMulticastDlg();
}

/******************************************************************
Function	: DeletepMulticastDlg
Date		: 2015-05-09 16:52:40
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ������ڴ�
******************************************************************/
void CControlCenter::DeletepMulticastDlg()
{
	if (m_pMulticastDlg != NULL)
	{
		delete m_pMulticastDlg;
		m_pMulticastDlg = NULL;
	}
}

/******************************************************************
Function	: DeleteSocketMulticast
Date		: 2015-05-09 16:53:34
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ر�SOCKET ����
******************************************************************/
void CControlCenter::DeleteSocketMulticast()
{
	if (m_socketMulticast != INVALID_SOCKET)
	{
		closesocket(m_socketMulticast);
		m_socketMulticast = INVALID_SOCKET;
	}
}

/******************************************************************
Function	: DeletepScreenMonitor
Date		: 2015-05-09 16:54:11
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ������ڴ�
******************************************************************/
void CControlCenter::DeletepScreenMonitor()
{
	if (m_pScreenMonitor != NULL)
	{
		delete m_pScreenMonitor;
		m_pScreenMonitor = NULL;
	}
}

/******************************************************************
Function	: DeleteSocketMsg
Date		: 2015-05-09 16:55:02
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ر�SOCKET ����
******************************************************************/
void CControlCenter::DeleteSocketMsg()
{
	if (m_socketMsg != INVALID_SOCKET)
	{
		closesocket(m_socketMsg);
		m_socketMsg = INVALID_SOCKET;
	}
}

/******************************************************************
Function	: SetConnectInfo
Date		: 2015-05-09 10:39:10
Author		: xiaoheike
Parameter	: IP--IP��ַ
port--�˿ں�
Return		: void
Desc		: ����IP ��ַ�Ͷ˿ڵ���Ϣ
******************************************************************/
void CControlCenter::SetConnectInfo(char* IP, int port)
{
	strcpy_s(m_IP, IP);
	m_port = port;
}

/******************************************************************
Function	: ConnectClient
Date		: 2015-05-09 10:40:37
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���ӽ�ʦ�����õ�SOCKET
******************************************************************/
void CControlCenter::ConnectClient()
{
	while (true)
	{
		m_socketMsg = m_socketCenter.InitSocket(m_IP, m_port);
		if (m_socketMsg == SOCKET_ERROR)
		{
			Sleep(3000);
			continue;
		}
		else
		{
			break;
		}
	}
	RecvRequest();
}

/******************************************************************
Function	: RecvRequest
Date		: 2015-05-09 10:48:24
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���ϼ���������ʦ��������
******************************************************************/
void CControlCenter::RecvRequest()
{
	// ������ʦ���㲥��SOCKET��IP �鲥������
	m_socketMulticast = m_socketCenter.InitMulticastSocket(MULTICAST_TRANS_PORT,
		MULTICAST_IP);

	MSGTYPE msgType;
	while (true)
	{
		memset(&msgType, 0, sizeof(MSGTYPE));
		// ���ϼ������Կͻ��˵�����
		int result = m_socketCenter.RecvDataTCP(m_socketMsg,
			(char*)&msgType, sizeof(MSGTYPE));
		if (result == 0)
		{
			AfxMessageBox(_T("result = 0"));
			break;
		}
		this->ExecuteRequest(msgType);
	}
}

/******************************************************************
Function	: ExecuteRequest
Date		: 2015-05-09 10:50:44
Author		: xiaoheike
Parameter	: msgType--�Զ���ṹ�壬��Ϣ����Ϣ
Return		: void
Desc		: ִ�н��յ�������
******************************************************************/
void CControlCenter::ExecuteRequest(MSGTYPE msgType)
{
	switch (msgType.msgID)
	{
	case STUDENTINFO:
		SendStuInfo();
		break;
	case BEGINSCREENMONITOR:
		BeginScreenMonitor();
		break;
	case ENDSCREENMONITOR:
		EndScreenMonitor();
		break;
	case BEGINMULTICAST:
		BeginMulticast();
		break;
	case ENDMULTICAST:
		EndMulticast();
		break;
	default:
		AfxMessageBox(_T("Unkonw message"));
		return;
	}
}

/******************************************************************
Function	: BeginMulticast
Date		: 2015-05-09 16:56:18
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �������Խ�ʦ������Ļͼ�����ݲ�������ʾ����
******************************************************************/
void CControlCenter::BeginMulticast()
{
	DeletepMulticastDlg();
	m_pMulticastDlg = new CMulticastDlg();
	m_pMulticastDlg->SetSocketMulticast(m_socketMulticast);
	m_pMulticastDlg->SetIsMulticastStop(false);
	CloseHandle(::CreateThread(0, 0, OnShowMulticastDlg, (LPVOID)m_pMulticastDlg, 0, NULL));
	m_hMulticast = ::CreateThread(0, 0, OnBeginMulticast,
		(LPVOID)m_pMulticastDlg, 0, NULL);
}

/******************************************************************
Function	: OnBeginMulticast
Date		: 2015-05-09 17:12:14
Author		: xiaoheike
Parameter	: self--CMulticast �����
Return		: DWORD WINAPI
Desc		: ��ʦ���㲥�߳����з���
******************************************************************/
DWORD WINAPI CControlCenter::OnBeginMulticast(LPVOID self)
{
	CMulticastDlg* multicastDlg = (CMulticastDlg*)self;
	multicastDlg->SetScreenData();
	//	multicastDlg->DoModal();
	return 0;
}

/******************************************************************
Function	: OnShowMulticastDlg
Date		: 2015-05-09 17:23:20
Author		: xiaoheike
Parameter	: self--CMulticastDlg �����
Return		: DWORD WINAPI
Desc		: ��ʾ��ʦ���㲥ģ̬�Ի�����߳����з���
******************************************************************/
DWORD WINAPI CControlCenter::OnShowMulticastDlg(LPVOID self)
{
	CMulticastDlg* multicastDlg = (CMulticastDlg*)self;
	multicastDlg->DoModal();
	// 	multicastDlg->Create(IDD_MULTICAST);
	// 	multicastDlg->ShowWindow(SW_NORMAL);
	return 0;
}

/******************************************************************
Function	: EndMulticast
Date		: 2015-05-09 16:59:31
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ֹͣ�������Խ�ʦ������Ļͼ������
to-do       : �̻߳�����SOCKET ��RecvFrom �϶������߳��޷�����
******************************************************************/
void CControlCenter::EndMulticast()
{
	m_pMulticastDlg->SetIsMulticastStop(true);
	DWORD exitCode = 0;
	// �ر�ģ̬�Ի���
	m_pMulticastDlg->CloseModalDlg();

	GetExitCodeThread(m_hMulticast, &exitCode);
	while (STILL_ACTIVE == exitCode)
	{
		Sleep(100);
		GetExitCodeThread(m_hMulticast, &exitCode);
	}
	//	TerminateThread(m_hMulticast, exitCode);

	CloseHandle(m_hMulticast);
	m_hMulticast = NULL;
	DeletepMulticastDlg();
	//	delete m_pMulticastDlg;
	//	m_pMulticastDlg = NULL;
}

/******************************************************************
Function	: BeginScreenBmpData
Date		: 2015-05-09 11:05:34
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���͵�ǰ���ӵ���Ļͼ�����ݵ���ʦ��
******************************************************************/
void CControlCenter::BeginScreenMonitor()
{
	DeletepScreenMonitor();
	m_pScreenMonitor = new CScreenMonitor();
	m_pScreenMonitor->SetSocketMsg(m_socketMsg);
	m_pScreenMonitor->SetIsScreenMonitorEnd(true);
	m_hScreenDlg = (::CreateThread(0, 0, OnBeginScreenMonitor,
		(LPVOID)m_pScreenMonitor, 0, NULL));
}

/******************************************************************
Function	: OnBeginScreenMonitor
Date		: 2015-05-09 17:20:57
Author		: xiaoheike
Parameter	: self--CScreenMonitor �����
Return		: DWORD WINAPI
Desc		: ѧ������Ļ����߳����з���
******************************************************************/
DWORD WINAPI CControlCenter::OnBeginScreenMonitor(LPVOID self)
{
	CScreenMonitor* screenThread = (CScreenMonitor*)self;
	screenThread->SendScreenData();
	return 0;
}

/******************************************************************
Function	: EndScreenMonitor
Date		: 2015-05-09 16:57:10
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ֹͣ���ͱ�������Ļͼ�����ݵ���ʦ��
******************************************************************/
void CControlCenter::EndScreenMonitor()
{
	m_pScreenMonitor->SetIsScreenMonitorEnd(false);
	DWORD exitCode = 0;
	::GetExitCodeThread(m_hScreenDlg, &exitCode);
	while (exitCode == STILL_ACTIVE)
	{
		Sleep(10);
		::GetExitCodeThread(m_hScreenDlg, &exitCode);
	}
	CloseHandle(m_hScreenDlg);
	DeletepScreenMonitor();
// 	delete m_pScreenMonitor;
// 	m_pScreenMonitor = NULL;
}

/******************************************************************
Function	: SendStuInfo
Date		: 2015-05-09 17:02:05
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���͵�¼��ѧ������Ϣ����ʦ��
******************************************************************/
void CControlCenter::SendStuInfo()
{
	CStuInfo StuInfo;
	StuInfo.SendStuInfo(m_socketMsg, m_stuInfo);
}