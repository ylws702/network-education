// ScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenMonitorDlg.h"
#include "afxdialogex.h"
//#include "ItemData.h"

// CScreenDlg �Ի���

IMPLEMENT_DYNAMIC(CScreenMonitorDlg, CDialogEx)

CScreenMonitorDlg::CScreenMonitorDlg(CWnd* pParent /*=NULL*/, int widthCount /*=0*/)
: CDialogEx(CScreenMonitorDlg::IDD, pParent)
, m_widthCount(widthCount)
, m_pBitMapInfo(NULL)
, m_pBmpCompressData(NULL)
{
}

CScreenMonitorDlg::~CScreenMonitorDlg()
{
	CleanData();
}

void CScreenMonitorDlg::CleanData()
{
	// 	if (m_screenSocket != INVALID_SOCKET)
	// 	{
	// 		closesocket(m_screenSocket);
	// 		m_screenSocket = NULL;
	// 	}
	DeletepBmpCompressData();
	DeletepBitMapInfo();
}

/******************************************************************
Function	: DeletepBitMapInfo
Date		: 2015-05-12 10:04:05
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CScreenMonitorDlg::DeletepBitMapInfo()
{
	if (m_pBitMapInfo != NULL)
	{
		LocalFree(m_pBitMapInfo);
		m_pBitMapInfo = NULL;
	}
}

/******************************************************************
Function	: DeletepBmpTransData
Date		: 2015-05-12 10:04:21
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CScreenMonitorDlg::DeletepBmpCompressData()
{
	if (m_pBmpCompressData != NULL)
	{
		delete[] m_pBmpCompressData;
		m_pBmpCompressData = NULL;
	}
}

void CScreenMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenMonitorDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CScreenDlg ��Ϣ�������

void CScreenMonitorDlg::OnClose()
{
	// ������Ϣ��TeacherDlg �������ͽ�����Ļ��ص���Ϣ��ѧ����
	::SendMessage(m_pParentWnd->m_hWnd, ID_MONITOREND, 0, 0);
	DestroyWindow();
	CDialogEx::OnClose();
}

// void CScreenMonitorDlg::SetScreenTranEnd(bool screenMonitorEnd)
// {
// 	m_isScreenMonitorEnd = screenMonitorEnd;
// }

BOOL CScreenMonitorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	// ����ESC ��ENTER ��
	if (WM_KEYDOWN == pMsg->message)
	{
		int nVirtKey = (int)pMsg->wParam;
		if (nVirtKey == VK_RETURN || nVirtKey == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CScreenMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	//	CDialogEx::OnSize(nType, cx, cy);
	CDialogEx::OnSize(nType, cx, cy);

	//	GetClientRect(&m_rect);
	this->GetClientRect(&m_rect);
	m_blockWidth = m_rect.Width() / m_widthCount;
	m_blockHeight = m_rect.Height() / m_widthCount;
}

// void CScreenMonitorDlg::SetScreenData() //��ʾͼ��
// {
//
// }

/*
	���ܣ���ʾѧ����������
	Input:
	bmp--�ṹ�壬��߱�����ѧ���������ͼ������
	itemOrder--ѧ������˳��ȡֵΪ0-15������������
	Output:

	*/
void CScreenMonitorDlg::ShowBmp(BMP* bmp, int itemOrder)
{
	m_pBmpCompressData = UnCompressData(bmp->pBMPINFO->bmiHeader.biSizeImage,
		bmp->bmpCompressSize, bmp->pBmpCompressData);

	int xDest = 0;
	int yDest = 0;
	DetermineShowRegion(itemOrder, xDest, yDest);

	HDC hdc = GetDC()->m_hDC;
	if (hdc != NULL)
	{
		::StretchDIBits(hdc,
			xDest,
			yDest,
			m_blockWidth,
			m_blockHeight,
			0,
			0,
			bmp->pBMPINFO->bmiHeader.biWidth,
			bmp->pBMPINFO->bmiHeader.biHeight,
			m_pBmpCompressData, //λͼ����
			bmp->pBMPINFO, //BITMAPINFO λͼ��Ϣͷ
			DIB_RGB_COLORS,
			SRCCOPY
			);
	}

	// 	for (uLongf i = 0; i < bmp->bmpCompressSize; i++)
	// 	{
	// 		delete bmp->pBmpCompressData[i];
	// 	}
	// 	BYTE* temp1 = bmp->pBmpCompressData;
	// 	delete[] temp1;
	// 	temp1 = NULL;
	//
	// 	BITMAPINFO* temp2 = bmp->pBMPINFO;
	// 	LocalFree(temp2);
	// 	temp2 = NULL;
	// 	m_pBitMapInfo = (BITMAPINFO*)LocalAlloc(LPTR, bmp->pBMPINFO->bmiHeader.biSize);
	// 	memcpy_s(m_pBitMapInfo, bmp->pBMPINFO->bmiHeader.biSize,
	// 		bmp->pBMPINFO, bmp->pBMPINFO->bmiHeader.biSize);
	//	std::unique_lock<std::mutex> lock(m_mtx);
	// 	if (false == isLButtonUp)
	// 	{
	//		CleanData();
	// 	}
	//	lock.unlock();

	delete[] m_pBmpCompressData;
	m_pBmpCompressData = NULL;
}

