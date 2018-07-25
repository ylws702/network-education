
// StudentDlg.h : ͷ�ļ�
//

#pragma once
#include "ControlCenter.h"
#define WM_SHOWTASKICON WM_USER + 4
#define IDM_SHOW_WINDOW WM_USER + 5
// CStudentDlg �Ի���
class CStudentDlg : public CDialogEx
{
// ����
public:
	CStudentDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STUDENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnShowTaskIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();

	bool IsEdtDataEmpty();

	void InitNofIconData();

	static DWORD WINAPI OnBeginServer(LPVOID self);
//	void BeginServer();
//	afx_msg void OnShowDlg();
public:
private:
	CString m_stuName;
	// ѧ���İ༶
	CString m_stuClass;
	// ѧ����ѧ��
	CString m_stuID;
	// ѧ����רҵ
	CString m_stuMajor;
	NOTIFYICONDATA m_notifyIconData;
	CControlCenter*  m_pCtrlCenter;
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
};
