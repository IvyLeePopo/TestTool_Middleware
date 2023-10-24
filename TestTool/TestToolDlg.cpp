
// TestTool_SiChuanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestTool.h"
#include "TestToolDlg.h"
#include "afxdialogex.h"
#include "DVJsonParamDebitMoney.h"
#include "JsonParamOperate.h"
#include "CodeConv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CTestToolDlg *g_DevIFDlg;

#define FILE_NAME_INIT_CONFIG   _T("HTSMConfig.ini")		//初始化配置文件名

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTestToolDlg 对话框
int CTestToolDlg::No = 1000;
int CTestToolDlg::VehPlateTail = 100;
void* CTestToolDlg::m_pIndPtr = new char[2048];

void MyNetPayCallBackFun(int iOption, int iResult, void* pIndPtr)
{
	int Option = iOption;
	int Result = iResult;
	char *ptr = (char *)pIndPtr;

	//回调函数,暂时屏蔽
	//g_DevIFDlg->m_pMainWnd->PostMessage(WM_THREAD_NETPAY_CALLBACK_SHOWLOG, (WPARAM)Option, (LPARAM)Result);

	return;
}

CTestToolDlg::CTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bLoaded = false;
	ReceiveMsgThreadID = 0;
	//m_enCurProvince = PROVINCE_TYPE::Jiangxi;
	//m_enMidllewareType = MIDLLEWARE_TYPE::BaseNoEvent;

	m_enCurProvince = PROVINCE_TYPE::ShanXi;
	m_enMidllewareType = MIDLLEWARE_TYPE::BaseAndEvent;

	m_pGuiPlay1Thread = NULL;
	m_pGuiPlay2Thread = NULL;
	m_pGuiPlay3Thread = NULL;
	m_pGuiPlayTotalThread = NULL;
	m_bThread1Running = false;
	m_bThread2Running = false;
	m_bThread3Running = false;
	m_bThreadTotalRunning = false;

}

void CTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, edt_info);
	DDX_Control(pDX, IDC_EDIT_EtcEventParam, EditParam);
}

BEGIN_MESSAGE_MAP(CTestToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LoadDll, &CTestToolDlg::OnBnClickedButtonLoaddll)
	ON_BN_CLICKED(IDC_BUTTON_InitNetPayEnv, &CTestToolDlg::OnBnClickedButtonInitnetpayenv)
	ON_BN_CLICKED(IDC_BTN_debit, &CTestToolDlg::OnBnClickedBtndebit)
	ON_BN_CLICKED(IDC_BTN_CancelDebit, &CTestToolDlg::OnBnClickedBtnCanceldebit)


	ON_MESSAGE(WM_THREAD_ETCEVENT_CALLBACK_SHOWLOG, OnMsgShowEtcEventLog)
	ON_MESSAGE(WM_HWND_ETCEVENT_MESSAGE_SHOWLOG, OnHWNDMsgShowEtcEventLog)
	ON_MESSAGE(WM_HWND_NETPAY_RESULT_SHOWLOG, OnHWNDMsgShowNetPayResultLog)
	ON_BN_CLICKED(IDC_BTN_NetPayDevStatus, &CTestToolDlg::OnBnClickedBtnNetpaydevstatus)
	ON_BN_CLICKED(IDC_BTN_INITENV, &CTestToolDlg::OnBnClickedBtnInitenv)
	ON_BN_CLICKED(IDC_BTN_EventDestroy, &CTestToolDlg::OnBnClickedBtnEventdestroy)
	ON_BN_CLICKED(IDC_BTN_EVENTSTART, &CTestToolDlg::OnBnClickedBtnEventstart)
	ON_BN_CLICKED(IDC_BTN_CHECKVEHQUEUE, &CTestToolDlg::OnBnClickedBtnCheckvehqueue)
	ON_BN_CLICKED(IDC_BTN_CHECKVEHQUEUEDEL, &CTestToolDlg::OnBnClickedBtnCheckvehqueuedel)
	ON_BN_CLICKED(IDC_BTN_CHECKVEHINFO, &CTestToolDlg::OnBnClickedBtnCheckvehinfo)
	ON_BN_CLICKED(IDC_BTN_CHECKENTRYINFO, &CTestToolDlg::OnBnClickedBtnCheckentryinfo)
	ON_BN_CLICKED(IDC_BTN_CHECKENTRYINFO_NONE, &CTestToolDlg::OnBnClickedBtnCheckentryinfoNone)
	ON_BN_CLICKED(IDC_BTN_SHOWFEEINFOSINGLE, &CTestToolDlg::OnBnClickedBtnShowfeeinfosingle)
	ON_BN_CLICKED(IDC_BTN_SHOWFEEINFO, &CTestToolDlg::OnBnClickedBtnShowfeeinfo)
	ON_BN_CLICKED(IDC_BTN_PAYRESULTSHOW, &CTestToolDlg::OnBnClickedBtnPayresultshow)
	ON_BN_CLICKED(IDC_BTN_PAYRESULTSHOWFAIL, &CTestToolDlg::OnBnClickedBtnPayresultshowfail)
	ON_BN_CLICKED(IDC_BTN_SWIPECARD, &CTestToolDlg::OnBnClickedBtnSwipecard)
	ON_BN_CLICKED(IDC_BTN_RWCARD, &CTestToolDlg::OnBnClickedBtnRwcard)
	ON_BN_CLICKED(IDC_BTN_FIXVEHQUEUE, &CTestToolDlg::OnBnClickedBtnFixvehqueue)
	ON_BN_CLICKED(IDC_BTN_FeeAuthorize, &CTestToolDlg::OnBnClickedBtnFeeauthorize)
	ON_BN_CLICKED(IDC_BTN_Authorize, &CTestToolDlg::OnBnClickedBtnAuthorize)
	ON_BN_CLICKED(IDC_BTN_DelVehQueueResult, &CTestToolDlg::OnBnClickedBtnDelvehqueueresult)
	ON_BN_CLICKED(IDC_BTN_EtcDevStatus, &CTestToolDlg::OnBnClickedBtnEtcdevstatus)
	ON_BN_CLICKED(IDC_BTN_EtcGetLastErrorDescrip, &CTestToolDlg::OnBnClickedBtnEtcgetlasterrordescrip)
	ON_BN_CLICKED(ID_BTN_Change2ETCMode, &CTestToolDlg::OnBnClickedBtnChange2etcmode)
	ON_BN_CLICKED(IDC_BTN_EVENTSTOP, &CTestToolDlg::OnBnClickedBtnEventstop)
	ON_BN_CLICKED(IDC_BTN_DestroyEtcEvent, &CTestToolDlg::OnBnClickedBtnDestroyetcevent)
	ON_BN_CLICKED(IDC_BTN_DestroyNetPay, &CTestToolDlg::OnBnClickedBtnDestroynetpay)
	ON_BN_CLICKED(IDC_BTN_FreeDll, &CTestToolDlg::OnBnClickedBtnFreedll)
	ON_BN_CLICKED(IDC_BTN_GUIPlay1Start, &CTestToolDlg::OnBnClickedBtnGuiplay1start)
	ON_BN_CLICKED(IDC_BTN_GUIPlay1Stop, &CTestToolDlg::OnBnClickedBtnGuiplay1stop)
	ON_BN_CLICKED(IDC_BTN_GUIPlay2Start, &CTestToolDlg::OnBnClickedBtnGuiplay2start)
	ON_BN_CLICKED(IDC_BTN_GUIPlay2Stop, &CTestToolDlg::OnBnClickedBtnGuiplay2stop)
	ON_BN_CLICKED(IDC_BTN_GUIPlay3Start, &CTestToolDlg::OnBnClickedBtnGuiplay3start)
	ON_BN_CLICKED(IDC_BTN_GUIPlay3Stop, &CTestToolDlg::OnBnClickedBtnGuiplay3stop)
	ON_BN_CLICKED(IDC_BTN_GUIPlayTotalStart, &CTestToolDlg::OnBnClickedBtnGuiplaytotalstart)
	ON_BN_CLICKED(IDC_BTN_GUIPlayTotalStop, &CTestToolDlg::OnBnClickedBtnGuiplaytotalstop)
	ON_BN_CLICKED(IDC_BTN_AsyncExecCmd, &CTestToolDlg::OnBnClickedBtnAsyncexeccmd)
	ON_BN_CLICKED(IDC_BTN_ClearQrCodeCmd, &CTestToolDlg::OnBnClickedBtnClearqrcodecmd)
END_MESSAGE_MAP()


// CTestToolDlg 消息处理程序

