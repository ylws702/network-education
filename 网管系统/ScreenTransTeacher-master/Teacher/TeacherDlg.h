// TeacherDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
//////////////////////////////////////////////////////////////////////////
// ZLIB ���ͷ�ļ�
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zdll.lib")
//////////////////////////////////////////////////////////////////////////
#include "ScreenMonitorDlg.h"
#include "Multicast.h"

// �����ID ���빤��������ʱ��ֵ��ID �Ƕ�Ӧ��
#define ID_MULTICAST 1001
#define ID_SCREENMONITOR	 1002

// CTeacherDlg �Ի���
class CTeacherDlg : public CDialogEx
{
	// ����
public:
	CTeacherDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_TEACHER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	afx_msg void OnPaint();

	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	static DWORD WINAPI OnMsgListen(LPVOID self);

	afx_msg LRESULT OnAddStudent(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnLoginOutStudent(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnSetBmpCompressData(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnEndMonitor(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnCreatOneStudentDlg(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnSetOneStuBmpCompressData(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnEndOneStudentMonitor(WPARAM wparam, LPARAM lparam);

	afx_msg void OnBeginScreenMonitor();

	afx_msg void OnMulticast();

private:
	CListCtrl m_list;
	CImageList m_imagelist;
	CToolBar m_toolBar;
	CStatusBar m_statusBar;
	CRect m_rect;
	SOCKET m_socketMsg;
	CScreenMonitorDlg* m_pScreenDlg;
	// ��¼����ص�ѧ������ID
	int m_item[MAX_MONITOR];
	COneStuScreenDlg* m_oneStuScreenDlg;
	CMulticast m_multicast;
	CMulticast* m_pMulticast;
	bool m_isStopMulticast;
public:
	afx_msg void OnDestroy();

	void DeletepMulticast();

	void DeletepScreenDlg();

	void DeletepOneStuScreenDlg();

	void DeleteSocketMsg();

	void BeginScreenMonitor(int selCount);

	static DWORD WINAPI OnSendScreenData(LPVOID self);

	void SendScreenData();

	void ShowStudentInfo();

	void SetMainDlgSytle(int port);

	void InsertColumn();

	void CreateStatusBar(int port);

	void CreatToolBar();

	void LoadBitBmp();

	bool ListenStudentLogin();

	void SetLoginStudentID();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
