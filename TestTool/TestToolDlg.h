
// TestTool_SiChuanDlg.h : ͷ�ļ�
//

#pragma once

#include <string>
#include "ThreadReceiveRespMsg.h"

#define ROOT_PAY 1 //�������ƶ�֧��

//�м��������
enum MIDLLEWARE_TYPE
{
	BaseNoEvent = 1,//��ͨ����������м��
	BaseAndEvent = 2,//�����鴦����м��
	RobotNoEvent = 3,//��������������м��
};

//ʡ�ݵ�
enum PROVINCE_TYPE
{
	Jiangxi = 1,
	HeNan = 2,
	SiChuan = 3,
};


// TWSDNetPay_V2.1.dll
//��ʼ���������					-- �ӿ�1
typedef bool  (WINAPI  *defIF_InitEnvironment)(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStationID, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID);
//������Դ							-- �ӿ�2
typedef bool  (WINAPI *defIF_Destroy)(void);
//��ȡ�ƶ�֧�����ϴδ�����Ϣ		-- �ӿ�3
typedef char*       (WINAPI *defIF_GetLastErrorDesc) (void);
//������ȡ�û���Ϣ����				-- �ӿ�4
typedef bool  (WINAPI *defIF_StartGetAccountInfo) (const int& iOvertime);
//ֹͣ��ȡ�û���Ϣ����				-- �ӿ�5
typedef bool  (WINAPI *defIF_StopGetAccountInfo) (void);
//��ȡ�û���Ϣ						-- �ӿ�6
typedef bool  (WINAPI *defIF_GetAccountInfo) (char* szParamContext, int& iParamSize, const int& iFormatType);
//ִ�пۿ����						-- �ӿ�7
typedef bool	(WINAPI *defIF_DebitMoney) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//ִ�г�������						-- �ӿ�8
typedef bool	(WINAPI *defIF_DebitCancel) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡ�ۿ���						-- �ӿ�9
typedef bool	(WINAPI *defIF_GetDebitResult) (char* szParamContext, int& iParamSize, const int& iFormatType);
//���ý�������						-- �ӿ�10
typedef bool	(WINAPI *defIF_SetMMI) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡӲ������						-- �ӿ�11
typedef bool	(WINAPI *defIF_GetParam) (const char* szParamContext, const int& iParamSize, const int& iFormatType, char* szResultContext, int& iResultSize);
//��ȡ�ƶ�֧���豸״̬				-- �ӿ�12
typedef bool	(WINAPI *defIF_GetComponentStatus) (unsigned int& uStatus);
//ִ�д���ؼ���Ϣ���ݲ���          -- �ӿ�13
typedef bool (WINAPI *defIF_TranslateData) (const char* szParamContext, const int& iParamSize, const int& iFormatType);

//�������ӷ�Ʊ�ӿ�
//չʾ���ӷ�Ʊ��ά��ӿڣ�cmdType=2001��ʾ��2002���
typedef bool (WINAPI *defIF_AsyncExecCmd) (const IN int cmdType, IN const char* szDataInfo, IN const int iSize);


/////////////////////////////////////////////////////// ������ص� /////////////////////////////////////////////////////// 
//��ʼ���������
typedef bool (WINAPI *IF_EventInitEnvironment3)(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void(*fun)(int option, int result, void* pIndPtr));
//������Դ
typedef bool (WINAPI *IF_EventDestroy)(void);
//��ȡ��һ�δ�����Ϣ
typedef const char* (WINAPI *IF_EventGetLastErrorDesc)(void);
//��ȡ���״̬
typedef bool (WINAPI *IF_EventGetComponentStatus)(IN OUT unsigned int& uStatus);
//���鴦��ʼ
typedef bool (WINAPI *IF_EventDealStart)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//��������ȷ��
typedef bool (WINAPI *IF_EventCheckVehQueue)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//������Ϣȷ��
typedef bool (WINAPI *IF_EventCheckVehInfo)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//�����Ϣȷ��
typedef bool (WINAPI *IF_EventCheckEntryInfo)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//�Ѷ���ʾ
typedef bool (WINAPI *IF_EventShowFeeInfo)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//֧��״̬��ʾ
typedef bool (WINAPI *IF_EventPayResultDisplay)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//���鴦�����
typedef bool (WINAPI *IF_EventDealStop)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//ETC��������ʾ
typedef bool (WINAPI *IF_EventCardOperationNotify)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//�޸ĳ�������
typedef bool (WINAPI *IF_EventModifyVehQueue)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//����ɨ��
typedef bool (WINAPI *IF_EventStartScan)(IN const int& iFeeValue);
//ֹͣɨ��
typedef bool (WINAPI *IF_EventStopScan)();
//��С�Ѷ���Ȩ
typedef bool (WINAPI *IF_EventFeeAuthorize)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//������Ȩ
typedef bool (WINAPI *IF_EventAuthorize)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//ɾ�����н��
typedef bool (WINAPI *IF_EventDelVehQueueResult)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//������������
typedef bool (WINAPI *IF_EventFreeVoicePlay)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);