BOOL CTestToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	g_DevIFDlg = this;

	SetDlgItemText(IDC_EDIT_Money, _T("1"));
	SetDlgItemText(IDC_EDIT_Weight, _T("1234"));

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_type);
	pCombo->SetCurSel(1);

	SetDlgItemText(IDC_EDIT_axisnum, _T("2"));
	SetDlgItemText(IDC_EDIT_plate, _T("川A12345"));
	SetDlgItemText(IDC_EDIT_entry, _T("云龙"));
	SetDlgItemText(IDC_EDIT_timeout, _T("30"));
	SetDlgItemText(IDC_EDIT_operator, _T("收费员"));
	SetDlgItemText(IDC_EDIT_operatorID, _T("13579"));
	SetDlgItemText(IDC_EDIT_balance, _T("9999"));
	SetDlgItemText(IDC_EDIT_cardno, _T("999988887777"));

	((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->SetCheck(TRUE);	//选上默认的
	((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->SetCheck(FALSE);

	log("开始读取移动支付配置信息");	
	//读取移动支付共享库的配置信息
	char szIP[256] = { 0 };
	CString strCfgFilePath;
	strCfgFilePath.Format(_T("%s\\TWSDNetPayConfig.ini"), theApp.m_strCommDir);
	::GetPrivateProfileString(_T("ReflectWeb"), _T("IP"), _T("101.200.214.203"), szIP, _countof(szIP), strCfgFilePath);
	m_strServerIP.Format(_T("%s"), szIP);
	m_wServerPort = (WORD)::GetPrivateProfileInt(_T("ReflectWeb"), _T("Port"), 8081, strCfgFilePath);

	//读取测试工具的配置信息
	strCfgFilePath.Format(_T("%s\\TWNetPayTest.ini"), theApp.m_strCommDir);
	m_iProvinceID = ::GetPrivateProfileInt(_T("Setup"), _T("ProvinceID"), 44, strCfgFilePath);
	m_iAreaID = ::GetPrivateProfileInt(_T("Setup"), _T("AreaID"), 4412, strCfgFilePath);
	m_iRoadID = ::GetPrivateProfileInt(_T("Setup"), _T("RoadID"), 30, strCfgFilePath);
	m_iStationID = ::GetPrivateProfileInt(_T("Setup"), _T("StationID"), 9, strCfgFilePath);
	m_iLaneID = ::GetPrivateProfileInt(_T("Setup"), _T("LaneID"), 1, strCfgFilePath);

	char chs[50] = { 0 };
	::GetPrivateProfileString(_T("Setup"), _T("StationName"), _T("广东路段"), chs, _countof(chs), strCfgFilePath);
	m_strStationName = chs;
	::GetPrivateProfileString(_T("Setup"), _T("ProvinceID"), "44", chs, _countof(chs), strCfgFilePath);
	m_strProvinceID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("AreaID"), "4412", chs, _countof(chs), strCfgFilePath);
	m_strAreaID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("RoadID"), "30", chs, _countof(chs), strCfgFilePath);
	m_strRoadID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("StationID"), "9", chs, _countof(chs), strCfgFilePath);
	m_strStationID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("LaneID"), "1", chs, _countof(chs), strCfgFilePath);
	m_strLaneID = chs;

	theApp.m_strServerIP = m_strServerIP;
	theApp.m_wServerPort = m_wServerPort;
	theApp.m_iProvinceID = m_iProvinceID;
	theApp.m_strAreaID = m_strAreaID;
	theApp.m_strRoadID = m_strRoadID;
	theApp.m_strStationID = m_strStationID;
	theApp.m_strLaneID = m_strLaneID;
	theApp.m_strStationName = m_strStationName;

	CString strCfgInfo;
	strCfgInfo.Format(_T("ProvinceID: %s, AreaID: %s, RoadID: %s, StationID: %s, LaneID: %s"), m_strProvinceID, m_strAreaID, m_strRoadID, m_strStationID, m_strLaneID);
	//DisplayInfo(strCfgInfo);
	log(strCfgInfo.GetBuffer(0));

	m_bNetPayEnvInitedFlag = false;
	m_bEtcEventEnvInitedFlag = false;

	//启动线程	
	pThreadReceiveMsg = (CThreadReceiveRespMsg *)::AfxBeginThread(RUNTIME_CLASS(CThreadReceiveRespMsg), THREAD_PRIORITY_NORMAL);
	if (pThreadReceiveMsg != NULL)
	{
		log(_T("线程启动成功 \n"));
		ReceiveMsgThreadID = pThreadReceiveMsg->m_nThreadID;
	}
	else
	{
		log(_T("线程启动失败 \n"));
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestToolDlg::log(std::string str)
{
	CString tmp;
	edt_info.GetWindowText(tmp);
	CString rt;
	rt.Format(_T("\%s\r\n%s"), tmp, str.c_str());
	edt_info.SetWindowText(rt);
	edt_info.LineScroll(edt_info.GetLineCount(), 0);
}


void CTestToolDlg::loadDll()
{
	log("开始加载TWSDNetPay.dll库...（基础没有特情处理）\n");

	fn_EventInitEnvironment3 = NULL;

	fn_InitEnvironment = NULL;
	fn_Destroy = NULL;
	fn_GetLastErrorDesc = NULL;
	fn_StartGetAccountInfo = NULL;
	fn_StopGetAccountInfo = NULL;
	fn_GetAccountInfo = NULL;
	fn_DebitMoney = NULL;
	fn_DebitCancel = NULL;
	fn_GetDebitResult = NULL;
	fn_SetMMI = NULL;
	fn_GetParam = NULL;
	fn_GetComponentStatus = NULL;
	fn_TranslateData = NULL;

	gd_dll = ::LoadLibraryA("TWSDNetPay.dll");

	if (gd_dll == NULL)
	{
		log("load TWSDNetPay.dll失败!!! \n");
		return;
	}

	fn_InitEnvironment = (defIF_InitEnvironment)::GetProcAddress(gd_dll, "IF_InitEnvironment");
	fn_Destroy = (defIF_Destroy)::GetProcAddress(gd_dll, "IF_Destroy");
	fn_GetLastErrorDesc = (defIF_GetLastErrorDesc)::GetProcAddress(gd_dll, "IF_GetLastErrorDesc");
	fn_StartGetAccountInfo = (defIF_StartGetAccountInfo)::GetProcAddress(gd_dll, "IF_StartGetAccountInfo");
	fn_StopGetAccountInfo = (defIF_StopGetAccountInfo)::GetProcAddress(gd_dll, "IF_StopGetAccountInfo");
	fn_GetAccountInfo = (defIF_GetAccountInfo)::GetProcAddress(gd_dll, "IF_GetAccountInfo");
	fn_DebitMoney = (defIF_DebitMoney)::GetProcAddress(gd_dll, "IF_DebitMoney");
	fn_DebitCancel = (defIF_DebitCancel)::GetProcAddress(gd_dll, "IF_DebitCancel");
	fn_GetDebitResult = (defIF_GetDebitResult)::GetProcAddress(gd_dll, "IF_GetDebitResult");
	fn_SetMMI = (defIF_SetMMI)::GetProcAddress(gd_dll, "IF_SetMMI");
	fn_GetParam = (defIF_GetParam)::GetProcAddress(gd_dll, "IF_GetParam");
	fn_GetComponentStatus = (defIF_GetComponentStatus)::GetProcAddress(gd_dll, "IF_GetComponentStatus");
	fn_TranslateData = (defIF_TranslateData)::GetProcAddress(gd_dll, "IF_TranslateData");

	// 特情相关
	fn_EventInitEnvironment3 = (IF_EventInitEnvironment3)::GetProcAddress(gd_dll, "IF_EventInitEnvironment3");

	m_bLoaded = fn_InitEnvironment != NULL |
		fn_Destroy != NULL |
		fn_GetLastErrorDesc != NULL |
		fn_StartGetAccountInfo != NULL |
		fn_StopGetAccountInfo != NULL |
		fn_GetAccountInfo != NULL |
		fn_DebitMoney != NULL |
		fn_DebitCancel != NULL |
		fn_GetDebitResult != NULL |
		fn_SetMMI != NULL |
		fn_GetParam != NULL |
		fn_GetComponentStatus != NULL |
		fn_TranslateData != NULL |
		fn_EventInitEnvironment3 != NULL;

	if (m_enCurProvince == PROVINCE_TYPE::Jiangxi)
	{
		fn_AsyncExecCmd = (defIF_AsyncExecCmd)::GetProcAddress(gd_dll, "IF_AsyncExecCmd");
		m_bLoaded = m_bLoaded | fn_AsyncExecCmd != NULL;
	}

	m_bLoaded ? log("加载TWSDNetPay.dll成功! \n") : log("加载TWSDNetPay.dll失败!!! \n");
}

void CTestToolDlg::loadDllEvent()
{
	log("开始加载TWSDNetPay.dll库...（基础+特情处理）\n");

	fn_InitEnvironment = NULL;
	fn_InitEnvironment3 = NULL;
	fn_Destroy = NULL;
	fn_GetLastErrorDesc = NULL;
	fn_StartGetAccountInfo = NULL;
	fn_StopGetAccountInfo = NULL;
	fn_GetAccountInfo = NULL;
	fn_DebitMoney = NULL;
	fn_DebitCancel = NULL;
	fn_GetDebitResult = NULL;
	fn_SetMMI = NULL;
	fn_GetParam = NULL;
	fn_GetComponentStatus = NULL;
	fn_TranslateData = NULL;

	fn_EventInitEnvironment3 = NULL;
	fn_EventDestroy = NULL;
	fn_EventGetLastErrorDesc = NULL;
	fn_EventGetComponentStatus = NULL;
	fn_EventDealStart = NULL;
	fn_EventCheckVehQueue = NULL;
	fn_EventCheckVehInfo = NULL;
	fn_EventCheckEntryInfo = NULL;
	fn_EventShowFeeInfo = NULL;
	fn_EventPayResultDisplay = NULL;
	fn_EventDealStop = NULL;
	fn_EventStartScan = NULL;
	fn_EventStopScan = NULL;
	fn_EventCardOperationNotify = NULL;
	fn_EventModifyVehQueue = NULL;
	fn_EventFeeAuthorize = NULL;
	fn_EventAuthorize = NULL;
	fn_EventDelVehQueueResult = NULL;
	fn_EventFreeVoicePlay = NULL;

	gd_dll = ::LoadLibraryA("TWSDNetPay.dll");

	if (gd_dll == NULL)
	{
		log("load TWSDNetPay.dll fail!!! \n");
		return;
	}


	// 中间业务库
	fn_InitEnvironment = (defIF_InitEnvironment)::GetProcAddress(gd_dll, "IF_InitEnvironment");
	fn_InitEnvironment3 = (defIF_InitEnvironment3)::GetProcAddress(gd_dll, "IF_InitEnvironment3");
	fn_Destroy = (defIF_Destroy)::GetProcAddress(gd_dll, "IF_Destroy");
	fn_GetLastErrorDesc = (defIF_GetLastErrorDesc)::GetProcAddress(gd_dll, "IF_GetLastErrorDesc");
	fn_StartGetAccountInfo = (defIF_StartGetAccountInfo)::GetProcAddress(gd_dll, "IF_StartGetAccountInfo");
	fn_StopGetAccountInfo = (defIF_StopGetAccountInfo)::GetProcAddress(gd_dll, "IF_StopGetAccountInfo");
	fn_GetAccountInfo = (defIF_GetAccountInfo)::GetProcAddress(gd_dll, "IF_GetAccountInfo");
	fn_DebitMoney = (defIF_DebitMoney)::GetProcAddress(gd_dll, "IF_DebitMoney");
	fn_DebitCancel = (defIF_DebitCancel)::GetProcAddress(gd_dll, "IF_DebitCancel");
	fn_GetDebitResult = (defIF_GetDebitResult)::GetProcAddress(gd_dll, "IF_GetDebitResult");
	fn_SetMMI = (defIF_SetMMI)::GetProcAddress(gd_dll, "IF_SetMMI");
	fn_GetParam = (defIF_GetParam)::GetProcAddress(gd_dll, "IF_GetParam");
	fn_GetComponentStatus = (defIF_GetComponentStatus)::GetProcAddress(gd_dll, "IF_GetComponentStatus");
	fn_TranslateData = (defIF_TranslateData)::GetProcAddress(gd_dll, "IF_TranslateData");

	// 特情相关
	fn_EventInitEnvironment3 = (IF_EventInitEnvironment3)::GetProcAddress(gd_dll, "IF_EventInitEnvironment3");
	fn_EventDestroy = (IF_EventDestroy)::GetProcAddress(gd_dll, "IF_EventDestroy");
	fn_EventGetLastErrorDesc = (IF_EventGetLastErrorDesc)::GetProcAddress(gd_dll, "IF_EventGetLastErrorDesc");
	fn_EventGetComponentStatus = (IF_EventGetComponentStatus)::GetProcAddress(gd_dll, "IF_EventGetComponentStatus");
	fn_EventDealStart = (IF_EventDealStart)::GetProcAddress(gd_dll, "IF_EventDealStart");
	fn_EventCheckVehQueue = (IF_EventCheckVehQueue)::GetProcAddress(gd_dll, "IF_EventCheckVehQueue");
	fn_EventCheckVehInfo = (IF_EventCheckVehInfo)::GetProcAddress(gd_dll, "IF_EventCheckVehInfo");
	fn_EventCheckEntryInfo = (IF_EventCheckEntryInfo)::GetProcAddress(gd_dll, "IF_EventCheckEntryInfo");
	fn_EventShowFeeInfo = (IF_EventShowFeeInfo)::GetProcAddress(gd_dll, "IF_EventShowFeeInfo");
	fn_EventPayResultDisplay = (IF_EventPayResultDisplay)::GetProcAddress(gd_dll, "IF_EventPayResultDisplay");
	fn_EventDealStop = (IF_EventDealStop)::GetProcAddress(gd_dll, "IF_EventDealStop");
	fn_EventStartScan = (IF_EventStartScan)::GetProcAddress(gd_dll, "IF_EventStartScan");
	fn_EventStopScan = (IF_EventStopScan)::GetProcAddress(gd_dll, "IF_EventStopScan");
	fn_EventCardOperationNotify = (IF_EventCardOperationNotify)::GetProcAddress(gd_dll, "IF_EventCardOperationNotify");
	fn_EventModifyVehQueue = (IF_EventModifyVehQueue)::GetProcAddress(gd_dll, "IF_EventModifyVehQueue");
	fn_EventFeeAuthorize = (IF_EventFeeAuthorize)::GetProcAddress(gd_dll, "IF_EventFeeAuthorize");
	fn_EventAuthorize = (IF_EventAuthorize)::GetProcAddress(gd_dll, "IF_EventAuthorize");
	fn_EventDelVehQueueResult = (IF_EventDelVehQueueResult)::GetProcAddress(gd_dll, "IF_EventDelVehQueueResult");
	//fn_EventFreeVoicePlay = (IF_EventFreeVoicePlay)::GetProcAddress(gd_dll, "IF_EventFreeVoicePlay");

	if (fn_EventInitEnvironment3 == NULL ||
		fn_EventDestroy == NULL ||
		fn_EventGetLastErrorDesc == NULL ||
		fn_EventGetComponentStatus == NULL ||
		fn_EventDealStart == NULL ||
		fn_EventCheckVehQueue == NULL ||
		fn_EventCheckVehInfo == NULL ||
		fn_EventCheckEntryInfo == NULL ||
		fn_EventShowFeeInfo == NULL ||
		fn_EventPayResultDisplay == NULL ||
		fn_EventDealStop == NULL ||
		fn_EventStartScan == NULL ||
		fn_EventStopScan == NULL ||
		fn_EventCardOperationNotify == NULL ||
		fn_EventModifyVehQueue == NULL ||
		fn_EventFeeAuthorize == NULL ||
		fn_EventAuthorize == NULL ||
		fn_EventDelVehQueueResult == NULL ||
		//fn_EventFreeVoicePlay == NULL ||

		fn_InitEnvironment == NULL ||
		fn_Destroy == NULL ||
		fn_GetLastErrorDesc == NULL ||
		fn_StartGetAccountInfo == NULL ||
		fn_StopGetAccountInfo == NULL ||
		fn_GetAccountInfo == NULL ||
		fn_GetLastErrorDesc == NULL ||
		fn_DebitMoney == NULL ||
		fn_DebitCancel == NULL ||
		fn_GetDebitResult == NULL ||
		fn_SetMMI == NULL ||
		fn_GetParam == NULL ||
		fn_GetComponentStatus == NULL ||
		fn_TranslateData == NULL)
	{

		log("加载TWSDNetPay.dll失败!!! \n");
		return;
	}

	log("加载TWSDNetPay.dll成功! \n");
	m_bLoaded = true;
}

void CTestToolDlg::loadDllRobot()
{
	log("开始加载TWSDNetPay.dll库...（卡机的移动支付且没有特情的）\n");

	fn_InitEnvironment = NULL;
	fn_Destroy = NULL;
	fn_GetLastErrorDesc = NULL;
	fn_StartGetAccountInfo = NULL;
	fn_StopGetAccountInfo = NULL;
	fn_GetAccountInfo = NULL;
	fn_DebitMoney = NULL;
	fn_DebitCancel = NULL;
	fn_GetDebitResult = NULL;
	fn_SetMMI = NULL;
	fn_GetParam = NULL;
	fn_GetComponentStatus = NULL;
	fn_TranslateData = NULL;

	gd_dll = ::LoadLibraryA("TWSDNetPay.dll");

	if (gd_dll == NULL)
	{
		log("load TWSDNetPay.dll失败!!! \n");
		return;
	}

	fn_InitEnvironment = (defIF_InitEnvironment)::GetProcAddress(gd_dll, "IF_InitEnvironment");
	fn_Destroy = (defIF_Destroy)::GetProcAddress(gd_dll, "IF_Destroy");
	fn_GetLastErrorDesc = (defIF_GetLastErrorDesc)::GetProcAddress(gd_dll, "IF_GetLastErrorDesc");
	fn_StartGetAccountInfo = (defIF_StartGetAccountInfo)::GetProcAddress(gd_dll, "IF_StartGetAccountInfo");
	fn_StopGetAccountInfo = (defIF_StopGetAccountInfo)::GetProcAddress(gd_dll, "IF_StopGetAccountInfo");
	fn_GetAccountInfo = (defIF_GetAccountInfo)::GetProcAddress(gd_dll, "IF_GetAccountInfo");
	fn_DebitMoney = (defIF_DebitMoney)::GetProcAddress(gd_dll, "IF_DebitMoney");
	fn_DebitCancel = (defIF_DebitCancel)::GetProcAddress(gd_dll, "IF_DebitCancel");
	fn_GetDebitResult = (defIF_GetDebitResult)::GetProcAddress(gd_dll, "IF_GetDebitResult");
	fn_SetMMI = (defIF_SetMMI)::GetProcAddress(gd_dll, "IF_SetMMI");
	fn_GetParam = (defIF_GetParam)::GetProcAddress(gd_dll, "IF_GetParam");
	fn_GetComponentStatus = (defIF_GetComponentStatus)::GetProcAddress(gd_dll, "IF_GetComponentStatus");
	fn_TranslateData = (defIF_TranslateData)::GetProcAddress(gd_dll, "IF_TranslateData");

	if (fn_InitEnvironment == NULL ||
		fn_Destroy == NULL ||
		fn_GetLastErrorDesc == NULL ||
		fn_StartGetAccountInfo == NULL ||
		fn_StopGetAccountInfo == NULL ||
		fn_GetAccountInfo == NULL ||
		fn_GetLastErrorDesc == NULL ||
		fn_DebitMoney == NULL ||
		fn_DebitCancel == NULL ||
		fn_GetDebitResult == NULL ||
		fn_SetMMI == NULL ||
		fn_GetParam == NULL ||
		fn_GetComponentStatus == NULL ||
		fn_TranslateData == NULL)
	{

		log("加载TWSDNetPay.dll失败!!! \n");
		return;
	}

	log("加载TWSDNetPay.dll成功! \n");
	m_bLoaded = true;
}

void CTestToolDlg::loadShanXiDll()
{
	log("开始加载（山西）TWSDNetPay.dll库...\n");

	fn_InitEnvironment = NULL;
	fn_InitEnvironment3 = NULL;
	fn_Destroy = NULL;
	fn_GetLastErrorDesc = NULL;
	fn_StartGetAccountInfo = NULL;
	fn_StopGetAccountInfo = NULL;
	fn_GetAccountInfo = NULL;
	fn_DebitMoney = NULL;
	fn_DebitCancel = NULL;
	fn_GetDebitResult = NULL;
	fn_SetMMI = NULL;
	fn_GetParam = NULL;
	fn_GetComponentStatus = NULL;
	fn_TranslateData = NULL;

	fn_EventInitEnvironment3 = NULL;
	fn_EventDestroy = NULL;
	fn_EventGetLastErrorDesc = NULL;
	fn_EventGetComponentStatus = NULL;
	fn_EventDealStart = NULL;
	fn_EventCheckVehQueue = NULL;
	fn_EventCheckVehInfo = NULL;
	fn_EventCheckEntryInfo = NULL;
	fn_EventShowFeeInfo = NULL;
	fn_EventPayResultDisplay = NULL;
	fn_EventDealStop = NULL;
	fn_EventStartScan = NULL;
	fn_EventStopScan = NULL;
	fn_EventCardOperationNotify = NULL;
	fn_EventModifyVehQueue = NULL;
	fn_EventFeeAuthorize = NULL;
	fn_EventAuthorize = NULL;
	fn_EventDelVehQueueResult = NULL;
	fn_EventFreeVoicePlay = NULL;

	gd_dll = ::LoadLibraryA("TWSDNetPay.dll");

	if (gd_dll == NULL)
	{
		log("load TWSDNetPay.dll fail!!! \n");
		return;
	}


	// 中间业务库
	fn_InitEnvironment = (defIF_InitEnvironment)::GetProcAddress(gd_dll, "IF_InitEnvironment");
	fn_InitEnvironment3 = (defIF_InitEnvironment3)::GetProcAddress(gd_dll, "IF_InitEnvironment3");
	fn_Destroy = (defIF_Destroy)::GetProcAddress(gd_dll, "IF_Destroy");
	fn_GetLastErrorDesc = (defIF_GetLastErrorDesc)::GetProcAddress(gd_dll, "IF_GetLastErrorDesc");
	fn_StartGetAccountInfo = (defIF_StartGetAccountInfo)::GetProcAddress(gd_dll, "IF_StartGetAccountInfo");
	fn_StopGetAccountInfo = (defIF_StopGetAccountInfo)::GetProcAddress(gd_dll, "IF_StopGetAccountInfo");
	fn_GetAccountInfo = (defIF_GetAccountInfo)::GetProcAddress(gd_dll, "IF_GetAccountInfo");
	fn_DebitMoney = (defIF_DebitMoney)::GetProcAddress(gd_dll, "IF_DebitMoney");
	fn_DebitCancel = (defIF_DebitCancel)::GetProcAddress(gd_dll, "IF_DebitCancel");
	fn_GetDebitResult = (defIF_GetDebitResult)::GetProcAddress(gd_dll, "IF_GetDebitResult");
	fn_SetMMI = (defIF_SetMMI)::GetProcAddress(gd_dll, "IF_SetMMI");
	fn_GetParam = (defIF_GetParam)::GetProcAddress(gd_dll, "IF_GetParam");
	fn_GetComponentStatus = (defIF_GetComponentStatus)::GetProcAddress(gd_dll, "IF_GetComponentStatus");
	fn_TranslateData = (defIF_TranslateData)::GetProcAddress(gd_dll, "IF_TranslateData");

	// 特情相关
	fn_EventInitEnvironment3 = (IF_EventInitEnvironment3)::GetProcAddress(gd_dll, "IF_EventInitEnvironment3");
	fn_EventDestroy = (IF_EventDestroy)::GetProcAddress(gd_dll, "IF_EventDestroy");
	fn_EventGetLastErrorDesc = (IF_EventGetLastErrorDesc)::GetProcAddress(gd_dll, "IF_EventGetLastErrorDesc");
	fn_EventGetComponentStatus = (IF_EventGetComponentStatus)::GetProcAddress(gd_dll, "IF_EventGetComponentStatus");
	fn_EventDealStart = (IF_EventDealStart)::GetProcAddress(gd_dll, "IF_EventDealStart");
	fn_EventCheckVehQueue = (IF_EventCheckVehQueue)::GetProcAddress(gd_dll, "IF_EventCheckVehQueue");
	fn_EventCheckVehInfo = (IF_EventCheckVehInfo)::GetProcAddress(gd_dll, "IF_EventCheckVehInfo");
	fn_EventCheckEntryInfo = (IF_EventCheckEntryInfo)::GetProcAddress(gd_dll, "IF_EventCheckEntryInfo");
	fn_EventShowFeeInfo = (IF_EventShowFeeInfo)::GetProcAddress(gd_dll, "IF_EventShowFeeInfo");
	fn_EventPayResultDisplay = (IF_EventPayResultDisplay)::GetProcAddress(gd_dll, "IF_EventPayResultDisplay");
	fn_EventDealStop = (IF_EventDealStop)::GetProcAddress(gd_dll, "IF_EventDealStop");
	fn_EventStartScan = (IF_EventStartScan)::GetProcAddress(gd_dll, "IF_EventStartScan");
	fn_EventStopScan = (IF_EventStopScan)::GetProcAddress(gd_dll, "IF_EventStopScan");
	fn_EventCardOperationNotify = (IF_EventCardOperationNotify)::GetProcAddress(gd_dll, "IF_EventCardOperationNotify");
	fn_EventModifyVehQueue = (IF_EventModifyVehQueue)::GetProcAddress(gd_dll, "IF_EventModifyVehQueue");
	fn_EventFeeAuthorize = (IF_EventFeeAuthorize)::GetProcAddress(gd_dll, "IF_EventFeeAuthorize");
	fn_EventAuthorize = (IF_EventAuthorize)::GetProcAddress(gd_dll, "IF_EventAuthorize");
	fn_EventDelVehQueueResult = (IF_EventDelVehQueueResult)::GetProcAddress(gd_dll, "IF_EventDelVehQueueResult");
	fn_EventFreeVoicePlay = (IF_EventFreeVoicePlay)::GetProcAddress(gd_dll, "IF_EventFreeVoicePlay");

	if (fn_EventInitEnvironment3 == NULL ||
		fn_EventDestroy == NULL ||
		fn_EventGetLastErrorDesc == NULL ||
		fn_EventGetComponentStatus == NULL ||
		fn_EventDealStart == NULL ||
		fn_EventCheckVehQueue == NULL ||
		fn_EventCheckVehInfo == NULL ||
		fn_EventCheckEntryInfo == NULL ||
		fn_EventShowFeeInfo == NULL ||
		fn_EventPayResultDisplay == NULL ||
		fn_EventDealStop == NULL ||
		fn_EventStartScan == NULL ||
		fn_EventStopScan == NULL ||
		fn_EventCardOperationNotify == NULL ||
		fn_EventModifyVehQueue == NULL ||
		fn_EventFeeAuthorize == NULL ||
		fn_EventAuthorize == NULL ||
		fn_EventDelVehQueueResult == NULL ||
		fn_EventFreeVoicePlay == NULL ||

		fn_InitEnvironment == NULL ||
		fn_InitEnvironment3 == NULL ||
		fn_Destroy == NULL ||
		fn_GetLastErrorDesc == NULL ||
		fn_StartGetAccountInfo == NULL ||
		fn_StopGetAccountInfo == NULL ||
		fn_GetAccountInfo == NULL ||
		fn_GetLastErrorDesc == NULL ||
		fn_DebitMoney == NULL ||
		fn_DebitCancel == NULL ||
		fn_GetDebitResult == NULL ||
		fn_SetMMI == NULL ||
		fn_GetParam == NULL ||
		fn_GetComponentStatus == NULL ||
		fn_TranslateData == NULL)
	{

		log("加载TWSDNetPay.dll失败!!! \n");
		return;
	}

	log("加载TWSDNetPay.dll成功! \n");
	m_bLoaded = true;
}

bool CTestToolDlg::loadDllSucceed()
{
	if (!m_bLoaded)
		log("未加载共享库!");

	return m_bLoaded;
}

bool CTestToolDlg::initNetPayEnvSucceed()
{
	if (!m_bNetPayEnvInitedFlag)
		log("未初始化支付环境!");

	return m_bNetPayEnvInitedFlag;
}

bool CTestToolDlg::initEtcEventEnvSucceed()
{
	if (!m_bEtcEventEnvInitedFlag)
		log("未初始化特情环境!");

	return m_bEtcEventEnvInitedFlag;
}

CString CTestToolDlg::GetCurrentDirNew()
{

	TCHAR szfile[255] = _T("");
	CString csDir = _T("");

	//对于应用程序（调用者），此方式正确。对于动态库（被调用者），此方式获取的是程序路径
	DWORD dwLen = GetModuleFileName(NULL, szfile, 255);

	//调整为动态库自身的路径
	csDir.Format(_T("%s"), szfile);
	csDir = csDir.Left(csDir.ReverseFind(_T('\\')));
	return csDir;
}


void CTestToolDlg::GetUIDebitInfo()
{
	//int iDebitMoney;
	TCHAR ch[32] = { 0 };
	//GetDlgItem(IDC_EDIT_Money)->GetWindowText(ch, 32);
	//m_iDebitMoney = _tstoi(ch);

	CString strMoney;
	GetDlgItemText(IDC_EDIT_Money, strMoney);
	m_iDebitMoney = _ttoi(strMoney);

	//int iAxisNum;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_axisnum)->GetWindowText(ch, 32);
	m_iAxisNum = _ttoi(ch);

	//int iWeight;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_Weight)->GetWindowText(ch, 32);
	m_iWeight = _ttoi(ch);

	//int iTimeOut;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_timeout)->GetWindowText(ch, 32);
	m_iTimeOut = _ttoi(ch);

	//int iBalance;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_balance)->GetWindowText(ch, 32);
	m_iBalance = _ttoi(ch);

	m_iVType = 1;
	m_iVClass = 1;
	m_iVLColor = 0;


	//CString strPlate;
	GetDlgItemText(IDC_EDIT_plate, m_strPlate);
	//CString strEntry;
	GetDlgItemText(IDC_EDIT_entry, m_strEntry);
	//CString strOperator;
	GetDlgItemText(IDC_EDIT_operator, m_strOperator);
	//CString strOperatotID;	
	GetDlgItemText(IDC_EDIT_operatorID, m_strOperatotID);
	//CString strCardNo; 
	GetDlgItemText(IDC_EDIT_cardno, m_strCardNo);

	return;
}


const CString CTestToolDlg::guid(IN const int iType)
{
	CString strReturn;
	static char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		strReturn.Format(_T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}
	if (1 == iType)
	{
		strReturn.Replace(_T("-"), _T(""));
		strReturn.Replace(_T("{"), _T(""));
		strReturn.Replace(_T("}"), _T(""));
	}
	return strReturn;
}



////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：FillpPayKeyItemsStruct
//
// 功能描述：填充结构体
//
// 输入参数：IN  int _OperationMode //扣款模式
// 输入参数：IN int _PayCertificateType //支付凭证类型
// 输入参数：IN CString _PayCertificateCode  //第三方支付凭证
// 输出参数：OUT tagPayKeyItems& _ppPayKeyItems //传出结构体
// 返回值：成功 返回 TRUE，失败 返回 FALSE
// 编写人员：FORTUNE
// 编写时间：星期日 2016年11月27日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTestToolDlg::FillpPayKeyItemsStruct(IN  int _OperationMode, IN int _PayCertificateType, IN CString _PayCertificateCode, OUT tagPayKeyItems& _ppPayKeyItems)
{

	BOOL bRet = FALSE;
	if (NULL != &_ppPayKeyItems)
	{
		_ppPayKeyItems.OverTime = m_iTimeOut * 1000;
		_ppPayKeyItems.OperationMode = _OperationMode;
		_ppPayKeyItems.PayIdentifier = guid() + "1231";
		m_strLastPayIdentifier = _ppPayKeyItems.PayIdentifier;
		_ppPayKeyItems.Money = m_iDebitMoney;
		_ppPayKeyItems.SubTime = CTime::GetCurrentTime();
		_ppPayKeyItems.DebitMainType = 0x01;
		_ppPayKeyItems.PayCertificateCode = _PayCertificateCode;
		_ppPayKeyItems.PayCertificateType = _PayCertificateType;

		_ppPayKeyItems.VehicleType = m_iVType;
		_ppPayKeyItems.VehicleClass = m_iVClass;
		_ppPayKeyItems.VLColor = m_iVLColor;
		_ppPayKeyItems.VehicleLicense = m_strPlate;
		_ppPayKeyItems.TotalWeight = m_iWeight;

		_ppPayKeyItems.AxisCount = m_iAxisNum;

		_ppPayKeyItems.PassCertificateType = 0x01;
		_ppPayKeyItems.CardID = m_strCardNo;//_T("12345678");//90123456789000000000      ");
		CString strTemp;
		strTemp.Format(_T("%04d%02d%02d"), CTime::GetCurrentTime().GetYear(), CTime::GetCurrentTime().GetMonth(), CTime::GetCurrentTime().GetDay());
		_ppPayKeyItems.TollDate = _ttoi(strTemp);
		_ppPayKeyItems.ShiftID = 0x01;

		_ppPayKeyItems.EntryAreaID = 49;
		_ppPayKeyItems.EntryRoadID = 49;
		_ppPayKeyItems.EntryStationID = 43011288;
		_ppPayKeyItems.EntryStationName = m_strEntry;
		_ppPayKeyItems.EntryTime = CTime::GetCurrentTime() - CTimeSpan(0, 0, 10, 0);
		_ppPayKeyItems.EntryLane = 0x001;
		_ppPayKeyItems.EntryVehicleLicense = m_strPlate;
		_ppPayKeyItems.EntryVLColor = 0x00;
		_ppPayKeyItems.EntryVehicleType = 0x01;
		_ppPayKeyItems.EntryVehicleClass = 0x02;

		_ppPayKeyItems.OperatorName = m_strOperator;
		_ppPayKeyItems.OperatorID = m_strOperatotID;

		_ppPayKeyItems.AutoLicense = m_strPlate;
		_ppPayKeyItems.AutoColor = 0x00;
		_ppPayKeyItems.Distance = 0x10;
		_ppPayKeyItems.TicketNo = "123";

		//把交易识别码显示出来
		SetDlgItemText(IDC_EDIT_PayIdentifier, m_strLastPayIdentifier);

		bRet = TRUE;
	}

	return bRet;
}


CString CTestToolDlg::MyGetCurrenTime()
{
	CTime time = CTime::GetCurrentTime();
	CString curTime;
	curTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return curTime;
}


CString CTestToolDlg::GetUniqueTimeStr()
{
	CTime time = CTime::GetCurrentTime();
	CString curTime;
	curTime.Format("%04d%02d%02d%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return curTime;
}


CString CTestToolDlg::GetEventTypeName(int EventTypeNo)
{
	CString EventTypeName;

	switch (EventTypeNo)
	{
	case 1:
		EventTypeName = "未感应到标签";
		break;
	case 2:
		EventTypeName = "读标签失败";
		break;
	case 3:
		EventTypeName = "标签异常";
		break;
	case 4:
		EventTypeName = "标签黑名单";
		break;
	case 5:
		EventTypeName = "标签超过有效期";
		break;
	case 6:
		EventTypeName = "标签拆卸";
		break;
	case 7:
		EventTypeName = "写卡失败";
		break;
	case 8:
		EventTypeName = "读卡失败";
		break;
	case 9:
		EventTypeName = "卡异常";
		break;
	case 10:
		EventTypeName = "黑名单卡";
		break;
	case 11:
		EventTypeName = "卡超过有效期";
		break;
	case 12:
		EventTypeName = "卡未插好";
		break;
	case 13:
		EventTypeName = "卡签入口不一致";
		break;
	case 14:
		EventTypeName = "卡签车牌不一致";
		break;
	case 15:
		EventTypeName = "无入口信息";
		break;
	case 16:
		EventTypeName = "入口无效";
		break;
	case 17:
		EventTypeName = "余额不足";
		break;
	case 18:
		EventTypeName = "计费异常";
		break;
	case 19:
		EventTypeName = "运营禁行";
		break;
	case 100:
		EventTypeName = "其他特情";
		break;
	default:
		EventTypeName = "其他特情";
	}

	return EventTypeName;
}

CString CTestToolDlg::GetEventHandleMethodName(int EventHandleMethodNo)
{
	CString EventHandleMethodName;
	switch (EventHandleMethodNo)
	{
	case 1:
		EventHandleMethodName = "终端处理";
		break;
	case 2:
		EventHandleMethodName = "特情误报";
		break;
	case 3:
		EventHandleMethodName = "引出车道";
		break;
	case 100:
		EventHandleMethodName = "其他处理方式";
		break;
	default:
		EventHandleMethodName = "终端处理";
	}

	return EventHandleMethodName;
}

bool CTestToolDlg::ParseVehInfoCheckResp(char *pBuffer)
{
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckVehInfoResp;
	Json::Value  Data;

	if (!reader.parse(pBuffer, root))
	{
		log("车辆信息确认应答json解析错误!");
	}

	if (root["EventCheckVehInfoResp"].type() != Json::nullValue)
	{
		EventCheckVehInfoResp = root["EventCheckVehInfoResp"];
	}
	else
	{
		log("车辆信息确认应答json解析错误,无EventCheckVehInfoResp对象!");
		return false;
	}

	if (EventCheckVehInfoResp["Data"].type() != Json::nullValue)
	{
		Data = EventCheckVehInfoResp["Data"];
	}
	else
	{
		log("车辆信息确认应答json解析错误,无Data对象!");
		return false;
	}

	CString tmpStr = Data["VehPlate"].asCString();
	if (tmpStr.IsEmpty() == false)
		m_CurrentVehPlate = Data["VehPlate"].asCString();
	int tmpN = Data["VehPlateColor"].asInt();
	if (tmpN != -1)
		m_CurrentVehPlateColor = Data["VehPlateColor"].asInt();
	tmpN = Data["VehClass"].asInt();
	if (tmpN != -1)
		m_CurrentVehClass = Data["VehClass"].asInt();
	tmpN = Data["VehType"].asInt();
	if (tmpN != -1)
		m_CurrentVehType = Data["VehType"].asInt();
	tmpN = Data["AxisCount"].asInt();
	if (tmpN != -1)
		m_CurrentAxisCount = Data["AxisCount"].asInt();
	tmpStr = Data["EntryTime"].asCString();
	if (tmpStr.IsEmpty() == false)
		m_CurrentEntryTime = Data["EntryTime"].asCString();

	CString LogStr;
	LogStr.Format("收到车辆信息确认应答,更新当前特情车辆信息：VehPlate:%s,VehPlateColor:%d,VehClass:%d,VehType:%d,AxisCount:%d,EntryTime:%s", m_CurrentVehPlate.GetBuffer(0), m_CurrentVehPlateColor, m_CurrentVehClass, m_CurrentVehType, m_CurrentAxisCount, m_CurrentEntryTime.GetBuffer());
	std::string logstr(LogStr.GetBuffer(0));
	log(logstr);


	return true;
}



UINT CTestToolDlg::GuiPlay1ThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUI循环播放1开始启动...");

	while (true)
	{
		dlg->log("GUI循环播放1: 特情开始(卡未插好)");
		dlg->GuiPlay1Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放1: 请刷卡");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放1: 费额展示");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放1: 刷卡支付成功");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放1: 特情结束");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}

	return 0;
}

