/******************************************************************* 
FileName: ScreenMonitor.cpp
Author	: xiaoheike
Date	: 2015-05-10 14:43:29
Desc	: ��Ļ����࣬ʵ����Ļ�����ط�����������Ļͼ�����ݵĴ���
		  ��ȡ��
*******************************************************************/ 
#include "stdafx.h"
#include "ScreenMonitor.h"
#include <math.h>

CScreenMonitor::CScreenMonitor()
	: m_pBmpTransData(NULL)
	, m_pBitMapInfo(NULL)
	, m_isScreenMonitorEnd(true)
	, m_compressBmpDataLen(0)
{
}

CScreenMonitor::~CScreenMonitor()
{
	CleanData();
}

void CScreenMonitor::CleanData()
{
	DeletepBitMapInfo();

	DeletepBmpTransData();
}

/******************************************************************
Function	: DeletepBmpTransData
Date		: 2015-05-09 21:20:38
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CScreenMonitor::DeletepBmpTransData()
{
	if (m_pBmpTransData != NULL)
	{
		delete[] m_pBmpTransData;
		m_pBmpTransData = NULL;
	}
}

/******************************************************************
Function	: DeletepBitMapInfo
Date		: 2015-05-09 21:21:10
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CScreenMonitor::DeletepBitMapInfo()
{
	if (m_pBitMapInfo != NULL)
	{
		LocalFree(m_pBitMapInfo);
		m_pBitMapInfo = NULL;
	}
}

/******************************************************************
Function	: SetSocketMsg
Date		: 2015-05-09 21:21:24
Author		: xiaoheike
Parameter	: socketMsg--���ӽ�ʦ����SOCKET
Return		: void
Desc		: �����ԱSOCKET ��ֵ
******************************************************************/
void CScreenMonitor::SetSocketMsg(SOCKET socketMsg)
{
	m_socketMsg = socketMsg;
	// 	m_socketScreen = socketScreen;
}

/******************************************************************
Function	: GetDeskScreeData
Date		: 2015-05-09 21:22:41
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ��������ͼ�����ݲ�����ѹ������
******************************************************************/
void CScreenMonitor::GetDeskScreeData()
{
	CDC memDC; //����һ���ڴ滭��
	CBitmap bmp;
	BITMAP bitmap;
	this->GetDeskScreenDC(memDC, bmp, bitmap);

	this->SetCursorIcon(memDC);

	// ����ԭʼ����ͼ�����ݵ�����
	BYTE* pBmpOriginData = new BYTE[m_imgTotalSize];
	// ��ͼ������ݿ�����pBmpOriginalData ��
	if (::GetDIBits(memDC.m_hDC, bmp, 0, bitmap.bmHeight,
		pBmpOriginData, m_pBitMapInfo, DIB_RGB_COLORS) == 0)
	{
		AfxMessageBox(_T("GetDIBits Error"));
		this->DeletepBitMapInfo();
		this->DeletepBmpOriginData(pBmpOriginData);
		return;
	}

	// ѹ��ͼ������
	this->CompressBmpData(pBmpOriginData);
	this->DeletepBmpOriginData(pBmpOriginData);
	DeleteDC(memDC);
	DeleteObject(bmp);
}

/******************************************************************
Function	: DeletepBmpOriginData
Date		: 2015-05-09 21:23:51
Author		: xiaoheike
Parameter	: pBmpOriginData--��Ҫ���ͷŵ�ָ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CScreenMonitor::DeletepBmpOriginData(BYTE* pBmpOriginData)
{
	if (pBmpOriginData != NULL)
	{
		delete[] pBmpOriginData;
		pBmpOriginData = NULL;
	}
}

/******************************************************************
Function	: GetDeskScreenDC
Date		: 2015-05-09 21:25:28
Author		: xiaoheike
Parameter	: memDC--CDC��  bmp--λͼ   bitmap--λͼ�ṹ��
Return		: void
Desc		: �������ͼ��
******************************************************************/
void CScreenMonitor::GetDeskScreenDC(CDC &memDC, CBitmap &bmp, BITMAP &bitmap)
{
	CDC* pDeskDC = CWnd::GetDesktopWindow()->GetDC(); //��ȡ���滭������
	int width = GetSystemMetrics(SM_CXSCREEN); //��ȡ��Ļ�Ŀ��
	int height = GetSystemMetrics(SM_CYSCREEN); //��ȡ��Ļ�ĸ߶�

	memDC.CreateCompatibleDC(pDeskDC); //����һ�����ݵĻ���

	bmp.CreateCompatibleBitmap(pDeskDC, width, height); //��������λͼ
	memDC.SelectObject(&bmp); //ѡ��λͼ����

	bmp.GetBitmap(&bitmap);
	memDC.BitBlt(0, 0, width, height, pDeskDC, 0, 0, SRCCOPY);
	m_imgTotalSize = bitmap.bmWidthBytes * bitmap.bmHeight;
	InitBITMAPINFO(bitmap, height, width);

	pDeskDC->DeleteDC();
}

