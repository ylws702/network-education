/******************************************************************* 
FileName: TeacherDlg.cpp
Author	: xiaoheike
Date	: 2015-05-11 15:13:57
Desc	: ��ʦ�������Ի�������һ����תվ�������Լ����ݾ��������ת
������������
*******************************************************************/ 
#include "stdafx.h"
#include "Teacher.h"
#include "TeacherDlg.h"
#include "afxdialogex.h"
#include "ItemData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTeacherDlg �Ի���



CTeacherDlg::CTeacherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeacherDlg::IDD, pParent)
	, m_socketMsg(INVALID_SOCKET)
	, m_pScreenDlg(NULL)
	, m_oneStuScreenDlg(NULL)
	, m_pMulticast(NULL)
	, m_isStopMulticast(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_item, -1, sizeof(int)*MAX_MONITOR);
}

void CTeacherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CTeacherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(ID_STUDENTLONGIN, OnAddStudent)
	ON_MESSAGE(ID_STUDENTLOGINOUT, OnLoginOutStudent)
	ON_MESSAGE(ID_SETBMPDATA, OnSetBmpCompressData)
	ON_MESSAGE(ID_MONITOREND, OnEndMonitor)
	ON_MESSAGE(ID_ONESTUINSCREEN, OnCreatOneStudentDlg)
	ON_MESSAGE(ID_SETONESTUBMPDATA, OnSetOneStuBmpCompressData)
	ON_MESSAGE(ID_ONESTUINSCREENEND, OnEndOneStudentMonitor)
	ON_COMMAND(ID_MULTICAST, OnMulticast)
	ON_COMMAND(ID_SCREENMONITOR, OnBeginScreenMonitor)
ON_WM_DESTROY()
ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTeacherDlg::OnNMClickList1)
ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTeacherDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// CTeacherDlg ��Ϣ�������

BOOL CTeacherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	SetMainDlgSytle(LISTEN_PORT);
	// ����һ���߳����ڼ������������SOCKET
	::CloseHandle(CreateThread(0, 0, OnMsgListen, (LPVOID)this, 0, NULL));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CTeacherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CTeacherDlg::OnPaint()
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
HCURSOR CTeacherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/******************************************************************
Function	: OnMsgListen
Date		: 2015-05-11 15:15:25
Author		: xiaoheike
Parameter	: self--CTeacherDlg��
Return		: DWORD WINAPI
Desc		: ����ѧ������¼�̵߳����з���
******************************************************************/
DWORD WINAPI CTeacherDlg::OnMsgListen(LPVOID self)
{
	CTeacherDlg* teacherDlg = (CTeacherDlg*)self;
	teacherDlg->ListenStudentLogin();
	return 0;
}

/******************************************************************
Function	: ListenStudentLogin
Date		: 2015-05-11 15:16:18
Author		: xiaoheike
Parameter	: ��
Return		: bool
Desc		: ����ѧ����¼������Ψһ��ID
******************************************************************/
bool CTeacherDlg::ListenStudentLogin()
{
	CSocketCenter mySocket;
	mySocket.InitSocketInfo(LISTEN_PORT, MAX_LISTEN);
	//	SOCKET m_servSocket;
	SOCKADDR_IN Seraddr;
	int Seraddrsize = sizeof(Seraddr);

	while (true)
	{
		m_socketMsg = accept(mySocket.GetSocketInit(), (sockaddr *)&Seraddr, &Seraddrsize);
		if (m_socketMsg != INVALID_SOCKET)
		{
			SetLoginStudentID();
		}
	}
	return true;
}

/******************************************************************
Function	: SetLoginStudentID
Date		: 2015-05-11 15:19:50
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ����¼ѧ��������Ψһ��ID
******************************************************************/
void CTeacherDlg::SetLoginStudentID()
{
	int id;
	CItemData *itemData = NULL;
	int Count = m_list.GetItemCount();
	if (Count == 0)
	{
		id = 0;
	}
	else
	{
		id = 0;
		for (int i = 0; i < Count; i++)
		{
			itemData = (CItemData*)m_list.GetItemData(i);
			if (itemData->m_id == id)
			{
				i = 0;
				id = id + 1;
				continue;
			}
		}
	}
	itemData = new CItemData(id, m_hWnd, m_socketMsg);
	// ������Ϣ���߳�����
	itemData->Run();
}

