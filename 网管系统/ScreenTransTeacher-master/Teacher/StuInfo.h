#pragma once
#include "SocketCenter.h"
#include "Common.h"
class CStuInfo
{
public:
	CStuInfo();

	~CStuInfo();
	void SendStuInfoReq(SOCKET socket);

	void GetStuInfo(SOCKET socket);

	CString GetStuName() const;

	CString GetStuID() const;

	CString GetStuMajor() const;

	CString GetStuClass() const;
private:
	// ѧ������
	CString m_stuName;
	// ѧ���İ༶
	CString m_stuClass;
	// ѧ����ѧ��
	CString m_stuID;
	// ѧ����רҵ
	CString m_stuMajor;

};