void CScreenMonitorDlg::DetermineShowRegion(int itemOrder, int &xDest, int &yDest)
{
	// 	int blockWidth = m_rect.Width() / m_widthCount;
	// 	int blockHeight = m_rect.Height() / m_widthCount;
	switch (m_widthCount)
	{
	case 1:
		OneRegion(itemOrder, xDest, yDest);
		break;
	case 2:
		FourRegion(itemOrder, xDest, yDest);
		break;
	case 3:
		NineRegion(itemOrder, xDest, yDest);
		break;
	case 4:
		SixteenRegion(itemOrder, xDest, yDest);
		break;
	}
}

/******************************************************************
Function	: SixteenRegion
Date		: 2015-05-12 10:27:07
Author		: xiaoheike
Parameter	: itemOrder--ѧ�����ڶԻ�������ʾ��˳��ȡֵΪ0-15
			  xDest--��ʾ�ڶԻ�������Ͻǵ�x����
			  yDest--��ʾ�ڶԻ�������Ͻǵ�y����
Return		: void
Desc		: ͬʱ��ص�ѧ�������������ڻ����16̨
******************************************************************/
void CScreenMonitorDlg::SixteenRegion(int itemOrder, int &xDest, int &yDest)
{
	NineRegion(itemOrder, xDest, yDest);
	if (3 * m_widthCount <= itemOrder
		&& itemOrder < 4 * m_widthCount)
	{
		xDest = (itemOrder - 3 * m_widthCount) * m_blockWidth;
		yDest = 3 * m_blockHeight;
	}
}

/******************************************************************
Function	: NineRegion
Date		: 2015-05-12 10:30:05
Author		: xiaoheike
Parameter	: itemOrder--ѧ�����ڶԻ�������ʾ��˳��ȡֵΪ0-8
			  xDest--��ʾ�ڶԻ�������Ͻǵ�x����
			  yDest--��ʾ�ڶԻ�������Ͻǵ�y����
Return		: void
Desc		: ͬʱ��ص�ѧ�������������ڻ����9̨
******************************************************************/
void CScreenMonitorDlg::NineRegion(int itemOrder, int &xDest, int &yDest)
{
	FourRegion(itemOrder, xDest, yDest);
	if (2 * m_widthCount <= itemOrder
		&& itemOrder < 3 * m_widthCount)
	{
		xDest = (itemOrder - 2 * m_widthCount) * m_blockWidth;
		yDest = 2 * m_blockHeight;
	}
}

/******************************************************************
Function	: FourRegion
Date		: 2015-05-12 10:30:37
Author		: xiaoheike
Parameter	: itemOrder--ѧ�����ڶԻ�������ʾ��˳��ȡֵΪ0-3
			  xDest--��ʾ�ڶԻ�������Ͻǵ�x����
			  yDest--��ʾ�ڶԻ�������Ͻǵ�y����
Return		: void
Desc		: ͬʱ��ص�ѧ�������������ڻ����4̨
******************************************************************/
void CScreenMonitorDlg::FourRegion(int itemOrder, int &xDest, int &yDest)
{
	OneRegion(itemOrder, xDest, yDest);
	if (m_widthCount <= itemOrder
		&& itemOrder < 2 * m_widthCount)
	{
		xDest = (itemOrder - m_widthCount) * m_blockWidth;
		yDest = m_blockHeight;
	}
}

/******************************************************************
Function	: OneRegion
Date		: 2015-05-12 10:30:57
Author		: xiaoheike
Parameter	: itemOrder--ѧ�����ڶԻ�������ʾ��˳��ȡֵΪ0
			  xDest--��ʾ�ڶԻ�������Ͻǵ�x����
			  yDest--��ʾ�ڶԻ�������Ͻǵ�y����
Return		: void
Desc		: ͬʱ��ص�ѧ��������������1̨
******************************************************************/
void CScreenMonitorDlg::OneRegion(int itemOrder, int &xDest, int &yDest)
{
	if (itemOrder < m_widthCount)
	{
		xDest = itemOrder * m_blockWidth;
		yDest = 0;
	}
}