/******************************************************************
Function	: SetMainDlgSytle
Date		: 2015-05-11 15:20:51
Author		: xiaoheike
Parameter	: port--����ѧ����¼�Ķ˿ں�
Return		: void
Desc		: ���ý�ʦ����������
******************************************************************/
void CTeacherDlg::SetMainDlgSytle(int port)
{
	LoadBitBmp();

	CreatToolBar();

	CreateStatusBar(port);

	InsertColumn();

	GetClientRect(&m_rect);
}


/******************************************************************
Function	: InsertColumn
Date		: 2015-05-11 15:29:24
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ΪListControl ������ͷ
******************************************************************/
void CTeacherDlg::InsertColumn()
{
	// ����List Control ���Ϊ����ѡ�У����񣬴��и�ѡ�򣨿ؼ���View����ֵҪ����ΪReport��
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// ��List Control �����
	m_list.InsertColumn(0, _T("����"), LVCFMT_CENTER, 180);
	m_list.InsertColumn(1, _T("����"), LVCFMT_CENTER, 180);
	m_list.InsertColumn(2, _T("ѧ��"), LVCFMT_CENTER, 180);
	m_list.InsertColumn(3, _T("רҵ"), LVCFMT_CENTER, 180);
	m_list.InsertColumn(4, _T("�༶"), LVCFMT_CENTER, 180);
	m_list.DeleteColumn(0);
}

/******************************************************************
Function	: CreateStatusBar
Date		: 2015-05-11 15:30:17
Author		: xiaoheike
Parameter	: port--����ѧ����¼�Ķ˿ں�
Return		: void
Desc		: ����״̬��
******************************************************************/
void CTeacherDlg::CreateStatusBar(int port)
{
	// �ײ�״̬���Ĵ���
	BOOL hbar = m_statusBar.Create(this);
	// ״̬����ID
	UINT b[2] = { 1009, 1010 };
	hbar = m_statusBar.SetIndicators(b, 2);
	m_statusBar.SetPaneInfo(0, b[0], SBPS_NORMAL, 400);
	m_statusBar.SetPaneInfo(1, b[1], SBPS_NORMAL, 180);
	CString ListemPort;
	ListemPort.Format(_T("�����˿ڣ�%d"), port);
	m_statusBar.SetPaneText(0, ListemPort);
	m_statusBar.SetPaneText(1, _T("����������0 ̨"));
	// ��ʾ��������״̬��
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

/******************************************************************
Function	: CreatToolBar
Date		: 2015-05-11 15:31:02
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ����������
******************************************************************/
void CTeacherDlg::CreatToolBar()
{
	// ����ÿһ����������ID��֮��Ҫ�����Ϣ��Ӧ
	const UINT t[2] = { 1001, 1002 };
	m_toolBar.CreateEx(this);
	m_toolBar.SetButtons(t, 2);
	m_toolBar.SetSizes(CSize(60, 56), CSize(24, 24));
	// �������������
	m_toolBar.SetButtonText(0, _T("��ĻͶӰ"));
	m_toolBar.SetButtonText(1, _T("��Ļ���"));

	// ���������ͼƬ
	m_toolBar.GetToolBarCtrl().SetImageList(&m_imagelist);
}

/******************************************************************
Function	: LoadBitBmp
Date		: 2015-05-11 15:32:49
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ����λͼ��Դ
******************************************************************/
void CTeacherDlg::LoadBitBmp()
{
	m_imagelist.Create(32, 32, ILC_COLOR24 | ILC_MASK, 1, 1);
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_MULTICAST);
	m_imagelist.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmapW(IDB_SCREEN);
	m_imagelist.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
}

/******************************************************************
Function	: OnAddStudent
Date		: 2015-05-11 15:40:25
Author		: xiaoheike
Parameter	: wparam--��Ӧ��Ϣʱ�������ֵ��������CItemData�����
lparam--��Ӧ��Ϣʱ�������ֵ������û��ʹ��
Return		: LRESULT
Desc		: ��ӦID_STUDENTLOGIN ��Ϣ
******************************************************************/
LRESULT CTeacherDlg::OnAddStudent(WPARAM wparam, LPARAM lparam)
{
	CItemData *t = (CItemData*)wparam;
	int i = m_list.GetItemCount();
	m_list.InsertItem(i, t->m_stuInfo.GetStuName(), 0);
	m_list.SetItemText(i, 1, t->m_stuInfo.GetStuID());
	m_list.SetItemText(i, 2, t->m_stuInfo.GetStuMajor());
	m_list.SetItemText(i, 3, t->m_stuInfo.GetStuClass());
	m_list.SetItemData(i, (DWORD)t);
	ShowStudentInfo();
	return 0;
}

