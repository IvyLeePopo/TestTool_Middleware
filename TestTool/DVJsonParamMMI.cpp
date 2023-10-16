#include "StdAfx.h"
#include "DVJsonParamMMI.h"

CDVJsonParamMMI::CDVJsonParamMMI(void)
:EntryVehicleLicense(_T(""))
,EntryVLColor(0)
,EntryVehicleType(0)
,EntryVehicleClass(0)
,EntryStationID(0)
,EntryStationName(_T(""))
,AxisCount(0)
,Weight(0)
,LimitWeight(0)
,Money(0)
,nBalance(0)
,nCroseNum(0)
,nProgram(0)
,csRemark1(_T(""))
,csRemark2(_T(""))
{
	Version.Format(_T("1.0"));
}

CDVJsonParamMMI::~CDVJsonParamMMI(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CDVJsonParamMMI::operator=
//     
// �������������������
//     
// ���������IN const CDVJsonParamMMI& cSrc
// ���������CDVJsonParamMMI&
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CDVJsonParamMMI& CDVJsonParamMMI::operator=(IN const CDVJsonParamMMI& cSrc)
{
	if(this != &cSrc)
	{
	}

	return *this;
}