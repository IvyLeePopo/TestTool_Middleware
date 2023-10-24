
// TestTool_SiChuanDlg.cpp : ʵ���ļ�
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

#define FILE_NAME_INIT_CONFIG   _T("HTSMConfig.ini")		//��ʼ�������ļ���

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


// CTestToolDlg �Ի���
int CTestToolDlg::No = 1000;
int CTestToolDlg::VehPlateTail = 100;
void* CTestToolDlg::m_pIndPtr = new char[2048];

void MyNetPayCallBackFun(int iOption, int iResult, void* pIndPtr)
{
	int Option = iOption;
	int Result = iResult;
	char *ptr = (char *)pIndPtr;

	//�ص�����,��ʱ����
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


// CTestToolDlg ��Ϣ�������

BOOL CTestToolDlg::OnInitDialog()
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
	g_DevIFDlg = this;

	SetDlgItemText(IDC_EDIT_Money, _T("1"));
	SetDlgItemText(IDC_EDIT_Weight, _T("1234"));

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_type);
	pCombo->SetCurSel(1);

	SetDlgItemText(IDC_EDIT_axisnum, _T("2"));
	SetDlgItemText(IDC_EDIT_plate, _T("��A12345"));
	SetDlgItemText(IDC_EDIT_entry, _T("����"));
	SetDlgItemText(IDC_EDIT_timeout, _T("30"));
	SetDlgItemText(IDC_EDIT_operator, _T("�շ�Ա"));
	SetDlgItemText(IDC_EDIT_operatorID, _T("13579"));
	SetDlgItemText(IDC_EDIT_balance, _T("9999"));
	SetDlgItemText(IDC_EDIT_cardno, _T("999988887777"));

	((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->SetCheck(TRUE);	//ѡ��Ĭ�ϵ�
	((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->SetCheck(FALSE);

	log("��ʼ��ȡ�ƶ�֧��������Ϣ");	
	//��ȡ�ƶ�֧��������������Ϣ
	char szIP[256] = { 0 };
	CString strCfgFilePath;
	strCfgFilePath.Format(_T("%s\\TWSDNetPayConfig.ini"), theApp.m_strCommDir);
	::GetPrivateProfileString(_T("ReflectWeb"), _T("IP"), _T("101.200.214.203"), szIP, _countof(szIP), strCfgFilePath);
	m_strServerIP.Format(_T("%s"), szIP);
	m_wServerPort = (WORD)::GetPrivateProfileInt(_T("ReflectWeb"), _T("Port"), 8081, strCfgFilePath);

	//��ȡ���Թ��ߵ�������Ϣ
	strCfgFilePath.Format(_T("%s\\TWNetPayTest.ini"), theApp.m_strCommDir);
	m_iProvinceID = ::GetPrivateProfileInt(_T("Setup"), _T("ProvinceID"), 44, strCfgFilePath);
	m_iAreaID = ::GetPrivateProfileInt(_T("Setup"), _T("AreaID"), 4412, strCfgFilePath);
	m_iRoadID = ::GetPrivateProfileInt(_T("Setup"), _T("RoadID"), 30, strCfgFilePath);
	m_iStationID = ::GetPrivateProfileInt(_T("Setup"), _T("StationID"), 9, strCfgFilePath);
	m_iLaneID = ::GetPrivateProfileInt(_T("Setup"), _T("LaneID"), 1, strCfgFilePath);

	char chs[50] = { 0 };
	::GetPrivateProfileString(_T("Setup"), _T("StationName"), _T("�㶫·��"), chs, _countof(chs), strCfgFilePath);
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

	//�����߳�	
	pThreadReceiveMsg = (CThreadReceiveRespMsg *)::AfxBeginThread(RUNTIME_CLASS(CThreadReceiveRespMsg), THREAD_PRIORITY_NORMAL);
	if (pThreadReceiveMsg != NULL)
	{
		log(_T("�߳������ɹ� \n"));
		ReceiveMsgThreadID = pThreadReceiveMsg->m_nThreadID;
	}
	else
	{
		log(_T("�߳�����ʧ�� \n"));
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestToolDlg::OnPaint()
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
	log("��ʼ����TWSDNetPay.dll��...������û�����鴦��\n");

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
		log("load TWSDNetPay.dllʧ��!!! \n");
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

	// �������
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

	m_bLoaded ? log("����TWSDNetPay.dll�ɹ�! \n") : log("����TWSDNetPay.dllʧ��!!! \n");
}

void CTestToolDlg::loadDllEvent()
{
	log("��ʼ����TWSDNetPay.dll��...������+���鴦��\n");

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


	// �м�ҵ���
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

	// �������
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

		log("����TWSDNetPay.dllʧ��!!! \n");
		return;
	}

	log("����TWSDNetPay.dll�ɹ�! \n");
	m_bLoaded = true;
}

void CTestToolDlg::loadDllRobot()
{
	log("��ʼ����TWSDNetPay.dll��...���������ƶ�֧����û������ģ�\n");

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
		log("load TWSDNetPay.dllʧ��!!! \n");
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

		log("����TWSDNetPay.dllʧ��!!! \n");
		return;
	}

	log("����TWSDNetPay.dll�ɹ�! \n");
	m_bLoaded = true;
}

void CTestToolDlg::loadShanXiDll()
{
	log("��ʼ���أ�ɽ����TWSDNetPay.dll��...\n");

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


	// �м�ҵ���
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

	// �������
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

		log("����TWSDNetPay.dllʧ��!!! \n");
		return;
	}

	log("����TWSDNetPay.dll�ɹ�! \n");
	m_bLoaded = true;
}

bool CTestToolDlg::loadDllSucceed()
{
	if (!m_bLoaded)
		log("δ���ع����!");

	return m_bLoaded;
}

bool CTestToolDlg::initNetPayEnvSucceed()
{
	if (!m_bNetPayEnvInitedFlag)
		log("δ��ʼ��֧������!");

	return m_bNetPayEnvInitedFlag;
}

bool CTestToolDlg::initEtcEventEnvSucceed()
{
	if (!m_bEtcEventEnvInitedFlag)
		log("δ��ʼ�����黷��!");

	return m_bEtcEventEnvInitedFlag;
}

CString CTestToolDlg::GetCurrentDirNew()
{

	TCHAR szfile[255] = _T("");
	CString csDir = _T("");

	//����Ӧ�ó��򣨵����ߣ����˷�ʽ��ȷ�����ڶ�̬�⣨�������ߣ����˷�ʽ��ȡ���ǳ���·��
	DWORD dwLen = GetModuleFileName(NULL, szfile, 255);

	//����Ϊ��̬�������·��
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
// �������ƣ�FillpPayKeyItemsStruct
//
// �������������ṹ��
//
// ���������IN  int _OperationMode //�ۿ�ģʽ
// ���������IN int _PayCertificateType //֧��ƾ֤����
// ���������IN CString _PayCertificateCode  //������֧��ƾ֤
// ���������OUT tagPayKeyItems& _ppPayKeyItems //�����ṹ��
// ����ֵ���ɹ� ���� TRUE��ʧ�� ���� FALSE
// ��д��Ա��FORTUNE
// ��дʱ�䣺������ 2016��11��27��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
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

		//�ѽ���ʶ������ʾ����
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
		EventTypeName = "δ��Ӧ����ǩ";
		break;
	case 2:
		EventTypeName = "����ǩʧ��";
		break;
	case 3:
		EventTypeName = "��ǩ�쳣";
		break;
	case 4:
		EventTypeName = "��ǩ������";
		break;
	case 5:
		EventTypeName = "��ǩ������Ч��";
		break;
	case 6:
		EventTypeName = "��ǩ��ж";
		break;
	case 7:
		EventTypeName = "д��ʧ��";
		break;
	case 8:
		EventTypeName = "����ʧ��";
		break;
	case 9:
		EventTypeName = "���쳣";
		break;
	case 10:
		EventTypeName = "��������";
		break;
	case 11:
		EventTypeName = "��������Ч��";
		break;
	case 12:
		EventTypeName = "��δ���";
		break;
	case 13:
		EventTypeName = "��ǩ��ڲ�һ��";
		break;
	case 14:
		EventTypeName = "��ǩ���Ʋ�һ��";
		break;
	case 15:
		EventTypeName = "�������Ϣ";
		break;
	case 16:
		EventTypeName = "�����Ч";
		break;
	case 17:
		EventTypeName = "����";
		break;
	case 18:
		EventTypeName = "�Ʒ��쳣";
		break;
	case 19:
		EventTypeName = "��Ӫ����";
		break;
	case 100:
		EventTypeName = "��������";
		break;
	default:
		EventTypeName = "��������";
	}

	return EventTypeName;
}

