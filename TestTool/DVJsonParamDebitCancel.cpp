/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonParamDebitCancel.cpp
 * ժ    Ҫ�������ӿڲ���JSON���ݸ�ʽ������ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#include "StdAfx.h"
#include "DVJsonParamDebitCancel.h"

CDVJsonParamDebitCancel::CDVJsonParamDebitCancel(void)
	:PayIdentifier(_T(""))
{
	Version.Format(_T("1.0"));
}

CDVJsonParamDebitCancel::~CDVJsonParamDebitCancel(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CDVJsonParamDebitCancel::operator=
//     
// �������������������
//     
// ���������IN const CDVJsonParamDebitCancel& cSrc
// ���������CDVJsonParamDebitCancel&
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CDVJsonParamDebitCancel& CDVJsonParamDebitCancel::operator=(IN const CDVJsonParamDebitCancel& cSrc)
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
		PayIdentifier			=	cSrc.PayIdentifier;
	}
	return *this;
}