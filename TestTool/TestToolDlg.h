
// TestTool_SiChuanDlg.h : 头文件
//

#pragma once

#include <string>
#include "ThreadReceiveRespMsg.h"

#define ROOT_PAY 1 //卡机的移动支付

//中间件的类型
enum MIDLLEWARE_TYPE
{
	BaseNoEvent = 1,//普通不带特情的中间件
	BaseAndEvent = 2,//带特情处理的中间件
	RobotNoEvent = 3,//卡机不带特情的中间件
};

//省份的
enum PROVINCE_TYPE
{
	Jiangxi = 1,
	HeNan = 2,
	SiChuan = 3,
};


// TWSDNetPay_V2.1.dll
//初始化组件环境					-- 接口1
typedef bool  (WINAPI  *defIF_InitEnvironment)(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStationID, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID);
//清理资源							-- 接口2
typedef bool  (WINAPI *defIF_Destroy)(void);
//获取移动支付的上次错误信息		-- 接口3
typedef char*       (WINAPI *defIF_GetLastErrorDesc) (void);
//启动获取用户信息操作				-- 接口4
typedef bool  (WINAPI *defIF_StartGetAccountInfo) (const int& iOvertime);
//停止获取用户信息操作				-- 接口5
typedef bool  (WINAPI *defIF_StopGetAccountInfo) (void);
//获取用户信息						-- 接口6
typedef bool  (WINAPI *defIF_GetAccountInfo) (char* szParamContext, int& iParamSize, const int& iFormatType);
//执行扣款操作						-- 接口7
typedef bool	(WINAPI *defIF_DebitMoney) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//执行撤单操作						-- 接口8
typedef bool	(WINAPI *defIF_DebitCancel) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//获取扣款结果						-- 接口9
typedef bool	(WINAPI *defIF_GetDebitResult) (char* szParamContext, int& iParamSize, const int& iFormatType);
//设置交互界面						-- 接口10
typedef bool	(WINAPI *defIF_SetMMI) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//获取硬件参数						-- 接口11
typedef bool	(WINAPI *defIF_GetParam) (const char* szParamContext, const int& iParamSize, const int& iFormatType, char* szResultContext, int& iResultSize);
//获取移动支付设备状态				-- 接口12
typedef bool	(WINAPI *defIF_GetComponentStatus) (unsigned int& uStatus);
//执行传输关键信息数据操作          -- 接口13
typedef bool (WINAPI *defIF_TranslateData) (const char* szParamContext, const int& iParamSize, const int& iFormatType);

//江西电子发票接口
//展示电子发票二维码接口：cmdType=2001显示，2002清除
typedef bool (WINAPI *defIF_AsyncExecCmd) (const IN int cmdType, IN const char* szDataInfo, IN const int iSize);


/////////////////////////////////////////////////////// 特情相关的 /////////////////////////////////////////////////////// 
//初始化组件环境
typedef bool (WINAPI *IF_EventInitEnvironment3)(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void(*fun)(int option, int result, void* pIndPtr));
//清理资源
typedef bool (WINAPI *IF_EventDestroy)(void);
//获取上一次错误信息
typedef const char* (WINAPI *IF_EventGetLastErrorDesc)(void);
//获取组件状态
typedef bool (WINAPI *IF_EventGetComponentStatus)(IN OUT unsigned int& uStatus);
//特情处理开始
typedef bool (WINAPI *IF_EventDealStart)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//车辆队列确认
typedef bool (WINAPI *IF_EventCheckVehQueue)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//车辆信息确认
typedef bool (WINAPI *IF_EventCheckVehInfo)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//入口信息确认
typedef bool (WINAPI *IF_EventCheckEntryInfo)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//费额显示
typedef bool (WINAPI *IF_EventShowFeeInfo)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//支付状态显示
typedef bool (WINAPI *IF_EventPayResultDisplay)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//特情处理结束
typedef bool (WINAPI *IF_EventDealStop)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//ETC卡操作提示
typedef bool (WINAPI *IF_EventCardOperationNotify)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//修改车辆队列
typedef bool (WINAPI *IF_EventModifyVehQueue)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//启动扫码
typedef bool (WINAPI *IF_EventStartScan)(IN const int& iFeeValue);
//停止扫码
typedef bool (WINAPI *IF_EventStopScan)();
//最小费额授权
typedef bool (WINAPI *IF_EventFeeAuthorize)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//其他授权
typedef bool (WINAPI *IF_EventAuthorize)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//删除队列结果
typedef bool (WINAPI *IF_EventDelVehQueueResult)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//自由语音播报
typedef bool (WINAPI *IF_EventFreeVoicePlay)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);


