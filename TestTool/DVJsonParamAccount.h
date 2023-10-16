/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonAccount.h
 * ժ    Ҫ����ȡ�û���ϢJSON���ݸ�ʽ�����������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#pragma once
#include "dvjsonbase.h"

class CDVJsonParamAccount :
	public CDVJsonBase
{
public:
	CDVJsonParamAccount(void);
	~CDVJsonParamAccount(void);
//���������
	CDVJsonParamAccount& operator=(IN const CDVJsonParamAccount& cSrc);
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
	//��������
	int	DataType;
	//������Դ
	int	DataSrc;
	//��������
	CString DataContext;
};
