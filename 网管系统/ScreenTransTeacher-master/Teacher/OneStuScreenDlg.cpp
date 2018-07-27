// OneStuScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Teacher.h"
#include "OneStuScreenDlg.h"
#include "afxdialogex.h"


// COneStuScreenDlg �Ի���

IMPLEMENT_DYNAMIC(COneStuScreenDlg, CDialogEx)

COneStuScreenDlg::COneStuScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COneStuScreenDlg::IDD, pParent)
{

}

COneStuScreenDlg::~COneStuScreenDlg()
{
}

void COneStuScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COneStuScreenDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// COneStuScreenDlg ��Ϣ�������
void COneStuScreenDlg::ShowBmp(BMP* bmp, int itemOrder)
{
	m_itemOrder = itemOrder;
	BYTE* bmpShowData = UnCompressData(bmp->pBMPINFO->bmiHeader.biSizeImage,
		bmp->bmpCompressSize, bmp->pBmpCompressData);

	HDC hdc = GetDC()->m_hDC;
	if (hdc != NULL)
	{
		::StretchDIBits(hdc,
			0,
			0,
			m_clientRect.Width(),
			m_clientRect.Height(),
			0,
			0,
			bmp->pBMPINFO->bmiHeader.biWidth,
			bmp->pBMPINFO->bmiHeader.biHeight,
			bmpShowData, //λͼ����
			bmp->pBMPINFO, //BITMAPINFO λͼ��Ϣͷ
			DIB_RGB_COLORS,
			SRCCOPY
			);
	}


	delete[] bmpShowData;
	bmpShowData = NULL;
}

void COneStuScreenDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	this->GetClientRect(m_clientRect);
}


BYTE* COneStuScreenDlg::UnCompressData(uLongf biSizeImage, uLongf bmpCompressSize, BYTE* pBmpCompressData)
{
	uLongf unCompressDataLen = (uLongf)((biSizeImage + 12)*(100.1 / 100)) + 1;
	BYTE* pUnCompressData = new BYTE[unCompressDataLen];
	int err = uncompress(pUnCompressData, &unCompressDataLen,
		pBmpCompressData, bmpCompressSize);
	if (err != Z_OK)
	{
		CString str;
		str.Format(_T("uncompess error: %d"), err);
		MessageBox(str, _T("OneStuScreenDlg"));
		exit(1);
	}

	BYTE* bmpShowData = new BYTE[unCompressDataLen];
	memcpy(bmpShowData, pUnCompressData, unCompressDataLen);

	delete[] pUnCompressData;
	pUnCompressData = NULL;

	return bmpShowData;
}

void COneStuScreenDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SendMessage(m_pParentWnd->m_hWnd, ID_ONESTUINSCREENEND, 0, (LPARAM)&m_itemOrder);
	CDialogEx::OnClose();
}
