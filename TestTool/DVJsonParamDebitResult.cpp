/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonParamDebitResult.cpp
 * ժ    Ҫ����ȡ�ۿ�ӿڲ���JSON���ݸ�ʽ������ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#include "StdAfx.h"
#include "DVJsonParamDebitResult.h"

CDVJsonParamDebitResult::CDVJsonParamDebitResult(void)
	:ResultType(0)
	,ExecuteCode(-1)
	,ExecuteDesc(_T(""))

	,PayIdentifier(_T(""))
	,PayPlatformType(0)
	,TradeSecurityCode(_T(""))
	,DebitOrder(_T(""))
	,PrintTicketResult( 0 )
	,m_iETCAfterBalance( 0 )
	,m_iETCBeforeBalance( 0 )
{
	Version.Format(_T("1.0"));
}

CDVJsonParamDebitResult::~CDVJsonParamDebitResult(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CDVJsonParamDebitResult::operator=
//     
// ����������
//     
// ���������IN const CDVJsonParamDebitResult& cSrc
// ���������CDVJsonParamDebitResult&
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CDVJsonParamDebitResult& CDVJsonParamDebitResult::operator=(IN const CDVJsonParamDebitResult& cSrc)
{
	if(this != &cSrc)
	{
		//�������ݸ���
		m_hCallerWnd			=	cSrc.m_hCallerWnd;
		m_dwThreadID			=	cSrc.m_dwThreadID;

		Version					=	cSrc.Version;
		PackageType				=	cSrc.PackageType;
		ProvinceID				=	cSrc.ProvinceID;
		Token					=	cSrc.Token;
		SubmitCount				=	cSrc.SubmitCount;
		SubmitTime				=	cSrc.SubmitTime;
		QRCodeData				=	cSrc.QRCodeData;
		QRCodeType				=	cSrc.QRCodeType;
		m_hCallerWnd			=	cSrc.m_hCallerWnd;
		m_dwThreadID			=	cSrc.m_dwThreadID;
		ServerResponseCode		=	cSrc.ServerResponseCode;
		ServerReceiveDataTime	=	cSrc.ServerReceiveDataTime;
		ServerResponseDataTime	=	cSrc.ServerResponseDataTime;
		//���������ݸ���
		ResultType				=	cSrc.ResultType;
		ExecuteCode				=	cSrc.ExecuteCode;
		ExecuteDesc				=	cSrc.ExecuteDesc;
	
		PayIdentifier			=	cSrc.PayIdentifier;
		PayPlatformType			=	cSrc.PayPlatformType;
		TradeSecurityCode		=	cSrc.TradeSecurityCode;
		DebitOrder				=	cSrc.DebitOrder;
	}
	return *this;
}