// CTestToolDlg �Ի���
class CTestToolDlg : public CDialogEx
{
// ����
public:
	CTestToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTTOOL_SICHUAN_DIALOG };

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
public:
	void log(std::string str);

public:
	UINT ReceiveMsgThreadID;
	CThreadReceiveRespMsg *pThreadReceiveMsg;
	CThreadReceiveRespMsg *pThreadReceiveMsgV15;

private:
	void loadDll();//��ͨ����������м��
	void loadDllEvent();//�����鴦����м��
	void loadDllRobot();//�������ƶ�֧��
	bool loadDllSucceed();//�����м���Ƿ�ɹ�
	bool initNetPayEnvSucceed();//��ʼ���ƶ�֧���Ƿ�ɹ�
	bool initEtcEventEnvSucceed();//��ʼ�����黷���Ƿ�ɹ�
	CString GetCurrentDirNew();
	void GetUIDebitInfo();
	const CString guid(IN const int iType = 1);
	BOOL FillpPayKeyItemsStruct(IN  int _OperationMode, IN int _PayCertificateType, IN CString _PayCertificateCode, OUT tagPayKeyItems& _ppPayKeyItems);

	// ������ص�
	CString MyGetCurrenTime();
	CString GetUniqueTimeStr();
	CString GetEventTypeName(int EventTypeNo);
	CString GetEventHandleMethodName(int EventHandleMethodNo);
	bool ParseVehInfoCheckResp(char *pBuffer);

	// ���顪GUIѭ������
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
	bool m_bLoaded;//�����м����̬��
	bool m_bNetPayEnvInitedFlag;//��ʼ���ƶ�֧���Ļ���
	bool m_bEtcEventEnvInitedFlag;//��ʼ�����黷��

	///////////////////////// �������//////////////////////////////////////////////////////////////////
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

	//����
	defIF_AsyncExecCmd				fn_AsyncExecCmd;

	/**************************�ƶ�֧����Ϣ**********************************************/

	//TWSDNetPayConfig.ini �����ļ�����
	//�ڲ�����IP
	CString m_strServerIP;
	//�ڲ����Ӷ˿�
	WORD	m_wServerPort;

	//TWNetPayTest.ini �����ļ�����
	//ʡ�ݱ���
	int		m_iProvinceID;
	CString m_strProvinceID;
	//ʡ������
	CString m_strStationName;
	//�������
	int		m_iAreaID;
	CString m_strAreaID;
	//·�α���
	int		m_iRoadID;
	CString m_strRoadID;
	//վ�����
	int		m_iStationID;
	CString m_strStationID;
	//��������
	int		m_iLaneID;
	CString m_strLaneID;

	//UI�ۿ���Ϣ
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

	/// �������
	CEdit EditParam;
	static int No;
	std::string m_EventNo;
	static int VehPlateTail;	
	
	/*********��ǰ���鳵����Ϣ********************/
	CString m_CurrentVehPlate;
	int m_CurrentVehPlateColor;
	int m_CurrentVehClass;
	int m_CurrentVehType;
	int m_CurrentAxisCount;
	CString m_CurrentEntryTime;

	//����ETC����������
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
	afx_msg LRESULT OnMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam);  //�ص���������
	afx_msg LRESULT OnHWNDMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam); //����յ���Ϣ�����
	afx_msg LRESULT OnHWNDMsgShowNetPayResultLog(WPARAM wParam, LPARAM lParam); //����յ���Ϣ�����

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