/******************************************************************
Function	: ShowStudentInfo
Date		: 2015-05-11 15:43:48
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ��ʾ��¼ѧ��������Ϣ
******************************************************************/
void  CTeacherDlg::ShowStudentInfo()
{
	int c = m_list.GetItemCount();
	CString online;
	TCHAR* szText;
	online.Format(_T("����������%d ̨"), c);
	szText = online.GetBuffer(online.GetLength());
	::SendMessageW(m_statusBar.m_hWnd, SB_SETTEXTW, (WPARAM)1, (LPARAM)szText);
	online.ReleaseBuffer();
}

/******************************************************************
Function	: OnLoginOutStudent
Date		: 2015-05-11 15:44:36
Author		: xiaoheike
Parameter	: wparam--��Ӧ��Ϣʱ�������ֵ�������ǵ�¼ѧ����ID ֵ
lparam--��Ӧ��Ϣʱ�������ֵ������û��ʹ��
Return		: LRESULT
Desc		: ��¼ѧ�����ߣ���ӦID_STUDENTLOGINOUT ��Ϣ
******************************************************************/
LRESULT CTeacherDlg::OnLoginOutStudent(WPARAM wparam, LPARAM lparam)
{
	CItemData* t;
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		t = (CItemData*)m_list.GetItemData(i);
		if (t->m_id == (int)wparam)
		{
			::closesocket(t->GetSocket());
			delete t;
			m_list.DeleteItem(i);
		}
	}
	ShowStudentInfo();
	return 0;
}


/******************************************************************
Function	: OnSetBmpCompressData
Date		: 2015-05-11 15:47:17
Author		: xiaoheike
Parameter	: wparam--��Ӧ��Ϣʱ�������ֵ��������BMP�ṹ�����
lparam--��Ӧ��Ϣʱ�������ֵ������û��ʹ��
Return		: LRESULT
Desc		: ��������ѧ��������Ļͼ������ID_SETBMPDATA ��Ϣ
******************************************************************/
LRESULT CTeacherDlg::OnSetBmpCompressData(WPARAM wparam, LPARAM lparam)
{
	BMP* bmp = (BMP*)wparam;
	int* itemOrder = (int*)lparam;
	m_pScreenDlg->ShowBmp(bmp, *itemOrder);
	return 0;
}

/******************************************************************
Function	: OnEndMonitor
Date		: 2015-05-11 15:48:39
Author		: xiaoheike
Parameter	: wparam--��Ӧ��Ϣʱ�������ֵ������û��ʹ��
lparam--��Ӧ��Ϣʱ�������ֵ������û��ʹ��
Return		: LRESULT
Desc		: ������ѧ�����ļ����ӦID_MONITOREND
******************************************************************/
LRESULT CTeacherDlg::OnEndMonitor(WPARAM wparam, LPARAM lparam)
{
	for (int i = 0; i < MAX_MONITOR; i++)
	{
		if (-1 != m_item[i])
		{
			CItemData* itemData = (CItemData*)m_list.GetItemData(m_item[i]);
			itemData->EndScreenMonitor();
		}
		else if (-1 == m_item[i])
		{
			break;
		}
	}
	// ʹ����������Ļ��ذ�ť��������Ч
	m_toolBar.GetToolBarCtrl().EnableButton(ID_SCREENMONITOR, true);
	return 0;
}

/******************************************************************
Function	: OnCreatOneStudentDlg
Date		: 2015-05-11 15:50:39
Author		: xiaoheike
Parameter	: wparam--��Ӧ��Ϣʱ�������ֵ������û��ʹ��
lparam--��Ӧ��Ϣʱ�������ֵ��������ѧ����һ�Զ�Ի�������ʾ�����
Return		: LRESULT
Desc		: ��Ӧһ�Զ���Ļ���˫��ѡ��ѧ����ʵ��һ��һ��Ļ���
��ӦID_ONSTUINSCREEN ��Ϣ
******************************************************************/
LRESULT CTeacherDlg::OnCreatOneStudentDlg(WPARAM wparam, LPARAM lparam)
{
	int* itemOrder = (int*)lparam;
	int item = m_item[*itemOrder];
	if (-1 != item)
	{
		CItemData* itemData = (CItemData*)m_list.GetItemData(item);
		itemData->EndScreenMonitor();
		if (m_oneStuScreenDlg != NULL)
		{
			delete m_oneStuScreenDlg;
			m_oneStuScreenDlg = NULL;
		}
		m_oneStuScreenDlg = new COneStuScreenDlg(this);
		m_oneStuScreenDlg->Create(IDD_SCREEN_DIALOG1);
		m_oneStuScreenDlg->ShowWindow(SW_NORMAL);
		itemData->BeginMonitor(true, *itemOrder);
	}

	return 0;
}

