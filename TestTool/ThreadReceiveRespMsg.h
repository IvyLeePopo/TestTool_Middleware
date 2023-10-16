#pragma once
#include "Resource.h"
#include "ThreadBase.h"

class CThreadReceiveRespMsg : public CThreadBase
{
	DECLARE_DYNCREATE(CThreadReceiveRespMsg)

public:
	CThreadReceiveRespMsg();           // protected constructor used by dynamic creation
	virtual ~CThreadReceiveRespMsg();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
//���غ���
protected:
	//��������������Դ
	void fnReleaseResource();
	
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMsgThreadShowEtcEventLog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMsgThreadShowNetPayLog(WPARAM wParam, LPARAM lParam);
};