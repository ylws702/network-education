#include "stdafx.h"
#include "SocketCenter.h"
//#include <winsock2.h>


CSocketCenter::CSocketCenter()
{
}


CSocketCenter::~CSocketCenter()
{
//	WSACleanup();
}

/******************************************************************
Function	: InitSocket
Date		: 2015-06-07 15:07:41
Author		: xiaoheike
Parameter	: 		IP--���ӵ�IP��ַ
port--���ӵĶ˿ں�
Return		: socketConnect--���ӽ�ʦ���ɹ����׽���
Desc		: ��ʼ��SOCKET �����ӷ�����
******************************************************************/
SOCKET CSocketCenter::InitSocket(char* IP, int port)
{
	WSADATA wasData;
	WORD w = MAKEWORD(2, 2);
	int err = ::WSAStartup(w, &wasData);
	if (err != 0) 
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 0;
	}
	if (LOBYTE(wasData.wVersion) != 2 ||
		HIBYTE(wasData.wVersion) != 2) 
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		Cleanup();
		return 0;
	}
 	SOCKET socketConnect;
	socketConnect = ::socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(IP);

	if (::connect(socketConnect, (sockaddr*)&addr, sizeof(addr))
				== SOCKET_ERROR)
	{
//		DWORD e = GetLastError();
//		AfxMessageBox(_T("Connect Error"));
		closesocket(socketConnect);
		socketConnect = INVALID_SOCKET;
		Cleanup();
		return SOCKET_ERROR;
	}
	else
	{
//		AfxMessageBox(_T("Connect Success!"));
		//printf("Connect Success!\n");
		return socketConnect;
	}
}

/******************************************************************
Function	: SendDataTCP
Date		: 2015-06-07 15:07:14
Author		: xiaoheike
Parameter	: 		socket--������ʼ����SOCKET����
sendBuf--��Ҫ���͵�������Ϣ����
sendBytes--��Ҫ���͵����ݵĴ�С
Return		: int--���ͳɹ��������ֽ���
Desc		: �����ݷ��͵���ʦ��
******************************************************************/
int CSocketCenter::SendDataTCP(SOCKET socket, const char* sendBuf, int sendBytes)
{
	const char *pNeedToSend = sendBuf;
	while (sendBytes > 0)
	{
		int sendedAmount = send(socket, pNeedToSend, sendBytes, 0);
		if (sendedAmount < 0)
		{
			int err = GetLastError();
			CString str;
			str.Format(_T("Socket_Error: %d"), err);
// 			MessageBox(AfxGetMainWnd()->m_hWnd, str, _T("error"), MB_OK);
			AfxMessageBox(str);
			//printf("Socket_Error: %d\n", err);
			return 0;
		}
		else if (sendedAmount == 0)
		{
			//printf("Socket_Error\n");
			//MessageBox(AfxGetMainWnd()->m_hWnd, _T("SOCKET ERROR"), _T("error"), MB_OK);
			AfxMessageBox(_T("CSocketCenter::SendDataToClient"));
			break;
		}
		sendBytes = sendBytes - sendedAmount;
		pNeedToSend += sendedAmount;
	}
	return pNeedToSend - sendBuf;
}

/******************************************************************
Function	: RecvDataTCP
Date		: 2015-06-07 15:06:39
Author		: xiaoheike
Parameter	: 		socket--������ʼ����SOCKET����
getBuf--�ӿͻ��˽��յ����ݵı�������
sendBytes--�ӿͻ��˽��յ����ݵĴ�С
Return		: int--���ճɹ����ֽ���
Desc		: ���ս�ʦ��������
******************************************************************/
int CSocketCenter::RecvDataTCP(SOCKET socket, char* getBuf, int recvBytes)
{
	char *pNeedToRecv = (char*)getBuf;
	while (recvBytes > 0)
	{
		int RecvedAmount = recv(socket, pNeedToRecv, recvBytes, 0);
		if (RecvedAmount == SOCKET_ERROR)
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

		if (RecvedAmount < 0)
		{
			return 0;
		}
		else if (RecvedAmount == 0)
		{
			break;
		}
		recvBytes = recvBytes - RecvedAmount;
		pNeedToRecv = pNeedToRecv + RecvedAmount;
	}
	return pNeedToRecv - getBuf;
}