/******************************************************************
Function	: OnSetOneStuBmpCompressData
Date		: 2015-05-11 15:52:26
Author		: xiaoheike
Parameter	: wparam--��Ӧ��Ϣʱ�������ֵ��������BMP�ṹ�����
lparam--��Ӧ��Ϣʱ�������ֵ��������ѧ����һ�Զ�Ի�������ʾ�����
Return		: LRESULT
Desc		: ��������ѧ��������Ļͼ��������Ӧ��ϢID_STEONSTUSCREENDATA
******************************************************************/
LRESULT CTeacherDlg::OnSetOneStuBmpCompressData(WPARAM wparam, LPARAM lparam)
{
	BMP* bmp = (BMP*)wparam;
	int* itemOrder = (int*)lparam;
	m_oneStuScreenDlg->ShowBmp(bmp, *itemOrder);
	return 0;
}

/******************************************************************
Function	: OnEndOneStudentMonitor
Date		: 2015-05-11 15:54:21
Author		: xiaoheike
Parameter	: wparam--��Ӧ��Ϣʱ�������ֵ������û��ʹ��
lparam--��Ӧ��Ϣʱ�������ֵ��������ѧ����һ�Զ�Ի�������ʾ�����
Return		: LRESULT
Desc		: ����һ��һ��Ļ���
******************************************************************/
LRESULT CTeacherDlg::OnEndOneStudentMonitor(WPARAM wparam, LPARAM lparam)
{
	int* itemOrder = (int*)lparam;
	int item = m_item[*itemOrder];
	if (-1 != item)
	{
		CItemData* itemData = (CItemData*)m_list.GetItemData(item);
		itemData->EndScreenMonitor();
		itemData->BeginMonitor(*itemOrder);
	}


	return 0;
}

/******************************************************************
Function	: OnBeginScreenMonitor
Date		: 2015-05-11 15:55:07
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ��Ӧ��ϢID_ONSCREENMONITOR�����÷�����Ļ�����Ϣ��
ѧ�����ķ���
******************************************************************/
void CTeacherDlg::OnBeginScreenMonitor()
{
	int selCount = m_list.GetSelectedCount();
	if (selCount > 16)
	{
		MessageBox(_T("ͬʱ��ص�ѧ�������������ܳ���16̨"));
		return;
	}
	if (0 == selCount)
	{
		MessageBox(_T("��ѡ��Ҫ��ص�ѧ����"));
		return;
	}
	DeletepScreenDlg();
	BeginScreenMonitor(selCount);
}

/******************************************************************
Function	: BeginScreenMonitor
Date		: 2015-05-11 15:57:07
Author		: xiaoheike
Parameter	: selCount--Ҫ��ص�ѧ����������
Return		: void
Desc		: ���Ϳ�ʼ��Ļ�����Ϣ����ʦ��
******************************************************************/
void CTeacherDlg::BeginScreenMonitor(int selCount)
{
	int widthCount = (int)ceil((sqrt(double(selCount))));
	m_pScreenDlg = new CScreenMonitorDlg(this, widthCount);
	m_pScreenDlg->Create(IDD_SCREEN_DIALOG);
	m_pScreenDlg->ShowWindow(SW_NORMAL);


 	POSITION pos_s;
	pos_s = m_list.GetFirstSelectedItemPosition();
	int itemOrder = 0;
	memset(m_item, -1, sizeof(int)*MAX_MONITOR);
	while (pos_s != NULL)
	{
		int item = m_list.GetNextSelectedItem(pos_s);
		m_item[itemOrder] = item;
		CItemData* itemData = (CItemData*)m_list.GetItemData(item);
		itemData->BeginMonitor(itemOrder);
		itemOrder = itemOrder + 1;
	}

// 	int itemOrder = 0;
// 	memset(m_item, -1, sizeof(int)*MAX_MONITOR);
// //	pos_s = m_list.GetFirstSelectedItemPosition();
// 
// 	for (int i = 0; i < m_list.GetItemCount(); i++)
// 	{
// 		if (m_list.GetCheck(i))
// 		{
// 			CString str;
// 			str.Format(_T("��%d�е�checkboxΪѡ��״̬"), i);
// 			AfxMessageBox(str);
// //			int item = m_list.GetNextSelectedItem(pos_s);
// // 			m_item[itemOrder] = i;
// // 			CItemData* itemData = (CItemData*)m_list.GetItemData(i);
// // 			itemData->BeginMonitor(itemOrder);
// // 			itemOrder = itemOrder + 1;
// 
// 		}
// 	}
	// ʹ����������Ļ��ذ�ť�������Ч
	m_toolBar.GetToolBarCtrl().EnableButton(ID_SCREENMONITOR, false);
}

