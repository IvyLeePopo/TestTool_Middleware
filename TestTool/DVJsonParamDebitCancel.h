/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonParamDebitCancel.h
 * ժ    Ҫ�������ӿڲ���JSON���ݸ�ʽ�����������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#pragma once
#include "dvjsonbase.h"

class CDVJsonParamDebitCancel :
	public CDVJsonBase
{
public:
	CDVJsonParamDebitCancel(void);
	~CDVJsonParamDebitCancel(void);
//���������
	CDVJsonParamDebitCancel& operator=(IN const CDVJsonParamDebitCancel& cSrc);
//Interface
public:
	//��������
	void  Reset(IN const int& iType=0);
	//��������У����
	DWORD CreateInnerVerifyCode(void){return 0;}
	//�ڲ�������У��
	BOOL  InnerVerify(void){return TRUE;}
//����
public:
	//����ʶ����
	CString PayIdentifier;
};
