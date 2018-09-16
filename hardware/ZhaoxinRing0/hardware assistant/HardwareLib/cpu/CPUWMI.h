#pragma once

#include "..\WMI\WMIExtern.h"

class CPUWMI : public CWMI
{
public:
	CPUWMI();
private:
	HRESULT Excute(CComPtr<IWbemServices> pSvc);
public:
	std::string ProcessorId;
	UINT ExtClock;
	UINT Core;
	UINT Thread;
	UINT Revision;
	UINT MaxClockSpeed;
};