/******************************************************************
Function	: UnCompressData
Date		: 2015-05-12 10:35:47
Author		: xiaoheike
Parameter	: biSize--ѹ��ǰ��ͼ���С
			  ImagebmpCompressSize--ѹ�����ͼ���С
			  pBmpCompressData--ѹ����ͼ������
Return		: BYTE*
Desc		: ��ѹ��ѧ�������յ�����Ļͼ������
******************************************************************/
BYTE* CScreenMonitorDlg::UnCompressData(uLongf biSizeImage, uLongf bmpCompressSize, BYTE* pBmpCompressData)
{
	uLongf unCompressDataLen = (uLongf)((biSizeImage + 12)*(100.1 / 100)) + 1;
	BYTE* pUnCompressData = new BYTE[unCompressDataLen];
	int err = uncompress(pUnCompressData, &unCompressDataLen,
		pBmpCompressData, bmpCompressSize);
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

// BYTE* CScreenMonitorDlg::UnCompressData(uLongf biSizeImage, uLongf bmpCompressSize)
// {
// 	uLongf unCompressDataLen = (uLongf)((biSizeImage + 12)*(100.1 / 100)) + 1;
// 	BYTE* pUnCompressData = new BYTE[unCompressDataLen];
// 	int err = uncompress(pUnCompressData, &unCompressDataLen,
// 		m_pBmpCompressData, bmpCompressSize);
// 	if (err != Z_OK)
// 	{
// 		CString str;
// 		str.Format(_T("uncompess error: %d"), err);
// 		MessageBox(str);
// 		exit(1);
// 	}
// 
// 	BYTE* bmpShowData = new BYTE[unCompressDataLen];
// 	memcpy(bmpShowData, pUnCompressData, unCompressDataLen);
// 
// 	delete[] pUnCompressData;
// 	pUnCompressData = NULL;
// 
// 	return bmpShowData;
// }

BOOL CScreenMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CScreenMonitorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nSBCode != SB_ENDSCROLL)
	{
		SCROLLINFO hStructure;
		GetScrollInfo(SB_HORZ, &hStructure);
		hStructure.nPos = nPos;
		SetScrollInfo(SB_HORZ, &hStructure);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CScreenMonitorDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nSBCode != SB_ENDSCROLL)
	{
		SCROLLINFO vStructure;
		GetScrollInfo(SB_VERT, &vStructure);
		vStructure.nPos = nPos;
		SetScrollInfo(SB_VERT, &vStructure);
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

/******************************************************************
Function	: FourRegionClk
Date		: 2015-05-12 10:13:17
Author		: xiaoheike
Parameter	: point--�������
			  itemOrder--ѧ������ʾ��˳��0-3
Return		: void
Desc		: ͬʱ��ص�ѧ�������������ڵ���4̨���ж�ѧ���������
���ĸ�ѧ����
******************************************************************/
void CScreenMonitorDlg::FourRegionClk(CPoint point, int& itemOrder)
{
	if (0 < point.x && point.x < m_blockWidth)
	{
		FourRegionClk(point, itemOrder, 0);
	}
	else if (m_blockWidth < point.x && point.x < 2 * m_blockWidth)
	{
		FourRegionClk(point, itemOrder, 1);
	}
}

/******************************************************************
Function	: NineRegionClk
Date		: 2015-05-12 10:15:41
Author		: xiaoheike
Parameter	: point--�������
			  itemOrder--ѧ������ʾ��˳��0-8
Return		: void
Desc		: ͬʱ��ص�ѧ�������������ڵ���9̨���ж�
��������ĸ�ѧ����
******************************************************************/
void CScreenMonitorDlg::NineRegionClk(CPoint point, int& itemOrder)
{
	if (0 < point.x && point.x < m_blockWidth)
	{
		NineRegionClk(point, itemOrder, 0);
	}
	else if (m_blockWidth < point.x && point.x < 2 * m_blockWidth)
	{
		NineRegionClk(point, itemOrder, 1);
	}
	else if (2 * m_blockWidth < point.x && point.x < 3 * m_blockWidth)
	{
		NineRegionClk(point, itemOrder, 2);
	}
}

/******************************************************************
Function	: SixteenRegionClk
Date		: 2015-05-12 10:16:53
Author		: xiaoheike
Parameter	: point--�������
			  itemOrder--ѧ������ʾ��˳��0-15
Return		: void
Desc		: ͬʱ��ص�ѧ�������������ڵ���16̨���ж�
��������ĸ�ѧ����
******************************************************************/
void CScreenMonitorDlg::SixteenRegionClk(CPoint point, int& itemOrder)
{
	if (0 < point.x && point.x < m_blockWidth)
	{
		SixteenRegionClk(point, itemOrder, 0);
	}
	else if (m_blockWidth < point.x && point.x < 2 * m_blockWidth)
	{
		SixteenRegionClk(point, itemOrder, 1);
	}
	else if (2 * m_blockWidth < point.x && point.x < 3 * m_blockWidth)
	{
		SixteenRegionClk(point, itemOrder, 2);
	}
	else if (3 * m_blockWidth < point.x && point.y < 4 * m_blockWidth)
	{
		SixteenRegionClk(point, itemOrder, 3);
	}
}

/******************************************************************
Function	: FourRegionClk
Date		: 2015-05-12 10:17:35
Author		: xiaoheike
Parameter	: point--�������λ��
			  itemOrder--ѧ�����ڶԻ�������ʾ��˳��
			  offset--ƫ�Ƶ�һ�е�λ��ȡֵΪ0��1
Return		: void
Desc		: ��offset = 0�������ѧ������0��2��
��offset = 1 �����ѧ������1 ��3
******************************************************************/
void CScreenMonitorDlg::FourRegionClk(CPoint point, int& itemOrder, int offset)
{
	if (0 < point.y && point.y < m_blockHeight)
	{
		itemOrder = 0 * m_widthCount + offset;
	}
	else if (m_blockHeight < point.y && point.y < 2 * m_blockHeight)
	{
		itemOrder = 1 * m_widthCount + offset;
	}
}

/******************************************************************
Function	: NineRegionClk
Date		: 2015-05-12 10:20:26
Author		: xiaoheike
Parameter	: point--�������λ��
			  itemOrder--ѧ�����ڶԻ�������ʾ��˳��
			  offset--ƫ�Ƶ�һ�е�λ��ȡֵΪ0��1��2
Return		: void
Desc		: ��offset = 0�������ѧ������0��3��6��
��offset = 1 �����ѧ������1��4��7
��offset = 2 �����ѧ������2��5��8
******************************************************************/
void CScreenMonitorDlg::NineRegionClk(CPoint point, int& itemOrder, int offset)
{
	FourRegionClk(point, itemOrder, offset);
	if (2 * m_blockHeight < point.y && point.y < 3 * m_blockHeight)
	{
		itemOrder = 2 * m_widthCount + offset;
	}
}

/******************************************************************
Function	: SixteenRegionClk
Date		: 2015-05-12 10:31:52
Author		: xiaoheike
Parameter	: point--�������λ��
			  itemOrder--ѧ�����ڶԻ�������ʾ��˳��
			  offset--ƫ�Ƶ�һ�е�λ��ȡֵΪ0��1��2
Return		: void
Desc		: ��offset = 0�������ѧ������0��4��8��12��
��offset = 1 �����ѧ������1��5��9��13
��offset = 2 �����ѧ������2��6��10��14
��offset = 3 �����ѧ������3��7��11��15
******************************************************************/
void CScreenMonitorDlg::SixteenRegionClk(CPoint point, int& itemOrder, int offset)
{
	NineRegionClk(point, itemOrder, offset);
	if (3 * m_blockHeight < point.y && 4 * m_blockHeight)
	{
		itemOrder = 3 * m_widthCount + offset;
	}
}

/******************************************************************
Function	: OnLButtonDblClk
Date		: 2015-05-12 09:47:11
Author		: xiaoheike
Parameter	: nFlags       point
Return		: void
Desc		: ˫����Ϣ����˫��ѧ����ʵ��1��1��Ļ���
******************************************************************/
void CScreenMonitorDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int itemOrder = 0;
	switch (m_widthCount)
	{
	case 1:
		break;
	case 2:
		FourRegionClk(point, itemOrder);
		break;
	case 3:
		NineRegionClk(point, itemOrder);
		break;
	case 4:
		SixteenRegionClk(point, itemOrder);
		break;
	default:
		break;
	}
	if (m_widthCount != 1)
	{
		// ������Ϣ��TeacherDlg �Ի��������������������
		::SendMessage(m_pParentWnd->m_hWnd, ID_ONESTUINSCREEN, 0, (LPARAM)&itemOrder);
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}