// CTestToolDlg 对话框
class CTestToolDlg : public CDialogEx
{
// 构造
public:
	CTestToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTTOOL_SICHUAN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void log(std::string str);

public:
	UINT ReceiveMsgThreadID;
	CThreadReceiveRespMsg *pThreadReceiveMsg;
	CThreadReceiveRespMsg *pThreadReceiveMsgV15;

private:
	void loadDll();//普通不带特情的中间件
	void loadDllEvent();//带特情处理的中间件
	void loadDllRobot();//卡机的移动支付
	bool loadDllSucceed();//加载中间件是否成功
	bool initNetPayEnvSucceed();//初始化移动支付是否成功
	bool initEtcEventEnvSucceed();//初始化特情环境是否成功
	CString GetCurrentDirNew();
	void GetUIDebitInfo();
	const CString guid(IN const int iType = 1);
	BOOL FillpPayKeyItemsStruct(IN  int _OperationMode, IN int _PayCertificateType, IN CString _PayCertificateCode, OUT tagPayKeyItems& _ppPayKeyItems);

	// 特情相关的
	CString MyGetCurrenTime();
	CString GetUniqueTimeStr();
	CString GetEventTypeName(int EventTypeNo);
	CString GetEventHandleMethodName(int EventHandleMethodNo);
	bool ParseVehInfoCheckResp(char *pBuffer);

	// 特情—GUI循环播放
	static UINT GuiPlay1ThreadFun(LPVOID pParam);
	static UINT GuiPlay2ThreadFun(LPVOID pParam);
	static UINT GuiPlay3ThreadFun(LPVOID pParam);
	static UINT GuiPlayTotalThreadFun(LPVOID pParam);

	void GuiPlay1Eventstart();
	void GuiPlay2Eventstart();
	void GuiPlay3Eventstart();
	void GuiPlayXSwipeCardNotify();
	void GuiPlayXShowFeeInfo();
	void GuiPlay13PayResultShow();
	void GuiPlay2PayResultShow();
	void GuiPlay3CheckEntryInfoNone();
	void GuiPlayXEventstop();


private:
	CEdit edt_info;
	HMODULE gd_dll;
	bool m_bLoaded;//加载中间件动态库
	bool m_bNetPayEnvInitedFlag;//初始化移动支付的环境
	bool m_bEtcEventEnvInitedFlag;//初始化特情环境

	///////////////////////// 特情相关//////////////////////////////////////////////////////////////////
	IF_EventInitEnvironment3		fn_EventInitEnvironment3;
	IF_EventDestroy					fn_EventDestroy;
	IF_EventGetLastErrorDesc		fn_EventGetLastErrorDesc;
	IF_EventGetComponentStatus		fn_EventGetComponentStatus;
	IF_EventDealStart				fn_EventDealStart;
	IF_EventCheckVehQueue			fn_EventCheckVehQueue;
	IF_EventCheckVehInfo			fn_EventCheckVehInfo;
	IF_EventCheckEntryInfo			fn_EventCheckEntryInfo;
	IF_EventShowFeeInfo				fn_EventShowFeeInfo;
	IF_EventPayResultDisplay		fn_EventPayResultDisplay;
	IF_EventDealStop				fn_EventDealStop;
	IF_EventCardOperationNotify		fn_EventCardOperationNotify;
	IF_EventModifyVehQueue			fn_EventModifyVehQueue;
	IF_EventStartScan				fn_EventStartScan;
	IF_EventStopScan				fn_EventStopScan;
	IF_EventFeeAuthorize			fn_EventFeeAuthorize;
	IF_EventAuthorize				fn_EventAuthorize;
	IF_EventDelVehQueueResult		fn_EventDelVehQueueResult;
	IF_EventFreeVoicePlay			fn_EventFreeVoicePlay;
	////////////////////////////////////////////////////////////////////////////////////////////////////

	defIF_InitEnvironment			fn_InitEnvironment;
	defIF_Destroy					fn_Destroy;
	defIF_GetLastErrorDesc			fn_GetLastErrorDesc;
	defIF_StartGetAccountInfo	    fn_StartGetAccountInfo;
	defIF_StopGetAccountInfo	    fn_StopGetAccountInfo;
	defIF_GetAccountInfo			fn_GetAccountInfo;
	defIF_DebitMoney				fn_DebitMoney;
	defIF_DebitCancel				fn_DebitCancel;
	defIF_GetDebitResult			fn_GetDebitResult;
	defIF_SetMMI					fn_SetMMI;
	defIF_GetParam					fn_GetParam;
	defIF_GetComponentStatus		fn_GetComponentStatus;
	defIF_TranslateData				fn_TranslateData;

	//江西
	defIF_AsyncExecCmd				fn_AsyncExecCmd;

	/**************************移动支付信息**********************************************/

	//TWSDNetPayConfig.ini 配置文件内容
	//内部链接IP
	CString m_strServerIP;
	//内部链接端口
	WORD	m_wServerPort;