/******************************************************************
Function	: OnMulticast
Date		: 2015-05-11 15:57:50
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ��ʼ�㲥��Ӧ��ϢID_MULTICAST
******************************************************************/
void CTeacherDlg::OnMulticast()
{
	m_pMulticast = new CMulticast();
	if (false == m_isStopMulticast)
	{
		m_pMulticast->SetIsMulticastEnd(false);
		for (int i = 0; i < m_list.GetItemCount(); i++)
		{
			CItemData* itemData = (CItemData*)m_list.GetItemData(i);
			itemData->BeginMulticast();
		}
		::CloseHandle(CreateThread(0, 0, OnSendScreenData, (LPVOID)this, 0, NULL));
		m_isStopMulticast = true;
	}
	else
	{
		for (int i = 0; i < m_list.GetItemCount(); i++)
		{
		 	CItemData* itemData = (CItemData*)m_list.GetItemData(i);
		 	itemData->EndMulticast();
		}
		m_pMulticast->SetIsMulticastEnd(true);
		m_isStopMulticast = false;
		DeletepMulticast();
	}
}

/******************************************************************
Function	: OnSendScreenData
Date		: 2015-05-11 15:58:37
Author		: xiaoheike
Parameter	: self--CTeacherDlg�����
Return		: DWORD WINAPI
Desc		: ���ͽ�ʦ����Ļͼ�������̵߳ĵ��÷���
******************************************************************/
DWORD WINAPI CTeacherDlg::OnSendScreenData(LPVOID self)
{
	CTeacherDlg* t = (CTeacherDlg*)self;
	t->SendScreenData();
	return 0;
}

/******************************************************************
Function	: SendScreenData
Date		: 2015-05-11 15:59:38
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: ���ͽ�ʦ����Ļͼ�����ݵ�ѧ����
******************************************************************/
void CTeacherDlg::SendScreenData()
{
	m_pMulticast->SendScreenData();
}

void CTeacherDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	DeletepScreenDlg();
	DeletepOneStuScreenDlg();
	DeleteSocketMsg();
}

void CTeacherDlg::DeletepMulticast()
{
	if (m_pMulticast != NULL)
	{
		delete m_pMulticast;
		m_pMulticast = NULL;
	}
}

/******************************************************************
Function	: DeletepScreenDlg
Date		: 2015-05-11 16:00:08
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CTeacherDlg::DeletepScreenDlg()
{
	if (m_pScreenDlg != NULL)
	{
		delete m_pScreenDlg;
		m_pScreenDlg = NULL;
	}
}

/******************************************************************
Function	: DeletepOneStuScreenDlg
Date		: 2015-05-11 16:00:23
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�new ���ڴ�
******************************************************************/
void CTeacherDlg::DeletepOneStuScreenDlg()
{
	if (m_oneStuScreenDlg != NULL)
	{
		delete m_oneStuScreenDlg;
		m_oneStuScreenDlg = NULL;
	}
}

/******************************************************************
Function	: DeleteSocketMsg
Date		: 2015-05-11 16:07:15
Author		: xiaoheike
Parameter	: ��
Return		: void
Desc		: �ͷ�SOCKET ��Դ
******************************************************************/
void CTeacherDlg::DeleteSocketMsg()
{
	if (m_socketMsg != INVALID_SOCKET)
	{
		closesocket(m_socketMsg);
		m_socketMsg = NULL;
	}
}


void CTeacherDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
// 	static bool isClick = false;
// 	// û���б�ѡ����iItm = -1
// 	if (pNMItemActivate->iItem != -1)
// 	{
// 		if (false == isClick)
// 		{
// 			m_list.SetCheck(pNMItemActivate->iItem, true);
// 			isClick = true;
// 		}
// 		else
// 		{
// 			m_list.SetCheck(pNMItemActivate->iItem, false);
// 			isClick = false;
// 		}
// 	}
	*pResult = 0;
}


/******************************************************************
Function	: OnNMDblclkList1
Date		: 2015-05-12 17:20:11
Author		: xiaoheike
Parameter	: pNMHDR
			  pResult
Return		: void
Desc		: CListControl˫����Ϣ
******************************************************************/
void CTeacherDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnBeginScreenMonitor();
	*pResult = 0;
}
