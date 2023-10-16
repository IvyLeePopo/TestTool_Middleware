
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#include "json\json.h"


#define WM_THREAD_NETPAY_NOTIFY					WM_APP + 1
#define WM_THREAD_NETPAY_START_SCAN_CODE		WM_APP + 2
#define WM_THREAD_NETPAY_STOP_SCAN_CODE			WM_APP + 3
#define WM_THREAD_NETPAY_SHOW_INFO				WM_APP + 4
#define WM_THREAD_NETPAY_DEBIT_MONEY			WM_APP + 5
#define WM_THREAD_NETPAY_DEBIT_CANCEL			WM_APP + 6

#define WM_THREAD_NETPAY_INITENV      			WM_APP + 7
#define WM_THREAD_ETCEVENT_CALLBACK_SHOWLOG		WM_APP + 8  //����Ϣʹ��EtcEvent�Ļص�����
#define WM_HWND_NETPAY_RESULT_SHOWLOG			WM_APP + 9
#define WM_HWND_ETCEVENT_MESSAGE_SHOWLOG		WM_APP + 10  //����Ϣ�Ǿ�����յ���Ϣ֪ͨ

#define WM_THREAD_ETCEVENT_MESSAGE_SHOWLOG		WM_APP + 11  //����Ϣ���߳̽��յ���Ϣ֪ͨ
#define WM_THREAD_NETPAY_MESSAGE_SHOWLOG		WM_APP + 12  //����Ϣ���߳̽��յ���Ϣ֪ͨ

//JSON���ݸ�ʽ����������
enum JSON_DATA_TYPE
{
	JSON_DT_PARAM_ACCOUNT		 = 0xFF01,	//�û�����JSON��ʽ
	JSON_DT_PARAM_DEBIT_MONEY	 = 0xFF02,	//�ۿ����JSON��ʽ
	JSON_DT_PARAM_DEBIT_CANCLE	 = 0xFF03,	//��������JSON��ʽ
	JSON_DT_PARAM_DEBIT_RESULT	 = 0xFF04,	//�ۿ�������JSON��ʽ
	JSON_DT_PARAM_MMI			 = 0xFF05,	//�����������JSON��ʽ
};

//���׹ؼ���Ϣ
typedef struct _tagPayKeyItems
{
	//��ʱ�ȴ�ʱ��
	int		OverTime;
	//�ۿ�ģʽ
	int		OperationMode;

	/*****************Debit��ϸ��************************/
	//����ʶ����
	CString	PayIdentifier;
	//���׽��(��λ����)
	int		Money;
	//����ʱ��
	CTime	SubTime;
	//����������
	int		DebitMainType;
	//������֧��ƾ֤
	CString PayCertificateCode;
	//֧��ƾ֤����
	int		PayCertificateType;
	//���
	int		Balance;

	//add 2018-03
	CString AutoLicense;
	int AutoColor;

	/*****************Vehicle��ϸ��************************/
	//���� 0x01 -- 0xFF
	int		VehicleType;
	//���� 0�� 1��
	int		VehicleClass;
	//������ɫ
	int		VLColor;
	//����
	CString	VehicleLicense;
	//����
	int		AxisCount;
	//����
	int		TotalWeight;
	//����
	int		LimitWeight;

	/*****************Operation��ϸ��************************/
	//ͨ��ƾ֤����
	int		PassCertificateType;
	//ͨ��ƾ֤����
	CString CardID;
	//ͳ����
	int		TollDate;
	//ͳ�ư��
	int		ShiftID;
	//��ӡƱ�ݺ�
	CString TicketNo;
	//add 2018-03
	int Distance;

	/*****************Entry��ϸ��************************/
	//���վ��
	int		EntryStationID;
	//���վ��
	CString EntryStationName;
	//���ʱ��
	CTime	EntryTime;
	//��ڳ���
	int		EntryLane;
	//��ڳ���
	CString	EntryVehicleLicense;
	//��ڳ�����ɫ
	int		EntryVLColor;
	//��ڳ���
	int		EntryVehicleType;
	//��ڳ���
	int		EntryVehicleClass;
	//����������
	int		EntryAreaID;
	//���·�κ�
	int		EntryRoadID;

	//�շ�Ա����
	CString OperatorName;
	//�շ�ԱId
	CString OperatorID;
}tagPayKeyItems;


//MMI���������ֲ����ṹ��Ϣ
typedef struct _tagMMIItemText
{
	int	Type;
	int Sub;
	int Row;
	BOOL  IsScroll;
	TCHAR Context[48];
}tagMMIItemText;

//MMI���������������ṹ��Ϣ
typedef struct _tagMMIItemVoice
{
	int	Type;
	int No;
}tagMMIItemVoice;

//MMI������ͼ������ṹ��Ϣ
typedef struct _tagMMIItemGraph
{
	int	Type;
	int No;
}tagMMIItemGraph;


typedef struct _tagETCMMIInfo
{
	int	ETCMMIType;
	CString ETCMMIData;
}tagETCMMIInfo;



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