	//TWNetPayTest.ini 配置文件内容
	//省份编码
	int		m_iProvinceID;
	CString m_strProvinceID;
	//省份名称
	CString m_strStationName;
	//区域编码
	int		m_iAreaID;
	CString m_strAreaID;
	//路段编码
	int		m_iRoadID;
	CString m_strRoadID;
	//站点编码
	int		m_iStationID;
	CString m_strStationID;
	//车道编码
	int		m_iLaneID;
	CString m_strLaneID;

	//UI扣款信息
	int m_iDebitMoney;
	int m_iAxisNum;
	int m_iWeight;
	int m_iTimeOut;
	int m_iBalance;
	int m_iVType;
	int m_iVClass;
	int m_iVLColor;

	CString m_strPlate;
	CString m_strEntry;
	CString m_strOperator;
	CString m_strOperatotID;
	CString m_strCardNo;

	CString m_strLastPayIdentifier;

	static void* m_pIndPtr;

	/// 特情相关
	CEdit EditParam;
	static int No;
	std::string m_EventNo;
	static int VehPlateTail;	
	
	/*********当前特情车辆信息********************/
	CString m_CurrentVehPlate;
	int m_CurrentVehPlateColor;
	int m_CurrentVehClass;
	int m_CurrentVehType;
	int m_CurrentAxisCount;
	CString m_CurrentEntryTime;

	//新增ETC特情配置项
	int m_EventTypeNo;
	int m_EventHandleMethodNo;

	CWinThread* m_pGuiPlay1Thread;
	CWinThread* m_pGuiPlay2Thread;
	CWinThread* m_pGuiPlay3Thread;
	CWinThread* m_pGuiPlayTotalThread;
	bool m_bThread1Running;
	bool m_bThread2Running;
	bool m_bThread3Running;
	bool m_bThreadTotalRunning;

	PROVINCE_TYPE m_enCurProvince;
	MIDLLEWARE_TYPE m_enMidllewareType;

public:
	afx_msg LRESULT OnMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam);  //回调函数调用
	afx_msg LRESULT OnHWNDMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam); //句柄收到消息后调用
	afx_msg LRESULT OnHWNDMsgShowNetPayResultLog(WPARAM wParam, LPARAM lParam); //句柄收到消息后调用

	afx_msg void OnBnClickedButtonLoaddll();
	afx_msg void OnBnClickedButtonInitnetpayenv();

	afx_msg void OnBnClickedBtndebit();
	afx_msg void OnBnClickedBtnCanceldebit();
	afx_msg void OnBnClickedBtnNetpaydevstatus();
	afx_msg void OnBnClickedBtnInitenv();
	afx_msg void OnBnClickedBtnEventdestroy();
	afx_msg void OnBnClickedBtnEventstart();
	afx_msg void OnBnClickedBtnCheckvehqueue();
	afx_msg void OnBnClickedBtnCheckvehqueuedel();
	afx_msg void OnBnClickedBtnCheckvehinfo();
	afx_msg void OnBnClickedBtnCheckentryinfo();
	afx_msg void OnBnClickedBtnCheckentryinfoNone();
	afx_msg void OnBnClickedBtnShowfeeinfosingle();
	afx_msg void OnBnClickedBtnShowfeeinfo();
	afx_msg void OnBnClickedBtnPayresultshow();
	afx_msg void OnBnClickedBtnPayresultshowfail();
	afx_msg void OnBnClickedBtnSwipecard();
	afx_msg void OnBnClickedBtnRwcard();
	afx_msg void OnBnClickedBtnFixvehqueue();
	afx_msg void OnBnClickedBtnFeeauthorize();
	afx_msg void OnBnClickedBtnAuthorize();
	afx_msg void OnBnClickedBtnDelvehqueueresult();
	afx_msg void OnBnClickedBtnEtcdevstatus();
	afx_msg void OnBnClickedBtnEtcgetlasterrordescrip();
	afx_msg void OnBnClickedBtnChange2etcmode();
	afx_msg void OnBnClickedBtnEventstop();
	afx_msg void OnBnClickedBtnDestroyetcevent();
	afx_msg void OnBnClickedBtnDestroynetpay();
	afx_msg void OnBnClickedBtnFreedll();
	afx_msg void OnBnClickedBtnGuiplay1start();
	afx_msg void OnBnClickedBtnGuiplay1stop();
	afx_msg void OnBnClickedBtnGuiplay2start();
	afx_msg void OnBnClickedBtnGuiplay2stop();
	afx_msg void OnBnClickedBtnGuiplay3start();
	afx_msg void OnBnClickedBtnGuiplay3stop();
	afx_msg void OnBnClickedBtnGuiplaytotalstart();
	afx_msg void OnBnClickedBtnGuiplaytotalstop();
	afx_msg void OnBnClickedBtnAsyncexeccmd();
	afx_msg void OnBnClickedBtnClearqrcodecmd();
};

void MyCallBackFun(int iOption, int iResult, void* pIndPtr);