#pragma once
#include <stdio.h>
#include "Comment.h"
#include "SocketCenter.h"
#include "ScreenMonitor.h"
#include "MulticastDlg.h"
class CControlCenter
{
public:
	CControlCenter();

	CControlCenter(STUINFODATA stuInfo);

	~CControlCenter();

	void DeletepMulticastDlg();

	void DeleteSocketMulticast();

	void DeletepScreenMonitor();

	void DeleteSocketMsg();

	void ConnectClient();

	void SetConnectInfo(char* IP, int port);

	void RecvRequest();

	void ExecuteRequest(MSGTYPE msgInfo);

	void BeginMulticast();

	void EndScreenMonitor();

	void BeginScreenMonitor();

	void SendStuInfo();

	void EndMulticast();

	static DWORD WINAPI OnBeginScreenMonitor(LPVOID self);

	static DWORD WINAPI OnBeginMulticast(LPVOID self);

	static DWORD WINAPI OnShowMulticastDlg(LPVOID self);
private:
	// ���ӽ�ʦ����SOCKET��������Ϣ�Ĵ���
	SOCKET m_socketMsg;
	CSocketCenter m_socketCenter;
	// IP ��ַ
	char m_IP[MAX_PATH];
	// SOCKET ���ӵĶ˿ں�
	int m_port;
	CScreenMonitor* m_pScreenMonitor;
	CMulticastDlg* m_pMulticastDlg;
	SOCKET m_socketMulticast;
	sockaddr_in m_addr;
	HANDLE m_hMulticast;
	STUINFODATA m_stuInfo;
	HANDLE m_hScreenDlg;
};
