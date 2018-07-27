/******************************************************************* 
FileName: ItemData.cpp
Author	: xiaoheike
Date	: 2015-05-12 15:50:53
Desc	: �������ѧ��������Ļͼ�����ݲ�ת������ʾ�Ի���


*******************************************************************/ 
#include "stdafx.h"
#include "ItemData.h"

CItemData::CItemData()
{
}

CItemData::~CItemData()
{
}

CItemData::CItemData(int id, HWND m_hWnd, SOCKET msgSocket)
	: m_id(id)
	, m_hWnd(m_hWnd)
	, m_socketMsg(msgSocket)
	, m_pBitMapInfo(NULL)
	, m_pBmpCompressData()
	, m_endMonitor(false)
	, m_isOneStuInScreen(false)
{
}

/******************************************************************
Function	: OnListen
Date		: 2015-05-12 15:19:28
Author		: xiaoheike
Parameter	: lp--CItemData ����
Return		: DWORD WINAPI
Desc		: ��������Ϣ���̵߳��ú���
******************************************************************/
DWORD WINAPI CItemData::OnListen(LPVOID lp)
{
	CItemData* t = (CItemData*)lp;
	t->OnBeginListen();
	return 0;
}

/******************************************************************
Function	: Run
Date		: 2015-05-12 15:23:40
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ����������������Ϣ���߳�
******************************************************************/
void CItemData::Run()
{
	::CloseHandle(CreateThread(0, 0, OnListen, (LPVOID)this, 0, NULL));
}

/******************************************************************
Function	: SendScreenBmpData
Date		: 2015-05-12 15:24:04
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ��������ѧ��������Ļͼ�����ݲ������ݷ��͸���ʾͼ��ĶԻ���
******************************************************************/
void CItemData::SetScreenBmpData()
{
	BMPDATA bmpData;
	memset(&bmpData, 0, sizeof(BMPDATA));
	int nRet = m_Mysocket.RecvDataTCP(m_socketMsg, (char*)&bmpData, sizeof(BMPDATA));

	switch (bmpData.infoType)
	{
	case 1: // λͼ������Ϣ
		SetBmpTransDataNotLast(bmpData);
		break;
	case 2: // �������һ�η��͵�����
		SetBmpTransDataLast(bmpData);
		break;
	default:
		MessageBox(_T("δ֪��ͼ������ID"), _T("��ʾ"), MB_OK);
		CleanData();
		exit(1);
	}
	SendBmpDataToDlg(bmpData);
}

/******************************************************************
Function	: SendBmpDataToDlg
Date		: 2015-05-12 15:26:18
Author		: xiaoheike
Parameter	: bmpData--MBPDATAͼ�����ݵĽṹ�壬ͼ��ı�Ҫ��Ϣ
Return		: void
Desc		: ��ѧ�������յ�����Ļͼ�����ݷ��͵���ʾͼ��ĶԻ���
******************************************************************/
void CItemData::SendBmpDataToDlg(BMPDATA &bmpData)
{
	if (bmpData.isShow && false == m_endMonitor)
	{
		BMP bmp;
		bmp.pBmpCompressData = m_pBmpCompressData;
		bmp.pBMPINFO = m_pBitMapInfo;
		bmp.bmpCompressSize = bmpData.bmpCompressSize;
		if (false == m_isOneStuInScreen)
		{
			::SendMessage(this->m_hWnd, ID_SETBMPDATA, (WPARAM)&bmp, (LPARAM)&m_itemOrder);
		}
		else
		{
			::SendMessage(this->m_hWnd, ID_SETONESTUBMPDATA, (WPARAM)&bmp, (LPARAM)&m_itemOrder);
		}

		// ���͵���Ϣ��������ɷ���֮��Ż�����֮��Ĵ���
		delete[] m_pBmpCompressData;
		m_pBmpCompressData = NULL;
		LocalFree(m_pBitMapInfo);
		m_pBitMapInfo = NULL;
	}
}

