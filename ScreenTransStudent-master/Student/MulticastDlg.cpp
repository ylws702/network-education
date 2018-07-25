/*******************************************************************
FileName: MulticastDlg.cpp
Author	: xiaoheike
Date	: 2015-05-10 14:46:17
Desc	: ��Ļ�㲥�Ի����࣬ʵ����ʾ�Ի��򣬻����Ļͼ�������Լ����ݵķ��͵�
*******************************************************************/
#include "stdafx.h"
#include "MulticastDlg.h"

// CMulticastDlg �Ի���

IMPLEMENT_DYNAMIC(CMulticastDlg, CDialogEx)

CMulticastDlg::CMulticastDlg(CWnd* pParent/* = NULL*/)
: CDialogEx(CMulticastDlg::IDD, pParent)
, m_isMulticastStop(false)
, m_pBmpTransData(NULL)
{
}

CMulticastDlg::CMulticastDlg(sockaddr_in addr, SOCKET socketMulticast, CWnd* pParent)
	: CDialogEx(CMulticastDlg::IDD, pParent)
	, m_socketMulticast(socketMulticast)
	, m_isMulticastStop(false)
	, m_pBmpTransData(NULL)
{
	// 	// ����Ƿ����ڴ�й¶
	// 	_CrtDumpMemoryLeaks();
	// 	// �ڴ�й¶��λ��
	// 	_CrtSetBreakAlloc(919);
}

CMulticastDlg::~CMulticastDlg()
{
	CleanData();
}

void CMulticastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMulticastDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CMulticastDlg ��Ϣ�������
/******************************************************************
Function	: SetIsMulticastStop
Date		: 2015-05-10 16:02:55
Author		: xiaoheike
Parameter	: isMulticastStop--���ս�ʦ����Ļͼ������ֹͣ�ı�־
Return		: void
Desc		: m_isMulticastStop--true �������ս�ʦ����Ļͼ������
m_isMulticastStop--false ���ս�ʦ����Ļͼ�������߳̽���
******************************************************************/
void CMulticastDlg::SetIsMulticastStop(bool isMulticastStop)
{
	m_isMulticastStop = isMulticastStop;
}

/******************************************************************
Function	: SetSocketMulticast
Date		: 2015-05-10 16:04:25
Author		: xiaoheike
Parameter	: socketMulticast--���ӽ�ʦ���Ĺ㲥 SOCKET
Return		: void
Desc		: ���㲥SOCKET ��ֵ
******************************************************************/
void CMulticastDlg::SetSocketMulticast(SOCKET socketMulticast)
{
	m_socketMulticast = socketMulticast;
}

/******************************************************************
Function	: SetScreenData
Date		: 2015-05-10 16:05:25
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���ս�ʦ����������Ļͼ�����ݲ�������ʾ����
******************************************************************/
void CMulticastDlg::SetScreenData()
{
	// ���յ���ʦ����һ����Ļͼ�����ݵĵڼ�������
	int id = 0;
	while (false == m_isMulticastStop)
	{
		MULTICASTDATA multicastData;
		memset(&multicastData, 0, sizeof(MULTICASTDATA));

		m_socketCenter.RecvDataUDP(m_socketMulticast, (char*)&multicastData,
			sizeof(MULTICASTDATA));

		switch (multicastData.infoType)
		{
		case 1: // λͼ������Ϣ
			SetBmpTransDataNotLast(multicastData, id);
			break;
		case 2: // �������һ�η��͵�����
			SetBmpTransDataLast(multicastData, id);
			break;
		default:
			MessageBox(_T("δ֪��ͼ��������Ϣ"), _T("��ʾ"), MB_OK);
			CleanData();
			exit(1);
		}
		ShowBmp(multicastData);
	}
//	CleanData();
}