CString CTestToolDlg::GetEventHandleMethodName(int EventHandleMethodNo)
{
	CString EventHandleMethodName;
	switch (EventHandleMethodNo)
	{
	case 1:
		EventHandleMethodName = "�ն˴���";
		break;
	case 2:
		EventHandleMethodName = "������";
		break;
	case 3:
		EventHandleMethodName = "��������";
		break;
	case 100:
		EventHandleMethodName = "��������ʽ";
		break;
	default:
		EventHandleMethodName = "�ն˴���";
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
		log("������Ϣȷ��Ӧ��json��������!");
	}

	if (root["EventCheckVehInfoResp"].type() != Json::nullValue)
	{
		EventCheckVehInfoResp = root["EventCheckVehInfoResp"];
	}
	else
	{
		log("������Ϣȷ��Ӧ��json��������,��EventCheckVehInfoResp����!");
		return false;
	}

	if (EventCheckVehInfoResp["Data"].type() != Json::nullValue)
	{
		Data = EventCheckVehInfoResp["Data"];
	}
	else
	{
		log("������Ϣȷ��Ӧ��json��������,��Data����!");
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
	LogStr.Format("�յ�������Ϣȷ��Ӧ��,���µ�ǰ���鳵����Ϣ��VehPlate:%s,VehPlateColor:%d,VehClass:%d,VehType:%d,AxisCount:%d,EntryTime:%s", m_CurrentVehPlate.GetBuffer(0), m_CurrentVehPlateColor, m_CurrentVehClass, m_CurrentVehType, m_CurrentAxisCount, m_CurrentEntryTime.GetBuffer());
	std::string logstr(LogStr.GetBuffer(0));
	log(logstr);


	return true;
}



UINT CTestToolDlg::GuiPlay1ThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUIѭ������1��ʼ����...");

	while (true)
	{
		dlg->log("GUIѭ������1: ���鿪ʼ(��δ���)");
		dlg->GuiPlay1Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������1: ��ˢ��");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������1: �Ѷ�չʾ");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������1: ˢ��֧���ɹ�");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������1: �������");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}

	return 0;
}

UINT CTestToolDlg::GuiPlay2ThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUIѭ������2��ʼ����...");
	while (true)
	{
		dlg->log("GUIѭ������2: ���鿪ʼ(����)");
		dlg->GuiPlay2Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������2: ��ˢ��");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������2: �Ѷ�չʾ");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������2: �ƶ�֧���ɹ�");
		dlg->GuiPlay2PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������2: �������");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}

	return 0;
}

UINT CTestToolDlg::GuiPlay3ThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUIѭ������3��ʼ����...");
	while (true)
	{
		dlg->log("GUIѭ������3: ���鿪ʼ(�������Ϣ)");
		dlg->GuiPlay3Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������3: ��ˢ��");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������3: ��̨����ȷ�������Ϣ");
		dlg->GuiPlay3CheckEntryInfoNone();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������3: �Ѷ�չʾ");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������3: ˢ��֧���ɹ�");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUIѭ������3: �������");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}

	return 0;
}

UINT CTestToolDlg::GuiPlayTotalThreadFun(LPVOID pParam)
{
	CTestToolDlg *dlg = (CTestToolDlg*)pParam;
	dlg->log("GUI��ѭ�����ſ�ʼ����...");

	while (true)
	{
		dlg->log("GUI��ѭ������: ���鿪ʼ(��δ���)");
		dlg->GuiPlay1Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: ��ˢ��");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: �Ѷ�չʾ");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: ˢ��֧���ɹ�");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: �������");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);


		dlg->log("GUI��ѭ������: ���鿪ʼ(����)");
		dlg->GuiPlay2Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: ��ˢ��");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: �Ѷ�չʾ");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: �ƶ�֧���ɹ�");
		dlg->GuiPlay2PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: �������");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);


		dlg->log("GUI��ѭ������: ���鿪ʼ(�������Ϣ)");
		dlg->GuiPlay3Eventstart();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: ��ˢ��");
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: ��̨����ȷ�������Ϣ");
		dlg->GuiPlay3CheckEntryInfoNone();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: �Ѷ�չʾ");
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: ˢ��֧���ɹ�");
		dlg->GuiPlay13PayResultShow();
		Sleep(10 * 1000);

		dlg->log("GUI��ѭ������: �������");
		dlg->GuiPlayXEventstop();
		Sleep(10 * 1000);
	}
}

