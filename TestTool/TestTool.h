
// TestTool_SiChuan.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTestTool_SiChuanApp: 
// 有关此类的实现，请参阅 TestTool_SiChuan.cpp
//

class CTestTool_SiChuanApp : public CWinApp
{
public:
	CTestTool_SiChuanApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	CMutex *g_clsMutex;	
	//程序目录
	CString m_strCommDir;

	//服务器IP
	CString m_strServerIP;
	//服务器端口
	WORD	m_wServerPort;

	//省份编码
	int		m_iProvinceID;
	//区域编码
	CString		m_strAreaID;
	//路段编码
	CString		m_strRoadID;
	//站点编码
	CString		m_strStationID;
	//站名字
	CString     m_strStationName;
	//车道
	CString     m_strLaneID;
	//参数类型， 1:xml， 2:json
	int m_iDataParamType;

	DECLARE_MESSAGE_MAP()
};

extern CTestTool_SiChuanApp theApp;