/******************************************************************
Function	: ShowBmp
Date		: 2015-05-10 16:06:48
Author		: xiaoheike
Parameter	: multicastData--��Ļͼ�����ݵĽṹ��
Return		: void
Desc		: �����յ������Խ�ʦ������Ļͼ��������ʾ���Ի���
******************************************************************/
void CMulticastDlg::ShowBmp(MULTICASTDATA &multicastData)
{
	//�жϴ������Ժ��Ƿ������ʾͼ��
	if (multicastData.isShow)
	{
		BYTE* bmpShowData = UnCompressData(m_pBitMapInfo->bmiHeader.biSizeImage,
			multicastData.bmpCompressSize);
		CDC* dc = GetDC();
		if (dc != NULL)
		{
			::StretchDIBits(dc->m_hDC,
				0,
				0,
				m_rectClient.Width(),
				m_rectClient.Height(),
				0,
				0,
				m_pBitMapInfo->bmiHeader.biWidth,
				m_pBitMapInfo->bmiHeader.biHeight,
				bmpShowData, //λͼ����
				m_pBitMapInfo, //BITMAPINFO λͼ��Ϣͷ
				DIB_RGB_COLORS,
				SRCCOPY
				);
			ReleaseDC(dc);
		}
		delete[] bmpShowData;
		bmpShowData = NULL;
		DeletepBitMapInfo();
	}
}

/******************************************************************
Function	: SetBmpTransDataNotLast
Date		: 2015-05-10 16:08:21
Author		: xiaoheike
Parameter	: multicastData--��Ļͼ�����ݵĽṹ��
id--һ����Ļͼ�����ݴ���˳������
Return		: void
Desc		: �������Խ�ʦ������Ļͼ�����ݣ�����һ��ͼ������һ�Σ�
******************************************************************/
void CMulticastDlg::SetBmpTransDataNotLast(MULTICASTDATA &multicastData, int& id)
{
	if (m_pBmpTransData == NULL)
	{
		int bmpHeadInfoSize = multicastData.bmpHeadInfo.bmiHeader.biSize;
		m_pBitMapInfo = (BITMAPINFO*)LocalAlloc(LPTR, bmpHeadInfoSize);
		memcpy(m_pBitMapInfo, &multicastData.bmpHeadInfo, bmpHeadInfoSize);

		m_pBmpTransData = new BYTE[multicastData.bmpCompressSize];
		memset(m_pBmpTransData, 0, multicastData.bmpCompressSize);

		id = 0;
	}
	// ��������˵�����ݰ���ʧ��
	if (id == multicastData.ID)
	{
		memcpy_s(m_pBmpTransData + multicastData.beginPos,
			MULTICAST_TRANS_SIZE, multicastData.transData, MULTICAST_TRANS_SIZE);
		id++;
	}
	else
	{
		DeletepBmpTransData();

		DeletepBitMapInfo();
	}
	//	return id;
}

/******************************************************************
Function	: SetBmpTransDataLast
Date		: 2015-05-10 16:10:59
Author		: xiaoheike
Parameter	: multicastData--��Ļͼ�����ݵĽṹ��
id--һ����Ļͼ�����ݴ���˳������
Return		: void
Desc		: �������Խ�ʦ������Ļͼ�����ݣ�һ��ͼ������һ�Σ�
******************************************************************/
void CMulticastDlg::SetBmpTransDataLast(MULTICASTDATA &multicastData, int& id)
{
	if (id == multicastData.ID)
	{
		unsigned long lastTransSize = multicastData.bmpCompressSize - multicastData.beginPos;
		memcpy_s(m_pBmpTransData + multicastData.beginPos, lastTransSize,
			multicastData.transData, lastTransSize);
	}
	else
	{
		multicastData.isShow = false;
		id = 0;
		DeletepBmpTransData();

		DeletepBitMapInfo();
	}
}