UINT CTestToolDlg::GuiPlay2ThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUI循环播放2开始启动...");
	while (true)
	{
		dlg->log("GUI循环播放2: 特情开始(余额不足)");
		dlg->GuiPlay2Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放2: 请刷卡");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放2: 费额展示");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放2: 移动支付成功");
		dlg->GuiPlay2PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放2: 特情结束");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}

	return 0;
}

UINT CTestToolDlg::GuiPlay3ThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUI循环播放3开始启动...");
	while (true)
	{
		dlg->log("GUI循环播放3: 特情开始(无入口信息)");
		dlg->GuiPlay3Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放3: 请刷卡");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放3: 后台正在确认入口信息");
		dlg->GuiPlay3CheckEntryInfoNone();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放3: 费额展示");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放3: 刷卡支付成功");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI循环播放3: 特情结束");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}

	return 0;
}

UINT CTestToolDlg::GuiPlayTotalThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUI总循环播放开始启动...");

	while (true)
	{
		dlg->log("GUI总循环播放: 特情开始(卡未插好)");
		dlg->GuiPlay1Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 请刷卡");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 费额展示");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 刷卡支付成功");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 特情结束");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);


		dlg->log("GUI总循环播放: 特情开始(余额不足)");
		dlg->GuiPlay2Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 请刷卡");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 费额展示");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 移动支付成功");
		dlg->GuiPlay2PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 特情结束");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);


		dlg->log("GUI总循环播放: 特情开始(无入口信息)");
		dlg->GuiPlay3Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 请刷卡");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 后台正在确认入口信息");
		dlg->GuiPlay3CheckEntryInfoNone();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 费额展示");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 刷卡支付成功");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI总循环播放: 特情结束");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}
}

