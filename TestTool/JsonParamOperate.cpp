/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�JsonParamOperate.cpp
 * ժ    Ҫ���ӿڲ���JSON��ʽ�����࣬ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#include "StdAfx.h"
#include "JsonParamOperate.h"

CJsonParamOperate::CJsonParamOperate(void)
{
}

CJsonParamOperate::~CJsonParamOperate(void)
{
}

time_t OleTimeToTime(DATE dTime)
{
	COleDateTimeSpan tSpan;
	COleDateTime t1 = (time_t)0;
	COleDateTime t2 = dTime;
	tSpan = t2 - t1;
	return time_t(tSpan.GetTotalSeconds());
}

time_t CString2time_t(LPCTSTR lpstime)
{
	CString strTemp(lpstime);
	COleDateTime oleGetTime;
	oleGetTime.ParseDateTime(strTemp);
	return OleTimeToTime(oleGetTime.m_dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CJsonParamOperate::FormatParamData2Json
//     
// ���������������յ������ݽṹת��Ϊ��׼��JSON��ʽ -- ֧������������ݱ任(�۷�)
//     
// ���������IN const CDVJsonParamDebitMoney* pDVJsonParamDebitMoney
// ���������CString
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamDebitMoney* pDVJsonParamDebitMoney)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamDebitMoney)
			throw -1;
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶ�û�����
		Json::Value DebitParamContext;
		// ��ʶ��ϸ������
		Json::Value Data;
		// ��ʶ�۷�������
		Json::Value Debit;
		// ��ʶ����������
		Json::Value Vehicle;
		// ��ʶҵ��������
		Json::Value Operation;
		// ��ʶ���������
		Json::Value Entry;

		//��ʶ�汾
		DebitParamContext["Version"]	= Json::Value(pDVJsonParamDebitMoney->Version);
		
		Data["OverTime"]				= Json::Value(pDVJsonParamDebitMoney->OverTime);
		Data["OperationMode"]			= Json::Value(pDVJsonParamDebitMoney->OperationMode);
	
		Debit["PayIdentifier"]			= Json::Value(pDVJsonParamDebitMoney->PayIdentifier);
		Debit["Money"]					= Json::Value(pDVJsonParamDebitMoney->Money);
		Debit["SubTime"]				= Json::Value(pDVJsonParamDebitMoney->SubTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		Debit["Type"]					= Json::Value(pDVJsonParamDebitMoney->DebitMainType);
		Debit["PayCertificateCode"]		= Json::Value(pDVJsonParamDebitMoney->PayCertificateCode);
		Debit["PayCertificateType"]		= Json::Value(pDVJsonParamDebitMoney->PayCertificateType);
		
		Vehicle["Type"]					= Json::Value(pDVJsonParamDebitMoney->VehicleType);
		Vehicle["Class"]				= Json::Value(pDVJsonParamDebitMoney->VehicleClass);
		Vehicle["License"]				= Json::Value(pDVJsonParamDebitMoney->VehicleLicense);
		Vehicle["VLColor"]				= Json::Value(pDVJsonParamDebitMoney->VLColor);
		Vehicle["AxisCount"]			= Json::Value(pDVJsonParamDebitMoney->AxisCount);
		Vehicle["Weight"]				= Json::Value(pDVJsonParamDebitMoney->Weight);
		//add 2018-03
		Json::Value AutoPlate;
		AutoPlate["AutoLicense"]=Json::Value(pDVJsonParamDebitMoney->AutoLicense);
		AutoPlate["AutoColor"]=Json::Value(pDVJsonParamDebitMoney->AutoColor);
		Vehicle["AutoPlate"] = AutoPlate;

		Operation["PassCertificateType"]= Json::Value(pDVJsonParamDebitMoney->PassCertificateType);
		//Operation["PassCertificateType"]= 3;

		Operation["CardID"]				= Json::Value(pDVJsonParamDebitMoney->CardID);
		Operation["TollDate"]			= Json::Value(pDVJsonParamDebitMoney->TollDate);
		Operation["ShiftID"]			= Json::Value(pDVJsonParamDebitMoney->ShiftID);
		Operation["OperatorName"]			= Json::Value(pDVJsonParamDebitMoney->OperatorName);
		Operation["OperatorID"]			= Json::Value(pDVJsonParamDebitMoney->OperatorID);
		Operation["Distance"]			= Json::Value(pDVJsonParamDebitMoney->Distance);
		Operation["TicketNo"]			= Json::Value(pDVJsonParamDebitMoney->TicketNo);
		Operation["AutoPrint"]			= Json::Value(pDVJsonParamDebitMoney->AutoPrint);
		
		
	
		Entry["AreaID"]					= Json::Value(pDVJsonParamDebitMoney->EntryAreaID);
		Entry["RoadID"]					= Json::Value(pDVJsonParamDebitMoney->EntryRoadID);
		Entry["StationID"]				= Json::Value(pDVJsonParamDebitMoney->EntryStationID);
		Entry["StationName"]			= Json::Value(pDVJsonParamDebitMoney->EntryStationName);
		Entry["Time"]					= Json::Value(pDVJsonParamDebitMoney->EntryTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		Entry["LaneID"]					= Json::Value(pDVJsonParamDebitMoney->EntryLane);
		Entry["License"]				= Json::Value(pDVJsonParamDebitMoney->EntryVehicleLicense);
		Entry["VColor"]					= Json::Value(pDVJsonParamDebitMoney->EntryVLColor);
		Entry["VType"]					= Json::Value(pDVJsonParamDebitMoney->EntryVehicleType);
		Entry["VClass"]					= Json::Value(pDVJsonParamDebitMoney->EntryVehicleClass);
		
		Data["Debit"]		= Debit;
		Data["Vehicle"]		= Vehicle;
		Data["Operation"]	= Operation;
		Data["Entry"]		= Entry;
		
		DebitParamContext["Data"]	= Data;
		root["DebitParamContext"]	= DebitParamContext;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());

	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CJsonParamOperate::FormatParamData2Json
//     
// ���������������յ������ݸ�ʽת��Ϊ��׼��JSON��ʽ -- ֧������������ݱ任(����)
//     
// ���������IN const CDVJsonParamDebitCancel* pDVJsonParamDebitCancel
// ���������CString
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamDebitCancel* pDVJsonParamDebitCancel)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamDebitCancel)
			throw -1;
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶ�û�����
		Json::Value DebitCancel;
		// ��ʶ��ϸ������
		Json::Value Data;
		// ��ʶ�ؼ�������
		Json::Value KeyItem;

		//��ʶ�汾
		DebitCancel["Version"]			= Json::Value(pDVJsonParamDebitCancel->Version);
		
		KeyItem["PayIdentifier"]		= Json::Value(pDVJsonParamDebitCancel->PayIdentifier);

		Data["TradeKeyItem"]			= KeyItem;
		DebitCancel["Data"]				= Data;
		root["DebitCancelParamContext"]	= DebitCancel;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CJsonParamOperate::FormatParamData2Json