/******************************************************************
Function	: SetCursorIcon
Date		: 2015-05-09 21:27:16
Author		: xiaoheike
Parameter	: memDC--CDC��
Return		: void
Desc		: ��������Ϣ�����Ƶ�����ͼ����
******************************************************************/
void CScreenMonitor::SetCursorIcon(CDC &memDC)
{
	//��ȡ��ǰ��꼰��λ��
	HCURSOR hCursor = GetCursor();
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	//��ȡ����ͼ������
	ICONINFO IconInfo;
	if (GetIconInfo(hCursor, &IconInfo))
	{
		ptCursor.x -= ((int)IconInfo.xHotspot);
		ptCursor.y -= ((int)IconInfo.yHotspot);
		if (IconInfo.hbmMask != NULL)
			DeleteObject(IconInfo.hbmMask);
		if (IconInfo.hbmColor != NULL)
			DeleteObject(IconInfo.hbmColor);
	}
	//�ڼ����豸�������ϻ����ù��
	DrawIconEx(
		memDC.m_hDC,         // handle to device context
		ptCursor.x, ptCursor.y,
		hCursor,         // handle to icon to draw
		0, 0,          // width of the icon
		0,           // index of frame in animated cursor
		NULL,          // handle to background brush
		DI_NORMAL | DI_COMPAT      // icon-drawing flags
		);
}

/******************************************************************
Function	: SendScreenBmpData
Date		: 2015-05-09 21:28:07
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ������Ļͼ�����ݵ���ʦ��
******************************************************************/
void CScreenMonitor::SendScreenBmpData()
{
	BMPDATA bmpData;
	int count = this->InitBmpData(bmpData);
	for (int i = 0; i < count; i++)
	{
		// ֪ͨ��ʦ��׼��������Ļͼ������
		m_socketCenter.SendReadyInfo(m_socketMsg, BEGINSCREENMONITOR);

		SetBmpData(bmpData, i, count);

		// ����ͼ�����ݵ���ʦ��
		m_socketCenter.SendDataTCP(m_socketMsg, (char*)&bmpData, sizeof(BMPDATA));
	}
}

/******************************************************************
Function	: SetBmpData
Date		: 2015-05-09 21:29:46
Author		: xiaoheike
Parameter	: bmpData--��Ҫ����ֵ��BMPDATA �ṹ��  i--һ������ͼ��ĵڼ��δ���
count--һ������ͼ����Ҫ������Ĵ���
Return		: void
Desc		: ����Ļͼ�����ݸ�ֵ��BMPDATA �ṹ����
******************************************************************/
void CScreenMonitor::SetBmpData(BMPDATA &bmpData, int i, int count)
{
	UINT beginPos = 0;
	memset(bmpData.screenBmpData, 0, SCREEN_TRANS_SIZE);
	beginPos = i * SCREEN_TRANS_SIZE;
	bmpData.beginPos = beginPos;
	// ����һ����Ļͼ������һ��
	if (i == count - 1)
	{
		bmpData.isShow = true;
		bmpData.infoType = 2;
		uLongf last = m_compressBmpDataLen - beginPos;
		memcpy_s(bmpData.screenBmpData, last,
			m_pBmpTransData + beginPos, last);
	}
	else
	{
		memcpy_s(bmpData.screenBmpData, SCREEN_TRANS_SIZE,
			m_pBmpTransData + beginPos, SCREEN_TRANS_SIZE);
	}
}

/******************************************************************
Function	: InitBmpData
Date		: 2015-05-09 21:32:51
Author		: xiaoheike
Parameter	: bmpData--����ͼ�����ݵĽṹ��
Return		: int--һ������ͼ����Ҫ������Ĵ���
Desc		: Ϊ����ͼ��ṹ��BMPDATA��ʼ��һЩ��Ҫ��Ϣ
******************************************************************/
int CScreenMonitor::InitBmpData(BMPDATA &bmpData)
{
	memset(&bmpData, 0, sizeof(BMPDATA));
	// infoType = 1 ���͵Ĳ���һ������ͼ������һ��
	// infoType = 2 ���͵���һ������ͼ������ؿ�
	bmpData.infoType = 1;
	int count = int(ceil(double(m_compressBmpDataLen) / SCREEN_TRANS_SIZE));

	memcpy(&bmpData.bmpHeadInfo, m_pBitMapInfo, m_bmpHeadTotalSize);
	bmpData.bmpCompressSize = m_compressBmpDataLen;

	bmpData.isShow = false;
	return count;
}