/************************************界面循环播放*********************************************/

void CTestToolDlg::GuiPlay1Eventstart()
{
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// 标识详细Data数据项
	Json::Value Data;
	CString str1 = "豫A123";
	CString temp;
	VehPlateTail++;
	temp.Format(_T("%d"), VehPlateTail);
	Data["VehPlate"] = Json::Value(str1 + temp);
	m_CurrentVehPlate = str1 + temp;
	Data["VehPlateColor"] = Json::Value(1);
	m_CurrentVehPlateColor = 1;
	Data["VehClass"] = Json::Value(1);
	m_CurrentVehClass = 1;
	Data["VehType"] = Json::Value(1);
	m_CurrentVehType = 1;
	Data["VehSeatCount"] = Json::Value(5);
	int m_CurrentAxisCount = 2;
	Data["axleCount"] = Json::Value(2);
	Data["CardNum"] = Json::Value("123456");
	Data["VehSeatCount"] = Json::Value(5);

	Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
	Data["EventDescribe"] = Json::Value("卡未插好");
	Data["EventTypeNo"] = Json::Value(12);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(12));

	EventDealStart["Data"] = Data;
	root["EventDealStart"] = EventDealStart;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("特情事件处理开始:");
	log(str);

	//GBK编码
	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);

	return;
}