//     
// ���������������յ������ݽṹת��Ϊ��׼��JSON��ʽ -- ���潻���������ݱ任
//     
// ���������IN const CDVJsonParamMMI* pDVJsonParamMMI
// ���������CString
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamMMI* pDVJsonParamMMI)
{
	
		/***
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamMMI)
			throw -1;
		Json::Value root;
		Json::Value MMIParamContext;
		Json::Value Data;


		MMIParamContext["Version"]  = Json::Value(pDVJsonParamMMI->Version);
		if(!(pDVJsonParamMMI->ClearMMI)) //��������������
		{
			Data["VType"]       = Json::Value(pDVJsonParamMMI->EntryVehicleType);
			Data["VClass"]      = Json::Value(pDVJsonParamMMI->EntryVehicleClass);
			Data["Money"]       = Json::Value(pDVJsonParamMMI->Money);
			Data["Weight"]      = Json::Value(pDVJsonParamMMI->Weight);
			Data["AxisCount"]   = Json::Value(pDVJsonParamMMI->AxisCount);
			Data["LimitWeight"] = Json::Value(pDVJsonParamMMI->LimitWeight);
			Data["Balance"]     = Json::Value(pDVJsonParamMMI->nBalance);
			Data["StationName"] = Json::Value(pDVJsonParamMMI->EntryStationName);

			Data["VColor"]      = Json::Value(pDVJsonParamMMI->EntryVLColor);
			Data["License"]     = Json::Value(pDVJsonParamMMI->EntryVehicleLicense);
			Data["CroseNum"]    = Json::Value(pDVJsonParamMMI->nCroseNum);
			Data["Program"]     = Json::Value(pDVJsonParamMMI->nProgram);
			Data["Remark1"]     = Json::Value(pDVJsonParamMMI->csRemark1);
			Data["Remark2"]     = Json::Value(pDVJsonParamMMI->csRemark2);
		}

		Data["ClearMMI"]		= Json::Value(pDVJsonParamMMI->ClearMMI);
		MMIParamContext["Data"] = Data;
		root["MMIParamContext"] = MMIParamContext;
		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& errCode)
	{
		switch(errCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
    **/

	
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamMMI)
			throw -1;
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶ��������
		Json::Value MMIParamContext;
		// ��ʶ��ϸ������
		Json::Value Data;
		// ��ʶ��������������
		Json::Value ItemData;
		// ��ʶ��ʾ��������
		Json::Value ViewText;
		// ��ʶ��ʾ��������
		Json::Value ViewVoice;
		// ��ʶ��ʾͼ������
		Json::Value ViewGraph;

		Json::Value item;

		//��ʶ�汾
		MMIParamContext["Version"]		= Json::Value(pDVJsonParamMMI->Version);

		Data["ClearMMI"]				= Json::Value(pDVJsonParamMMI->ClearMMI);

		Data["ItemCount"]				= Json::Value(int(pDVJsonParamMMI->Text.GetSize() + pDVJsonParamMMI->Voice.GetSize() + pDVJsonParamMMI->Graph.GetSize()));

		for(int i = 0;i<pDVJsonParamMMI->Text.GetSize();++i)
		{
			item["Type"]		= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Type);
			item["Sub"]			= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Sub);
			item["Row"]			= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Row);
			item["IsScroll"]	= Json::Value(pDVJsonParamMMI->Text.GetAt(i).IsScroll);
			item["Context"]		= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Context);

			ViewText.append(item);
		}

		for(int i = 0;i<pDVJsonParamMMI->Voice.GetSize();++i)
		{
			item["Type"]		= Json::Value(pDVJsonParamMMI->Voice.GetAt(i).Type);
			item["VoiceNo"]		= Json::Value(pDVJsonParamMMI->Voice.GetAt(i).No);

			ViewVoice.append(item);
		}

		for(int i = 0;i<pDVJsonParamMMI->Graph.GetSize();++i)
		{
			item["Type"]		= Json::Value(pDVJsonParamMMI->Graph.GetAt(i).Type);
			item["VoiceNo"]		= Json::Value(pDVJsonParamMMI->Graph.GetAt(i).No);

			ViewGraph.append(item);
		}

		ItemData["Text"]  = ViewText;
		ItemData["Voice"] = ViewVoice;
		ItemData["Graph"] = ViewGraph;

		Data["ItemData"]  = ItemData;

		MMIParamContext["Data"]		= Data;

		root["MMIParamContext"]		= MMIParamContext;

		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;

	/****************************************************************/
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CJsonParamOperate::FormatParamData2Json
//     
// ���������������յ������ݽṹת��Ϊ��׼��JSON��ʽ -- ֧������������ݱ任(�۷ѽ��)
//     
// ���������IN const CDVJsonParamDebitResult* pDVJsonParamDebitResult
// ���������CString
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamDebitResult* pDVJsonParamDebitResult)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamDebitResult)
			throw -1;
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶ�û�����
		Json::Value ResultContext;
		// ��ʶ��ϸ������
		Json::Value Data;
		// ��ʶ�ؼ���
		Json::Value KeyItem;
		/***********************************************************����JSAO���ݽṹ***********************************************************/
		
		//��ʶ�汾
		ResultContext["Version"]		= Json::Value(pDVJsonParamDebitResult->Version);
		//��ʶ�ۿ�����
		Data["ResultType"]				= Json::Value(pDVJsonParamDebitResult->ResultType);
		//��ʶִ��״̬
		Data["ExecuteCode"]				= Json::Value(pDVJsonParamDebitResult->ExecuteCode);
		//��ʶִ�н������
		Data["ExecuteDesc"]				= Json::Value(pDVJsonParamDebitResult->ExecuteDesc);

		//�ؼ������� -- ���װ�ȫ��
		KeyItem["TradeSecurityCode"]	= Json::Value(pDVJsonParamDebitResult->TradeSecurityCode);
		//�ؼ������� -- ������֧��ƽ̨����
		KeyItem["PayPlatformType"]		= Json::Value(pDVJsonParamDebitResult->PayPlatformType);
		//�ؼ������� -- ����ʶ����
		KeyItem["PayIdentifier"]		= Json::Value(pDVJsonParamDebitResult->PayIdentifier);
		//�ؼ������� -- ������
		KeyItem["DebitOrder"]			= Json::Value(pDVJsonParamDebitResult->DebitOrder);
		//�ؼ������� -- ����˿ۿ�ʱ��
		KeyItem["DebitTime"]			= Json::Value(pDVJsonParamDebitResult->DebitTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		//�ؼ������� -- ����ʱ��
		KeyItem["ConsumeTime"]			= Json::Value(pDVJsonParamDebitResult->ConsumeTime);

		Data["TradeKeyItem"]		= KeyItem;
		ResultContext["Data"]		= Data;
		root["DebitResultContext"]	= ResultContext;
		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CJsonParamOperate::FormatParamData2Json
//     
// ���������������յ������ݸ�ʽת��Ϊ��׼��JSON��ʽ -- �û����ݲ������ݱ任
//     
// ���������IN const CDVJsonParamAccount* pDVJsonParamAccount
// ���������CString
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamAccount* pDVJsonParamAccount)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamAccount)
			throw -1;
		// ��ʾ���� json ����
		Json::Value root;
		// ��ʶ�û�����
		Json::Value ParamAccount;
		// ��ʶ��ϸ������
		Json::Value Data;
		/***********************************************************����JSAO���ݽṹ***********************************************************/
		
		//��ʶ�汾
		ParamAccount["Version"]	= Json::Value(pDVJsonParamAccount->Version);
		//��ʶ��������
		Data["Type"]			= Json::Value(pDVJsonParamAccount->DataType);
		//��ʶ������Դ
		Data["Source"]			= Json::Value(pDVJsonParamAccount->DataSrc);
		//��ʶ��������
		Data["Context"]			= Json::Value(pDVJsonParamAccount->DataContext);

		ParamAccount["Data"]	= Data;
		root["AccountContext"]	= ParamAccount;
		//ת��Ϊ�ַ���
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}

	return strReturn;
}