/******************************************************************
Function	: SendReadyInfo
Date		: 2015-06-07 15:05:22
Author		: xiaoheike
Parameter	: socket--���ʦ��������socket
msgID--��Ҫ���͸���ʦ������Ϣ
Return		: void
Desc		: ����Ϣ���͵���ʦ��
******************************************************************/
void CSocketCenter::SendReadyInfo(SOCKET socket, int msgID)
{
	MSGTYPE msgType;
	msgType.msgID = msgID;
// 	msgType.flag = false;
	SendDataTCP(socket, (char*)&msgType, sizeof(MSGTYPE));
}

/******************************************************************
Function	: InitMulticastSocket
Date		: 2015-06-07 15:03:23
Author		: xiaoheike
Parameter	: port--���ӽ�ʦ���Ķ˿�
multicastIp--���ӽ�ʦ����IP��ַ
Return		: SOCKET--��ʼ�����SOCKET
Desc		: ��ʼ��UDP���ӣ�IP�鲥�������������Ϣ
******************************************************************/
SOCKET CSocketCenter::InitMulticastSocket(int port, char* multicastIp)
{
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Error in WSAStartup");
		return INVALID_SOCKET;
	}


	struct sockaddr_in addr;
	SOCKET socketRecv;
	struct ip_mreq mreq;
//	char msgbuf[MSGBUFSIZE];

	const u_int yes = 1;            /*** MODIFICATION TO ORIGINAL */

	/* create what looks like an ordinary UDP socket */
	if ((socketRecv = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket_ERROR");
		exit(1);
	}


	/**** MODIFICATION TO ORIGINAL */
	/* allow multiple sockets to use the same PORT number */
	if (setsockopt(socketRecv, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) < 0) {
		printf("Reusing ADDR failed");
		exit(1);
	}
	/*** END OF MODIFICATION TO ORIGINAL */

	/* set up destination address */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /* N.B.: differs from sender */
	addr.sin_port = htons(port);

	/* bind to receive address */
	if (bind(socketRecv, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		printf("bind");
		exit(1);
	}

	/* use setsockopt() to request that the kernel join a multicast group */
	mreq.imr_multiaddr.s_addr = inet_addr(multicastIp);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(socketRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0) {
		printf("setsockopt");
		exit(1);
	}

	return socketRecv;
}


/******************************************************************
Function	: RecvDataUDP
Date		: 2015-06-07 15:02:52
Author		: xiaoheike
Parameter	: 		socket--������ʼ����SOCKET����
getBuf--�ӿͻ��˽��յ����ݵı�������
sendBytes--�ӿͻ��˽��յ����ݵĴ�С
Return		: int--���ͳɹ��������ֽ���
Desc		: �������Խ�ʦ��������
******************************************************************/
int CSocketCenter::RecvDataUDP(SOCKET socket, char* getBuf, int recvBytes)
{
	sockaddr_in addr;
	int addrlen = sizeof(sockaddr_in);
	char *pNeedToRecv = (char*)getBuf;
	while (recvBytes > 0)
	{
		int RecvedAmount = recvfrom(socket, pNeedToRecv, recvBytes, 
			0, (sockaddr *)&addr, &addrlen);
		if (RecvedAmount < 0)
		{
			exit(0);
			//return 0;
		}
		else if (RecvedAmount == 0)
		{
			break;
		}
		recvBytes = recvBytes - RecvedAmount;
		pNeedToRecv = pNeedToRecv + RecvedAmount;
	}
	return pNeedToRecv - getBuf;
}

void CSocketCenter::Cleanup()
{
	WSACleanup();
}