void CTestToolDlg::GuiPlay2Eventstart()
{
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// 标识详细Data数据项
	Json::Value Data;
	CString str1 = "豫A123";
	CString temp;
	VehPlateTail++;
	temp.Format(_T("%d"), VehPlateTail);
	Data["VehPlate"] = Json::Value(str1 + temp);
	m_CurrentVehPlate = str1 + temp;
	Data["VehPlateColor"] = Json::Value(1);
	m_CurrentVehPlateColor = 1;
	Data["VehClass"] = Json::Value(1);
	m_CurrentVehClass = 1;
	Data["VehType"] = Json::Value(1);
	m_CurrentVehType = 1;
	Data["VehSeatCount"] = Json::Value(7);
	int m_CurrentAxisCount = 2;
	Data["axleCount"] = Json::Value(2);
	Data["CardNum"] = Json::Value("123456");
	Data["VehSeatCount"] = Json::Value(7);

	Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
	Data["EventDescribe"] = Json::Value("余额不足");
	Data["EventTypeNo"] = Json::Value(17);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(17));

	EventDealStart["Data"] = Data;
	root["EventDealStart"] = EventDealStart;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("特情事件处理开始:");
	log(str);

	//GBK编码
	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestToolDlg::GuiPlay3Eventstart()
{
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// 标识详细Data数据项
	Json::Value Data;
	CString str1 = "豫A123";
	CString temp;
	VehPlateTail++;
	temp.Format(_T("%d"), VehPlateTail);
	Data["VehPlate"] = Json::Value(str1 + temp);
	//Data["VehPlate"] = Json::Value(str1);
	m_CurrentVehPlate = str1 + temp;
	//m_CurrentVehPlate = str1;
	Data["VehPlateColor"] = Json::Value(1);
	m_CurrentVehPlateColor = 1;
	Data["VehClass"] = Json::Value(1);
	m_CurrentVehClass = 1;
	Data["VehType"] = Json::Value(1);
	m_CurrentVehType = 1;
	Data["VehSeatCount"] = Json::Value(7);
	int m_CurrentAxisCount = 2;
	Data["axleCount"] = Json::Value(2);
	Data["CardNum"] = Json::Value("123456");
	Data["VehSeatCount"] = Json::Value(7);

	Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
	Data["EventDescribe"] = Json::Value("无入口信息");
	Data["EventTypeNo"] = Json::Value(15);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(15));

	EventDealStart["Data"] = Data;
	root["EventDealStart"] = EventDealStart;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("特情事件处理开始:");
	log(str);

	//GBK编码
	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);

	return;
}

void CTestToolDlg::GuiPlayXSwipeCardNotify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value CardOperationNotify;
	CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
	CardOperationNotify["MessageCode"] = Json::Value("009");

	Json::Value Data;
	Data["NotifyType"] = Json::Value(1);
	CardOperationNotify["Data"] = Data;

	root["CardOperationNotify"] = CardOperationNotify;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);

	return;
}

void CTestToolDlg::GuiPlayXShowFeeInfo()
{
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventShowFeeInfo;
	EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
	EventShowFeeInfo["MessageCode"] = Json::Value("005");

	EventShowFeeInfo["PayType"] = Json::Value(1);
	EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
	EventShowFeeInfo["FeeCount"] = Json::Value(2);

	Json::Value Array;
	Json::Value Item;
	Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
	Item["PathInfo"] = Json::Value("0729-0730-0731");
	Item["Fee"] = Json::Value("3000");
	Item["ChargeType"] = Json::Value(2);
	Item["ItemNo"] = Json::Value(1);
	Array.append(Item);

	Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
	Item["PathInfo"] = Json::Value("0729-0730-0731");
	Item["Fee"] = Json::Value("4000");
	Item["ChargeType"] = Json::Value(2);
	Item["ItemNo"] = Json::Value(2);
	Array.append(Item);
	EventShowFeeInfo["FeeInfo"] = Array;

	EventShowFeeInfo["TotalVehNum"] = Json::Value(4);
	Json::Value VehQueue;
	Json::Value VehItem;
	VehItem["Index"] = Json::Value(0);
	VehItem["VehPlate"] = Json::Value("豫A121121");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(0);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(1);
	VehItem["VehPlate"] = Json::Value("豫A122122");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(0);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(2);
	VehItem["VehPlate"] = Json::Value("豫A123123");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(1);
	VehItem["FailReason"] = Json::Value("余额不足");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(3);
	VehItem["VehPlate"] = Json::Value("豫A124124");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(2);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);
	EventShowFeeInfo["VehQueue"] = VehQueue;

	root["EventShowFeeInfo"] = EventShowFeeInfo;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestToolDlg::GuiPlay13PayResultShow()
{
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventPayResultShow;
	EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
	EventPayResultShow["MessageCode"] = Json::Value("006");

	//EventInfo
	Json::Value EventInfo;
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["PayResult"] = Json::Value(0);
	EventInfo["Describe"] = Json::Value("刷卡支付成功");
	EventInfo["TimeLimit"] = Json::Value("360");
	EventPayResultShow["EventInfo"] = EventInfo;

	root["EventPayResultShow"] = EventPayResultShow;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();
	fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestToolDlg::GuiPlay2PayResultShow()
{
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventPayResultShow;
	EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
	EventPayResultShow["MessageCode"] = Json::Value("006");

	//EventInfo
	Json::Value EventInfo;
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["PayResult"] = Json::Value(0);
	EventInfo["Describe"] = Json::Value("移动支付成功");
	EventInfo["TimeLimit"] = Json::Value("360");
	EventPayResultShow["EventInfo"] = EventInfo;

	root["EventPayResultShow"] = EventPayResultShow;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();
	fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestToolDlg::GuiPlay3CheckEntryInfoNone()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventCheckEntryInfo;
	EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
	EventCheckEntryInfo["MessageCode"] = Json::Value("005");
	CString str1 = "豫A123";
	CString temp;
	temp.Format(_T("%d"), VehPlateTail);
	EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);

	//EventInfo
	Json::Value EventInfo;
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
	EventInfo["MMIDemand"] = Json::Value("交互诉求");
	EventInfo["TimeLimit"] = Json::Value("360");

	EventCheckEntryInfo["EventInfo"] = EventInfo;

	Json::Value Array;
	EventCheckEntryInfo["EntryInfo"] = Array;

	root["EventCheckEntryInfo"] = EventCheckEntryInfo;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());
	int iSize = strJson.GetLength();

	fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);

	return;
}

void CTestToolDlg::GuiPlayXEventstop()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStop;
	EventDealStop["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStop["MessageCode"] = Json::Value("006");

	//EventEndCases
	Json::Value EventEndCases;
	EventEndCases["EventVehPlate"] = Json::Value(m_CurrentVehPlate);
	EventEndCases["EventEndTime"] = Json::Value(MyGetCurrenTime());
	EventEndCases["EventDescribe"] = Json::Value("车辆无入口信息");
	EventEndCases["EventEndReason"] = Json::Value("特情处理完成，支付成功");
	EventEndCases["EventHandleMethodNo"] = Json::Value(m_EventHandleMethodNo);
	EventEndCases["EventHandleMethodName"] = Json::Value(GetEventHandleMethodName(m_EventHandleMethodNo));
	EventDealStop["EventEndCases"] = EventEndCases;

	root["EventDealStop"] = EventDealStop;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStop(strJson.GetBuffer(0), iSize, 2);

	return;
}