/******************************************************************
Function	: SetIsScreenMonitorEnd
Date		: 2015-05-09 21:34:35
Author		: xiaoheike
Parameter	: isScreenMonitorEnd
Return		: void
Desc		: ����ͼ�����ݵ���ʦ�����߳��Ƿ�����ı�־
isScreenMonitorEnd--true�����̣߳�falseֹͣ�߳�
******************************************************************/
void CScreenMonitor::SetIsScreenMonitorEnd(bool isScreenMonitorEnd)
{
	//	m_isInitBITMAPINFO = true;
	m_isScreenMonitorEnd = isScreenMonitorEnd;
}

/******************************************************************
Function	: SendScreenData
Date		: 2015-05-09 21:35:48
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���Ϸ������ݵ���ʦ��
******************************************************************/
void CScreenMonitor::SendScreenData()
{
	while (true == m_isScreenMonitorEnd)
	{
		this->GetDeskScreeData();
		SendScreenBmpData();
		Sleep(300);
	}
	//	delete this;
	CleanData();
}

/******************************************************************
Function	: CompressBmpData
Date		: 2015-05-09 21:37:17
Author		: xiaoheike
Parameter	: pBmpOriginalData--����ͼ�������
Return		: void
Desc		: ʹ��ZLIB ��ѹ������ͼ������
******************************************************************/
void CScreenMonitor::CompressBmpData(BYTE* pBmpOriginalData)
{
	// ��Ҫһ���㹻��Ŀռ�
	m_compressBmpDataLen = (uLongf)((m_imgTotalSize + 12)*(100.1 / 100)) + 1;

	BYTE* pCompressData = new BYTE[m_compressBmpDataLen];
	// ������ͼ������ѹ����pCompressData ��
	int err = compress(pCompressData, &m_compressBmpDataLen,
		pBmpOriginalData, m_imgTotalSize);

	if (err != Z_OK) {
		exit(1);
	}
	if (m_pBmpTransData != NULL)
	{
		delete[] m_pBmpTransData;
		m_pBmpTransData = NULL;
	}

	// ��ѹ��������ݱ��浽m_pBmpTransData ��
	m_pBmpTransData = new BYTE[m_compressBmpDataLen];
	memcpy(m_pBmpTransData, pCompressData, m_compressBmpDataLen);
	delete[] pCompressData;
	pCompressData = NULL;
}

/******************************************************************
Function	: InitBITMAPINFO
Date		: 2015-05-09 21:40:20
Author		: xiaoheike
Parameter	: bitmap--ͼ��Ĵ�С����Ϣ  height--����ĸ�
width--����Ŀ�
Return		: void
Desc		: ��ʼ��ͼ�����Ϣͷ�ṹ�� BITMAPINFO
******************************************************************/
void CScreenMonitor::InitBITMAPINFO(BITMAP &bitmap, int height, int width)
{
	double paletteSize = 0; //��¼��ɫ���С
	if (bitmap.bmBitsPixel < 16) //�ж��Ƿ�Ϊ���ɫλͼ
	{
		paletteSize = (1 << bitmap.bmBitsPixel)*sizeof(RGBQUAD);
	}
	m_bmpHeadTotalSize = (int)paletteSize + sizeof(BITMAPINFO);

	m_pBitMapInfo = (BITMAPINFO*)LocalAlloc(LPTR, m_bmpHeadTotalSize);
	m_pBitMapInfo->bmiHeader.biBitCount = bitmap.bmBitsPixel;
	m_pBitMapInfo->bmiHeader.biClrImportant = 0;
	m_pBitMapInfo->bmiHeader.biCompression = 0;
	m_pBitMapInfo->bmiHeader.biHeight = height;
	m_pBitMapInfo->bmiHeader.biPlanes = bitmap.bmPlanes;
	m_pBitMapInfo->bmiHeader.biSize = m_bmpHeadTotalSize;//sizeof(BITMAPINFO);
	m_pBitMapInfo->bmiHeader.biSizeImage = m_imgTotalSize;
	m_pBitMapInfo->bmiHeader.biWidth = width;
	m_pBitMapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitMapInfo->bmiHeader.biYPelsPerMeter = 0;
}