/************************************����ѭ������*********************************************/

void CTestToolDlg::GuiPlay1Eventstart()
{
	CString strJson;

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// ��ʶ��ϸData������
	Json::Value Data;
	CString str1 = "ԥA123";
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
	Data["EventDescribe"] = Json::Value("��δ���");
	Data["EventTypeNo"] = Json::Value(12);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(12));

	EventDealStart["Data"] = Data;
	root["EventDealStart"] = EventDealStart;

	//ת��Ϊ�ַ���
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("�����¼�����ʼ:");
	log(str);

	//GBK����
	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);

	return;
}



void CTestToolDlg::GuiPlay2Eventstart()
{
	CString strJson;

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// ��ʶ��ϸData������
	Json::Value Data;
	CString str1 = "ԥA123";
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
	Data["EventDescribe"] = Json::Value("����");
	Data["EventTypeNo"] = Json::Value(17);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(17));

	EventDealStart["Data"] = Data;
	root["EventDealStart"] = EventDealStart;

	//ת��Ϊ�ַ���
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("�����¼�����ʼ:");
	log(str);

	//GBK����
	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestToolDlg::GuiPlay3Eventstart()
{
	CString strJson;

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// ��ʶ��ϸData������
	Json::Value Data;
	CString str1 = "ԥA123";
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
	Data["EventDescribe"] = Json::Value("�������Ϣ");
	Data["EventTypeNo"] = Json::Value(15);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(15));

	EventDealStart["Data"] = Data;
	root["EventDealStart"] = EventDealStart;

	//ת��Ϊ�ַ���
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("�����¼�����ʼ:");
	log(str);

	//GBK����
	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);

	return;
}

void CTestToolDlg::GuiPlayXSwipeCardNotify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strJson;
	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value CardOperationNotify;
	CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
	CardOperationNotify["MessageCode"] = Json::Value("009");

	Json::Value Data;
	Data["NotifyType"] = Json::Value(1);
	CardOperationNotify["Data"] = Data;

	root["CardOperationNotify"] = CardOperationNotify;

	//ת��Ϊ�ַ���
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

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventShowFeeInfo;
	EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
	EventShowFeeInfo["MessageCode"] = Json::Value("005");

	EventShowFeeInfo["PayType"] = Json::Value(1);
	EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
	EventShowFeeInfo["FeeCount"] = Json::Value(2);

	Json::Value Array;
	Json::Value Item;
	Item["Entry"] = Json::Value("�㶫-ݸ�����-ʯ��վ");
	Item["PathInfo"] = Json::Value("0729-0730-0731");
	Item["Fee"] = Json::Value("3000");
	Item["ChargeType"] = Json::Value(2);
	Item["ItemNo"] = Json::Value(1);
	Array.append(Item);

	Item["Entry"] = Json::Value("�㶫-ݸ�����-ʯ��վ");
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
	VehItem["VehPlate"] = Json::Value("ԥA121121");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(0);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(1);
	VehItem["VehPlate"] = Json::Value("ԥA122122");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(0);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(2);
	VehItem["VehPlate"] = Json::Value("ԥA123123");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(1);
	VehItem["FailReason"] = Json::Value("����");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(3);
	VehItem["VehPlate"] = Json::Value("ԥA124124");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(2);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);
	EventShowFeeInfo["VehQueue"] = VehQueue;

	root["EventShowFeeInfo"] = EventShowFeeInfo;

	//ת��Ϊ�ַ���
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

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventPayResultShow;
	EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
	EventPayResultShow["MessageCode"] = Json::Value("006");

	//EventInfo
	Json::Value EventInfo;
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["PayResult"] = Json::Value(0);
	EventInfo["Describe"] = Json::Value("ˢ��֧���ɹ�");
	EventInfo["TimeLimit"] = Json::Value("360");
	EventPayResultShow["EventInfo"] = EventInfo;

	root["EventPayResultShow"] = EventPayResultShow;

	//ת��Ϊ�ַ���
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

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventPayResultShow;
	EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
	EventPayResultShow["MessageCode"] = Json::Value("006");

	//EventInfo
	Json::Value EventInfo;
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["PayResult"] = Json::Value(0);
	EventInfo["Describe"] = Json::Value("�ƶ�֧���ɹ�");
	EventInfo["TimeLimit"] = Json::Value("360");
	EventPayResultShow["EventInfo"] = EventInfo;

	root["EventPayResultShow"] = EventPayResultShow;

	//ת��Ϊ�ַ���
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();
	fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestToolDlg::GuiPlay3CheckEntryInfoNone()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strJson;

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventCheckEntryInfo;
	EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
	EventCheckEntryInfo["MessageCode"] = Json::Value("005");
	CString str1 = "ԥA123";
	CString temp;
	temp.Format(_T("%d"), VehPlateTail);
	EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);

	//EventInfo
	Json::Value EventInfo;
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["EventDescribe"] = Json::Value("ȷ�ϳ�����Ϣ�Ƿ���ȷ");
	EventInfo["MMIDemand"] = Json::Value("��������");
	EventInfo["TimeLimit"] = Json::Value("360");

	EventCheckEntryInfo["EventInfo"] = EventInfo;

	Json::Value Array;
	EventCheckEntryInfo["EntryInfo"] = Array;

	root["EventCheckEntryInfo"] = EventCheckEntryInfo;

	//ת��Ϊ�ַ���
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"), str.c_str());
	int iSize = strJson.GetLength();

	fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);

	return;
}

