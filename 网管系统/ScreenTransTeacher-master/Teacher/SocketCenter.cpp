/******************************************************************* 
FileName: MySocket.cpp
Author	: xiaoheike
Date	: 2015-05-12 16:11:57
Desc	: SOCKET �������ݴ���ĺ�����
*******************************************************************/ 
#include "stdafx.h"
#include "SocketCenter.h"


CSocketCenter::CSocketCenter()
// 	: m_screenSocketRecv(INVALID_SOCKET)
	: m_socketInit(INVALID_SOCKET)
// 	, m_diskDownLoadSocket(INVALID_SOCKET)
// 	, m_diskInfoSocket(INVALID_SOCKET)
{
}


CSocketCenter::~CSocketCenter()
{
	if (m_socketInit != INVALID_SOCKET)
	{
		closesocket(m_socketInit);
		m_socketInit = INVALID_SOCKET;
	}
}

void CSocketCenter::Clean()
{
	::WSACleanup();
}

/******************************************************************
Function	: SendDataTCP
Date		: 2015-05-12 16:13:52
Author		: xiaoheike
Parameter	: socket--SOCKET
			  buf--��������ݵĻ���ַ
			  bytes--��������ݵĴ�С��buf�����ݵĴ�С
Return		: int--���ͳɹ������ݵ��ֽ���
Desc		: ����TCP �������ݵ�ѧ����
******************************************************************/
int CSocketCenter::SendDataTCP(SOCKET socket, const char* buf, int bytes)
{
	const char *b = buf;
	while (bytes > 0)
	{
		int r = send(socket, b, bytes, 0);
// 		if (SOCKET_ERROR == r)
// 		{
// 			OutputError();
// 		}
		if (r < 0)
		{
			return r;
		}
		else if (r == 0)
		{
			break;
		}
		bytes -= r;
		b += r;
	}
	return b - (char*)buf;
}

/******************************************************************
Function	: OutputError
Date		: 2015-05-12 16:15:52
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���ݴ��������������Ϣ
******************************************************************/
void CSocketCenter::OutputError()
{
	int iError = WSAGetLastError();
	if (iError == WSAEWOULDBLOCK)
		AfxMessageBox(_T("recv failed with error : WSAEWOULDBLOCK\n"));
	else
	{
		CString str;
		str.Format(_T("recv failed with error: %ld\n"), iError);
		AfxMessageBox(str);
	}
}

/******************************************************************
Function	: RecvDataTCP
Date		: 2015-05-12 16:16:16
Author		: xiaoheike
Parameter	: socket--SOCKET
			  buf--��������ݵĻ���ַ  
			  bytes--��������ݵĴ�С��buf�����ݵĴ�С
Return		: int--����ɹ������ݵĴ�С
Desc		: ����TCP��������ѧ����������
******************************************************************/
int CSocketCenter::RecvDataTCP(SOCKET socket, char* buf, int bytes)
{
	char *b = (char*)buf;
	while (bytes > 0)
	{
		int r = recv(socket, b, bytes, 0);
// 		if (SOCKET_ERROR == r)
// 		{
// 			OutputError();
// 		}
		if (r < 0)
		{
			return 0;
		}
		else if (r == 0)
		{
			break;
		}
		bytes = bytes - r;
		b = b + r;
	}
	return b - (char*)buf;
}

/******************************************************************
Function	: InitSocketInfo
Date		: 2015-05-12 16:17:16
Author		: xiaoheike
Parameter	: port--�����Ķ˿ں�
			  listenCount--��������ѧ����������
Return		: bool ���ӳɹ����
Desc		: ��ʼ��SOCKET ��Ϣ��֮����Խ�������
******************************************************************/
bool CSocketCenter::InitSocketInfo(int port, int listenCount)
{
	WSADATA wsaData;
	int err = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 0 ;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return 0;
	}
//	SOCKET socketInit;
	m_socketInit = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketInit == SOCKET_ERROR)
	{
		AfxMessageBox(_T("��������ʧ��"));
		//		return SOCKET_ERROR;
		return false;
	}

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.S_un.S_addr = INADDR_ANY;
	saddr.sin_port = htons(port);

	int nRet;
	nRet = ::bind(m_socketInit, (SOCKADDR *)&saddr, sizeof(saddr));
	if (nRet == SOCKET_ERROR)
	{
		AfxMessageBox(_T("�󶨶˿�ʧ��"));
		return false;
	}

	nRet = listen(m_socketInit, listenCount);
	if (nRet == SOCKET_ERROR)
	{
		AfxMessageBox(_T("�����˿�ʧ��"));
		return false;
	}
	return true;
}

/******************************************************************
Function	: SendReadyInfo
Date		: 2015-05-12 16:18:49
Author		: xiaoheike
Parameter	: socket--��ѧ�������ӳɹ���SOCKET
			  msgID--��������ͣ�������Ļ���
Return		: void
Desc		: ����������Ϣ��ѧ����
******************************************************************/
void CSocketCenter::SendReadyInfo(SOCKET socket, int msgID)
{
	MSGTYPE msgType;
	msgType.msgID = msgID;
	// 	msgType.flag = false;
	SendDataTCP(socket, (char*)&msgType, sizeof(MSGTYPE));
}

/******************************************************************
Function	: GetSocketInit
Date		: 2015-05-12 16:20:00
Author		: xiaoheike
Parameter	: ��
Return		: SOCKET
Desc		: ��ó�ʼ����SOCKET���������ڽӷ�����
******************************************************************/
SOCKET CSocketCenter::GetSocketInit()
{
	return m_socketInit;
}


/******************************************************************
Function	: InitMulticastSocket
Date		: 2015-05-12 16:20:50
Author		: xiaoheike
Parameter	: port--�㲥��������Ϣ
			  multicastIp--�鲥IP
			  addr--sockaddr_in
Return		: 
Desc		: ��ʼ���㲥��SOCKET ��Ϣ
******************************************************************/
SOCKET CSocketCenter::InitMulticastSocket(int port, char* multicastIp, sockaddr_in& addr)
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Error in WSAStartup");
	}

	int fd;

	/* create what looks like an ordinary UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket");
		exit(1);
	}

	/* set up destination address */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(multicastIp);
	addr.sin_port = htons(port);

	return fd;
}


/******************************************************************
Function	: SendDataUDP
Date		: 2015-05-12 16:21:55
Author		: xiaoheike
Parameter	: socket--�㲥SOCKET
			  sendBuf--Ҫ���͵����ݵĴ�С
			  sendBytes--���͵����ݵĴ�С��sendBufָ������ݵĴ�С
			  addr--sockaddr_in
Return		: 
Desc		: ����UDP �������ݵ�ѧ����
******************************************************************/
int CSocketCenter::SendDataUDP(SOCKET socket, const char* sendBuf, int sendBytes, sockaddr_in addr)
{
	int addrlen = sizeof(sockaddr_in);
	const char *pNeedToSend = sendBuf;
	while (sendBytes > 0)
	{
		int sendedAmount = sendto(socket, pNeedToSend, sendBytes, 0, 
			(sockaddr *)&addr, addrlen);
		if (sendedAmount < 0)
		{
			int err = GetLastError();
			CString str;
			str.Format(_T("Socket_Error: %d"), err);
			AfxMessageBox(str);
			return sendedAmount;
		}
		else if (sendedAmount == 0)
		{
			AfxMessageBox(_T("CSocketCenter::SendDataUDP"));
			break;
		}
		sendBytes = sendBytes - sendedAmount;
		pNeedToSend += sendedAmount;
	}
	return pNeedToSend - sendBuf;
}