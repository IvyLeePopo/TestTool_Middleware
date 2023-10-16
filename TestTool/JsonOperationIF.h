/*
 * Copyright(C) 2015,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�JsonOperationIF.h
 * ժ    Ҫ��Json���׼��������л���ת���Ľӿڶ�����࣬�����ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2015��11��12��
 */
#pragma once
#include "DVJsonParamAccount.h"
#include "DVJsonParamDebitMoney.h"
#include "DVJsonParamDebitCancel.h"
#include "DVJsonParamDebitResult.h"
#include "DVJsonParamMMI.h"
//���Ƶ������
//#ifndef _JSON_DEBUG_LOG_STRING
//#define _JSON_DEBUG_LOG_STRING 1
//#endif

class CJsonOperationIF
{
public:
	CJsonOperationIF();
	~CJsonOperationIF(void);
//�ӿڶ���
public:
	//�����յ������ݽṹת��Ϊ��׼��JSON��ʽ -- ֧������������ݱ任(�۷ѽ��)
	virtual CString FormatParamData2Json(IN const CDVJsonParamDebitResult* pDVJsonParamDebitResult)=0;
	//�����յ������ݸ�ʽת��Ϊ��׼��JSON��ʽ -- �û����ݲ������ݱ任
	virtual CString FormatParamData2Json(IN const CDVJsonParamAccount* pDVJsonParamAccount)=0;

	//�����������ص�JSON��ʽת��Ϊ��׼���ݽṹ��
	virtual CDVJsonBase* FormatResponseResult2Class(IN const CString& strResponseResult,IN const int& iDataType)=0;
//����
protected:
	//��ʱ�����ߴ�����
	HWND	m_hCallerWnd;
	//�����Ĵ�����Ϣ����
	CString	m_strLastErrorDesc;
};
