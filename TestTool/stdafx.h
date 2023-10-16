
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#include "json\json.h"


#define WM_THREAD_NETPAY_NOTIFY					WM_APP + 1
#define WM_THREAD_NETPAY_START_SCAN_CODE		WM_APP + 2
#define WM_THREAD_NETPAY_STOP_SCAN_CODE			WM_APP + 3
#define WM_THREAD_NETPAY_SHOW_INFO				WM_APP + 4
#define WM_THREAD_NETPAY_DEBIT_MONEY			WM_APP + 5
#define WM_THREAD_NETPAY_DEBIT_CANCEL			WM_APP + 6

#define WM_THREAD_NETPAY_INITENV      			WM_APP + 7
#define WM_THREAD_ETCEVENT_CALLBACK_SHOWLOG		WM_APP + 8  //此消息使用EtcEvent的回调函数
#define WM_HWND_NETPAY_RESULT_SHOWLOG			WM_APP + 9
#define WM_HWND_ETCEVENT_MESSAGE_SHOWLOG		WM_APP + 10  //此消息是句柄接收的消息通知

#define WM_THREAD_ETCEVENT_MESSAGE_SHOWLOG		WM_APP + 11  //此消息是线程接收的消息通知
#define WM_THREAD_NETPAY_MESSAGE_SHOWLOG		WM_APP + 12  //此消息是线程接收的消息通知

//JSON数据格式的类型区分
enum JSON_DATA_TYPE
{
	JSON_DT_PARAM_ACCOUNT		 = 0xFF01,	//用户参数JSON格式
	JSON_DT_PARAM_DEBIT_MONEY	 = 0xFF02,	//扣款参数JSON格式
	JSON_DT_PARAM_DEBIT_CANCLE	 = 0xFF03,	//撤单参数JSON格式
	JSON_DT_PARAM_DEBIT_RESULT	 = 0xFF04,	//扣款结果参数JSON格式
	JSON_DT_PARAM_MMI			 = 0xFF05,	//交互界面参数JSON格式
};

//交易关键信息
typedef struct _tagPayKeyItems
{
	//超时等待时间
	int		OverTime;
	//扣款模式
	int		OperationMode;

	/*****************Debit详细项************************/
	//交易识别码
	CString	PayIdentifier;
	//交易金额(单位：分)
	int		Money;
	//过车时间
	CTime	SubTime;
	//交易主分类
	int		DebitMainType;
	//第三方支付凭证
	CString PayCertificateCode;
	//支付凭证类型
	int		PayCertificateType;
	//余额
	int		Balance;

	//add 2018-03
	CString AutoLicense;
	int AutoColor;

	/*****************Vehicle详细项************************/
	//车种 0x01 -- 0xFF
	int		VehicleType;
	//车型 0客 1货
	int		VehicleClass;
	//车牌颜色
	int		VLColor;
	//车牌
	CString	VehicleLicense;
	//轴数
	int		AxisCount;
	//重量
	int		TotalWeight;
	//限重
	int		LimitWeight;

	/*****************Operation详细项************************/
	//通行凭证类型
	int		PassCertificateType;
	//通行凭证编码
	CString CardID;
	//统计日
	int		TollDate;
	//统计班次
	int		ShiftID;
	//打印票据号
	CString TicketNo;
	//add 2018-03
	int Distance;

	/*****************Entry详细项************************/
	//入口站点
	int		EntryStationID;
	//入口站名
	CString EntryStationName;
	//入口时间
	CTime	EntryTime;
	//入口车道
	int		EntryLane;
	//入口车牌
	CString	EntryVehicleLicense;
	//入口车牌颜色
	int		EntryVLColor;
	//入口车种
	int		EntryVehicleType;
	//入口车型
	int		EntryVehicleClass;
	//入口区域编码
	int		EntryAreaID;
	//入口路段号
	int		EntryRoadID;

	//收费员名称
	CString OperatorName;
	//收费员Id
	CString OperatorID;
}tagPayKeyItems;


//MMI参数的文字参数结构信息
typedef struct _tagMMIItemText
{
	int	Type;
	int Sub;
	int Row;
	BOOL  IsScroll;
	TCHAR Context[48];
}tagMMIItemText;

//MMI参数的声音参数结构信息
typedef struct _tagMMIItemVoice
{
	int	Type;
	int No;
}tagMMIItemVoice;

//MMI参数的图像参数结构信息
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