LRESULT CTestToolDlg::OnMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam)
{
	int result = (int)wParam;
	switch (result)
	{
	case 601:
		log(">>>>>收到特情处理开始应答: " + std::string((char *)m_pIndPtr));
		break;
	case 602:
		log(">>>>>收到车辆队列确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 603:
		log(">>>>>收到车辆信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 604:
		log(">>>>>收到车辆入口信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 605:
		log(">>>>>收到费额显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 606:
		log(">>>>>收到支付状态显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 607:
		log(">>>>>收到特情结束应答: " + std::string((char *)m_pIndPtr));
		break;
	case 608:
		log(">>>>>收到扫描二维码数据应答: " + std::string((char *)m_pIndPtr));
		break;
	case 609:
		log(">>>>>收到停止寻卡应答");
		break;
	case 610:
		log(">>>>>收到主动核单应答: " + std::string((char *)m_pIndPtr));
		break;
	case 611:
		log(">>>>>收到ETC卡操作提示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 612:
		log(">>>>>收到修改车辆队列应答: " + std::string((char *)m_pIndPtr));
		break;
	case 613:
		log(">>>>>收到停止扫码应答");
		break;
	case 616:
		log(">>>>>收到其他授权应答: " + std::string((char *)m_pIndPtr));
		break;
	}
	if (result == 603)
	{
		char *strGBK = U2G((char *)m_pIndPtr);
		bool bRet;
		bRet = ParseVehInfoCheckResp(strGBK);
		if (bRet)
			log("解析车辆信息确认应答成功");
		else
			log("解析车辆信息确认应答失败!");

		delete strGBK;
	}

	//::PostMessage(WM_THREAD_DEVICE_START_GET_DATA, (WPARAM)0, (LPARAM)0);

	return 0;
}

LRESULT CTestToolDlg::OnHWNDMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam)
{
	unsigned int result = (int)wParam;
	unsigned short FunCode = LOWORD(result);
	unsigned short SubCode = HIWORD(result);
	//int SubCode = result;
	CString strTip;
	strTip.Format(_T("功能代码(低2位):%d 执行状态(高2位):%d"), FunCode, SubCode);
	log(strTip.GetBuffer(0));

	switch (SubCode)
	{
	case 601:
		log(">>>>>收到特情处理开始应答: " + std::string((char *)m_pIndPtr));
		break;
	case 602:
		log(">>>>>收到车辆队列确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 603:
		log(">>>>>收到车辆信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 604:
		log(">>>>>收到车辆入口信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 605:
		log(">>>>>收到费额显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 606:
		log(">>>>>收到支付状态显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 607:
		log(">>>>>收到特情结束应答: " + std::string((char *)m_pIndPtr));
		break;
	case 608:
		log(">>>>>收到扫描二维码数据应答: " + std::string((char *)m_pIndPtr));
		break;
	case 609:
		log(">>>>>收到停止寻卡应答");
		break;
	case 610:
		log(">>>>>收到主动核单应答: " + std::string((char *)m_pIndPtr));
		break;
	case 611:
		log(">>>>>收到ETC卡操作提示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 612:
		log(">>>>>收到修改车辆队列应答: " + std::string((char *)m_pIndPtr));
		break;
	case 613:
		log(">>>>>收到最小费额授权应答: " + std::string((char *)m_pIndPtr));
		break;
	case 614:
		log(">>>>>收到设备按键主动应答: " + std::string((char *)m_pIndPtr));
		break;
	case 699:
		log(">>>>>收到停止扫码应答");
		break;
	case 616:
		log(">>>>>收到其他授权应答: " + std::string((char *)m_pIndPtr));
		break;
	case 617:
		log(">>>>>收到设备删除车辆队列主动应答: " + std::string((char *)m_pIndPtr));
		break;
	case 618:
		log(">>>>>收到设备删除车辆队列结果应答: " + std::string((char *)m_pIndPtr));
		break;
	}
	if (result == 603)
	{
		char *strGBK = U2G((char *)m_pIndPtr);
		bool bRet;
		bRet = ParseVehInfoCheckResp(strGBK);
		if (bRet)
			log("解析车辆信息确认应答成功");
		else
			log("解析车辆信息确认应答失败!");

		delete strGBK;
	}

	return 0;
}

LRESULT CTestToolDlg::OnHWNDMsgShowNetPayResultLog(WPARAM wParam, LPARAM lParam)
{
	if (wParam != 0)
	{
		//获取结果
		BYTE byFunCode = LOBYTE(LOWORD(wParam));
		BYTE byExecuteState = HIBYTE(LOWORD(wParam));

		CString strTip;
		strTip.Format(_T("功能代码:%d 执行状态:%d"), byFunCode, byExecuteState);
		log(strTip.GetBuffer(0));

		switch (byFunCode)
		{
		case 1:		//扣费回馈
		{
			if (byExecuteState == 0)
				strTip.Format(_T(">>>>>>收到移动支付 扣费结果：扣费成功"));
			else if (byExecuteState == 1)
				strTip.Format(_T(">>>>>>收到移动支付 扣费结果：正在输入密码..."));
			else
				strTip.Format(_T(">>>>>>收到移动支付 扣费结果：扣费失败"));

			log(strTip.GetBuffer(0));
		}
		break;
		case 2:  //撤单回馈
		{
			if (byExecuteState == 0)
				strTip.Format(_T(">>>>>>收到移动支付 撤单结果：撤单成功"));
			else
				strTip.Format(_T(">>>>>>收到移动支付 撤单结果：撤单失败"));

			log(strTip.GetBuffer(0));
		}
		break;
		}
	}
	return 0;
}

void CTestToolDlg::OnBnClickedButtonLoaddll()
{
	switch (m_enCurProvince)
	{
	case Jiangxi:
		log("加载江西电子发票中间件！！！");
		break;
	case HeNan:
		log("加载河南移动支付中间件！！！");
		break;
	case SiChuan:
		log("加载四川移动支付中间件！！！");
		break;
	case ShanXi:
		loadShanXiDll();
		break;
	default:
		break;
	}

	//switch (m_enMidllewareType)
	//{
	//case BaseNoEvent:
	//	loadDll();
	//	break;
	//case BaseAndEvent:
	//	loadDllEvent();
	//	break;
	//case RobotNoEvent:
	//	loadDllRobot();
	//	break;
	//default:
	//	break;
	//}
}


void CTestToolDlg::OnBnClickedButtonInitnetpayenv()
{
	if (m_bLoaded == false)
	{
		log("未加载共享库---TWSDNetPay.dll!");
		return;
	}


	CString strTemp;
	if (m_bNetPayEnvInitedFlag == true)
	{
		log(_T("TWSDNetPay.dll-----移动支付环境已初始化成功，请不要重复初始化!"));
		return;
	}

	CString strAreaInfo;
	CString strStationInfo;
	CString strLaneInfo;
	CString strServerInfo;

	strAreaInfo.Format(_T("%s#%s"), g_DevIFDlg->m_strAreaID, g_DevIFDlg->m_strRoadID);
	strStationInfo.Format(_T("%s#%s"), g_DevIFDlg->m_strStationID, g_DevIFDlg->m_strStationName);

	strLaneInfo.Format(_T("%s"), g_DevIFDlg->m_strLaneID);
	strServerInfo.Format(_T("1#%s#%d"), g_DevIFDlg->m_strServerIP, g_DevIFDlg->m_wServerPort);

	bool bRet = false;

	//线程接收应答
	switch (m_enCurProvince)
	{
	case Jiangxi:
	case HeNan:
	case SiChuan:
		bRet = fn_InitEnvironment(ReceiveMsgThreadID, 0, WM_THREAD_NETPAY_MESSAGE_SHOWLOG, strAreaInfo, strStationInfo, strLaneInfo, strServerInfo, g_DevIFDlg->m_iProvinceID);
		break;
	case ShanXi:
	case HuNan:
		bRet = fn_InitEnvironment3(0, 0, NULL, NULL, NULL, NULL, 41, NULL, MyNetPayCallBackFun);
		break;
	}

	if (bRet == false)
	{
		log(_T("TWSDNetPay.dll-----移动支付环境初始化失败！"));
		m_bNetPayEnvInitedFlag = false;
	}
	else
	{
		log(_T("TWSDNetPay.dll-----移动支付环境初始化成功！"));
		m_bNetPayEnvInitedFlag = true;
	}

	return;
}

void CTestToolDlg::OnBnClickedBtndebit()
{
	if (!(loadDllSucceed() && initNetPayEnvSucceed()))
		return;

	GetUIDebitInfo();
	tagPayKeyItems* pPayKeyItems = new tagPayKeyItems();
	FillpPayKeyItemsStruct(0x01, 0x00, _T(""), *pPayKeyItems);

	CDVJsonParamDebitMoney jsonDebitMoney;
	CJsonParamOperate jsonOperation;

	jsonDebitMoney.OverTime = pPayKeyItems->OverTime;
	jsonDebitMoney.OperationMode = pPayKeyItems->OperationMode;
	jsonDebitMoney.PayIdentifier = pPayKeyItems->PayIdentifier;
	jsonDebitMoney.Money = pPayKeyItems->Money;
	jsonDebitMoney.SubTime = pPayKeyItems->SubTime;
	jsonDebitMoney.DebitMainType = pPayKeyItems->DebitMainType;
	jsonDebitMoney.PayCertificateCode = pPayKeyItems->PayCertificateCode;
	jsonDebitMoney.PayCertificateType = pPayKeyItems->PayCertificateType;
	jsonDebitMoney.VehicleType = pPayKeyItems->VehicleType;
	jsonDebitMoney.VehicleClass = pPayKeyItems->VehicleClass;
	jsonDebitMoney.VehicleLicense = pPayKeyItems->VehicleLicense;
	jsonDebitMoney.VLColor = pPayKeyItems->VLColor;
	jsonDebitMoney.AxisCount = pPayKeyItems->AxisCount;
	jsonDebitMoney.Weight = pPayKeyItems->TotalWeight;
	jsonDebitMoney.LimitWeight = pPayKeyItems->LimitWeight;
	jsonDebitMoney.PassCertificateType = pPayKeyItems->PassCertificateType;
	//jsonDebitMoney.PassCertificateType	= 3;
	jsonDebitMoney.CardID = pPayKeyItems->CardID;
	jsonDebitMoney.TollDate = pPayKeyItems->TollDate;
	jsonDebitMoney.ShiftID = pPayKeyItems->ShiftID;
	jsonDebitMoney.EntryStationID = pPayKeyItems->EntryStationID;
	jsonDebitMoney.EntryStationName = pPayKeyItems->EntryStationName;
	jsonDebitMoney.EntryTime = pPayKeyItems->EntryTime;
	jsonDebitMoney.EntryLane = pPayKeyItems->EntryLane;
	jsonDebitMoney.EntryVehicleLicense = pPayKeyItems->EntryVehicleLicense;
	jsonDebitMoney.EntryVLColor = pPayKeyItems->EntryVLColor;
	jsonDebitMoney.EntryVehicleClass = pPayKeyItems->EntryVehicleClass;
	jsonDebitMoney.EntryVehicleType = pPayKeyItems->EntryVehicleType;
	jsonDebitMoney.EntryRoadID = pPayKeyItems->EntryRoadID;
	jsonDebitMoney.EntryAreaID = pPayKeyItems->EntryAreaID;

	jsonDebitMoney.OperatorID = pPayKeyItems->OperatorID;
	jsonDebitMoney.OperatorName = pPayKeyItems->OperatorName;
	jsonDebitMoney.AutoColor = pPayKeyItems->AutoColor;
	jsonDebitMoney.AutoLicense = pPayKeyItems->AutoLicense;
	jsonDebitMoney.Distance = pPayKeyItems->Distance;
	jsonDebitMoney.AutoPrint = 1;

	CString strDataString;
	strDataString = jsonOperation.FormatParamData2Json(&jsonDebitMoney);

	log("扣费json参数:");
	log(strDataString.GetBuffer(0));

	int iSize = strDataString.GetLength();
	int DataParamType = 2;
	fn_DebitMoney(strDataString.GetBuffer(0), iSize, DataParamType);

	strDataString.ReleaseBuffer();
	delete pPayKeyItems;
	pPayKeyItems = NULL;
}


void CTestToolDlg::OnBnClickedBtnCanceldebit()
{
	if (!(loadDllSucceed() && initNetPayEnvSucceed()))
		return;
	
	tagPayKeyItems* pPayKeyItems = new tagPayKeyItems();
	if (NULL != pPayKeyItems)
	{
		pPayKeyItems->PayIdentifier.Format(_T("%s"), m_strLastPayIdentifier);

		CDVJsonParamDebitCancel jsonDebitCancel;
		CJsonParamOperate jsonOperation;
		jsonDebitCancel.PayIdentifier = pPayKeyItems->PayIdentifier;

		CString strDataString;
		strDataString = jsonOperation.FormatParamData2Json(&jsonDebitCancel);

		log("撤单json参数:");
		log(strDataString.GetBuffer(0));

		int iSize = strDataString.GetLength();
		int DataParamType = 2;
		fn_DebitCancel(strDataString.GetBuffer(0), iSize, DataParamType);
		strDataString.ReleaseBuffer();

		delete pPayKeyItems;
		pPayKeyItems = NULL;
	}
}

void CTestToolDlg::OnBnClickedBtnNetpaydevstatus()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CTestToolDlg::OnBnClickedBtnInitenv()
{
	if (m_enMidllewareType != MIDLLEWARE_TYPE::BaseAndEvent)
	{
		log(_T("该中间件没有特情处理相关的接口，返回！！！"));
		return;
	}

	if (!loadDllSucceed())
		return;

	if (m_bEtcEventEnvInitedFlag)
	{
		log(_T("EtcEvent环境已初始化成功，请不要重复初始化!"));
		return;
	}

	//配置信息从配置文件中读取
	CString	m_strAppDir;
	m_strAppDir.Format(_T("%s"), GetCurrentDirNew());
	CString	m_strConfigPathFile;
	m_strConfigPathFile.Format(_T("%s\\%s"), m_strAppDir, FILE_NAME_INIT_CONFIG);

	int m_provinceId;
	std::string m_areaId;
	std::string m_areaName = "区域名称";
	std::string m_roadId;
	std::string m_roadName = "路段名称";
	std::string m_stationId;
	std::string m_stationName;
	std::string m_laneId;
	char tmp_str[200] = { 0 };

	::GetPrivateProfileString(_T("Setup"), _T("AreaID"), "4412", tmp_str, 100, m_strConfigPathFile);
	m_areaId = tmp_str;
	::GetPrivateProfileString(_T("Setup"), _T("RoadID"), "2912", tmp_str, 100, m_strConfigPathFile);
	m_roadId = tmp_str;
	::GetPrivateProfileString(_T("Setup"), _T("StationID"), "29", tmp_str, 100, m_strConfigPathFile);
	m_stationId = tmp_str;
	::GetPrivateProfileString(_T("Setup"), _T("LaneID"), "36", tmp_str, 100, m_strConfigPathFile);
	m_laneId = tmp_str;
	::GetPrivateProfileString(_T("Setup"), _T("StationName"), "八斗", tmp_str, 100, m_strConfigPathFile);
	m_stationName = tmp_str;

	m_provinceId = ::GetPrivateProfileInt(_T("Setup"), _T("ProvinceID"), 44, m_strConfigPathFile);

	m_EventTypeNo = ::GetPrivateProfileInt(_T("EtcEventConfig"), _T("EventTypeNo"), 2, m_strConfigPathFile);
	m_EventHandleMethodNo = ::GetPrivateProfileInt(_T("EtcEventConfig"), _T("EventHandleMethodNo"), 2, m_strConfigPathFile);


	UINT nThreadID = (UINT)123;
	HWND hWnd = (HWND)444;
	char szAreaInfo[128] = { 0 };
	_snprintf_s(szAreaInfo, 127, "%s#%s#%s#%s", m_areaId.c_str(), m_areaName.c_str(), m_roadId.c_str(), m_roadName.c_str());
	log("区域信息:" + std::string(szAreaInfo));

	char szLoaclStation[48] = { 0 };
	_snprintf_s(szLoaclStation, 47, "%s#%s", m_stationId.c_str(), m_stationName.c_str());
	log("站信息:" + std::string(szLoaclStation));

	char* szServerInfo = "127.0.0.1";
	void* pIndPtr = m_pIndPtr;

	log("车道ID:" + m_laneId);
	log("省份ID:" + std::to_string(m_provinceId));

	m_bEtcEventEnvInitedFlag = fn_EventInitEnvironment3(ReceiveMsgThreadID, 0, WM_THREAD_ETCEVENT_MESSAGE_SHOWLOG, szAreaInfo, szLoaclStation, m_laneId.c_str(), szServerInfo, m_provinceId, pIndPtr, MyCallBackFun);
	m_bEtcEventEnvInitedFlag?log("ETC特情环境初始化成功!"):log("ETC特情环境初始化失败!");
}

void CTestToolDlg::OnBnClickedBtnEventdestroy()
{
	if (!loadDllSucceed())
		return;

	bool bRet = fn_EventDestroy();
	bRet ? log("ETC特情清理资源成功!") : log("ETC特情清理资源失败!");
}


void CTestToolDlg::OnBnClickedBtnEventstart()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;

	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("特情事件处理开始(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventDealStart;
		CString TimeStr = GetUniqueTimeStr();
		std::string timestr(TimeStr.GetBuffer());
		m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
		EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
		EventDealStart["MessageCode"] = Json::Value("001");

		// 标识详细Data数据项
		Json::Value Data;
		CString str1 = "粤A123";
		CString temp;
		VehPlateTail++;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		m_CurrentVehPlate = str1 + temp;
		Data["VehPlateColor"] = Json::Value(1);
		m_CurrentVehPlateColor = 1;
		Data["VehClass"] = Json::Value(1);
		m_CurrentVehClass = 1;
		Data["VehType"] = Json::Value(1);
		m_CurrentVehType = 1;
		Data["VehSeatCount"] = Json::Value(7);
		int m_CurrentAxisCount = 2;
		Data["axleCount"] = Json::Value(2);
		Data["CardNum"] = Json::Value("123456");
		Data["VehSeatCount"] = Json::Value(7);
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("无入口信息");
		Data["EventTypeNo"] = Json::Value(m_EventTypeNo);
		Data["EventTypeName"] = Json::Value(GetEventTypeName(m_EventTypeNo));

		EventDealStart["Data"] = Data;
		root["EventDealStart"] = EventDealStart;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("特情事件处理开始:");
		log(str);

		//GBK编码
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用特情事件处理开始接口返回失败!");
	}

	return;
}

void CTestToolDlg::OnBnClickedBtnCheckvehqueue()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;

	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆队列确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckVehQueue;
		EventCheckVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehQueue["MessageCode"] = Json::Value("002");
		EventCheckVehQueue["CheckFlag"] = Json::Value(0);
		// 标识详细Data数据项
		Json::Value Data;
		Data["ProvinceName"] = Json::Value("广东省");
		Data["ProvinceNo"] = Json::Value("44");
		Data["RoadName"] = Json::Value("广州北二环");
		Data["RoadNo"] = Json::Value("0094");
		Data["StationName"] = Json::Value("香雪收费站");
		Data["StationNo"] = Json::Value("009304");
		Data["LaneNo"] = Json::Value("1");
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		Data["VehPlateColor"] = Json::Value(1);
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("确认车辆队列是否正确");

		EventCheckVehQueue["Data"] = Data;
		root["EventCheckVehQueue"] = EventCheckVehQueue;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆队列确认特情:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}

	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventCheckVehQueue(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用车辆队列确认接口返回失败!");
	}

	return;
}

void CTestToolDlg::OnBnClickedBtnCheckvehqueuedel()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("取消车辆队列确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckVehQueue;
		EventCheckVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehQueue["MessageCode"] = Json::Value("002");
		EventCheckVehQueue["CheckFlag"] = Json::Value(1);
		// 标识详细Data数据项
		Json::Value Data;

		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		Data["VehPlateColor"] = Json::Value(1);
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("取消确认车辆队列特情");

		EventCheckVehQueue["Data"] = Data;
		root["EventCheckVehQueue"] = EventCheckVehQueue;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("取消车辆队列确认特情:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventCheckVehQueue(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用车辆队列确认接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnCheckvehinfo()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆信息确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckVehInfo;
		EventCheckVehInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehInfo["MessageCode"] = Json::Value("003");
		//EventInfo
		Json::Value EventInfo;
		EventInfo["VehPlateSnapImage"] = Json::Value("xxx");
		EventInfo["CameraSnapImage"] = Json::Value("xxx");
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
		EventInfo["MMIDemand"] = Json::Value("交互诉求");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventCheckVehInfo["EventInfo"] = EventInfo;

		//MMIContent
		Json::Value MMIContent;
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		MMIContent["VehPlate"] = Json::Value(str1 + temp);
		MMIContent["VehPlateColor"] = Json::Value(1);
		MMIContent["VehClass"] = Json::Value(1);
		MMIContent["VehType"] = Json::Value(1);
		MMIContent["AxisCount"] = Json::Value(2);
		MMIContent["EntryTime"] = Json::Value(MyGetCurrenTime());
		EventCheckVehInfo["MMIContent"] = MMIContent;

		root["EventCheckVehInfo"] = EventCheckVehInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆信息确认特情:");
		log(str);
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckVehInfo(strJson.GetBuffer(0), iSize, 2);
	if (!bRet)
	{
		log("调用确认车辆信息接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnCheckentryinfo()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆入口信息确认(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckEntryInfo;
		EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckEntryInfo["MessageCode"] = Json::Value("005");
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);

		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
		EventInfo["MMIDemand"] = Json::Value("交互诉求");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventCheckEntryInfo["EventInfo"] = EventInfo;

		Json::Value Array;
		Json::Value Item;
		Item["StationName"] = Json::Value("香雪收费站");
		Item["StationID"] = Json::Value("G1501440010090");
		Item["StationHex"] = Json::Value("44011D0B");
		Item["laneid"] = Json::Value("0161");
		Item["laneHex"] = Json::Value("A1");
		Item["MediaType"] = Json::Value("1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		Item["StationName"] = Json::Value("长平收费站");
		Item["StationID"] = Json::Value("G1501440010091");
		Item["StationHex"] = Json::Value("44011D0C");
		Item["laneid"] = Json::Value("0162");
		Item["laneHex"] = Json::Value("A2");
		Item["MediaType"] = Json::Value("1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		EventCheckEntryInfo["EntryInfo"] = Array;

		root["EventCheckEntryInfo"] = EventCheckEntryInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆入口信息确认:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);
	if (!bRet)
	{
		log("调用确认车辆入口信息接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnCheckentryinfoNone()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆入口信息确认(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckEntryInfo;
		EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckEntryInfo["MessageCode"] = Json::Value("005");
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);

		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
		EventInfo["MMIDemand"] = Json::Value("交互诉求");
		EventInfo["TimeLimit"] = Json::Value("360");

		EventCheckEntryInfo["EventInfo"] = EventInfo;

		Json::Value Array;
		EventCheckEntryInfo["EntryInfo"] = Array;

		root["EventCheckEntryInfo"] = EventCheckEntryInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆入口信息确认:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);
	if (!bRet)
	{
		log("调用确认车辆入口信息接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnShowfeeinfosingle()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("费额显示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventShowFeeInfo;
		EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventShowFeeInfo["MessageCode"] = Json::Value("005");
		EventShowFeeInfo["PayType"] = Json::Value(1);
		EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
		EventShowFeeInfo["FeeCount"] = Json::Value(1);

		Json::Value Array;
		Json::Value Item;
		Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
		Item["PathInfo"] = Json::Value("0729-0730-0731");
		Item["Fee"] = Json::Value("3000");
		Item["ChargeType"] = Json::Value(2);
		Item["ItemNo"] = Json::Value(1);
		Array.append(Item);

		EventShowFeeInfo["FeeInfo"] = Array;

		EventShowFeeInfo["TotalVehNum"] = Json::Value(4);
		Json::Value VehQueue;
		Json::Value VehItem;
		VehItem["Index"] = Json::Value(0);
		VehItem["VehPlate"] = Json::Value("粤A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("粤A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("粤A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("余额不足");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("粤A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventShowFeeInfo["VehQueue"] = VehQueue;

		root["EventShowFeeInfo"] = EventShowFeeInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("费额显示:");
		log(str);
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用费额显示接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnShowfeeinfo()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("费额显示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventShowFeeInfo;
		EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventShowFeeInfo["MessageCode"] = Json::Value("005");

		EventShowFeeInfo["PayType"] = Json::Value(1);
		EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
		EventShowFeeInfo["FeeCount"] = Json::Value(2);

		Json::Value Array;
		Json::Value Item;
		Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
		Item["PathInfo"] = Json::Value("0729-0730-0731");
		Item["Fee"] = Json::Value("3000");
		Item["ChargeType"] = Json::Value(2);
		Item["ItemNo"] = Json::Value(1);
		Array.append(Item);

		Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
		Item["PathInfo"] = Json::Value("0729-0730-0731");
		Item["Fee"] = Json::Value("4000");
		Item["ChargeType"] = Json::Value(2);
		Item["ItemNo"] = Json::Value(2);
		Array.append(Item);
		EventShowFeeInfo["FeeInfo"] = Array;

		EventShowFeeInfo["TotalVehNum"] = Json::Value(4);
		Json::Value VehQueue;
		Json::Value VehItem;
		VehItem["Index"] = Json::Value(0);
		VehItem["VehPlate"] = Json::Value("粤A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("粤A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("粤A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("余额不足");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("粤A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventShowFeeInfo["VehQueue"] = VehQueue;

		root["EventShowFeeInfo"] = EventShowFeeInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("费额显示:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用费额显示接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnPayresultshow()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("支付状态展示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventPayResultShow;
		EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
		EventPayResultShow["MessageCode"] = Json::Value("006");
		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["PayResult"] = Json::Value(0);
		EventInfo["Describe"] = Json::Value("支付宝用户支付成功");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventPayResultShow["EventInfo"] = EventInfo;

		root["EventPayResultShow"] = EventPayResultShow;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("支付状态展示:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用支付结果展示接口返回失败!");
	}
	return;
}


void CTestToolDlg::OnBnClickedBtnPayresultshowfail()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("支付状态展示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventPayResultShow;
		EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
		EventPayResultShow["MessageCode"] = Json::Value("006");

		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["PayResult"] = Json::Value(2);
		EventInfo["Describe"] = Json::Value("微信支付二维码非法");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventPayResultShow["EventInfo"] = EventInfo;

		root["EventPayResultShow"] = EventPayResultShow;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("支付状态展示:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用支付结果展示接口返回失败!");
	}
	return;
}


void CTestToolDlg::OnBnClickedBtnSwipecard()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("刷卡提示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value CardOperationNotify;
		CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
		CardOperationNotify["MessageCode"] = Json::Value("009");

		Json::Value Data;
		Data["NotifyType"] = Json::Value(1);
		CardOperationNotify["Data"] = Data;

		root["CardOperationNotify"] = CardOperationNotify;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("刷卡提示:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用ETC卡操作提示接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnRwcard()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("读写卡提示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value CardOperationNotify;
		CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
		CardOperationNotify["MessageCode"] = Json::Value("009");

		Json::Value Data;
		Data["NotifyType"] = Json::Value(2);
		CardOperationNotify["Data"] = Data;

		root["CardOperationNotify"] = CardOperationNotify;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("读写卡提示:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用ETC卡操作提示接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnFixvehqueue()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆队列修改(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventModifyVehQueue;
		EventModifyVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventModifyVehQueue["MessageCode"] = Json::Value("010");
		EventModifyVehQueue["Reasons"] = Json::Value("卡内车牌与特情车牌不一致");
		EventModifyVehQueue["TotalVehNum"] = Json::Value(4);

		Json::Value VehQueue;
		Json::Value VehItem;
		VehItem["Index"] = Json::Value(0);
		VehItem["VehPlate"] = Json::Value("粤A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("粤A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("粤A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("余额不足");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("粤A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventModifyVehQueue["VehQueue"] = VehQueue;

		root["EventModifyVehQueue"] = EventModifyVehQueue;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆队列修改:");
		log(str);
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventModifyVehQueue(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用车辆队列修改接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnFeeauthorize()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆信息确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value FeeAuthorize;
		FeeAuthorize["SpecialEventNo"] = Json::Value(m_EventNo);
		FeeAuthorize["MessageCode"] = Json::Value("011");

		Json::Value Data;
		Data["authorizeType"] = Json::Value(0);
		FeeAuthorize["Data"] = Data;

		root["Authorize"] = FeeAuthorize;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("最小费额授权:");
		log(str);

		strJson.Format("%s", str.c_str());
	}

	int iSize = strJson.GetLength();
	bool ret = false;
	ret = fn_EventFeeAuthorize(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用最小费额授权接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnAuthorize()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		strJson.Trim();
		log("车辆信息确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value Authorize;
		Authorize["SpecialEventNo"] = Json::Value(m_EventNo);
		Authorize["MessageCode"] = Json::Value("011");
		Authorize["VehPlate"] = Json::Value(m_CurrentVehPlate);
		Authorize["VehPlateColor"] = Json::Value("1");

		Json::Value Data;
		Data["authorizeType"] = Json::Value(1);
		Data["authorizeReason"] = Json::Value("在线计费异常");
		Data["authorizeOption"] = Json::Value("1:补缴放行#2:异议放行#3:拦截车辆");
		Authorize["Data"] = Data;

		root["Authorize"] = Authorize;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("其他授权:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}

	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventAuthorize(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("其他授权接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnDelvehqueueresult()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("删除车辆队列结果(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value DelVehQueueResult;
		DelVehQueueResult["SpecialEventNo"] = Json::Value(m_EventNo);
		DelVehQueueResult["MessageCode"] = Json::Value("011");

		Json::Value Data;
		Data["ExecuteType"] = Json::Value(0);
		Data["DelVehNum"] = Json::Value(1);
		Data["ExecuteResult"] = Json::Value(0);

		DelVehQueueResult["Data"] = Data;

		root["DelVehQueueResult"] = DelVehQueueResult;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("删除车辆队列结果:");
		log(str);

		strJson.Format("%s", str.c_str());
	}

	int iSize = strJson.GetLength();
	bool ret = false;
	ret = fn_EventDelVehQueueResult(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用删除车辆队列结果接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnEtcdevstatus()
{
	if (!initEtcEventEnvSucceed())
		return;

	bool bRet = false;
	unsigned int uStatus;
	bRet = fn_EventGetComponentStatus(uStatus);
	if (bRet)
	{
		log("调用ETC特情获取设备状态接口返回成功!");
		log("返回状态值:" + to_string(uStatus));
	}
	else
	{
		log("调用ETC特情获取设备状态接口返回失败!");
	}
	return;
}


void CTestToolDlg::OnBnClickedBtnEtcgetlasterrordescrip()
{
	if (!initEtcEventEnvSucceed())
		return;

	log("调用接口IF_EventGetLastErrorDesc()");
	static std::string ErrorDesc;
	ErrorDesc = fn_EventGetLastErrorDesc();
	log("返回上次错误信息:" + ErrorDesc);

	return;
}


void CTestToolDlg::OnBnClickedBtnChange2etcmode()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value CardOperationNotify;
	CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
	CardOperationNotify["MessageCode"] = Json::Value("009");

	Json::Value Data;
	Data["NotifyType"] = Json::Value(99);
	CardOperationNotify["Data"] = Data;

	root["CardOperationNotify"] = CardOperationNotify;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("切换到ETC自助工作模式:");
	log(str);

	strJson.Format(_T("%s"), str.c_str());
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用ETC卡操作提示接口返回失败!");
	}

	return;
}

void CTestToolDlg::OnBnClickedBtnEventstop()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("特情处理结束(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventDealStop;
		EventDealStop["SpecialEventNo"] = Json::Value(m_EventNo);
		EventDealStop["MessageCode"] = Json::Value("006");
		//EventEndCases
		Json::Value EventEndCases;

		EventEndCases["EventVehPlate"] = Json::Value(m_CurrentVehPlate);
		EventEndCases["EventVehPlateColor"] = Json::Value(m_CurrentVehPlateColor);
		EventEndCases["EventVehClass"] = Json::Value(m_CurrentVehClass);
		EventEndCases["EventVehType"] = Json::Value(m_CurrentVehType);
		EventEndCases["EventVehSeatCount"] = Json::Value(5);
		EventEndCases["EventVehAxleCount"] = Json::Value(2);
		EventEndCases["MediaType"] = Json::Value(2);
		EventEndCases["CardNum"] = Json::Value("1234567890");
		EventEndCases["enProvinceId"] = Json::Value(m_strProvinceID);
		EventEndCases["enStationId"] = Json::Value(m_strStationID);
		EventEndCases["enTollLaneId"] = Json::Value(m_strRoadID);
		EventEndCases["enTime"] = Json::Value(MyGetCurrenTime());
		EventEndCases["exitFeeType"] = Json::Value(4);

		EventEndCases["EventEndTime"] = Json::Value(MyGetCurrenTime());
		EventEndCases["EventDescribe"] = Json::Value("车辆无入口信息");
		EventEndCases["EventEndReason"] = Json::Value("特情处理完成，支付成功");
		EventEndCases["EventHandleMethodNo"] = Json::Value(m_EventHandleMethodNo);
		EventEndCases["EventHandleMethodName"] = Json::Value(GetEventHandleMethodName(m_EventHandleMethodNo));
		EventDealStop["EventEndCases"] = EventEndCases;

		root["EventDealStop"] = EventDealStop;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("特情处理结束:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventDealStop(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("调用特情处理结束接口返回失败!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnDestroyetcevent()
{
	if (!initEtcEventEnvSucceed())
		return;

	log("卸载EtcEvent环境接口开始调用");
	bool bRet = false;
	bRet = fn_EventDestroy();
	if (bRet)
	{
		log("调用卸载EtcEvent环境接口返回成功");
		m_bEtcEventEnvInitedFlag = false;
	}
	else
	{
		log("调用卸载EtcEvent环境接口返回失败");
	}
}


void CTestToolDlg::OnBnClickedBtnDestroynetpay()
{
	if (!loadDllSucceed())
		return;

	if (!initNetPayEnvSucceed())
		return;

	log("卸载移动支付环境接口开始调用");
	bool bRet = false;
	bRet = fn_Destroy();
	if (bRet)
	{
		log("调用卸载移动支付环境接口返回成功!");
		m_bNetPayEnvInitedFlag = false;
	}
	else
	{
		log("调用卸载移动支付环境接口返回失败!");
	}
	return;
}


void CTestToolDlg::OnBnClickedBtnFreedll()
{
	if (!loadDllSucceed())
		return;

	//框架库中有一个线程不能通过接口调用退出，此处直接卸载dll会崩溃。
	return;


	BOOL bRet = FreeLibrary(gd_dll);
	if (bRet == TRUE)
	{
		log("卸载中间件成功！");
		m_bLoaded = false;
	}
	else
	{
		log("卸载中间件失败！");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplay1start()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	//首先暂停其它两个线程的运行
	if (m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请先停止GUI循环播放2 ！！！");
		return;
	}

	if (m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请先停止GUI循环播放3 ！！！");
		return;
	}

	if (m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请先停止GUI总循环播放 ！！！");
		return;
	}

	if (m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请不要重复启动 ！！！");
		return;
	}

	log("启动GUI循环播放1.");
	if (m_pGuiPlay1Thread == NULL)
		m_pGuiPlay1Thread = AfxBeginThread(GuiPlay1ThreadFun, (LPVOID)this);
	else
		ResumeThread(m_pGuiPlay1Thread->m_hThread);

	m_bThread1Running = true;

	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplay1stop()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	if (m_bThread1Running == false)
	{
		log("停止GUI循环播放1，当前GUI循环播放1未运行！！！");
		return;
	}

	if (m_pGuiPlay1Thread != NULL)
	{
		log("停止GUI循环播放1.");
		//方法1
		DWORD exitCode = 12345;
		TerminateThread(m_pGuiPlay1Thread->m_hThread, exitCode);
		m_pGuiPlay1Thread = NULL;
	}
	m_bThread1Running = false;

	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplay2start()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	//首先暂停其它两个线程的运行
	if (m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请先停止GUI循环播放1 ！！！");
		return;
	}
	if (m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请先停止GUI循环播放3 ！！！");
		return;
	}
	if (m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请先停止GUI总循环播放 ！！！");
		return;
	}

	if (m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请不要重复启动 ！！！");
		return;
	}


	log("启动GUI循环播放2.");
	if (m_pGuiPlay2Thread == NULL)
		m_pGuiPlay2Thread = AfxBeginThread(GuiPlay2ThreadFun, (LPVOID)this);
	else
		ResumeThread(m_pGuiPlay2Thread->m_hThread);

	m_bThread2Running = true;
	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplay2stop()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	if (m_bThread2Running == false)
	{
		log("停止GUI循环播放2，当前GUI循环播放2未运行！！！");
		return;
	}

	if (m_pGuiPlay2Thread != NULL)
	{
		//方法1
		log("停止GUI循环播放2.");
		DWORD exitCode = 12345;
		TerminateThread(m_pGuiPlay2Thread->m_hThread, exitCode);
		m_pGuiPlay2Thread = NULL;
	}
	m_bThread2Running = false;

	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplay3start()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	//首先暂停其它两个线程的运行
	if (m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请先停止GUI循环播放1 ！！！");
		return;
	}
	if (m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请先停止GUI循环播放2 ！！！");
		return;
	}
	if (m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请先停止GUI总循环播放 ！！！");
		return;
	}

	if (m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请不要重复启动 ！！！");
		return;
	}

	log("启动GUI循环播放3.");
	if (m_pGuiPlay3Thread == NULL)
		m_pGuiPlay3Thread = AfxBeginThread(GuiPlay3ThreadFun, (LPVOID)this);
	else
		ResumeThread(m_pGuiPlay3Thread->m_hThread);

	m_bThread3Running = true;

	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplay3stop()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	if (m_bThread3Running == false)
	{
		log("停止GUI循环播放3，当前GUI循环播放3未运行！！！");
		return;
	}

	if (m_pGuiPlay3Thread != NULL)
	{
		log("停止GUI循环播放3.");
		DWORD exitCode = 12345;
		TerminateThread(m_pGuiPlay3Thread->m_hThread, exitCode);
		m_pGuiPlay3Thread = NULL;
	}

	m_bThread3Running = false;
	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplaytotalstart()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	//首先暂停其它两个线程的运行
	if (m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请先停止GUI循环播放1 ！！！");
		return;
	}
	if (m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请先停止GUI循环播放2 ！！！");
		return;
	}
	if (m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请先停止GUI循环播放3 ！！！");
		return;
	}
	if (m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请不要重复启动 ！！！");
		return;
	}

	log("启动GUI总循环播放.");
	if (m_pGuiPlayTotalThread == NULL)
		m_pGuiPlayTotalThread = AfxBeginThread(GuiPlayTotalThreadFun, (LPVOID)this);
	else
		ResumeThread(m_pGuiPlayTotalThread->m_hThread);

	m_bThreadTotalRunning = true;

	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplaytotalstop()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	if (m_bThreadTotalRunning == false)
	{
		log("停止GUI总循环播放，当前GUI总循环播放未运行！！！");
		return;
	}

	if (m_pGuiPlayTotalThread != NULL)
	{
		log("停止GUI总循环播放.");
		DWORD exitCode = 12345;
		TerminateThread(m_pGuiPlayTotalThread->m_hThread, exitCode);
		m_pGuiPlayTotalThread = NULL;
	}

	m_bThreadTotalRunning = false;
	return;
}


void CTestToolDlg::OnBnClickedBtnAsyncexeccmd()
{
	// 展示发票二维码
	if (m_enCurProvince != PROVINCE_TYPE::Jiangxi)
	{
		log("当前的库不是江西的中间件，没有此接口！！！");
		return;
	}

	if (fn_AsyncExecCmd == NULL)
		return;

	Json::Value InvoiceAsyncExecCmd;
	InvoiceAsyncExecCmd["CashInvoiceNo"] = Json::Value("33b2b009e44b15");
	InvoiceAsyncExecCmd["SubTime"] = Json::Value(CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));
	InvoiceAsyncExecCmd["License"] = Json::Value("川A12345");
	InvoiceAsyncExecCmd["EntryStationID"] = Json::Value("31121");
	InvoiceAsyncExecCmd["EntryStationName"] = Json::Value("上海路段");
	InvoiceAsyncExecCmd["EntryTime"] = Json::Value("");
	InvoiceAsyncExecCmd["QRCodeUrl"] = Json::Value("");

	bool ret = false;
	//ret = fn_AsyncExecCmd(2001, );

}


void CTestToolDlg::OnBnClickedBtnClearqrcodecmd()
{
	// 清除发票二维码
	if (m_enCurProvince != PROVINCE_TYPE::Jiangxi)
	{
		log("当前的库不是江西的中间件，没有此接口！！！");
		return;
	}

	if (fn_AsyncExecCmd == NULL)
		return;

	bool ret = false;
	//ret = fn_AsyncExecCmd(2002, );
}