////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CJsonParamOperate::FormatResponseResult2Class
//     
// ������������JSON��ʽ�ַ���ת��Ϊ��׼���ݽṹ��
//     
// ���������IN const CString& strResponseResult
//         ��IN const int& iDataType
// ���������CDVJsonBase*
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CDVJsonBase* CJsonParamOperate::FormatResponseResult2Class(IN const CString& strResponseResult,IN const int& iDataType)
{
	CDVJsonBase* pDVJsonBase = NULL;
	try
	{
		switch(iDataType)
		{
		case JSON_DT_PARAM_ACCOUNT:
			{
				pDVJsonBase = new CDVJsonParamAccount();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//�������ݸ�ʽת��
				Json::Reader reader;
				Json::Value  root;
				Json::Value  AccountContext;
				Json::Value  Data;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//��ȡ�û���Ϣ��
				if(Json::nullValue == root["AccountContext"].type())
					throw -4;
				AccountContext = root["AccountContext"];

				//��ȡ�汾
				if(Json::stringValue != AccountContext["Version"].type())
					throw -4;
				pDVJsonBase->Version = AccountContext["Version"].asCString();
				
				//��ȡ������Ϣ��
				if(Json::nullValue == AccountContext["Data"].type())
					throw -4;
				Data = AccountContext["Data"];

				//��ȡ�û���������
				if(Json::intValue != Data["Type"].type())
					throw -4;
				((CDVJsonParamAccount*)pDVJsonBase)->DataType = Data["Type"].asInt();

				//��ȡ�û�������Դ
				if(Json::intValue != Data["Source"].type())
					throw -4;
				((CDVJsonParamAccount*)pDVJsonBase)->DataSrc = Data["Source"].asInt();

				//��ȡ�û���������
				if(Json::stringValue != Data["Context"].type())
					throw -4;
				((CDVJsonParamAccount*)pDVJsonBase)->DataContext = Data["Context"].asCString();

			}
			break;
		case JSON_DT_PARAM_DEBIT_MONEY:
			{
				pDVJsonBase = new CDVJsonParamDebitMoney();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//�������ݸ�ʽת��
				Json::Reader reader;
				Json::Value  root;
				Json::Value  DebitParamContext;
				Json::Value  Data;
				Json::Value  Debit;
				Json::Value  Vehicle;
				Json::Value  Operation;
				Json::Value	 Entry;
				
				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//��ȡ����������
				if(Json::nullValue == root["DebitParamContext"].type())
					throw -4;
				DebitParamContext = root["DebitParamContext"];

				//��ȡ������ϸ��
				if(Json::nullValue == DebitParamContext["Data"].type())
					throw -4;
				Data = DebitParamContext["Data"];

				//��ȡ��ʱʱ��
				if(Json::intValue != Data["OverTime"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->OverTime = Data["OverTime"].asInt();

				//��ȡ�ۿ����ģʽ
				if(Json::intValue != Data["OperationMode"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->OperationMode = Data["OperationMode"].asInt();
				
				//��ȡ�ۿ���Ϣ��
				if(Json::nullValue == Data["Debit"].type())
					throw -4;
				Debit = Data["Debit"];

				//��ȡ�ۿ���
				if(Json::intValue != Debit["Money"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->Money = Debit["Money"].asInt();

				//��ȡ����ʶ����
				if(Json::stringValue != Debit["PayIdentifier"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PayIdentifier = Debit["PayIdentifier"].asCString();

				//��ȡ֧��ƾ֤����
				if(Json::intValue != Debit["PayCertificateType"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PayCertificateType = Debit["PayCertificateType"].asInt();

				//��ȡ֧��ƾ֤����
				if(Json::stringValue != Debit["PayCertificateCode"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PayCertificateCode = Debit["PayCertificateCode"].asCString();

				//��ȡ�ۿ�������
				if(Json::intValue != Debit["Type"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->DebitMainType = Debit["Type"].asInt();

				//��ȡ����ʱ��
				if(Json::stringValue != Debit["SubTime"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->SubTime = CTime(::CString2time_t(Debit["SubTime"].asCString()));

				//��ȡ������Ϣ��
				if(Json::nullValue == Data["Vehicle"].type())
					throw -4;
				Vehicle = Data["Vehicle"];
				
				//��ȡ����
				if(Json::intValue != Vehicle["Type"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VehicleType = Vehicle["Type"].asInt();

				//��ȡ����
				if(Json::intValue != Vehicle["Class"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VehicleClass = Vehicle["Class"].asInt();

				//��ȡ����
				if(Json::stringValue != Vehicle["License"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VehicleLicense = Vehicle["License"].asCString();

				//��ȡ������ɫ
				if(Json::intValue != Vehicle["VLColor"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VLColor = Vehicle["VLColor"].asInt();

				//��ȡ����
				if(Json::intValue != Vehicle["AxisCount"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->AxisCount = Vehicle["AxisCount"].asInt();
			
				//��ȡװ������
				if(Json::intValue != Vehicle["Weight"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->Weight = Vehicle["Weight"].asInt();

				//��ȡ����
				if(Json::intValue == Vehicle["LimitWeight"].type())
				{
					((CDVJsonParamDebitMoney*)pDVJsonBase)->LimitWeight = Vehicle["LimitWeight"].asInt();
				}

				//��ȡ������Ϣ��
				if(Json::nullValue == Data["Operation"].type())
					throw -4;
				Operation = Data["Operation"];

				//��ȡͨ��ƾ֤����
				if(Json::intValue != Operation["PassCertificateType"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PassCertificateType = Operation["PassCertificateType"].asInt();

				//��ȡͨ��ƾ֤����
				if(Json::stringValue != Operation["CardID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->CardID = Operation["CardID"].asCString();

				//��ȡͳ����
				if(Json::intValue == Operation["TollDate"].type())
				{
					((CDVJsonParamDebitMoney*)pDVJsonBase)->TollDate = Operation["TollDate"].asInt();
				}
				if(0 == ((CDVJsonParamDebitMoney*)pDVJsonBase)->TollDate)
				{
					if(Json::stringValue != Operation["TollDate"].type())
						throw -4;
					((CDVJsonParamDebitMoney*)pDVJsonBase)->TollDate = _ttoi(Operation["TollDate"].asCString());
				}

				//��ȡͳ�ư��
				if(Json::intValue != Operation["ShiftID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->ShiftID = Operation["ShiftID"].asInt();

				//��ȡ��ӡƱ�ݺ�
				if(Json::stringValue == Vehicle["TicketNo"].type())
				{
					((CDVJsonParamDebitMoney*)pDVJsonBase)->TicketNo = Vehicle["TicketNo"].asCString();
				}

				//��ȡ�����Ϣ��
				if(Json::nullValue == Data["Entry"].type())
					throw -4;
				Entry = Data["Entry"];

				//��ȡ����������
				if(Json::intValue != Entry["AreaID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryAreaID = Entry["AreaID"].asInt();

				//��ȡ���·�α���
				if(Json::intValue != Entry["RoadID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryRoadID = Entry["RoadID"].asInt();

				//��ȡ���վ�����
				if(Json::intValue != Entry["StationID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryStationID = Entry["StationID"].asInt();

				//��ȡ���վ������
				if(Json::stringValue != Entry["StationName"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryStationName = Entry["StationName"].asCString();
				
				//��ȡ��ڳ�������
				if(Json::intValue != Entry["LaneID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryLane = Entry["LaneID"].asInt();

				//��ȡ��ڳ���
				if(Json::intValue != Entry["VType"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVehicleType = Entry["VType"].asInt();

				//��ȡ��ڳ���
				if(Json::intValue != Entry["VClass"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVehicleClass = Entry["VClass"].asInt();

				//��ȡ��ڳ���
				if(Json::stringValue != Entry["License"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVehicleLicense = Entry["License"].asCString();

				//��ȡ��ڳ�����ɫ
				if(Json::intValue != Entry["VColor"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVLColor = Entry["VColor"].asInt();

				//��ȡ���ʱ��
				if(Json::stringValue != Entry["Time"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryTime = CTime(::CString2time_t(Entry["Time"].asCString()));
			}
			break;
		case JSON_DT_PARAM_DEBIT_CANCLE:
			{
				pDVJsonBase = new CDVJsonParamDebitCancel();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//�������ݸ�ʽת��
				Json::Reader reader;
				Json::Value  root;
				Json::Value  DebitCancelParamContext;
				Json::Value  Data;
				Json::Value  TradeKeyItem;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//��ȡ�û���Ϣ��
				if(Json::nullValue == root["DebitCancelParamContext"].type())
					throw -4;
				DebitCancelParamContext = root["DebitCancelParamContext"];

				//��ȡ�汾
				if(Json::stringValue != DebitCancelParamContext["Version"].type())
					throw -4;
				((CDVJsonParamDebitCancel*)pDVJsonBase)->Version = DebitCancelParamContext["Version"].asCString();
				
				//��ȡ������Ϣ��
				if(Json::nullValue == DebitCancelParamContext["Data"].type())
					throw -4;
				Data = DebitCancelParamContext["Data"];

				//��ȡ����������Ϣ��
		//		if(Json::nullValue == Data["TradeKeyItem"].type())
		//			throw -4;
		//		TradeKeyItem = Data["TradeKeyItem"];

				//��ȡ����ʶ����
				if(Json::stringValue != Data["PayIdentifier"].type())
					throw -4;
				((CDVJsonParamDebitCancel*)pDVJsonBase)->PayIdentifier = TradeKeyItem["PayIdentifier"].asCString();
			}
			break;
		case JSON_DT_PARAM_DEBIT_RESULT:
			{
				pDVJsonBase = new CDVJsonParamDebitResult();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//�������ݸ�ʽת��
				Json::Reader reader;
				Json::Value  root;
				Json::Value  DebitResultContext;
				Json::Value  Data;
				Json::Value  TradeKeyItem;
				Json::Value  ETCTradeInfo;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//��ȡ�ۿ�����Ϣ��
				if(Json::nullValue == root["DebitResultContext"].type())
					throw -4;
				DebitResultContext = root["DebitResultContext"];

				//��ȡ�汾
				if(Json::stringValue != DebitResultContext["Version"].type())
					throw -4;
				pDVJsonBase->Version = DebitResultContext["Version"].asCString();
				
				//��ȡ������Ϣ��
				if(Json::nullValue == DebitResultContext["Data"].type())
					throw -4;
				Data = DebitResultContext["Data"];

				//��ȡ�������
				if(Json::intValue != Data["ResultType"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ResultType = Data["ResultType"].asInt();

				//��ȡִ��״̬
				if(Json::intValue != Data["ExecuteCode"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ExecuteCode = Data["ExecuteCode"].asInt();
				
				//��ȡִ�н������
				if(Json::stringValue != Data["ExecuteDesc"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ExecuteDesc = Data["ExecuteDesc"].asCString();

				//��ȡ������Ϣ��
				if(Json::nullValue == Data["TradeKeyItem"].type())
					throw -4;
				TradeKeyItem = Data["TradeKeyItem"];


				//��ȡ����ʶ����
				if(Json::stringValue != TradeKeyItem["PayIdentifier"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->PayIdentifier = TradeKeyItem["PayIdentifier"].asCString();
				
				//��ȡ������֧��ƽ̨����
				if(Json::intValue != TradeKeyItem["PayPlatformType"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->PayPlatformType = TradeKeyItem["PayPlatformType"].asInt();
				
				//��ȡ���װ�ȫ��
				if(Json::stringValue != TradeKeyItem["TradeSecurityCode"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->TradeSecurityCode = TradeKeyItem["TradeSecurityCode"].asCString();
				
				//��ȡ��������
				if(Json::stringValue != TradeKeyItem["DebitOrder"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->DebitOrder = TradeKeyItem["DebitOrder"].asCString();
				
				//��ȡ�������ۿ�ʱ��
				if(Json::stringValue != TradeKeyItem["DebitTime"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->DebitTime = CString2time_t(TradeKeyItem["DebitTime"].asCString());//CTime(0);//AccountContext["Context"];
				
				//��ȡ����ʱ��
				if(Json::intValue != TradeKeyItem["ConsumeTime"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ConsumeTime = TradeKeyItem["ConsumeTime"].asInt();

				//��ȡ��Ʊ���
				if(Json::intValue != TradeKeyItem["PrintResultType"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->PrintTicketResult = TradeKeyItem["PrintResultType"].asInt();

				//ETC
				if( Json::nullValue == TradeKeyItem[ "ETCTradeInfo" ].type( ) )
					throw -4;
				ETCTradeInfo = TradeKeyItem[ "ETCTradeInfo" ];

				if( Json::stringValue != ETCTradeInfo[ "ETCProFile0015" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCProFile0015 = ETCTradeInfo[ "ETCProFile0015" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTerminalNo" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTerminalNo = ETCTradeInfo[ "ETCTerminalNo" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCProTradeNo" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCProTradeNo = ETCTradeInfo[ "ETCProTradeNo" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTerminalTradeNo" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTerminalTradeNo = ETCTradeInfo[ "ETCTerminalTradeNo" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTradeDate" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTradeDate = ETCTradeInfo[ "ETCTradeDate" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTradeTime" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTradeTime = ETCTradeInfo[ "ETCTradeTime" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTac" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTac = ETCTradeInfo[ "ETCTac" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTradeType" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTac = ETCTradeInfo[ "ETCTradeType" ].asCString();

				if( Json::intValue != ETCTradeInfo[" ETCBeforeBalance" ].type( ) )
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_iETCBeforeBalance = ETCTradeInfo[ "ETCBeforeBalance" ].asInt();

				if( Json::intValue != ETCTradeInfo[" ETCAfterBalance" ].type( ) )
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_iETCAfterBalance = ETCTradeInfo[ "ETCAfterBalance" ].asInt();


			}
			break;
		case JSON_DT_PARAM_MMI:
			{
				pDVJsonBase = new CDVJsonParamMMI();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//�������ݸ�ʽת��
				Json::Reader reader;
				Json::Value  root;
				Json::Value  MMIParamContext;
				Json::Value  Data;
				Json::Value  ItemData;
				Json::Value  aryTextView;
				Json::Value  aryVoiceView;
				Json::Value  aryGraphView;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//��ȡ��������������Ϣ��
				if(Json::nullValue == root["MMIParamContext"].type())
					throw -4;
				MMIParamContext = root["MMIParamContext"];

				//��ȡ�汾��
				if(Json::stringValue != MMIParamContext["Version"].type())
					throw -4;
				((CDVJsonParamMMI*)pDVJsonBase)->Version = MMIParamContext["Version"].asCString();

				//��ȡ������ʶ
				if( (Json::booleanValue != Data["ClearMMI"].type()) && (Json::intValue != Data["ClearMMI"].type()))
					throw -4;
				if(Json::booleanValue == Data["ClearMMI"].type())
				{
					((CDVJsonParamMMI*)pDVJsonBase)->ClearMMI = (int)Data["ClearMMI"].asBool();
				}
				else if(Json::intValue == Data["ClearMMI"].type())
				{
					((CDVJsonParamMMI*)pDVJsonBase)->ClearMMI = Data["ClearMMI"].asInt();
				}

				//��ȡ��������
				if(Json::nullValue == MMIParamContext["Data"].type())
					throw -4;
				Data = MMIParamContext["Data"];


				//������ָ��
				if(0 == ((CDVJsonParamMMI*)pDVJsonBase)->ClearMMI)
				{
					//��ȡ����������
					if(Json::intValue != Data["VType"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVehicleType = Data["VType"].asInt();

					if(Json::intValue != Data["VClass"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVehicleClass = Data["VClass"].asInt();

					if(Json::intValue != Data["Money"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->Money = Data["Money"].asInt();

					if(Json::intValue != Data["AixsCount"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->AxisCount = Data["AxisCount"].asInt();

					if(Json::intValue != Data["LimitWeight"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->LimitWeight = Data["LimitWeight"].asInt();
	
					if(Json::intValue != Data["Balance"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->nBalance = Data["Balance"].asInt();

					if(Json::stringValue != Data["StationName"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryStationName = Data["StationName"].asCString();

					if(Json::intValue != Data["VColor"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVLColor = Data["VColor"].asInt();

					if(Json::stringValue != Data["License"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVehicleLicense = Data["License"].asCString();

					if(Json::intValue != Data["CroseNum"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->nCroseNum = Data["CroseNum"].asInt();
	
					if(Json::intValue != Data["Program"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->nProgram = Data["Program"].asInt();
	
					if(Json::stringValue != Data["Remark1"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->csRemark1 = Data["Remark1"].asCString();

					if(Json::stringValue != Data["Remark2"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->csRemark2 = Data["Remark2"].asCString();

					}

					//��ȡ������ʾ��
					if(Json::nullValue == ItemData["Voice"].type())
						throw -4;
					aryVoiceView = root["Voice"];
					for (int i=0; i < aryVoiceView.size();++i)
					{
						tagMMIItemVoice sVoice = {0};
						if(Json::intValue == aryVoiceView[i]["Type"].type())
						{
							sVoice.Type = aryVoiceView[i]["Type"].asInt();
						}
						if(Json::intValue == aryVoiceView[i]["No"].type())
						{
							sVoice.No = aryVoiceView[i]["No"].asInt();
						}
						
						((CDVJsonParamMMI*)pDVJsonBase)->Voice.Add(sVoice);
					}

					//��ȡͼ����ʾ��
					if(Json::nullValue == ItemData["Voice"].type())
						throw -4;
					aryGraphView = root["Voice"];
					for (int i=0; i < aryGraphView.size();++i)
					{
						tagMMIItemGraph sGraph = {0};
						if(Json::intValue == aryGraphView[i]["Type"].type())
						{
							sGraph.Type = aryGraphView[i]["Type"].asInt();
						}
						if(Json::intValue == aryGraphView[i]["No"].type())
						{
							sGraph.No = aryGraphView[i]["No"].asInt();
						}
						
						((CDVJsonParamMMI*)pDVJsonBase)->Graph.Add(sGraph);
					}
				
			}
			break;
		}
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:

			break;
		case -2:
			break;
		case -3:
      		break;
		case -4:
			break;
		default:
			break;
		}
	}

	return pDVJsonBase;
}