/******************************************************************
Function	: SetBmpTransDataLast
Date		: 2015-05-12 15:27:38
Author		: xiaoheike
Parameter	: bmpData--MBPDATAͼ�����ݵĽṹ�壬ͼ��ı�Ҫ��Ϣ
Return		: void
Desc		: ��������ѧ������һ����Ļͼ�����ݵ����һ����
******************************************************************/
void CItemData::SetBmpTransDataLast(BMPDATA &bmpData)
{
	int lastTransSize = bmpData.bmpCompressSize - bmpData.beginPos;
	memcpy_s(m_pBmpCompressData + bmpData.beginPos, lastTransSize,
		bmpData.transData, lastTransSize);
}

void CItemData::SetBmpTransDataNotLast(BMPDATA &bmpData)
{
	if (NULL == m_pBmpCompressData)
	{
		int bmpHeadInfoSize = bmpData.bmpHeadInfo.bmiHeader.biSize;
		m_pBitMapInfo = (BITMAPINFO*)LocalAlloc(LPTR, bmpHeadInfoSize);
		memcpy(m_pBitMapInfo, &bmpData.bmpHeadInfo, bmpHeadInfoSize);

		m_pBmpCompressData = new BYTE[bmpData.bmpCompressSize];
		memset(m_pBmpCompressData, 0, bmpData.bmpCompressSize);
	}
	memcpy_s(m_pBmpCompressData + bmpData.beginPos,
		SCREEN_TRANS_SIZE, bmpData.transData, SCREEN_TRANS_SIZE);
}

/******************************************************************
Function	: UnCompressData
Date		: 2015-05-12 15:28:43
Author		: xiaoheike
Parameter	: biSizeImage--δѹ��ʱ��ͼ���С
			  bmpCompressSize--ѹ�����ͼ���С
Return		: BYTE*
Desc		: �����յ�����Ļͼ�����ݽ�ѹ
******************************************************************/
BYTE* CItemData::UnCompressData(uLongf biSizeImage, uLongf bmpCompressSize)
{
	uLongf unCompressDataLen = (uLongf)((biSizeImage + 12)*(100.1 / 100)) + 1;
	BYTE* pUnCompressData = new BYTE[unCompressDataLen];
	int err = uncompress(pUnCompressData, &unCompressDataLen,
		m_pBmpCompressData, bmpCompressSize);
	if (err != Z_OK)
	{
		CString str;
		str.Format(_T("uncompess error: %d"), err);
		MessageBox(str);
		exit(1);
	}

	BYTE* bmpShowData = new BYTE[unCompressDataLen];
	memcpy(bmpShowData, pUnCompressData, unCompressDataLen);

	delete[] pUnCompressData;
	pUnCompressData = NULL;

	return bmpShowData;
}

/******************************************************************
Function	: BeginMonitor
Date		: 2015-05-12 15:29:51
Author		: xiaoheike
Parameter	: itemOrder--�����ѧ�����ڶԻ�������ʾ�����
Return		: void
Desc		: 1�Զ���ʱ������Ļ�����Ϣ��ѧ��������ʾ��ʦ��Ҫ�����Ļ���
******************************************************************/
void CItemData::BeginMonitor(int itemOrder)
{
	m_itemOrder = itemOrder;
	m_endMonitor = false;
	m_isOneStuInScreen = false;
	m_Mysocket.SendReadyInfo(m_socketMsg, BEGINSCREENMONITOR);
}

/******************************************************************
Function	: BeginMonitor
Date		: 2015-05-12 15:31:13
Author		: xiaoheike
Parameter	: isOneSutInScreen--false ��ѧ������Ļͼ�����ݷ��͸�class CScreenMonitorDlg
true ��ѧ������Ļͼ�����ݷ��͸�class COneStuScreenDlg
			  itemOrder--ѧ����Ļ�ڶԻ�������ʾ�����
Return		: void
Desc		: 1��1���ʱ������Ļ�����Ϣ��ѧ��������ʾ��ʦ��Ҫ�����Ļ���
******************************************************************/
void CItemData::BeginMonitor(bool isOneSutInScreen, int itemOrder)
{
	m_itemOrder = itemOrder;
	m_isOneStuInScreen = isOneSutInScreen;
	m_endMonitor = false;
	m_Mysocket.SendReadyInfo(m_socketMsg, BEGINSCREENMONITOR);
}

