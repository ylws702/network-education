
// StudentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Student.h"
#include "StudentDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudentDlg �Ի���



CStudentDlg::CStudentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStudentDlg::IDD, pParent)
	, m_stuName(_T("���ʽ�"))
	, m_stuClass(_T("������"))
	, m_stuID(_T("20112798"))
	, m_stuMajor(_T("�������"))
	, m_pCtrlCenter(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDINAME, m_stuName);
	DDX_Text(pDX, IDC_EDTCLASS, m_stuClass);
	DDX_Text(pDX, IDC_EDTID, m_stuID);
	DDX_Text(pDX, IDC_EDTMAJOR, m_stuMajor);
}

BEGIN_MESSAGE_MAP(CStudentDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CStudentDlg::OnBnClickedOk)
	ON_MESSAGE(WM_SHOWTASKICON, &CStudentDlg::OnShowTaskIcon)
//	ON_COMMAND(IDM_SHOW_WINDOW, &CStudentDlg::OnShowDlg)
	ON_BN_CLICKED(IDCANCEL, &CStudentDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CStudentDlg ��Ϣ�������

BOOL CStudentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	// ��������ʾͼ��
	InitNofIconData();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStudentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStudentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStudentDlg::OnBnClickedOk()
{

	if (false == IsEdtDataEmpty())
	{
//		IsEdtDataEmpty();
		// ���ش���
		ShowWindow(SW_HIDE);
		STUINFODATA stuInfoData;
		wcscpy_s(stuInfoData.m_stuName, m_stuName);
		wcscpy_s(stuInfoData.m_stuID, m_stuID);
		wcscpy_s(stuInfoData.m_stuMajor, m_stuMajor);
		wcscpy_s(stuInfoData.m_stuClass, m_stuClass);

		m_pCtrlCenter = new CControlCenter(stuInfoData);
		// ����һ���߳����ڼ�����ʦ��������
		::CloseHandle(CreateThread(NULL, 0, OnBeginServer, (LPVOID)m_pCtrlCenter, 0, 0));
	}

}

DWORD WINAPI CStudentDlg::OnBeginServer(LPVOID self)
{
	CControlCenter* pCtrlCenter = (CControlCenter*)self;
	pCtrlCenter->SetConnectInfo(CONNECT_IP, CONNECT_PORT);
	pCtrlCenter->ConnectClient();
//	pCtrlCenter->RecvRequest();
 	delete pCtrlCenter;
	pCtrlCenter = NULL;
	return 0;
}


void CStudentDlg::InitNofIconData()
{
	m_notifyIconData.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_notifyIconData.hWnd = m_hWnd;
	m_notifyIconData.uID = IDI_TASKBARICON;
	m_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_notifyIconData.uCallbackMessage = WM_SHOWTASKICON; // �Զ������̲˵���Ϣ
	m_notifyIconData.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_TASKBARICON));
	wcscpy_s(m_notifyIconData.szTip, _T("���ӽ���ϵͳ"));
	Shell_NotifyIcon(NIM_ADD, &m_notifyIconData); // �����������ͼ��
}

LRESULT CStudentDlg::OnShowTaskIcon(WPARAM wParam, LPARAM lParam)
{
	// �ж�ͼ��ID�Ƿ����
	if (wParam != IDI_TASKBARICON) 
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP:  // ������Ҽ�
	{
		CPoint point;
		GetCursorPos(&point);
		CMenu menu;
		menu.CreatePopupMenu();
//		menu.AppendMenu(MF_STRING, IDM_SHOW_WINDOW, _T("��ʾ"));
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�˳�"));
		SetForegroundWindow();  // ����˵�ʧȥ���㲻��ʧ��BUG
		menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		menu.DestroyMenu();
	}
		break;
// 	case WM_LBUTTONDBLCLK: // ˫��������
// 		OnShowDlg();
	}
	return 0;
}

// void CStudentDlg::OnShowDlg()
// {
// 	if (IsWindowVisible())
// 	{
// 		OnBnClickedCancel();
// 
// // 		SendMessage(WM_SYSCOMMAND, SC_RESTORE, (LPARAM)m_hWnd);
// 	}
// 	else
// 		ShowWindow(SW_SHOW);
// }

bool CStudentDlg::IsEdtDataEmpty()
{
	bool isEmpty =  false;
	UpdateData(TRUE);
// 	isEmpty = false;
	CString warmStr(_T(""));
	if (m_stuName.IsEmpty())
	{
		warmStr += _T("��������Ϊ��");
		isEmpty = true;
	}
	if (m_stuID.IsEmpty())
	{
		warmStr += _T("ѧ�Ų���Ϊ��");
		isEmpty = true;
	}
	if (m_stuMajor.IsEmpty())
	{
		warmStr += _T("רҵ����Ϊ��");
		isEmpty = true;
	}
	if (m_stuClass.IsEmpty())
	{
		warmStr += _T("�༶����Ϊ��");
		isEmpty = true;
	}
	if (true == isEmpty)
	{
		MessageBoxW(warmStr);
	}
	return isEmpty;
}


void CStudentDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pCtrlCenter != NULL)
	{
		delete m_pCtrlCenter;
		m_pCtrlCenter = NULL;
	}
	Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData); // �����������ͼ��
	CDialogEx::OnCancel();
}


void CStudentDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������
	Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData); // �����������ͼ��
}
