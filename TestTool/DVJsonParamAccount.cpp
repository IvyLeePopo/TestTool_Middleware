/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonAccount.cpp
 * ժ    Ҫ����ȡ�û���ϢJSON���ݸ�ʽ������ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#include "StdAfx.h"
#include "DVJsonParamAccount.h"

CDVJsonParamAccount::CDVJsonParamAccount(void)
	:DataType(0)
	,DataSrc(0)
	,DataContext(_T(""))
{
	Version.Format(_T("1.0"));
}

CDVJsonParamAccount::~CDVJsonParamAccount(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CDVJsonParamAccount::operator=
//     
// �������������������
//     
// ���������IN const CDVJsonParamAccount& cSrc
// ���������CDVJsonParamAccount&
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CDVJsonParamAccount& CDVJsonParamAccount::operator=(IN const CDVJsonParamAccount& cSrc)
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
		DataType		=	cSrc.DataType;
		DataSrc			=	cSrc.DataSrc;
		DataContext		=	cSrc.DataContext;
	}
	return *this;
}