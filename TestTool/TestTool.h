
// TestTool_SiChuan.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestTool_SiChuanApp: 
// �йش����ʵ�֣������ TestTool_SiChuan.cpp
//

class CTestTool_SiChuanApp : public CWinApp
{
public:
	CTestTool_SiChuanApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	CMutex *g_clsMutex;	
	//����Ŀ¼
	CString m_strCommDir;

	//������IP
	CString m_strServerIP;
	//�������˿�
	WORD	m_wServerPort;

	//ʡ�ݱ���
	int		m_iProvinceID;
	//�������
	CString		m_strAreaID;
	//·�α���
	CString		m_strRoadID;
	//վ�����
	CString		m_strStationID;
	//վ����
	CString     m_strStationName;
	//����
	CString     m_strLaneID;
	//�������ͣ� 1:xml�� 2:json
	int m_iDataParamType;

	DECLARE_MESSAGE_MAP()
};

extern CTestTool_SiChuanApp theApp;