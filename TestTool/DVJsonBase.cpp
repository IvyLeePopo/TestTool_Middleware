/*
 * Copyright(C) 2015,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonBase.cpp
 * ժ    Ҫ��Json��������Ļ����࣬������̳�������ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2015��11��12��
 */
#include "StdAfx.h"
#include "DVJsonBase.h"

CDVJsonBase::CDVJsonBase(void)
	:m_dwConsumeTime(0)
	,m_hCallerWnd(NULL)
	,m_dwThreadID(0)

	,Version(_T(""))
	,PackageType(0)
	,ProvinceID(0)
	,Token(_T(""))
	,SubmitCount(0)
	,SubmitTime(_T(""))

	,QRCodeData(_T(""))
	,QRCodeType(0)
	,PayIdentifier(_T(""))

	,ServerResponseCode(0)
	,ServerReceiveDataTime(_T(""))
	,ServerResponseDataTime(_T(""))
{
}

CDVJsonBase::~CDVJsonBase(void)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CDVJsonBase::Reset
//     
// ������������������
//     
// ���������IN const int& iType -- ��������
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��1��22��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
void CDVJsonBase::Reset(IN const int& iType)
{
}