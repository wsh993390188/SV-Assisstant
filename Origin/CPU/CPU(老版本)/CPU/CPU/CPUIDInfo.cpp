#include "../stdafx.h"
#include "CPUIDinfo.h"

ERROR_CODE Get_cpuid::Read_CPUID(OUT int buffer[4], IN size_t function_id, IN size_t subfunction_id)
{
	if ((function_id < 0) && (subfunction_id < 0))
	{
		return ILLEGAL_OPERAND;
	}

	try
	{
		__cpuidex(buffer, (int)function_id, (int)subfunction_id);
	}
	catch(exception e)
	{
		return FAILED_TO_GET_INFO;
	}

	return GET_CPUID_SUCCESS;
}

CPU_Brands Get_cpuid::Judge_CPU_Brands()
{
	int cpuInfo[4] = { -1 };
	ERROR_CODE status;
	status = this->Read_CPUID(cpuInfo);

	if (status == GET_CPUID_SUCCESS)
	{
		wchar_t vendor[0x20];
		memset(vendor, 0, sizeof(vendor));
		*reinterpret_cast<int*>(vendor) = cpuInfo[1];
		*reinterpret_cast<int*>(vendor + 4) = cpuInfo[3];
		*reinterpret_cast<int*>(vendor + 8) = cpuInfo[2];

		wstring vendor_ = vendor;

		if (vendor_ == _T("GenuineIntel"))
		{
			return Intel;
		}
		else if (vendor_ == _T("AuthenticAMD"))
		{
			return AMD;
		}
		else if (vendor_ == _T("CentaurHauls"))
		{
			return ZhaoXin;
		}
		return Others;
	}
	else
	{
		return FAILED;
	}
}

Get_cpuid::Get_cpuid()
{
	
}

Get_cpuid::~Get_cpuid()
{
}



