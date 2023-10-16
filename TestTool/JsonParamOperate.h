/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�JsonParamOperate.h
 * ժ    Ҫ���ӿڲ���JSON��ʽ�����࣬�����ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#pragma once
#include "jsonoperationif.h"

class CJsonParamOperate :
	public CJsonOperationIF
{
public:
	CJsonParamOperate(void);
	~CJsonParamOperate(void);
//���غ���
public:
	//�����յ������ݽṹת��Ϊ��׼��JSON��ʽ -- ֧������������ݱ任(�۷�)
	CString FormatParamData2Json(IN const CDVJsonParamDebitMoney* pDVJsonParamDebitMoney);
	//�����յ������ݸ�ʽת��Ϊ��׼��JSON��ʽ -- ֧������������ݱ任(����)
	CString FormatParamData2Json(IN const CDVJsonParamDebitCancel* pDVJsonParamDebitCancel);
	//�����յ������ݽṹת��Ϊ��׼��JSON��ʽ -- ���潻���������ݱ任
	CString FormatParamData2Json(IN const CDVJsonParamMMI* pDVJsonParamMMI);

	//�����յ������ݽṹת��Ϊ��׼��JSON��ʽ -- ֧������������ݱ任(�۷ѽ��)
	CString FormatParamData2Json(IN const CDVJsonParamDebitResult* pDVJsonParamDebitResult);
	//�����յ������ݸ�ʽת��Ϊ��׼��JSON��ʽ -- �û����ݲ������ݱ任
	CString FormatParamData2Json(IN const CDVJsonParamAccount* pDVJsonParamAccount);

	//��JSON��ʽ�ַ���ת��Ϊ��׼���ݽṹ��
	CDVJsonBase* FormatResponseResult2Class(IN const CString& strResponseResult,IN const int& iDataType);
};