void CTestToolDlg::GuiPlayXEventstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strJson;

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value EventDealStop;
	EventDealStop["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStop["MessageCode"] = Json::Value("006");

	//EventEndCases
	Json::Value EventEndCases;
	EventEndCases["EventVehPlate"] = Json::Value(m_CurrentVehPlate);
	EventEndCases["EventEndTime"] = Json::Value(MyGetCurrenTime());
	EventEndCases["EventDescribe"] = Json::Value("�����������Ϣ");
	EventEndCases["EventEndReason"] = Json::Value("���鴦����ɣ�֧���ɹ�");
	EventEndCases["EventHandleMethodNo"] = Json::Value(m_EventHandleMethodNo);
	EventEndCases["EventHandleMethodName"] = Json::Value(GetEventHandleMethodName(m_EventHandleMethodNo));
	EventDealStop["EventEndCases"] = EventEndCases;

	root["EventDealStop"] = EventDealStop;

	//ת��Ϊ�ַ���
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
		log(">>>>>�յ����鴦��ʼӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 602:
		log(">>>>>�յ���������ȷ��Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 603:
		log(">>>>>�յ�������Ϣȷ��Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 604:
		log(">>>>>�յ����������Ϣȷ��Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 605:
		log(">>>>>�յ��Ѷ���ʾӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 606:
		log(">>>>>�յ�֧��״̬��ʾӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 607:
		log(">>>>>�յ��������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 608:
		log(">>>>>�յ�ɨ���ά������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 609:
		log(">>>>>�յ�ֹͣѰ��Ӧ��");
		break;
	case 610:
		log(">>>>>�յ������˵�Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 611:
		log(">>>>>�յ�ETC��������ʾӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 612:
		log(">>>>>�յ��޸ĳ�������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 613:
		log(">>>>>�յ�ֹͣɨ��Ӧ��");
		break;
	case 616:
		log(">>>>>�յ�������ȨӦ��: " + std::string((char *)m_pIndPtr));
		break;
	}
	if (result == 603)
	{
		char *strGBK = U2G((char *)m_pIndPtr);
		bool bRet;
		bRet = ParseVehInfoCheckResp(strGBK);
		if (bRet)
			log("����������Ϣȷ��Ӧ��ɹ�");
		else
			log("����������Ϣȷ��Ӧ��ʧ��!");

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
	strTip.Format(_T("���ܴ���(��2λ):%d ִ��״̬(��2λ):%d"), FunCode, SubCode);
	log(strTip.GetBuffer(0));

	switch (SubCode)
	{
	case 601:
		log(">>>>>�յ����鴦��ʼӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 602:
		log(">>>>>�յ���������ȷ��Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 603:
		log(">>>>>�յ�������Ϣȷ��Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 604:
		log(">>>>>�յ����������Ϣȷ��Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 605:
		log(">>>>>�յ��Ѷ���ʾӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 606:
		log(">>>>>�յ�֧��״̬��ʾӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 607:
		log(">>>>>�յ��������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 608:
		log(">>>>>�յ�ɨ���ά������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 609:
		log(">>>>>�յ�ֹͣѰ��Ӧ��");
		break;
	case 610:
		log(">>>>>�յ������˵�Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 611:
		log(">>>>>�յ�ETC��������ʾӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 612:
		log(">>>>>�յ��޸ĳ�������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 613:
		log(">>>>>�յ���С�Ѷ���ȨӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 614:
		log(">>>>>�յ��豸��������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 699:
		log(">>>>>�յ�ֹͣɨ��Ӧ��");
		break;
	case 616:
		log(">>>>>�յ�������ȨӦ��: " + std::string((char *)m_pIndPtr));
		break;
	case 617:
		log(">>>>>�յ��豸ɾ��������������Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	case 618:
		log(">>>>>�յ��豸ɾ���������н��Ӧ��: " + std::string((char *)m_pIndPtr));
		break;
	}
	if (result == 603)
	{
		char *strGBK = U2G((char *)m_pIndPtr);
		bool bRet;
		bRet = ParseVehInfoCheckResp(strGBK);
		if (bRet)
			log("����������Ϣȷ��Ӧ��ɹ�");
		else
			log("����������Ϣȷ��Ӧ��ʧ��!");

		delete strGBK;
	}

	return 0;
}

LRESULT CTestToolDlg::OnHWNDMsgShowNetPayResultLog(WPARAM wParam, LPARAM lParam)
{
	if (wParam != 0)
	{
		//��ȡ���
		BYTE byFunCode = LOBYTE(LOWORD(wParam));
		BYTE byExecuteState = HIBYTE(LOWORD(wParam));

		CString strTip;
		strTip.Format(_T("���ܴ���:%d ִ��״̬:%d"), byFunCode, byExecuteState);
		log(strTip.GetBuffer(0));

		switch (byFunCode)
		{
		case 1:		//�۷ѻ���
		{
			if (byExecuteState == 0)
				strTip.Format(_T(">>>>>>�յ��ƶ�֧�� �۷ѽ�����۷ѳɹ�"));
			else if (byExecuteState == 1)
				strTip.Format(_T(">>>>>>�յ��ƶ�֧�� �۷ѽ����������������..."));
			else
				strTip.Format(_T(">>>>>>�յ��ƶ�֧�� �۷ѽ�����۷�ʧ��"));

			log(strTip.GetBuffer(0));
		}
		break;
		case 2:  //��������
		{
			if (byExecuteState == 0)
				strTip.Format(_T(">>>>>>�յ��ƶ�֧�� ��������������ɹ�"));
			else
				strTip.Format(_T(">>>>>>�յ��ƶ�֧�� �������������ʧ��"));

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
		log("���ؽ������ӷ�Ʊ�м��������");
		break;
	case HeNan:
		log("���غ����ƶ�֧���м��������");
		break;
	case SiChuan:
		log("�����Ĵ��ƶ�֧���м��������");
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
		log("δ���ع����---TWSDNetPay.dll!");
		return;
	}


	CString strTemp;
	if (m_bNetPayEnvInitedFlag == true)
	{
		log(_T("TWSDNetPay.dll-----�ƶ�֧�������ѳ�ʼ���ɹ����벻Ҫ�ظ���ʼ��!"));
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

	//�߳̽���Ӧ��
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
		log(_T("TWSDNetPay.dll-----�ƶ�֧��������ʼ��ʧ�ܣ�"));
		m_bNetPayEnvInitedFlag = false;
	}
	else
	{
		log(_T("TWSDNetPay.dll-----�ƶ�֧��������ʼ���ɹ���"));
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

	log("�۷�json����:");
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

		log("����json����:");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CTestToolDlg::OnBnClickedBtnInitenv()
{
	if (m_enMidllewareType != MIDLLEWARE_TYPE::BaseAndEvent)
	{
		log(_T("���м��û�����鴦����صĽӿڣ����أ�����"));
		return;
	}

	if (!loadDllSucceed())
		return;

	if (m_bEtcEventEnvInitedFlag)
	{
		log(_T("EtcEvent�����ѳ�ʼ���ɹ����벻Ҫ�ظ���ʼ��!"));
		return;
	}

	//������Ϣ�������ļ��ж�ȡ
	CString	m_strAppDir;
	m_strAppDir.Format(_T("%s"), GetCurrentDirNew());
	CString	m_strConfigPathFile;
	m_strConfigPathFile.Format(_T("%s\\%s"), m_strAppDir, FILE_NAME_INIT_CONFIG);

	int m_provinceId;
	std::string m_areaId;
	std::string m_areaName = "��������";
	std::string m_roadId;
	std::string m_roadName = "·������";
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
	::GetPrivateProfileString(_T("Setup"), _T("StationName"), "�˶�", tmp_str, 100, m_strConfigPathFile);
	m_stationName = tmp_str;

	m_provinceId = ::GetPrivateProfileInt(_T("Setup"), _T("ProvinceID"), 44, m_strConfigPathFile);

	m_EventTypeNo = ::GetPrivateProfileInt(_T("EtcEventConfig"), _T("EventTypeNo"), 2, m_strConfigPathFile);
	m_EventHandleMethodNo = ::GetPrivateProfileInt(_T("EtcEventConfig"), _T("EventHandleMethodNo"), 2, m_strConfigPathFile);


	UINT nThreadID = (UINT)123;
	HWND hWnd = (HWND)444;
	char szAreaInfo[128] = { 0 };
	_snprintf_s(szAreaInfo, 127, "%s#%s#%s#%s", m_areaId.c_str(), m_areaName.c_str(), m_roadId.c_str(), m_roadName.c_str());
	log("������Ϣ:" + std::string(szAreaInfo));

	char szLoaclStation[48] = { 0 };
	_snprintf_s(szLoaclStation, 47, "%s#%s", m_stationId.c_str(), m_stationName.c_str());
	log("վ��Ϣ:" + std::string(szLoaclStation));

	char* szServerInfo = "127.0.0.1";
	void* pIndPtr = m_pIndPtr;

	log("����ID:" + m_laneId);
	log("ʡ��ID:" + std::to_string(m_provinceId));

	m_bEtcEventEnvInitedFlag = fn_EventInitEnvironment3(ReceiveMsgThreadID, 0, WM_THREAD_ETCEVENT_MESSAGE_SHOWLOG, szAreaInfo, szLoaclStation, m_laneId.c_str(), szServerInfo, m_provinceId, pIndPtr, MyCallBackFun);
	m_bEtcEventEnvInitedFlag?log("ETC���黷����ʼ���ɹ�!"):log("ETC���黷����ʼ��ʧ��!");
}

void CTestToolDlg::OnBnClickedBtnEventdestroy()
{
	if (!loadDllSucceed())
		return;

	bool bRet = fn_EventDestroy();
	bRet ? log("ETC����������Դ�ɹ�!") : log("ETC����������Դʧ��!");
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
		log("�����¼�����ʼ(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventDealStart;
		CString TimeStr = GetUniqueTimeStr();
		std::string timestr(TimeStr.GetBuffer());
		m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
		EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
		EventDealStart["MessageCode"] = Json::Value("001");

		// ��ʶ��ϸData������
		Json::Value Data;
		CString str1 = "��A123";
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
		Data["EventDescribe"] = Json::Value("�������Ϣ");
		Data["EventTypeNo"] = Json::Value(m_EventTypeNo);
		Data["EventTypeName"] = Json::Value(GetEventTypeName(m_EventTypeNo));

		EventDealStart["Data"] = Data;
		root["EventDealStart"] = EventDealStart;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("�����¼�����ʼ:");
		log(str);

		//GBK����
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("���������¼�����ʼ�ӿڷ���ʧ��!");
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
		log("��������ȷ������(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventCheckVehQueue;
		EventCheckVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehQueue["MessageCode"] = Json::Value("002");
		EventCheckVehQueue["CheckFlag"] = Json::Value(0);
		// ��ʶ��ϸData������
		Json::Value Data;
		Data["ProvinceName"] = Json::Value("�㶫ʡ");
		Data["ProvinceNo"] = Json::Value("44");
		Data["RoadName"] = Json::Value("���ݱ�����");
		Data["RoadNo"] = Json::Value("0094");
		Data["StationName"] = Json::Value("��ѩ�շ�վ");
		Data["StationNo"] = Json::Value("009304");
		Data["LaneNo"] = Json::Value("1");
		CString str1 = "��A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		Data["VehPlateColor"] = Json::Value(1);
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("ȷ�ϳ��������Ƿ���ȷ");

		EventCheckVehQueue["Data"] = Data;
		root["EventCheckVehQueue"] = EventCheckVehQueue;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("��������ȷ������:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}

	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventCheckVehQueue(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("���ó�������ȷ�Ͻӿڷ���ʧ��!");
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
		log("ȡ����������ȷ������(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventCheckVehQueue;
		EventCheckVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehQueue["MessageCode"] = Json::Value("002");
		EventCheckVehQueue["CheckFlag"] = Json::Value(1);
		// ��ʶ��ϸData������
		Json::Value Data;

		CString str1 = "��A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		Data["VehPlateColor"] = Json::Value(1);
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("ȡ��ȷ�ϳ�����������");

		EventCheckVehQueue["Data"] = Data;
		root["EventCheckVehQueue"] = EventCheckVehQueue;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("ȡ����������ȷ������:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventCheckVehQueue(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("���ó�������ȷ�Ͻӿڷ���ʧ��!");
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
		log("������Ϣȷ������(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventCheckVehInfo;
		EventCheckVehInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehInfo["MessageCode"] = Json::Value("003");
		//EventInfo
		Json::Value EventInfo;
		EventInfo["VehPlateSnapImage"] = Json::Value("xxx");
		EventInfo["CameraSnapImage"] = Json::Value("xxx");
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("ȷ�ϳ�����Ϣ�Ƿ���ȷ");
		EventInfo["MMIDemand"] = Json::Value("��������");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventCheckVehInfo["EventInfo"] = EventInfo;

		//MMIContent
		Json::Value MMIContent;
		CString str1 = "��A123";
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

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("������Ϣȷ������:");
		log(str);
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckVehInfo(strJson.GetBuffer(0), iSize, 2);
	if (!bRet)
	{
		log("����ȷ�ϳ�����Ϣ�ӿڷ���ʧ��!");
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
		log("���������Ϣȷ��(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventCheckEntryInfo;
		EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckEntryInfo["MessageCode"] = Json::Value("005");
		CString str1 = "��A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);

		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("ȷ�ϳ�����Ϣ�Ƿ���ȷ");
		EventInfo["MMIDemand"] = Json::Value("��������");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventCheckEntryInfo["EventInfo"] = EventInfo;

		Json::Value Array;
		Json::Value Item;
		Item["StationName"] = Json::Value("��ѩ�շ�վ");
		Item["StationID"] = Json::Value("G1501440010090");
		Item["StationHex"] = Json::Value("44011D0B");
		Item["laneid"] = Json::Value("0161");
		Item["laneHex"] = Json::Value("A1");
		Item["MediaType"] = Json::Value("1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		Item["StationName"] = Json::Value("��ƽ�շ�վ");
		Item["StationID"] = Json::Value("G1501440010091");
		Item["StationHex"] = Json::Value("44011D0C");
		Item["laneid"] = Json::Value("0162");
		Item["laneHex"] = Json::Value("A2");
		Item["MediaType"] = Json::Value("1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		EventCheckEntryInfo["EntryInfo"] = Array;

		root["EventCheckEntryInfo"] = EventCheckEntryInfo;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("���������Ϣȷ��:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);
	if (!bRet)
	{
		log("����ȷ�ϳ��������Ϣ�ӿڷ���ʧ��!");
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
		log("���������Ϣȷ��(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventCheckEntryInfo;
		EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckEntryInfo["MessageCode"] = Json::Value("005");
		CString str1 = "��A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);

		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("ȷ�ϳ�����Ϣ�Ƿ���ȷ");
		EventInfo["MMIDemand"] = Json::Value("��������");
		EventInfo["TimeLimit"] = Json::Value("360");

		EventCheckEntryInfo["EventInfo"] = EventInfo;

		Json::Value Array;
		EventCheckEntryInfo["EntryInfo"] = Array;

		root["EventCheckEntryInfo"] = EventCheckEntryInfo;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("���������Ϣȷ��:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);
	if (!bRet)
	{
		log("����ȷ�ϳ��������Ϣ�ӿڷ���ʧ��!");
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
		log("�Ѷ���ʾ(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventShowFeeInfo;
		EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventShowFeeInfo["MessageCode"] = Json::Value("005");
		EventShowFeeInfo["PayType"] = Json::Value(1);
		EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
		EventShowFeeInfo["FeeCount"] = Json::Value(1);

		Json::Value Array;
		Json::Value Item;
		Item["Entry"] = Json::Value("�㶫-ݸ�����-ʯ��վ");
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
		VehItem["VehPlate"] = Json::Value("��A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("��A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("��A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("����");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("��A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventShowFeeInfo["VehQueue"] = VehQueue;

		root["EventShowFeeInfo"] = EventShowFeeInfo;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("�Ѷ���ʾ:");
		log(str);
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("���÷Ѷ���ʾ�ӿڷ���ʧ��!");
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
		log("�Ѷ���ʾ(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventShowFeeInfo;
		EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventShowFeeInfo["MessageCode"] = Json::Value("005");

		EventShowFeeInfo["PayType"] = Json::Value(1);
		EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
		EventShowFeeInfo["FeeCount"] = Json::Value(2);

		Json::Value Array;
		Json::Value Item;
		Item["Entry"] = Json::Value("�㶫-ݸ�����-ʯ��վ");
		Item["PathInfo"] = Json::Value("0729-0730-0731");
		Item["Fee"] = Json::Value("3000");
		Item["ChargeType"] = Json::Value(2);
		Item["ItemNo"] = Json::Value(1);
		Array.append(Item);

		Item["Entry"] = Json::Value("�㶫-ݸ�����-ʯ��վ");
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
		VehItem["VehPlate"] = Json::Value("��A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("��A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("��A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("����");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("��A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventShowFeeInfo["VehQueue"] = VehQueue;

		root["EventShowFeeInfo"] = EventShowFeeInfo;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("�Ѷ���ʾ:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("���÷Ѷ���ʾ�ӿڷ���ʧ��!");
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
		log("֧��״̬չʾ(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventPayResultShow;
		EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
		EventPayResultShow["MessageCode"] = Json::Value("006");
		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["PayResult"] = Json::Value(0);
		EventInfo["Describe"] = Json::Value("֧�����û�֧���ɹ�");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventPayResultShow["EventInfo"] = EventInfo;

		root["EventPayResultShow"] = EventPayResultShow;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("֧��״̬չʾ:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("����֧�����չʾ�ӿڷ���ʧ��!");
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
		log("֧��״̬չʾ(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventPayResultShow;
		EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
		EventPayResultShow["MessageCode"] = Json::Value("006");

		//EventInfo
		Json::Value EventInfo;
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["PayResult"] = Json::Value(2);
		EventInfo["Describe"] = Json::Value("΢��֧����ά��Ƿ�");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventPayResultShow["EventInfo"] = EventInfo;

		root["EventPayResultShow"] = EventPayResultShow;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("֧��״̬չʾ:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("����֧�����չʾ�ӿڷ���ʧ��!");
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
		log("ˢ����ʾ(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value CardOperationNotify;
		CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
		CardOperationNotify["MessageCode"] = Json::Value("009");

		Json::Value Data;
		Data["NotifyType"] = Json::Value(1);
		CardOperationNotify["Data"] = Data;

		root["CardOperationNotify"] = CardOperationNotify;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("ˢ����ʾ:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("����ETC��������ʾ�ӿڷ���ʧ��!");
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
		log("��д����ʾ(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value CardOperationNotify;
		CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
		CardOperationNotify["MessageCode"] = Json::Value("009");

		Json::Value Data;
		Data["NotifyType"] = Json::Value(2);
		CardOperationNotify["Data"] = Data;

		root["CardOperationNotify"] = CardOperationNotify;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("��д����ʾ:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("����ETC��������ʾ�ӿڷ���ʧ��!");
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
		log("���������޸�(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value EventModifyVehQueue;
		EventModifyVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventModifyVehQueue["MessageCode"] = Json::Value("010");
		EventModifyVehQueue["Reasons"] = Json::Value("���ڳ��������鳵�Ʋ�һ��");
		EventModifyVehQueue["TotalVehNum"] = Json::Value(4);

		Json::Value VehQueue;
		Json::Value VehItem;
		VehItem["Index"] = Json::Value(0);
		VehItem["VehPlate"] = Json::Value("��A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("��A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("��A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("����");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("��A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventModifyVehQueue["VehQueue"] = VehQueue;

		root["EventModifyVehQueue"] = EventModifyVehQueue;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("���������޸�:");
		log(str);
		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventModifyVehQueue(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("���ó��������޸Ľӿڷ���ʧ��!");
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
		log("������Ϣȷ������(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value FeeAuthorize;
		FeeAuthorize["SpecialEventNo"] = Json::Value(m_EventNo);
		FeeAuthorize["MessageCode"] = Json::Value("011");

		Json::Value Data;
		Data["authorizeType"] = Json::Value(0);
		FeeAuthorize["Data"] = Data;

		root["Authorize"] = FeeAuthorize;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("��С�Ѷ���Ȩ:");
		log(str);

		strJson.Format("%s", str.c_str());
	}

	int iSize = strJson.GetLength();
	bool ret = false;
	ret = fn_EventFeeAuthorize(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("������С�Ѷ���Ȩ�ӿڷ���ʧ��!");
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
		log("������Ϣȷ������(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value Authorize;
		Authorize["SpecialEventNo"] = Json::Value(m_EventNo);
		Authorize["MessageCode"] = Json::Value("011");
		Authorize["VehPlate"] = Json::Value(m_CurrentVehPlate);
		Authorize["VehPlateColor"] = Json::Value("1");

		Json::Value Data;
		Data["authorizeType"] = Json::Value(1);
		Data["authorizeReason"] = Json::Value("���߼Ʒ��쳣");
		Data["authorizeOption"] = Json::Value("1:���ɷ���#2:�������#3:���س���");
		Authorize["Data"] = Data;

		root["Authorize"] = Authorize;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("������Ȩ:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}

	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventAuthorize(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("������Ȩ�ӿڷ���ʧ��!");
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
		log("ɾ���������н��(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
		Json::Value DelVehQueueResult;
		DelVehQueueResult["SpecialEventNo"] = Json::Value(m_EventNo);
		DelVehQueueResult["MessageCode"] = Json::Value("011");

		Json::Value Data;
		Data["ExecuteType"] = Json::Value(0);
		Data["DelVehNum"] = Json::Value(1);
		Data["ExecuteResult"] = Json::Value(0);

		DelVehQueueResult["Data"] = Data;

		root["DelVehQueueResult"] = DelVehQueueResult;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("ɾ���������н��:");
		log(str);

		strJson.Format("%s", str.c_str());
	}

	int iSize = strJson.GetLength();
	bool ret = false;
	ret = fn_EventDelVehQueueResult(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("����ɾ���������н���ӿڷ���ʧ��!");
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
		log("����ETC�����ȡ�豸״̬�ӿڷ��سɹ�!");
		log("����״ֵ̬:" + to_string(uStatus));
	}
	else
	{
		log("����ETC�����ȡ�豸״̬�ӿڷ���ʧ��!");
	}
	return;
}


void CTestToolDlg::OnBnClickedBtnEtcgetlasterrordescrip()
{
	if (!initEtcEventEnvSucceed())
		return;

	log("���ýӿ�IF_EventGetLastErrorDesc()");
	static std::string ErrorDesc;
	ErrorDesc = fn_EventGetLastErrorDesc();
	log("�����ϴδ�����Ϣ:" + ErrorDesc);

	return;
}


void CTestToolDlg::OnBnClickedBtnChange2etcmode()
{
	if (!initEtcEventEnvSucceed())
		return;

	CString strJson;

	// ��ʾ���� json ����
	Json::Value root;
	// ��ʶETC���齻������
	Json::Value CardOperationNotify;
	CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
	CardOperationNotify["MessageCode"] = Json::Value("009");

	Json::Value Data;
	Data["NotifyType"] = Json::Value(99);
	CardOperationNotify["Data"] = Data;

	root["CardOperationNotify"] = CardOperationNotify;

	//ת��Ϊ�ַ���
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("�л���ETC��������ģʽ:");
	log(str);

	strJson.Format(_T("%s"), str.c_str());
	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("����ETC��������ʾ�ӿڷ���ʧ��!");
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
		log("���鴦�����(ʹ���������ò���):");
		log(strJson.GetBuffer(0));
	}

	if (((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶETC���齻������
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
		EventEndCases["EventDescribe"] = Json::Value("�����������Ϣ");
		EventEndCases["EventEndReason"] = Json::Value("���鴦����ɣ�֧���ɹ�");
		EventEndCases["EventHandleMethodNo"] = Json::Value(m_EventHandleMethodNo);
		EventEndCases["EventHandleMethodName"] = Json::Value(GetEventHandleMethodName(m_EventHandleMethodNo));
		EventDealStop["EventEndCases"] = EventEndCases;

		root["EventDealStop"] = EventDealStop;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("���鴦�����:");
		log(str);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventDealStop(strJson.GetBuffer(0), iSize, 2);
	if (!ret)
	{
		log("�������鴦������ӿڷ���ʧ��!");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnDestroyetcevent()
{
	if (!initEtcEventEnvSucceed())
		return;

	log("ж��EtcEvent�����ӿڿ�ʼ����");
	bool bRet = false;
	bRet = fn_EventDestroy();
	if (bRet)
	{
		log("����ж��EtcEvent�����ӿڷ��سɹ�");
		m_bEtcEventEnvInitedFlag = false;
	}
	else
	{
		log("����ж��EtcEvent�����ӿڷ���ʧ��");
	}
}


void CTestToolDlg::OnBnClickedBtnDestroynetpay()
{
	if (!loadDllSucceed())
		return;

	if (!initNetPayEnvSucceed())
		return;

	log("ж���ƶ�֧�������ӿڿ�ʼ����");
	bool bRet = false;
	bRet = fn_Destroy();
	if (bRet)
	{
		log("����ж���ƶ�֧�������ӿڷ��سɹ�!");
		m_bNetPayEnvInitedFlag = false;
	}
	else
	{
		log("����ж���ƶ�֧�������ӿڷ���ʧ��!");
	}
	return;
}


void CTestToolDlg::OnBnClickedBtnFreedll()
{
	if (!loadDllSucceed())
		return;

	//��ܿ�����һ���̲߳���ͨ���ӿڵ����˳����˴�ֱ��ж��dll�������
	return;


	BOOL bRet = FreeLibrary(gd_dll);
	if (bRet == TRUE)
	{
		log("ж���м���ɹ���");
		m_bLoaded = false;
	}
	else
	{
		log("ж���м��ʧ�ܣ�");
	}

	return;
}


void CTestToolDlg::OnBnClickedBtnGuiplay1start()
{
	if (!(loadDllSucceed() && initEtcEventEnvSucceed()))
		return;

	//������ͣ���������̵߳�����
	if (m_bThread2Running)
	{
		log("GUIѭ������2���������У�����ֹͣGUIѭ������2 ������");
		return;
	}

	if (m_bThread3Running)
	{
		log("GUIѭ������3���������У�����ֹͣGUIѭ������3 ������");
		return;
	}

	if (m_bThreadTotalRunning)
	{
		log("GUI��ѭ���������������У�����ֹͣGUI��ѭ������ ������");
		return;
	}

	if (m_bThread1Running)
	{
		log("GUIѭ������1���������У��벻Ҫ�ظ����� ������");
		return;
	}

	log("����GUIѭ������1.");
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
		log("ֹͣGUIѭ������1����ǰGUIѭ������1δ���У�����");
		return;
	}

	if (m_pGuiPlay1Thread != NULL)
	{
		log("ֹͣGUIѭ������1.");
		//����1
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

	//������ͣ���������̵߳�����
	if (m_bThread1Running)
	{
		log("GUIѭ������1���������У�����ֹͣGUIѭ������1 ������");
		return;
	}
	if (m_bThread3Running)
	{
		log("GUIѭ������3���������У�����ֹͣGUIѭ������3 ������");
		return;
	}
	if (m_bThreadTotalRunning)
	{
		log("GUI��ѭ���������������У�����ֹͣGUI��ѭ������ ������");
		return;
	}

	if (m_bThread2Running)
	{
		log("GUIѭ������2���������У��벻Ҫ�ظ����� ������");
		return;
	}


	log("����GUIѭ������2.");
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
		log("ֹͣGUIѭ������2����ǰGUIѭ������2δ���У�����");
		return;
	}

	if (m_pGuiPlay2Thread != NULL)
	{
		//����1
		log("ֹͣGUIѭ������2.");
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

	//������ͣ���������̵߳�����
	if (m_bThread1Running)
	{
		log("GUIѭ������1���������У�����ֹͣGUIѭ������1 ������");
		return;
	}
	if (m_bThread2Running)
	{
		log("GUIѭ������2���������У�����ֹͣGUIѭ������2 ������");
		return;
	}
	if (m_bThreadTotalRunning)
	{
		log("GUI��ѭ���������������У�����ֹͣGUI��ѭ������ ������");
		return;
	}

	if (m_bThread3Running)
	{
		log("GUIѭ������3���������У��벻Ҫ�ظ����� ������");
		return;
	}

	log("����GUIѭ������3.");
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
		log("ֹͣGUIѭ������3����ǰGUIѭ������3δ���У�����");
		return;
	}

	if (m_pGuiPlay3Thread != NULL)
	{
		log("ֹͣGUIѭ������3.");
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

	//������ͣ���������̵߳�����
	if (m_bThread1Running)
	{
		log("GUIѭ������1���������У�����ֹͣGUIѭ������1 ������");
		return;
	}
	if (m_bThread2Running)
	{
		log("GUIѭ������2���������У�����ֹͣGUIѭ������2 ������");
		return;
	}
	if (m_bThread3Running)
	{
		log("GUIѭ������3���������У�����ֹͣGUIѭ������3 ������");
		return;
	}
	if (m_bThreadTotalRunning)
	{
		log("GUI��ѭ���������������У��벻Ҫ�ظ����� ������");
		return;
	}

	log("����GUI��ѭ������.");
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
		log("ֹͣGUI��ѭ�����ţ���ǰGUI��ѭ������δ���У�����");
		return;
	}

	if (m_pGuiPlayTotalThread != NULL)
	{
		log("ֹͣGUI��ѭ������.");
		DWORD exitCode = 12345;
		TerminateThread(m_pGuiPlayTotalThread->m_hThread, exitCode);
		m_pGuiPlayTotalThread = NULL;
	}

	m_bThreadTotalRunning = false;
	return;
}


void CTestToolDlg::OnBnClickedBtnAsyncexeccmd()
{
	// չʾ��Ʊ��ά��
	if (m_enCurProvince != PROVINCE_TYPE::Jiangxi)
	{
		log("��ǰ�Ŀⲻ�ǽ������м����û�д˽ӿڣ�����");
		return;
	}

	if (fn_AsyncExecCmd == NULL)
		return;

	Json::Value InvoiceAsyncExecCmd;
	InvoiceAsyncExecCmd["CashInvoiceNo"] = Json::Value("33b2b009e44b15");
	InvoiceAsyncExecCmd["SubTime"] = Json::Value(CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));
	InvoiceAsyncExecCmd["License"] = Json::Value("��A12345");
	InvoiceAsyncExecCmd["EntryStationID"] = Json::Value("31121");
	InvoiceAsyncExecCmd["EntryStationName"] = Json::Value("�Ϻ�·��");
	InvoiceAsyncExecCmd["EntryTime"] = Json::Value("");
	InvoiceAsyncExecCmd["QRCodeUrl"] = Json::Value("");

	bool ret = false;
	//ret = fn_AsyncExecCmd(2001, );

}


void CTestToolDlg::OnBnClickedBtnClearqrcodecmd()
{
	// �����Ʊ��ά��
	if (m_enCurProvince != PROVINCE_TYPE::Jiangxi)
	{
		log("��ǰ�Ŀⲻ�ǽ������м����û�д˽ӿڣ�����");
		return;
	}

	if (fn_AsyncExecCmd == NULL)
		return;

	bool ret = false;
	//ret = fn_AsyncExecCmd(2002, );
}
