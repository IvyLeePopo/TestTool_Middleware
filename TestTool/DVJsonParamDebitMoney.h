/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonParamDebit.h
 * ժ    Ҫ���ۿ�ӿڲ���JSON���ݸ�ʽ�����������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#pragma once
#include "dvjsonbase.h"

class CDVJsonParamDebitMoney :
	public CDVJsonBase
{
public:
	CDVJsonParamDebitMoney(void);
	~CDVJsonParamDebitMoney(void);
//���������
	CDVJsonParamDebitMoney& operator=(IN const CDVJsonParamDebitMoney& cSrc);
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
	//��ʱ�ȴ�ʱ��
	int		OverTime;
	//�ۿ�ģʽ
	int		OperationMode;
	/*****************Debit��ϸ��************************/
	//����ʶ����
	CString	PayIdentifier;
	//������֧��ƾ֤����
	CString PayCertificateCode;
	//������֧��ƾ֤���
	int		PayCertificateType;
	//���׽��(��λ����)
	int		Money;
	//����ʱ��
	CTime	SubTime;
	//����������
	int		DebitMainType;

	/*****************Vehicle��ϸ��************************/
	//����
	int		VehicleType;
	//����
	int		VehicleClass;
	//������ɫ
	int		VLColor;
	//����
	CString	VehicleLicense;
	//����
	int		AxisCount;
	//����
	int		Weight;
	//����
	int		LimitWeight;

	/*****************Operation��ϸ��************************/
	//ͨ��ƾ֤����
	int		PassCertificateType;
	//ͨ��ƾ֤����
	CString CardID;
	//ͳ����
	int		TollDate;
	//ͳ�ư��
	int		ShiftID;
	//��ӡƱ�ݺ�
	CString TicketNo;
	int AutoPrint;

	//�շ�Ա����
	CString OperatorName;
	//�շ�ԱId
	CString OperatorID;

	//add 2018-03
	CString AutoLicense;
	int AutoColor; 
	int Distance;

	/*****************Entry��ϸ��************************/
	//����������
	int		EntryAreaID;
	//���·�α���
	int		EntryRoadID;
	//���վ��
	int		EntryStationID;
	//���վ��
	CString EntryStationName;
	//���ʱ��
	CTime	EntryTime;
	//��ڳ���
	int		EntryLane;
	//��ڳ���
	CString	EntryVehicleLicense;
	//��ڳ�����ɫ
	int		EntryVLColor;
	//��ڳ���
	int		EntryVehicleType;
	//��ڳ���
	int		EntryVehicleClass;
};
