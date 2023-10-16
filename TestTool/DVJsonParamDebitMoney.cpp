/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonParamDebit.cpp
 * ժ    Ҫ���ۿ�ӿڲ���JSON���ݸ�ʽ������ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#include "StdAfx.h"
#include "DVJsonParamDebitMoney.h"

CDVJsonParamDebitMoney::CDVJsonParamDebitMoney(void)
	:OverTime(0)
	,OperationMode(0)

	,PayIdentifier(_T(""))
	,PayCertificateType(0)
	,PayCertificateCode(_T(""))
	,Money(0)
	,DebitMainType(0)

	,VehicleType(0)
	,VehicleClass(0)
	,VLColor(0)
	,VehicleLicense(_T(""))
	,AxisCount(0)
	,Weight(0)

	,PassCertificateType(0)
	,CardID(_T(""))
	,TollDate(0)
	,ShiftID(0)

	,EntryStationID(0)
	,EntryStationName(_T(""))
	,EntryLane(0)
	,EntryVehicleLicense(_T(""))
	,EntryVLColor(0)
	,EntryVehicleType(0)
	,EntryVehicleClass(0)
{
	Version.Format(_T("1.0"));
}

CDVJsonParamDebitMoney::~CDVJsonParamDebitMoney(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CDVJsonParamDebitMoney::operator=
//     
// �������������������
//     
// ���������IN const CDVJsonParamDebitMoney& cSrc
// ���������CDVJsonParamDebitMoney&
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CDVJsonParamDebitMoney& CDVJsonParamDebitMoney::operator=(IN const CDVJsonParamDebitMoney& cSrc)
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
		OverTime				=	cSrc.OverTime;
		OperationMode			=	cSrc.OperationMode;
	
		PayIdentifier			=	cSrc.PayIdentifier;
		PayCertificateCode		=	cSrc.PayCertificateCode;
		PayCertificateType		=	cSrc.PayCertificateType;
		Money					=	cSrc.Money;
		SubTime					=	cSrc.SubTime;
		DebitMainType			=	cSrc.DebitMainType;
		
		VehicleType				=	cSrc.VehicleType;
		VehicleClass			=	cSrc.VehicleClass;
		VLColor					=	cSrc.VLColor;
		VehicleLicense			=	cSrc.VehicleLicense;
		AxisCount				=	cSrc.AxisCount;
		Weight					=	cSrc.Weight;

		PassCertificateType		=	cSrc.PassCertificateType;
		CardID					=	cSrc.CardID;
		TollDate				=	cSrc.TollDate;
		ShiftID					=	cSrc.ShiftID;
	
		EntryStationID			=	cSrc.EntryStationID;
		EntryStationName		=	cSrc.EntryStationName;
		EntryTime				=	cSrc.EntryTime;
		EntryLane				=	cSrc.EntryLane;
		EntryVehicleLicense		=	cSrc.EntryVehicleLicense;
		EntryVLColor			=	cSrc.EntryVLColor;
		EntryVehicleType		=	cSrc.EntryVehicleType;
		EntryVehicleClass		=	cSrc.EntryVehicleClass;
	}
	return *this;
}