/******************************************************************
Function	: DeletepBitMapInfo
Date		: 2015-05-10 16:11:27
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CMulticastDlg::DeletepBitMapInfo()
{
	if (m_pBitMapInfo != NULL)
	{
		LocalFree(m_pBitMapInfo);
		m_pBitMapInfo = NULL;
	}
}

/******************************************************************
Function	: DeletepBmpTransData
Date		: 2015-05-10 16:11:43
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CMulticastDlg::DeletepBmpTransData()
{
	if (m_pBmpTransData != NULL)
	{
		delete[] m_pBmpTransData;
		m_pBmpTransData = NULL;
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
void CMulticastDlg::DeleteSocketMulticast()
{
	if (m_socketMulticast != INVALID_SOCKET)
	{
		closesocket(m_socketMulticast);
		m_socketMulticast = INVALID_SOCKET;
	}
}

/******************************************************************
Function	: OnSize
Date		: 2015-05-10 16:12:57
Author		: xiaoheike
Parameter	:
Return		: void
Desc		: ��ӦWM_SIZE ��Ϣ��öԻ���Ŀͻ����ߴ�
******************************************************************/
void CMulticastDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	GetClientRect(&m_rectClient);
}

void CMulticastDlg::CleanData()
{
	DeletepBmpTransData();

	DeletepBitMapInfo();
}

/******************************************************************
Function	: OnInitDialog
Date		: 2015-05-10 16:12:06
Author		: xiaoheike
Parameter	: ��
Return		: BOOL
Desc		: ��ӦWM_InitDialog ��Ϣ�Ի��򴴽����һ�����õķ������öԻ���ȫ����ʾ������Ļ
�رհ�ť
******************************************************************/
BOOL CMulticastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ģ̬�Ի��������ʾ
	//ShowWindow(SW_MAXIMIZE);
	ShowWindow(SW_NORMAL);
	// ʹ�Ի���Ĺرհ�ť��Ч
	CMenu *pSysMenu = GetSystemMenu(FALSE);
	ASSERT(pSysMenu != NULL);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CMulticastDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}

/******************************************************************
Function	: UnCompressData
Date		: 2015-05-10 16:14:13
Author		: xiaoheike
Parameter	: biSizeImage--ͼ������δѹ��ǰ�Ĵ�С
bmpCompressSize--ͼ������ѹ����Ĵ�С
Return		: BYTE*
Desc		: �������յ����Խ�ʦ������Ļͼ������
******************************************************************/
BYTE* CMulticastDlg::UnCompressData(uLongf biSizeImage, unsigned long bmpCompressSize)
{
	uLongf unCompressDataLen = (uLongf)((biSizeImage + 12)*(100.1 / 100)) + 1;

	BYTE* pUnCompressData = new BYTE[unCompressDataLen];
	int err = uncompress(pUnCompressData, &unCompressDataLen,
		m_pBmpTransData, bmpCompressSize);
	if (err != Z_OK) {
		CString str;
		str.Format(_T("uncompess error = %d,unCompressDataLen = %d, biSizeImage = %d, bmpCompressSize = %d"),
			err, unCompressDataLen, biSizeImage, bmpCompressSize);
		MessageBox(str);
		delete[] pUnCompressData;
		pUnCompressData = NULL;
		delete[]m_pBmpTransData;
		m_pBmpTransData = NULL;
		exit(0);
	}

	BYTE* bmpShowData = new BYTE[unCompressDataLen];
	memcpy(bmpShowData, pUnCompressData, unCompressDataLen);

	delete[] pUnCompressData;
	pUnCompressData = NULL;
	DeletepBmpTransData();
	// 	delete[]m_pBmpTransData;
	// 	m_pBmpTransData = NULL;
	return bmpShowData;
}

/******************************************************************
Function	: CloseModalDlg
Date		: 2015-05-10 15:55:00
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ر�Modal �Ի���
******************************************************************/
void CMulticastDlg::CloseModalDlg()
{
	CDialog::OnCancel();
}

/******************************************************************
Function	: OnNcDestroy
Date		: 2015-05-10 15:54:09
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ����������������ͷ��Լ��� �ڴ�(delete this)
******************************************************************/
void CMulticastDlg::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();
}

/******************************************************************
Function	: OnSysCommand
Date		: 2015-05-10 15:55:37
Author		: xiaoheike
Parameter	: nID--
Return		:
Desc		: ��ӦWM_SYSCOMMAND ��Ϣ���ڷ�������Ӵ�������ALT+F4��ݼ�
******************************************************************/
void CMulticastDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// �����ALT + F4 �򲻴��������Ϣ
	if ((nID & 0xFFF0) != SC_CLOSE)
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}