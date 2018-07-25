#pragma once
#include <math.h>
//////////////////////////////////////////////////////////////////////////
// zlib�� ��Ҫʹ�����е�ѹ��ͼƬ����
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zdll.lib")
//////////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "Comment.h"
#include "SocketCenter.h"
class CScreenMonitor
{
public:
	CScreenMonitor();

	~CScreenMonitor();

	void GetDeskScreeData();

	void DeletepBmpOriginData(BYTE* pBmpOriginalData);

	void GetDeskScreenDC(CDC &memDC, CBitmap &bmp, BITMAP &bitmap);

	void SetCursorIcon(CDC &memDC);

	void InitBITMAPINFO(BITMAP &bitmap, int height, int width);

	void CompressBmpData(BYTE* pBmpOriginalData);

	void SendScreenBmpData();

	void SetBmpData(BMPDATA &bmpData, int i, int count);

	int InitBmpData(BMPDATA &bmpData);

	void SendScreenData();

	void SetSocketMsg(SOCKET socketMsg);

	void SetIsScreenMonitorEnd(bool isSendFlag);

	void CleanData();

	void DeletepBmpTransData();

	void DeletepBitMapInfo();

private:
	int m_bmpHeadTotalSize;
	// ͼ���ͷ��Ϣ
	BITMAPINFO* m_pBitMapInfo;
	uLongf m_imgTotalSize;
	BYTE* m_pBmpTransData;
	CSocketCenter m_socketCenter;
	bool m_isScreenMonitorEnd;
	SOCKET m_socketMsg;
	uLongf  m_compressBmpDataLen;
};

