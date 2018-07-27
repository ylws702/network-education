#pragma once
#include "resource.h"
#include "SocketCenter.h"
// zlib�� ��Ҫʹ�����е�ѹ��ͼƬ����
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zdll.lib")
// CMulticastDlg �Ի���

class CMulticastDlg : public CDialogEx
	{
	DECLARE_DYNAMIC(CMulticastDlg)

public:
	CMulticastDlg(sockaddr_in addr, /*SOCKET socket = 0,*/ SOCKET socketMulticast = 0, CWnd* pParent = NULL);   // ��׼���캯��
	CMulticastDlg(CWnd* pParent = NULL);
//	CMulticastDlg(CWnd* pParent = NULL, SOCKET socketMulticast = 0);
	virtual ~CMulticastDlg();

// �Ի�������
	enum { IDD = IDD_MULTICAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socketMulticast;
	bool m_isMulticastStop;
	BITMAPINFO* m_pBitMapInfo;
	BYTE* m_pBmpTransData;
	CRect m_rectClient;
public:
	void SetScreenData();

	void ShowBmp(MULTICASTDATA &multicastData);

	void SetBmpTransDataNotLast(MULTICASTDATA &multicastData, int& id);

	void SetBmpTransDataLast(MULTICASTDATA &multicastData, int& id);

	void DeletepBitMapInfo();

	void DeletepBmpTransData();

	void DeleteSocketMulticast();

	BYTE* UnCompressData(uLongf biSizeImage, unsigned long bmpCompressSize);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CleanData();

	virtual BOOL OnInitDialog();

	afx_msg void OnClose();

	static DWORD WINAPI OnSetScreenData();

	static DWORD WINAPI OnSetScreenData(LPVOID self);

	void SetIsMulticastStop(bool isStop);

	void SetSocketMulticast(SOCKET multicastSocket);

	void CloseModalDlg();

	afx_msg void OnNcDestroy();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
private:
	CSocketCenter m_socketCenter;

};
