/*
 * Copyright(C) 2015,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonBase.h
 * ժ    Ҫ��Json��������Ļ����࣬������̳������������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2015��11��12��
 */
#pragma once

class CDVJsonBase
{
public:
	CDVJsonBase(void);
	~CDVJsonBase(void);
//Interface
public:
	//��������
	void  Reset(IN const int& iType=0);
	//��������У����
	DWORD CreateInnerVerifyCode(void){return 0;}
	//�ڲ�������У��
	BOOL  InnerVerify(void){return TRUE;}
//��������
public:
	//����ʱ�� -- һ��ָ���ݴӷ��͵����������صĴ���ʱ��
	DWORD	m_dwConsumeTime;
	//�����ߴ�����
	HWND	m_hCallerWnd;
	//�������߳�ID
	DWORD	m_dwThreadID;
	
	/**************************����������Ҫ��������(����)**************************/
	
	//���ݰ��汾
	CString	Version;
	//���ݰ�����
	int		PackageType;
	//���ݰ����͵�����ʡ��
	int		ProvinceID;
	//��ȫ��֤
	CString	Token;
	//�ݽ�����
	int		SubmitCount;
	//�ݽ�ʱ��
	CString	SubmitTime;
	//QR����������
	CString	QRCodeData;
	//QR����Դ
	int		QRCodeType;
	//�ۿ�ʶ����
	CString PayIdentifier;
	/**************************��̨������Ҫ��������(����)**************************/
	
	//����˷�������
	int		ServerResponseCode;	
	//������Ϣ����
	CString ServerResponseResultDesc;

	//���������յ������ʱ��
	CString ServerReceiveDataTime;
	//�������������ݵ�ʱ��
	CString ServerResponseDataTime;
};