/******************************************************************
Function	: EndMonitor
Date		: 2015-05-12 15:34:26
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ������Ϣ��������ѧ��������Ļͼ����
******************************************************************/
void CItemData::EndScreenMonitor()
{
	m_Mysocket.SendReadyInfo(m_socketMsg, ENDSCREENMONITOR);
	//	m_itemOrder = itemOrder;
	m_endMonitor = true;
}

/******************************************************************
Function	: BeginMulticast
Date		: 2015-05-12 15:35:26
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ��ʼ��ʦ����Ļ�㲥
******************************************************************/
void CItemData::BeginMulticast()
{
	m_Mysocket.SendReadyInfo(m_socketMsg, BEGINMULTICAST);
}

/******************************************************************
Function	: EndMulticast
Date		: 2015-05-12 15:35:47
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ������ʦ����Ļ�㲥
******************************************************************/
void CItemData::EndMulticast()
{
	m_Mysocket.SendReadyInfo(m_socketMsg, ENDMULTICAST);
}

void CItemData::CleanData()
{
	DeletepBmpCompressData();
	DeletepBitMapInfo();
}

/******************************************************************
Function	: DeletepBitMapInfo
Date		: 2015-05-12 15:36:36
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CItemData::DeletepBitMapInfo()
{
	if (m_pBitMapInfo != NULL)
	{
		LocalFree(m_pBitMapInfo);
		m_pBitMapInfo = NULL;
	}
}

/******************************************************************
Function	: DeletepBmpCompressData
Date		: 2015-05-12 15:36:40
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CItemData::DeletepBmpCompressData()
{
	if (m_pBmpCompressData != NULL)
	{
		delete[] m_pBmpCompressData;
		m_pBmpCompressData = NULL;
	}
}

/******************************************************************
Function	: OnBeginListen
Date		: 2015-05-12 15:37:08
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ��������ѧ��������Ϣ
******************************************************************/
void CItemData::OnBeginListen()
{
	int nRet;
	MSGTYPE msgType;
	m_stuInfo.SendStuInfoReq(m_socketMsg);
	while (true)
	{
		SendDataTCP(nRet, msgType);
		if (nRet  != 0)
		{
			switch (msgType.msgID)
			{
			case STUDENTINFO:
				m_stuInfo.GetStuInfo(m_socketMsg);
				::SendMessage(m_hWnd, ID_STUDENTLONGIN, (WPARAM)this, 0);
				break;
			case BEGINSCREENMONITOR:
				SetScreenBmpData();
				break;
			default:
				MessageBox(_T("��Ϣ������"), _T("����"), MB_OK);
				break;
			}
		}
	}
}

/******************************************************************
Function	: SendDataTCP
Date		: 2015-05-12 15:38:03
Author		: xiaoheike
Parameter	: nRet--������Ϣ�ķ���ֵ
			  msgType--����ѧ��������Ϣ����������
Return		: void
Desc		: ��������ѧ��������Ϣ
******************************************************************/
void CItemData::SendDataTCP(int &nRet, MSGTYPE& msgType)
{
	nRet = m_Mysocket.RecvDataTCP(m_socketMsg, (char*)&msgType, sizeof(MSGTYPE));
	// SOCKET �����Ƿ�ر�
	if (nRet == 0)
	{
		// SOCKET �Ƿ�������
		if (::WSAGetLastError() == WSAEWOULDBLOCK)
		{
			Sleep(50);
		}
		else
		{
			::closesocket(m_socketMsg);
			::SendMessage(this->m_hWnd, ID_STUDENTLOGINOUT, this->m_id, 0);
			return;
		}
	}
}

/******************************************************************
Function	: GetSocket
Date		: 2015-05-12 15:39:21
Author		: xiaoheike
Parameter	: ��
Return		: SOCKET
Desc		: ����һ�����ӵ�SOCKET
******************************************************************/
SOCKET CItemData::GetSocket()
{
	return this->m_socketMsg;
}
