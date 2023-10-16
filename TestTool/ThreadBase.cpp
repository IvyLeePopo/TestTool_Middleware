/*
 * Copyright(C) ������΢���� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�ThreadBase.cpp
 * ժ    Ҫ����������ڲ��߳���ͳһ�̳нӿڣ�ʵ���ļ�
 * ��ǰ�汾��1.0.0.0
 * ��    �ߣ������
 * �������ڣ�2020��10��18��
 */

#include "stdafx.h"
#include "ThreadBase.h"


// CThreadBase

IMPLEMENT_DYNCREATE(CThreadBase, CWinThread)

CThreadBase::CThreadBase()
	:m_bExit(FALSE)
	,m_hMainWnd(NULL)
	,m_nMainThreadID(0)
	,m_bWorking(FALSE)
{

}

CThreadBase::~CThreadBase()
{
}

BOOL CThreadBase::InitInstance()
{
	//��ʼ����������
//	::InitializeCriticalSection(&m_cs);
	return TRUE;
}

int CThreadBase::ExitInstance()
{
	//�����������
//	::DeleteCriticalSection(&m_cs);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadBase, CWinThread)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadBase::OnIdle
// ���������������߳�����д�����
// ���������LONG lCount
// ���������BOOL
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadBase::OnIdle(LONG lCount)
{
	if(m_bExit)
	{
		::PostQuitMessage(0);
		return FALSE;
	}
	Sleep(1);

	return TRUE;
}
