#include "../stdafx.h"
#include "CPUIDForIntel.h"

/************************************************************************/
/*                          CPU运算的函数                                */
/************************************************************************/
bool Get_Info_For_Intel::Get_ALL_CPUID()
{
	UINT MAX_CPUID_MAIN_LEAF = 0;				//可查询的CPUID最大值
	UINT Cpuid_sub_leaf = 0;					//可查询子叶的值
	wstring Mainleaf = "";						//保存主叶的字符
	wstring Subleaf = "";						//保存子叶的字符

	if (this->Read_CPUID(this->buffer) != GET_CPUID_SUCCESS)
	{
		return false;
	}
	//Basic CPUID Information
	MAX_CPUID_MAIN_LEAF = buffer[0];
	for (size_t i = 0; i <= MAX_CPUID_MAIN_LEAF; ++i)
	{
		Cpuid_sub_leaf = 0;
		this->Read_CPUID(buffer, i, 0);
		switch (i)
		{
		case 0x4:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{
				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[0] & 0xf) == 0)
					break;
				Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		case 0x7:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			Cpuid_sub_leaf = buffer[0];
			for (size_t j = 1; j <= Cpuid_sub_leaf; ++j)
			{
				this->Read_CPUID(buffer, i, j);
				Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(j);
				Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
				Cpuid_sub_leaf = buffer[0];
			}
			break;
		case 0x0B:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{
				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[2] & 0xff00) == 0)
					break;
				Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		case 0x0D:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{

				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((0 == buffer[0]) && (0 == buffer[1]) && (0 == buffer[2]) && (0 == buffer[3]))
					break;
				Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		case  0x0F:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			this->Read_CPUID(buffer, i, 1);
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(1);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			break;
		case 0x10:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			for (size_t j = 1; j < 4; ++j)
			{
				Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(j);
				Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			break;
		case 0x12:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			this->Read_CPUID(buffer, i, 1);
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(1);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			Cpuid_sub_leaf = 2;
			while (true)
			{
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[0] & 0xf) == 0x001b)
				{
					Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
					Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
					Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
					Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
					Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
					Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
					Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
					Cpuid_sub_leaf += 1;
				}
				else
				{
					break;
				}

			}
			Cpuid_sub_leaf = 0;
			break;
		case 0x13:	//啥都没有跳过
			break;
		case 0x14:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			this->Read_CPUID(buffer, i, 1);
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(1);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			break;
		case 0x17:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			Cpuid_sub_leaf = buffer[0];
			for (size_t j = 1; j <= Cpuid_sub_leaf; ++j)
			{
				this->Read_CPUID(buffer, i, j);
				Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(j);
				Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			break;
		case 0x18:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{
				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[0]) || (buffer[3] & 0xf))
				{
					break;
				}
				Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		default:
			Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			break;
		}
	}
	//Extended Function CPUID Information
	this->Read_CPUID(buffer, 0x80000000);
	MAX_CPUID_MAIN_LEAF = buffer[0];
	Cpuid_sub_leaf = 0;
	for (size_t i = 0x80000000; i <= MAX_CPUID_MAIN_LEAF; ++i)
	{
		Read_CPUID(buffer, i, Cpuid_sub_leaf);
		Intel_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
		Intel_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
		Intel_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
		Intel_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
		Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
		Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
		Intel_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
	}

	return true;
}

void Get_Info_For_Intel::excute_CPUID_FOR_Intel()
{
	this->Information_from_WMI();
	this->Information_from_CPUID0();
	this->Information_from_CPUID1();
	this->Information_from_CPUID2();
	this->Information_from_CPUID3();
	this->Information_from_CPUID4();
	this->Information_from_CPUID5();
	this->Information_from_CPUID6();
	this->Information_from_CPUID7();
	this->Information_from_CPUIDA();
	this->Information_from_CPUIDB();
	this->Information_from_CPUIDD();
//	this->GET_CODENAME(Intel_Data.Name);
}

Get_Info_For_Intel::Get_Info_For_Intel() : mposition(-1)
{
	memset(&Intel_Data.Manufacturer, 0, sizeof(Intel_Data.Manufacturer));
	Intel_cpuRegister.CPU_LEAF_NAME.reserve(30);
	Intel_cpuRegister.CPU_EAX.reserve(30);
	Intel_cpuRegister.CPU_EBX.reserve(30);
	Intel_cpuRegister.CPU_ECX.reserve(30);
	Intel_cpuRegister.CPU_EDX.reserve(30);
	this->UPdate();
}

Get_Info_For_Intel::~Get_Info_For_Intel()
{
}


CPU_RES Get_Info_For_Intel::Return_CPUID() const
{
	return Intel_cpuRegister;
}

CPUID_Data Get_Info_For_Intel::Return_info() const
{
	return Intel_Data;
}

void Get_Info_For_Intel::UPdate()
{
	Intel_Data.erase();
	Intel_cpuRegister.erase();
	this->Get_ALL_CPUID();
	this->excute_CPUID_FOR_Intel();
}

/************************************************************************/
/*                       CPUID数据解析的函数                             */
/************************************************************************/
int Get_Info_For_Intel::find_location(vector<wstring> vecIntegerArray, wstring search_string)
{
	vector <wstring>::iterator iElement = find(vecIntegerArray.begin(),
		vecIntegerArray.end(), search_string);
	if (iElement != vecIntegerArray.end())
	{
		int nPosition = (int)distance(vecIntegerArray.begin(), iElement);
		return nPosition;
	}
	return -1;
}


bool Get_Info_For_Intel::GET_FEATURE_INFO(IN int ECX, IN int EDX)
{
	//ECX
	(ECX & SSE3_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SSE3 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SSE3 = FALSE;
	(ECX & PCLMULQDQ_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PCLMULQDQ = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PCLMULQDQ = FALSE;
	(ECX & DTES64_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.DTES64 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.DTES64 = FALSE;
	(ECX & MONITOR_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.MONITOR = TRUE : Intel_Data.Feature_info.Intel_Feature_info.MONITOR = FALSE;
	(ECX & DS_CPL_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.DS_CPL = TRUE : Intel_Data.Feature_info.Intel_Feature_info.DS_CPL = FALSE;
	(ECX & VMX_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.VMX = TRUE : Intel_Data.Feature_info.Intel_Feature_info.VMX = FALSE;
	(ECX & SMX_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SMX = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SMX = FALSE;
	(ECX & EIST_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.EIST = TRUE : Intel_Data.Feature_info.Intel_Feature_info.EIST = FALSE;
	(ECX & TM2_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.TM2 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.TM2 = FALSE;
	(ECX & SSSE3_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SSSE3 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SSSE3 = FALSE;
	(ECX & CNXT_ID_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.CNXT_ID = TRUE : Intel_Data.Feature_info.Intel_Feature_info.CNXT_ID = FALSE;
	(ECX & SDBG_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SDBG = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SDBG = FALSE;
	(ECX & FMA_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.FMA = TRUE : Intel_Data.Feature_info.Intel_Feature_info.FMA = FALSE;
	(ECX & CMPXCHG16B_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.CMPXCHG16B = TRUE : Intel_Data.Feature_info.Intel_Feature_info.CMPXCHG16B = FALSE;
	(ECX & xTPR_Update_Control_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.xTPR_Update_Control = TRUE : Intel_Data.Feature_info.Intel_Feature_info.xTPR_Update_Control = FALSE;
	(ECX & PDCM_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PDCM = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PDCM = FALSE;
	(ECX & PCID_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PCID = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PCID = FALSE;
	(ECX & DCA_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.DCA = TRUE : Intel_Data.Feature_info.Intel_Feature_info.DCA = FALSE;
	(ECX & SSE4_1_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SSE4_1 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SSE4_1 = FALSE;
	(ECX & SSE4_2_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SSE4_2 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SSE4_2 = FALSE;
	(ECX & X2APIC_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.x2APIC = TRUE : Intel_Data.Feature_info.Intel_Feature_info.x2APIC = FALSE;
	(ECX & MOVBE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.MOVBE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.MOVBE = FALSE;
	(ECX & POPCNT_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.POPCNT = TRUE : Intel_Data.Feature_info.Intel_Feature_info.POPCNT = FALSE;
	(ECX & TSC_Deadline_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.TSC_Deadline = TRUE : Intel_Data.Feature_info.Intel_Feature_info.TSC_Deadline = FALSE;
	(ECX & AESNI_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.AESNI = TRUE : Intel_Data.Feature_info.Intel_Feature_info.AESNI = FALSE;
	(ECX & XSAVE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.XSAVE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.XSAVE = FALSE;
	(ECX & OSXSAVE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.OSXSAVE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.OSXSAVE = FALSE;
	(ECX & AVX_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.AVX = TRUE : Intel_Data.Feature_info.Intel_Feature_info.AVX = FALSE;
	(ECX & F16C_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.F16C = TRUE : Intel_Data.Feature_info.Intel_Feature_info.F16C = FALSE;
	(ECX & RDRAND_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.RDRAND = TRUE : Intel_Data.Feature_info.Intel_Feature_info.RDRAND = FALSE;
	//EDX
	(EDX & FPU_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.FPU = TRUE : Intel_Data.Feature_info.Intel_Feature_info.FPU = FALSE;
	(EDX & VME_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.VME = TRUE : Intel_Data.Feature_info.Intel_Feature_info.VME = FALSE;
	(EDX & DE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.DE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.DE = FALSE;
	(EDX & PSE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PSE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PSE = FALSE;
	(EDX & TSC_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.TSC = TRUE : Intel_Data.Feature_info.Intel_Feature_info.TSC = FALSE;
	(EDX & MSR_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.MSR = TRUE : Intel_Data.Feature_info.Intel_Feature_info.MSR = FALSE;
	(EDX & PAE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PAE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PAE = FALSE;
	(EDX & MCE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.MCE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.MCE = FALSE;
	(EDX & CX8_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.CX8 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.CX8 = FALSE;
	(EDX & APIC_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.APIC = TRUE : Intel_Data.Feature_info.Intel_Feature_info.APIC = FALSE;
	(EDX & SEP_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SEP = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SEP = FALSE;
	(EDX & MTRR_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.MTRR = TRUE : Intel_Data.Feature_info.Intel_Feature_info.MTRR = FALSE;
	(EDX & PGE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PGE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PGE = FALSE;
	(EDX & MCA_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.MCA = TRUE : Intel_Data.Feature_info.Intel_Feature_info.MCA = FALSE;
	(EDX & CMOV_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.CMOV = TRUE : Intel_Data.Feature_info.Intel_Feature_info.CMOV = FALSE;
	(EDX & PAT_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PAT = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PAT = FALSE;
	(EDX & PSE_36_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PSE_36 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PSE_36 = FALSE;
	(EDX & PSN_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PSN = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PSN = FALSE;
	(EDX & CLFSH_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.CLFSH = TRUE : Intel_Data.Feature_info.Intel_Feature_info.CLFSH = FALSE;
	(EDX & DS_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.DS = TRUE : Intel_Data.Feature_info.Intel_Feature_info.DS = FALSE;
	(EDX & ACPI_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.ACPI = TRUE : Intel_Data.Feature_info.Intel_Feature_info.ACPI = FALSE;
	(EDX & MMX_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.MMX = TRUE : Intel_Data.Feature_info.Intel_Feature_info.MMX = FALSE;
	(EDX & FXSR_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.FXSR = TRUE : Intel_Data.Feature_info.Intel_Feature_info.FXSR = FALSE;
	(EDX & SSE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SSE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SSE = FALSE;
	(EDX & SSE2_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SSE2 = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SSE2 = FALSE;
	(EDX & SS_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.SS = TRUE : Intel_Data.Feature_info.Intel_Feature_info.SS = FALSE;
	(EDX & HTT_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.HTT = TRUE : Intel_Data.Feature_info.Intel_Feature_info.HTT = FALSE;
	(EDX & TM_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.TM = TRUE : Intel_Data.Feature_info.Intel_Feature_info.TM = FALSE;
	(EDX & PBE_FLAGS) ? Intel_Data.Feature_info.Intel_Feature_info.PBE = TRUE : Intel_Data.Feature_info.Intel_Feature_info.PBE = FALSE;
	return true;
}

BOOL Get_Info_For_Intel::DF_DM_info(IN UINT DF_DM)
{
	switch (DF_DM)
	{
	case 0x0685:	Intel_Data.Architecture = _T("Future Intel(R) Xeon Phi(TM) Processor based on Knights Mill microarchitecture");
		Intel_Data.Code_Name = _T("Knight's Corner");
		break;
	case 0x0657:	Intel_Data.Architecture = _T("Intel(R) Xeon Phi(TM) Processor 3200, 5200, 7200 Series based on Knights Landing microarchitecture");
		Intel_Data.Code_Name = _T("Knight's Corner");
		break;
	case 0x0666:	Intel_Data.Architecture = _T("Future Intel(R) Core(TM) processors based on Cannon Lake microarchitecture");
		Intel_Data.Code_Name = _T("Cannon Lake");
		break;
	case 0x068E:	Intel_Data.Architecture = _T("7th generation Intel(R) Core(TM) processors based on Kaby Lake microarchitecture");
		Intel_Data.Code_Name = _T("Kaby Lake");
		break;
	case 0x069E:	Intel_Data.Architecture = _T("7th generation Intel(R) Core(TM) processors based on Kaby Lake microarchitecture");
		Intel_Data.Code_Name = _T("Kaby Lake");
		break;
	case 0x0655:	Intel_Data.Architecture = _T("Intel(R) Xeon(R) Processor Scalable Family based on Skylake microarchitecture");
		Intel_Data.Code_Name = _T("Skylake");
		break;
	case 0x064E:	Intel_Data.Architecture = _T("6th generation Intel Core processors and Intel Xeon processor E3-1500m v5 product family and E3-1200 v5 product family based on Skylake microarchitecture");
		Intel_Data.Code_Name = _T("Skylake");
		break;
	case 0x065E:	Intel_Data.Architecture = _T("6th generation Intel Core processors and Intel Xeon processor E3-1500m v5 product family and E3-1200 v5 product family based on Skylake microarchitecture");
		Intel_Data.Code_Name = _T("Skylake");
		break;
	case 0x0656:	Intel_Data.Architecture = _T("Intel Xeon processor D-1500 product family based on Broadwell microarchitecture");
		Intel_Data.Code_Name = _T("Broadwell");
		break;
	case 0x064F:	Intel_Data.Architecture = _T("Intel Xeon processor E5 v4 Family based on Broadwell microarchitecture, Intel Xeon processor E7 v4 Family, Intel Core i7-69xx Processor Extreme Edition");
		Intel_Data.Code_Name = _T("Broadwell");
		break;
	case 0x0647:	Intel_Data.Architecture = _T("5th generation Intel Core processors, Intel Xeon processor E3-1200 v4 product family based on Broadwell microarchitecture");
		Intel_Data.Code_Name = _T("Broadwell");
		break;
	case 0x063D:	Intel_Data.Architecture = _T("Intel Core M-5xxx Processor, 5th generation Intel Core processors based on Broadwell microarchitecture");
		Intel_Data.Code_Name = _T("Broadwell");
		break;
	case 0x063F:	Intel_Data.Architecture = _T("Intel Xeon processor E5-4600/2600/1600 v3 product families, Intel Xeon processor E7 v3 product families based on Haswell-E microarchitecture, Intel Core i7-59xx Processor Extreme Edition");
		Intel_Data.Code_Name = _T("Haswell-E");
		break;
	case 0x063C:	Intel_Data.Architecture = _T("4th Generation Intel Core processor and Intel Xeon processor E3-1200 v3 product family based on Haswell microarchitecture");
		Intel_Data.Code_Name = _T("Haswell");
		break;
	case 0x0645:	Intel_Data.Architecture = _T("4th Generation Intel Core processor and Intel Xeon processor E3-1200 v3 product family based on Haswell microarchitecture");
		Intel_Data.Code_Name = _T("Haswell");
		break;
	case 0x0646:	Intel_Data.Architecture = _T("4th Generation Intel Core processor and Intel Xeon processor E3-1200 v3 product family based on Haswell microarchitecture");
		Intel_Data.Code_Name = _T("Haswell");
		break;
	case 0x063E:	Intel_Data.Architecture = _T("Intel Xeon processor E7-8800/4800/2800 v2 product families based on Ivy Bridge-E microarchitecture \n Intel Xeon processor E5-2600/1600 v2 product families and Intel Xeon processor E5-2400 v2 product family based on Ivy Bridge-E microarchitecture, Intel Core i7-49xx Processor Extreme Edition");
		break;
	case 0x063A:	Intel_Data.Architecture = _T("3rd Generation Intel Core Processor and Intel Xeon processor E3-1200 v2 product family based on Ivy	Bridge microarchitecture");
		Intel_Data.Code_Name = _T("Ivy Bridge");
		break;
	case 0x062D:	Intel_Data.Architecture = _T("Intel Xeon processor E5 Family based on Intel microarchitecture code name Sandy Bridge, Intel Core i7-39xx Processor Extreme Edition");
		Intel_Data.Code_Name = _T("Sandy Bridge");
		break;
	case 0x062F:	Intel_Data.Architecture = _T("Intel Xeon Processor E7 Family");
		break;
	case 0x062A:	Intel_Data.Architecture = _T("Intel Xeon processor E3-1200 product family; 2nd Generation Intel Core i7, i5, i3 Processors 2xxx Series");
		break;
	case 0x062E:	Intel_Data.Architecture = _T("Intel Xeon processor 7500, 6500 series");
		break;
	case 0x0625:	Intel_Data.Architecture = _T("Intel Xeon processors 3600, 5600 series, Intel Core i7, i5 and i3 Processors");
		break;
	case 0x062C:	Intel_Data.Architecture = _T("Intel Xeon processors 3600, 5600 series, Intel Core i7, i5 and i3 Processors");
		break;
	case 0x061E:	Intel_Data.Architecture = _T("Intel Core i7 and i5 Processors");
		break;
	case 0x061F:	Intel_Data.Architecture = _T("Intel Core i7 and i5 Processors");
		break;
	case 0x061A:	Intel_Data.Architecture = _T("Intel Core i7 Processor, Intel Xeon processor 3400, 3500, 5500 series");
		break;
	case 0x061D:	Intel_Data.Architecture = _T("Intel Xeon processor MP 7400 series");
		break;
	case 0x0617:	Intel_Data.Architecture = _T("Intel Xeon processor 3100, 3300, 5200, 5400 series, Intel Core 2 Quad processors 8000, 9000 series");
		break;
	case 0x060F:	Intel_Data.Architecture = _T("Intel Xeon processor 3000, 3200, 5100, 5300, 7300 series, Intel Core 2 Quad processor 6000 series, Intel Core 2 Extreme 6000 series, Intel Core 2 Duo 4000, 5000, 6000, 7000 series processors, Intel Pentium dual-core processors");
		break;
	case 0x060E:	Intel_Data.Architecture = _T("Intel Core Duo, Intel Core Solo processors");
		break;
	case 0x060D:	Intel_Data.Architecture = _T("Intel Pentium M processor");
		break;
	case 0x067A:	Intel_Data.Architecture = _T("Future Intel(R) Atom(TM) processors based on Goldmont Plus Microarchitecture");
		Intel_Data.Code_Name = _T("Goldmont Plus");
		break;
	case 0x065F:	Intel_Data.Architecture = _T("Intel Atom processors based on Goldmont Microarchitecture (code name Denverton)");
		Intel_Data.Code_Name = _T("Denverton");
		break;
	case 0x065C:	Intel_Data.Architecture = _T("Intel Atom processors based on Goldmont Microarchitecture");
		Intel_Data.Code_Name = _T("Denverton");
		break;
	case 0x064C:	Intel_Data.Architecture = _T("Intel Atom processor X7-Z8000 and X5-Z8000 series based on Airmont Microarchitecture");
		Intel_Data.Code_Name = _T("Airmont");
		break;
	case 0x065D:	Intel_Data.Architecture = _T("Intel Atom processor X3-C3000 based on Silvermont Microarchitecture");
		Intel_Data.Code_Name = _T("Silvermont");
		break;
	case 0x065A:	Intel_Data.Architecture = _T("Intel Atom processor Z3500 series");
		break;
	case 0x064A:	Intel_Data.Architecture = _T("Intel Atom processor Z3400 series");
		break;
	case 0x0637:	Intel_Data.Architecture = _T("Intel Atom processor E3000 series, Z3600 series, Z3700 series");
		break;
	case 0x064D:	Intel_Data.Architecture = _T("Intel Atom processor C2000 series");
		break;
	case 0x0636:	Intel_Data.Architecture = _T("Intel Atom processor S1000 Series");
		break;
	case 0x061C:	Intel_Data.Architecture = _T("Intel Atom processor family, Intel Atom processor D2000, N2000, E2000, Z2000, C1000 series");
		break;
	case 0x0626:	Intel_Data.Architecture = _T("Intel Atom processor family, Intel Atom processor D2000, N2000, E2000, Z2000, C1000 series");
		break;
	case 0x0627:	Intel_Data.Architecture = _T("Intel Atom processor family, Intel Atom processor D2000, N2000, E2000, Z2000, C1000 series");
		break;
	case 0x0635:	Intel_Data.Architecture = _T("Intel Atom processor family, Intel Atom processor D2000, N2000, E2000, Z2000, C1000 series");
		break;
	case 0x0F06:	Intel_Data.Architecture = _T("Intel Xeon processor 7100, 5000 Series, Intel Xeon Processor MP, Intel Pentium 4, Pentium D	processors");
		break;
	case 0x0F03:	Intel_Data.Architecture = _T("Intel Xeon processor, Intel Xeon processor MP, Intel Pentium 4, Pentium D processors");
		break;
	case 0x0F04:	Intel_Data.Architecture = _T("Intel Xeon processor, Intel Xeon processor MP, Intel Pentium 4, Pentium D processors");
		break;
	case 0x0609:	Intel_Data.Architecture = _T("Intel Pentium M processor");
		break;
	case 0x0F02:	Intel_Data.Architecture = _T("Intel Xeon Processor, Intel Xeon processor MP, Intel Pentium 4 processors");
		break;
	case 0x0F00:	Intel_Data.Architecture = _T("Intel Xeon Processor, Intel Xeon processor MP, Intel Pentium 4 processors");
		break;
	case 0x0F01:	Intel_Data.Architecture = _T("Intel Xeon Processor, Intel Xeon processor MP, Intel Pentium 4 processors");
		break;
	case 0x0607:	Intel_Data.Architecture = _T("Intel Pentium III Xeon processor, Intel Pentium III processor");
		break;
	case 0x0608:	Intel_Data.Architecture = _T("Intel Pentium III Xeon processor, Intel Pentium III processor");
		break;
	case 0x060A:	Intel_Data.Architecture = _T("Intel Pentium III Xeon processor, Intel Pentium III processor");
		break;
	case 0x060B:	Intel_Data.Architecture = _T("Intel Pentium III Xeon processor, Intel Pentium III processor");
		break;
	case 0x0603:	Intel_Data.Architecture = _T("Intel Pentium II Xeon processor, Intel Pentium II processor");
		break;
	case 0x0605:	Intel_Data.Architecture = _T("Intel Pentium II Xeon processor, Intel Pentium II processor");
		break;
	case 0x0601:	Intel_Data.Architecture = _T("Intel Pentium Pro processor");
		break;
	case 0x0501:	Intel_Data.Architecture = _T("Intel Pentium processor, Intel Pentium processor with MMX Technology");
		break;
	case 0x0502:	Intel_Data.Architecture = _T("Intel Pentium processor, Intel Pentium processor with MMX Technology");
		break;
	case 0x0504:	Intel_Data.Architecture = _T("Intel Pentium processor, Intel Pentium processor with MMX Technology");
		break;
	case 0x0509:	Intel_Data.Architecture = _T("The Intel(R) Quark(TM) SoC X1000 processor");
		break;
	default:
		Intel_Data.Architecture = _T("");
		break;
	}
	return TRUE;
}

void Get_Info_For_Intel::cache_Descriptors(IN unsigned char cacheinfo, vector<wstring>& Cache_Temp)
{
	switch (cacheinfo)
	{
	case 0x00:
		break;
	case 0x01: Cache_Temp.emplace_back(_T("Instruction TLB: 4-KBPages, 4-way set associative, 32 entries"));
		break;
	case 0x02: Cache_Temp.emplace_back(_T("Instruction TLB: 4-MB Pages, fully associative, 2 entries"));
		break;
	case 0x03: Cache_Temp.emplace_back(_T("Data TLB: 4-KB Pages, 4-way set associative, 64 entries"));
		break;
	case 0x04: Cache_Temp.emplace_back(_T("Data TLB: 4-MB Pages, 4-way set associative, 8 entries"));
		break;
	case 0x05: Cache_Temp.emplace_back(_T("Data TLB: 4-MB Pages, 4-way set associative, 32 entries"));
		break;
	case 0x06: Cache_Temp.emplace_back(_T("1st-level instruction cache: 8-KB, 4-way set associative, 32-byte line size"));
		break;
	case 0x08: Cache_Temp.emplace_back(_T("1st-level instruction cache: 16-KB, 4-way set associative, 32-byte line size"));
		break;
	case 0x09: Cache_Temp.emplace_back(_T("1st-level instruction cache: 32-KBytes, 4-way set associative, 64 byte line size"));
		break;
	case 0x0A: Cache_Temp.emplace_back(_T("1st-level data cache: 8-KB, 2-way set associative, 32-byte line size"));
		break;
	case 0x0B: Cache_Temp.emplace_back(_T("Instruction TLB: 4-MByte pages, 4-way set associative, 4 entries"));
		break;
	case 0x0C: Cache_Temp.emplace_back(_T("1st-level data cache: 16-KB, 4-way set associative, 32-byte line size"));
		break;
	case 0x0D: Cache_Temp.emplace_back(_T("1st-level data cache: 16 KBytes, 4-way set associative, 64 byte line size"));
		break;
	case 0x0E: Cache_Temp.emplace_back(_T("1st-level data cache: 24 KBytes, 6-way set associative, 64 byte line size"));
		break;
	case 0x1D: Cache_Temp.emplace_back(_T("2nd-level cache: 128 KBytes, 2-way set associative, 64 byte line size"));
		break;
	case 0x21: Cache_Temp.emplace_back(_T("2nd-level cache: 256 KBytes, 8-way set associative, 64 byte line size"));
		break;
	case 0x22: Cache_Temp.emplace_back(_T("3rd-level cache: 512 KB, 4-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x23: Cache_Temp.emplace_back(_T("3rd-level cache: 1-MB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x24: Cache_Temp.emplace_back(_T("2nd-level cache: 1 MBytes, 16-way set associative, 64 byte line size"));
		break;
	case 0x25: Cache_Temp.emplace_back(_T("3rd-level cache: 2-MB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x29: Cache_Temp.emplace_back(_T("3rd-level cache: 4-MB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x2C: Cache_Temp.emplace_back(_T("1st-level data cache: 32-KB, 8-way set associative, 64-byte line size"));
		break;
	case 0x30: Cache_Temp.emplace_back(_T("1st-level instruction cache: 32-KB, 8-way set associative, 64-byte line size"));
		break;
	case 0x39: Cache_Temp.emplace_back(_T("2nd-level cache: 128-KB, 4-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x3A: Cache_Temp.emplace_back(_T("2nd-level cache: 192-KB, 6-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x3B: Cache_Temp.emplace_back(_T("2nd-level cache: 128-KB, 2-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x3C: Cache_Temp.emplace_back(_T("2nd-level cache: 256-KB, 4-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x3D: Cache_Temp.emplace_back(_T("2nd-level cache: 384-KB, 6-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x3E: Cache_Temp.emplace_back(_T("2nd-level cache: 512-KB, 4-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x40: Cache_Temp.emplace_back(_T("No 2nd-level cache or, if processor contains a valid 2nd-level cache, no 3rd-level cache"));
		break;
	case 0x41: Cache_Temp.emplace_back(_T("2nd-level cache: 128-KB, 4-way set associative, 32-byte line size"));
		break;
	case 0x42: Cache_Temp.emplace_back(_T("2nd-level cache: 256-KB, 4-way set associative, 32-byte line size"));
		break;
	case 0x43: Cache_Temp.emplace_back(_T("2nd-level cache: 512-KB, 4-way set associative, 32-byte line size"));
		break;
	case 0x44: Cache_Temp.emplace_back(_T("2nd-level cache: 1-MB, 4-way set associative, 32-byte line size"));
		break;
	case 0x45: Cache_Temp.emplace_back(_T("2nd-level cache: 2-MB, 4-way set associative, 32-byte line size"));
		break;
	case 0x46: Cache_Temp.emplace_back(_T("3rd-level cache: 4MB, 4-way set associative, 64-byte line size"));
		break;
	case 0x47: Cache_Temp.emplace_back(_T("3rd-level cache: 8MB, 8-way set associative, 64-byte line size"));
		break;
	case 0x48: Cache_Temp.emplace_back(_T("2nd-level cache: 3MByte, 12-way set associative, 64 byte line size"));
		break;
	case 0x49: Cache_Temp.emplace_back(_T("3rd-level cache: 4MB, 16-way set associative, 64-byte line size (Intel Xeon processor MP,Family 0Fh, Model 06h \n 2nd-level cache: 4MB, 16-way set associative, 64-byte line size"));
		break;
	case 0x4A: Cache_Temp.emplace_back(_T("3rd-level cache: 6MB, 12-way set associative, 64-byte line size"));
		break;
	case 0x4B: Cache_Temp.emplace_back(_T("3rd-level cache: 8MB, 16-way set associative, 64-byte line size"));
		break;
	case 0x4C: Cache_Temp.emplace_back(_T("3rd-level cache: 12MB, 12-way set associative, 64-byte line size"));
		break;
	case 0x4D: Cache_Temp.emplace_back(_T("3rd-level cache: 16MB, 16-way set associative, 64-byte line size"));
		break;
	case 0x4E: Cache_Temp.emplace_back(_T("2nd-level cache: 6MB, 24-way set associative, 64-byte line size"));
		break;
	case 0x4F: Cache_Temp.emplace_back(_T("Instruction TLB: 4 KByte pages, 32 entries"));
		break;
	case 0x50: Cache_Temp.emplace_back(_T("Instruction TLB: 4-KB, 2-MB or 4-MB pages, fully associative, 64 entries"));
		break;
	case 0x51: Cache_Temp.emplace_back(_T("Instruction TLB: 4-KB, 2-MB or 4-MB pages, fully associative, 128 entries"));
		break;
	case 0x52: Cache_Temp.emplace_back(_T("Instruction TLB: 4-KB, 2-MB or 4-MB pages, fully associative, 256 entries"));
		break;
	case 0x55: Cache_Temp.emplace_back(_T("Instruction TLB: 2-MByte or 4-MByte pages, fully associative, 7 entries"));
		break;
	case 0x56: Cache_Temp.emplace_back(_T("Data TLB0: 4 MByte pages, 4-way set associative, 16 entries"));
		break;
	case 0x57: Cache_Temp.emplace_back(_T("Data TLB0: 4 KByte pages, 4-way associative, 16 entries"));
		break;
	case 0x59: Cache_Temp.emplace_back(_T("Data TLB0: 4 KByte pages, fully associative, 16 entries"));
		break;
	case 0x5A: Cache_Temp.emplace_back(_T("Data TLB0: 2 MByte or 4 MByte pages, 4-way set associative, 32 entries"));
		break;
	case 0x5B: Cache_Temp.emplace_back(_T("Data TLB: 4-KB or 4-MB pages, fully associative, 64 entries"));
		break;
	case 0x5C: Cache_Temp.emplace_back(_T("Data TLB: 4-KB or 4-MB pages, fully associative, 128 entries"));
		break;
	case 0x5D: Cache_Temp.emplace_back(_T("Data TLB: 4-KB or 4-MB pages, fully associative, 256 entries"));
		break;
	case 0x60: Cache_Temp.emplace_back(_T("1st-level data cache: 16-KB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x61: Cache_Temp.emplace_back(_T("Instruction TLB: 4 KByte pages, fully associative, 48 entries"));
		break;
	case 0x63: Cache_Temp.emplace_back(_T("Data TLB: 2 MByte or 4 MByte pages, 4-way set associative, 32 entries and a separate array with 1 GByte pages, 4-way set associative, 4 entries"));
		break;
	case 0x64: Cache_Temp.emplace_back(_T("Data TLB: 4 KByte pages, 4-way set associative, 512 entries"));
		break;
	case 0x66: Cache_Temp.emplace_back(_T("1st-level data cache: 8-KB, 4-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x67: Cache_Temp.emplace_back(_T("1st-level data cache: 16-KB, 4-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x68: Cache_Temp.emplace_back(_T("1st-level data cache: 32-KB, 4 way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x6A: Cache_Temp.emplace_back(_T("uTLB: 4 KByte pages, 8-way set associative, 64 entries"));
		break;
	case 0x6B: Cache_Temp.emplace_back(_T("DTLB: 4 KByte pages, 8-way set associative, 256 entries"));
		break;
	case 0x6C: Cache_Temp.emplace_back(_T("DTLB: 2M/4M pages, 8-way set associative, 128 entries"));
		break;
	case 0x6D: Cache_Temp.emplace_back(_T("DTLB: 1 GByte pages, fully associative, 16 entries"));
		break;
	case 0x70: Cache_Temp.emplace_back(_T("Trace cache: 12K-uops, 8-way set associative"));
		break;
	case 0x71: Cache_Temp.emplace_back(_T("Trace cache: 16K-uops, 8-way set associative"));
		break;
	case 0x72: Cache_Temp.emplace_back(_T("Trace cache: 32K-uops, 8-way set associative"));
		break;
	case 0x76: Cache_Temp.emplace_back(_T("Instruction TLB: 2M/4M pages, fully associative, 8 entries"));
		break;
	case 0x78: Cache_Temp.emplace_back(_T("2nd-level cache: 1-MB, 4-way set associative, 64-byte line size"));
		break;
	case 0x79: Cache_Temp.emplace_back(_T("2nd-level cache: 128-KB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x7A: Cache_Temp.emplace_back(_T("2nd-level cache: 256-KB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x7B: Cache_Temp.emplace_back(_T("2nd-level cache: 512-KB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x7C: Cache_Temp.emplace_back(_T("2nd-level cache: 1-MB, 8-way set associative, sectored cache, 64-byte line size"));
		break;
	case 0x7D: Cache_Temp.emplace_back(_T("2nd-level cache: 2-MB, 8-way set associative, 64-byte line size"));
		break;
	case 0x7F: Cache_Temp.emplace_back(_T("2nd-level cache: 512-KB, 2-way set associative, 64-byte line size"));
		break;
	case 0x80: Cache_Temp.emplace_back(_T("2nd-level cache: 512 KByte, 8-way set associative, 64-byte line size"));
		break;
	case 0x82: Cache_Temp.emplace_back(_T("2nd-level cache: 256-KB, 8-way set associative, 32-byte line size"));
		break;
	case 0x83: Cache_Temp.emplace_back(_T("2nd-level cache: 512-KB, 8-way set associative, 32-byte line size"));
		break;
	case 0x84: Cache_Temp.emplace_back(_T("2nd-level cache: 1-MB, 8-way set associative, 32-byte line size"));
		break;
	case 0x85: Cache_Temp.emplace_back(_T("2nd-level cache: 2-MB, 8-way set associative, 32-byte line size"));
		break;
	case 0x86: Cache_Temp.emplace_back(_T("2nd-level cache: 512-KB, 4-way set associative, 64-byte line size"));
		break;
	case 0x87: Cache_Temp.emplace_back(_T("2nd-level cache: 1-MB, 8-way set associative, 64-byte line size"));
		break;
	case 0xA0: Cache_Temp.emplace_back(_T("DTLB: 4k pages, fully associative, 32 entries"));
		break;
	case 0xB0: Cache_Temp.emplace_back(_T("Instruction TLB: 4-KB Pages, 4-way set associative, 128 entries"));
		break;
	case 0xB1: Cache_Temp.emplace_back(_T("Instruction TLB: 2M pages, 4-way, 8 entries or 4M pages, 4-way, 4 entries"));
		break;
	case 0xB2: Cache_Temp.emplace_back(_T("Instruction TLB: 4KByte pages, 4-way set associative, 64 entries"));
		break;
	case 0xB3: Cache_Temp.emplace_back(_T("Data TLB: 4-KB Pages, 4-way set associative, 128 entries"));
		break;
	case 0xB4: Cache_Temp.emplace_back(_T("Data TLB: 4-KB Pages, 4-way set associative, 256 entries"));
		break;
	case 0xB5: Cache_Temp.emplace_back(_T("Instruction TLB: 4KByte pages, 8-way set associative, 64 entries"));
		break;
	case 0xB6: Cache_Temp.emplace_back(_T("Instruction TLB: 4KByte pages, 8-way set associative, 128 entries"));
		break;
	case 0xBA: Cache_Temp.emplace_back(_T("Data TLB1: 4 KByte pages, 4-way associative, 64 entries"));
		break;
	case 0xC0: Cache_Temp.emplace_back(_T("Data TLB: 4 KByte and 4 MByte pages, 4-way associative, 8 entries"));
		break;
	case 0xC1: Cache_Temp.emplace_back(_T("Shared 2nd-Level TLB: 4 KByte/2MByte pages, 8-way associative, 1024 entries"));
		break;
	case 0xC2: Cache_Temp.emplace_back(_T("DTLB: 4 KByte/2 MByte pages, 4-way associative, 16 entries"));
		break;
	case 0xC3: Cache_Temp.emplace_back(_T("Shared 2nd-Level TLB: 4 KByte /2 MByte pages, 6-way associative, 1536 entries. Also 1GBbyte pages, 4-way, 16 entries."));
		break;
	case 0xC4: Cache_Temp.emplace_back(_T("DTLB: 2M/4M Byte pages, 4-way associative, 32 entries"));
		break;
	case 0xCA: Cache_Temp.emplace_back(_T("Shared 2nd-Level TLB: 4 KByte pages, 4-way associative, 512 entries"));
		break;
	case 0xD0: Cache_Temp.emplace_back(_T("3rd-level cache: 512 KByte, 4-way set associative, 64 byte line size"));
		break;
	case 0xD1: Cache_Temp.emplace_back(_T("3rd-level cache: 1 MByte, 4-way set associative, 64 byte line size"));
		break;
	case 0xD2: Cache_Temp.emplace_back(_T("3rd-level cache: 2 MByte, 4-way set associative, 64 byte line size"));
		break;
	case 0xD6: Cache_Temp.emplace_back(_T("3rd-level cache: 1 MByte, 8-way set associative, 64 byte line size"));
		break;
	case 0xD7: Cache_Temp.emplace_back(_T("3rd-level cache: 2 MByte, 8-way set associative, 64 byte line size"));
		break;
	case 0xD8: Cache_Temp.emplace_back(_T("3rd-level cache: 4 MByte, 8-way set associative, 64 byte line size"));
		break;
	case 0xDC: Cache_Temp.emplace_back(_T("3rd-level cache: 1.5 MByte, 12-way set associative, 64 byte line size"));
		break;
	case 0xDD: Cache_Temp.emplace_back(_T("3rd-level cache: 3 MByte, 12-way set associative, 64 byte line size"));
		break;
	case 0xDE: Cache_Temp.emplace_back(_T("3rd-level cache: 6 MByte, 12-way set associative, 64 byte line size"));
		break;
	case 0xE2: Cache_Temp.emplace_back(_T("3rd-level cache: 2 MByte, 16-way set associative, 64 byte line size"));
		break;
	case 0xE3: Cache_Temp.emplace_back(_T("3rd-level cache: 4 MByte, 16-way set associative, 64 byte line size"));
		break;
	case 0xE4: Cache_Temp.emplace_back(_T("3rd-level cache: 8 MByte, 16-way set associative, 64 byte line size"));
		break;
	case 0xEA: Cache_Temp.emplace_back(_T("3rd-level cache: 12MByte, 24-way set associative, 64 byte line size"));
		break;
	case 0xEB: Cache_Temp.emplace_back(_T("3rd-level cache: 18MByte, 24-way set associative, 64 byte line size"));
		break;
	case 0xEC: Cache_Temp.emplace_back(_T("3rd-level cache: 24MByte, 24-way set associative, 64 byte line size"));
		break;
	case 0xF0: Cache_Temp.emplace_back(_T("64-byte Prefetching"));
		break;
	case 0xF1: Cache_Temp.emplace_back(_T("128-byte Prefetching"));
		break;
	default:
		break;
	}
}

bool Get_Info_For_Intel::GET_Power_Mgmt_INFO(IN int EAX, IN int EBX, IN int ECX)
{
	(EAX & DIGITAL_TEM_SENSOR_FLAGS) ? Intel_Data.Power_Mgmt.Digital_tem_sensor = TRUE : Intel_Data.Power_Mgmt.Digital_tem_sensor = FALSE;
	(EAX & TURBO_BOOST_FLAGS) ? Intel_Data.Power_Mgmt.Turbo_boost = TRUE : Intel_Data.Power_Mgmt.Turbo_boost = FALSE;
	(EAX & ARAT_FLAGS) ? Intel_Data.Power_Mgmt.ARAT = TRUE : Intel_Data.Power_Mgmt.ARAT = FALSE;
	(EAX & PLN_FLAGS) ? Intel_Data.Power_Mgmt.PLN = TRUE : Intel_Data.Power_Mgmt.PLN = FALSE;
	(EAX & ECMD_FLAGS) ? Intel_Data.Power_Mgmt.ECMD = TRUE : Intel_Data.Power_Mgmt.ECMD = FALSE;
	(EAX & PTM_FLAGS) ? Intel_Data.Power_Mgmt.PTM = TRUE : Intel_Data.Power_Mgmt.PTM = FALSE;
	(EAX & HWP_FLAGS) ? Intel_Data.Power_Mgmt.HWP = TRUE : Intel_Data.Power_Mgmt.HWP = FALSE;
	(EAX & HWP_NOTIFICATION_FLAGS) ? Intel_Data.Power_Mgmt.HWP_notification = TRUE : Intel_Data.Power_Mgmt.HWP_notification = FALSE;
	(EAX & HWP_ACTIVITY_WINDOW_FLAGS) ? Intel_Data.Power_Mgmt.HWP_Activity_Window = TRUE : Intel_Data.Power_Mgmt.HWP_Activity_Window = FALSE;
	(EAX & HWP_ENERGY_PERFORMANCE_FLAGS) ? Intel_Data.Power_Mgmt.HWP_Energy_Performance = TRUE : Intel_Data.Power_Mgmt.HWP_Energy_Performance = FALSE;
	(EAX & HWP_PACKAGE_LEVEL_REQUEST_FLAGS) ? Intel_Data.Power_Mgmt.HWP_Package_Level_Request = TRUE : Intel_Data.Power_Mgmt.HWP_Package_Level_Request = FALSE;
	(EAX & HDC_FLAGS) ? Intel_Data.Power_Mgmt.HDC = TRUE : Intel_Data.Power_Mgmt.HDC = FALSE;
	(EAX & TURBO_BOOST_MAX_3_FLAGS) ? Intel_Data.Power_Mgmt.Turbo_boost_3 = TRUE : Intel_Data.Power_Mgmt.Turbo_boost_3 = FALSE;
	Intel_Data.Power_Mgmt.Digital_thermal_sensor_num = (UINT)(EBX & DIGITAL_THERMAL_SENSOR_NUM);
	(ECX & HW_COORDINATION_FEEDBACK) ? Intel_Data.Power_Mgmt.Hardware_Coordination_Feedback = TRUE : Intel_Data.Power_Mgmt.Hardware_Coordination_Feedback = FALSE;
	(ECX & PERFORMANCE_ENERGY_BIAS) ? Intel_Data.Power_Mgmt.performance_energy_bias_preference = TRUE : Intel_Data.Power_Mgmt.performance_energy_bias_preference = FALSE;
	return true;
}

bool Get_Info_For_Intel::GET_EXTENED_FEATURE_INFO(IN int EBX, IN int ECX)
{
	(EBX & FSGSBASE_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.FSGSBASE = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.FSGSBASE = FALSE;
	(EBX & IA32_TSC_ADJUST_MSR_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.IA32_TSC_ADJUST_MSR = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.IA32_TSC_ADJUST_MSR = FALSE;
	(EBX & SGX_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.SGX = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.SGX = FALSE;
	(EBX & BMI1_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.BMI1 = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.BMI1 = FALSE;
	(EBX & HLE_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.HLE = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.HLE = FALSE;
	(EBX & AVX2_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.AVX2 = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.AVX2 = FALSE;
	(EBX & FDP_EXCPTN_ONLY_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.FDP_EXCPTN_ONLY = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.FDP_EXCPTN_ONLY = FALSE;
	(EBX & SMEP_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.SMEP = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.SMEP = FALSE;
	(EBX & BMI2_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.BMI2 = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.BMI2 = FALSE;
	(EBX & Enhanced_REP_MOVSB_OR_STOSB_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.Enhanced_REP_MOVSB_OR_STOSB = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.Enhanced_REP_MOVSB_OR_STOSB = FALSE;
	(EBX & INVPCID_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.INVPCID = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.INVPCID = FALSE;
	(EBX & RTM_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.RTM = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.RTM = FALSE;
	(EBX & RDT_M_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.RDT_M = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.RDT_M = FALSE;
	(EBX & FPU_CS_DS_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.FPU_CS_DS = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.FPU_CS_DS = FALSE;
	(EBX & MPX_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.MPX = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.MPX = FALSE;
	(EBX & RDSEED_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.RDSEED = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.RDSEED = FALSE;
	(EBX & ADX_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.ADX = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.ADX = FALSE;
	(EBX & SMAP_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.SMAP = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.SMAP = FALSE;
	(EBX & CLFLUSHOPT_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.CLFLUSHOPT = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.CLFLUSHOPT = FALSE;
	(EBX & CLWB_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.CLWB = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.CLWB = FALSE;
	(EBX & Intel_Processor_Trace_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.Intel_Processor_Trace = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.Intel_Processor_Trace = FALSE;
	(EBX & SHA_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.SHA = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.SHA = FALSE;
	(ECX & PREFETCHWT1_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.PREFETCHWT1 = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.PREFETCHWT1 = FALSE;
	(ECX & UMIP_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.UMIP = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.UMIP = FALSE;
	(ECX & PKU_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.PKU = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.PKU = FALSE;
	(ECX & OSPKE_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.OSPKE = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.OSPKE = FALSE;
	Intel_Data.Extened_Feature.Intel_Extened_Fearture.MAWAU_Value = (ECX & MAWAU_VALUE_BY_BNDLDX) >> 17;
	(ECX & RDPID_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.RDPID = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.RDPID = FALSE;
	(ECX & SGX_LC_FLAGS) ? Intel_Data.Extened_Feature.Intel_Extened_Fearture.SGX_LC = TRUE : Intel_Data.Extened_Feature.Intel_Extened_Fearture.SGX_LC = FALSE;
	return true;
}

bool Get_Info_For_Intel::GET_ARCHITECTURAL_PERFORMANCE_MONITORING(IN int EAX, IN int EBX, IN int EDX)
{
	Intel_Data.Architectural_performance_monitoring.Version_ID = (EAX & VERSION_ID_OF_P_MONITOR);
	Intel_Data.Architectural_performance_monitoring.Number_of_General_purpose = (EAX & NUMBER_GENERAL_PURPOSE) >> 8;
	Intel_Data.Architectural_performance_monitoring.BitWidth_of_General_purpose = (EAX & BITWIDTH_GENERAL_PURPOSE) >> 16;
	Intel_Data.Architectural_performance_monitoring.Length_of_EBX = (EAX & LENGTH_OF_EBX_BIT) >> 24;

	(EBX & CORE_CYCLE_EVENT) ? Intel_Data.Architectural_performance_monitoring.Core_Cycles = FALSE : Intel_Data.Architectural_performance_monitoring.Core_Cycles = TRUE;
	(EBX & INSTRUCTION_RETIRED_EVENT) ? Intel_Data.Architectural_performance_monitoring.Instruction_retired_event = FALSE : Intel_Data.Architectural_performance_monitoring.Instruction_retired_event = TRUE;
	(EBX & REFERENCE_CYCLES_EVENT) ? Intel_Data.Architectural_performance_monitoring.Reference_cycles_event = FALSE : Intel_Data.Architectural_performance_monitoring.Reference_cycles_event = TRUE;
	(EBX & LAST_LEVEL_CACHE_REFERENCE_EVENT) ? Intel_Data.Architectural_performance_monitoring.Last_level_cache_reference_event = FALSE : Intel_Data.Architectural_performance_monitoring.Last_level_cache_reference_event = TRUE;
	(EBX & LAST_LEVEL_CACHE_MISSES_EVENT) ? Intel_Data.Architectural_performance_monitoring.Last_level_cache_misses_event = FALSE : Intel_Data.Architectural_performance_monitoring.Last_level_cache_misses_event = TRUE;
	(EBX & BRANCH_INSTRUCTION_RETIRED_EVENT) ? Intel_Data.Architectural_performance_monitoring.Branch_instruction_retired_event = FALSE : Intel_Data.Architectural_performance_monitoring.Branch_instruction_retired_event = TRUE;
	(EBX & BRANCH_MISPREDICT_RETIRED_EVENT) ? Intel_Data.Architectural_performance_monitoring.Branch_mispredict_retired_event = FALSE : Intel_Data.Architectural_performance_monitoring.Branch_mispredict_retired_event = TRUE;
	
	Intel_Data.Architectural_performance_monitoring.Number_of_fixed_function = (EDX & NUMBER_OF_FIXED_FUNCTION) >> 0;
	Intel_Data.Architectural_performance_monitoring.Bitwidth_of_fixed_function = (EDX & BITWIDTH_OF_FIXED_FUNCTION) >> 5;
	Intel_Data.Architectural_performance_monitoring.Any_thread_deprecation = (EDX & ANY_THREAD_DEPRECATION) >> 15;
	return true;
}

bool Get_Info_For_Intel::GET_CODENAME(const string & CPUName)
{
	if (CPUName.empty())
		return false;
	Microarchitecture IntelMicroarchitecture;
	std::stringstream split(CPUName);
	vector<string> res;
	string result;
	while (split >> result)
	{
		res.emplace_back(result);
	}

	if (res[1].find("Core") != -1)
		IntelMicroarchitecture = Core;
	else if (res[1].find("Xeon") != -1)
		IntelMicroarchitecture = Xeon;
	else
		IntelMicroarchitecture = Unknown;
	switch (IntelMicroarchitecture)
	{
	case Get_Info_For_Intel::Unknown:
		Intel_Data.Code_Name = "Unknown";
		break;
	case Get_Info_For_Intel::Core:
		Intel_Data.Code_Name = Core_CodeName(res[2]);
		break;
	case Get_Info_For_Intel::Xeon:
		break;
	case Get_Info_For_Intel::Itanium:
		break;
	}
	return true;
}

string Get_Info_For_Intel::Core_CodeName(const string& model)
{
	return "";
}
// {
// 	string res = "";
// 	if (model.find("i3-") != -1)
// 	{
// 		int x = 0;
// 		sscanf_s(model.c_str(), "i3-%d", &x);
// 		switch (x)
// 		{
// 		default:
// 			break;
// 		}
// 	}
// 	else if (model.find("i5-") != -1)
// 	{
// 		int x = 0;
// 		sscanf_s(model.c_str(), "i5-%d", &x);
// 		switch (x)
// 		{
// 		default:
// 			break;
// 		}
// 	}
// 	else if (model.find("i7-") != -1)
// 	{
// 		int x = 0;
// 		char s = 0;
// 		sscanf_s(model.c_str(), "i7-%d", &x);
// 		switch (x)
// 		{
// 		case 2600:
// 		case 3820:
// 		case 3930:
// 		case 3960:
// 		case 3970:
// 			res = "Sandy Bridge-E";
// 			break;
// 		case 3770:
// 		case 3940:
// 			res = "Ivy Bridge";
// 			break;
// 		case 4820:
// 		case 4930:
// 			res = "Ivy Bridge-E";
// 		case 5820:
// 		case 5930:
// 		case 5960:
// 			res = "Haswell - E";
// 			break;
// 		case 5775:
// 			res = "Broadwell-H";
// 			break;
// 		case 6800:
// 		case 6850:
// 		case 6900:
// 		case 6950:
// 			res = "Broadwell-E";
// 			break;
// 		case 6700:
// 		case 6970:
// 		case 6870:
// 		case 6822:
// 		case 6820:
// 		case 6785:
// 		case 6770:
// 		case 6660:
// 		case 6650:
// 		case 6567:
// 		case 6560:
// 		case 6500:
// 			res = "Skylake";
// 			break;
// 		case 8850:
// 		case 8750:
// 			res = "Coffee Lake-H";
// 			break;
// 		case 8709:
// 		case 8809:
// 		case 8705:
// 		case 8706:
// 			res = "Kaby Lake G";
// 			break;
// 		case 8700:
// 			res = "Coffee Lake";
// 			break;
// 		case 8550:
// 		case 8650:
// 			res = "Kaby Lake-R";
// 			break;
// 		case 7740:
// 			res = "Kaby Lake-X";
// 			break;
// 		case 7800:
// 		case 7820:
// 			res = "Skylake-X";
// 			break;
// 		case 7700:
// 			res = "Kaby Lake";
// 			break;
// 		case 5557:
// 		case 5500:
// 		case 5550:
// 		case 5600:
// 		case 5650:
// 			res = "Broadwell-U";
// 			break;
// 		case 4870:
// 		case 4860:
// 		case 4760:
// 		case 4980:
// 		case 4950:
// 		case 4850:
// 		case 4750:
// 			res = "Crystalwell";
// 			break;
// 		case 4790:
// 		case 4610:
// 		case 4940:
// 		case 4910:
// 		case 4810:
// 		case 4771:
// 		case 4765:
// 		case 4785:
// 		case 4900:
// 		case 4800:
// 		case 4700:
// 		case 4702:
// 		case 4710:
// 		case 4712:
// 		case 4720:
// 		case 4722:
// 			res = "Haswell";
// 			break;
// 		case 4770:
// 			if (model.find("HQ") != -1)
// 				res = "Crystalwell";
// 			else
// 				res = "Haswell";
// 			break;
// 		case 4960:
// 			if (model.find("HQ") != -1)
// 				res = "Haswell";
// 			else if (model.find("X") != -1)
// 				res = "Ivy Bridge-E";
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// 	else if (model.find("i9-") != -1)
// 	{
// 		int x = 0;
// 		sscanf_s(model.c_str(), "i9-%d", &x);
// 		switch (x)
// 		{
// 		case 7980:
// 		case 7960:
// 		case 7940:
// 		case 7920:
// 		case 7900:
// 		case 7800:
// 		case 7820:
// 			res = "Skylake-X";
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// 	return res;
// }

void Get_Info_For_Intel::Information_from_WMI()
{
	//从WMI中获取信息
	CSynQueryData recvnotify(L"root\\CIMV2", L"SELECT Name,CurrentClockSpeed ,MaxClockSpeed,ExtClock,MaxClockSpeed,NumberOfCores,ProcessorId,Revision,SocketDesignation,UpgradeMethod,NumberOfLogicalProcessors FROM Win32_Processor");
	recvnotify.ExcuteFun();
	vector<string> out_put_data = recvnotify.ClassData();
	vector<string> out_put_name = recvnotify.ClassName();
	mposition = this->find_location(out_put_name, _T("Name"));
	if (mposition != -1)	Intel_Data.Name = out_put_data[mposition];
	else Intel_Data.Name = _T("Unknown");
	mposition = this->find_location(out_put_name, _T("CurrentClockSpeed"));
	if (mposition != -1)	Intel_Data.CurrentClockSpeed = stoi(out_put_data[mposition]);
	else Intel_Data.CurrentClockSpeed = 0;
	mposition = this->find_location(out_put_name, _T("ExtClock"));
	if (mposition != -1)	Intel_Data.ExtClock = stoi(out_put_data[mposition]);
	else Intel_Data.ExtClock = 0;
	mposition = this->find_location(out_put_name, _T("NumberOfCores"));
	if (mposition != -1)	Intel_Data.Core = stoi(out_put_data[mposition]);
	else Intel_Data.Core = 0;
	mposition = this->find_location(out_put_name, _T("ProcessorId"));
	if (mposition != -1)	Intel_Data.ProcessorID = out_put_data[mposition];
	else Intel_Data.ProcessorID = _T("Unknown");
	mposition = this->find_location(out_put_name, _T("Revision"));
	if (mposition != -1)	Intel_Data.Revision = stoi(out_put_data[mposition]);
	else Intel_Data.Revision = 0;
	mposition = this->find_location(out_put_name, _T("SocketDesignation"));
	if (mposition != -1)	Intel_Data.SocketDesignation = out_put_data[mposition];
	else Intel_Data.SocketDesignation = _T("Unknown");
	mposition = this->find_location(out_put_name, _T("UpgradeMethod"));
	if (mposition != -1)	Intel_Data.UpgradeMethod = stoi(out_put_data[mposition]);
	else Intel_Data.UpgradeMethod = 0;
	mposition = this->find_location(out_put_name, _T("MaxClockSpeed"));
	if (mposition != -1)	Intel_Data.MaxClockSpeed = stoi(out_put_data[mposition]);
	else Intel_Data.MaxClockSpeed = 0;
	mposition = this->find_location(out_put_name, _T("NumberOfLogicalProcessors"));
	if (mposition != -1)	Intel_Data.Thread = stoi(out_put_data[mposition]);
	else Intel_Data.Thread = 0;
}

void Get_Info_For_Intel::Information_from_CPUID0()
{
	/************************************************************************/
	/*                          本段解析CPUID= 0时的含义                     */
	/************************************************************************/
	mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 0 Sub Leaf 0"));
	*reinterpret_cast<int*>(Intel_Data.Manufacturer) = Intel_cpuRegister.CPU_EBX[mposition];
	*reinterpret_cast<int*>(Intel_Data.Manufacturer + 4) = Intel_cpuRegister.CPU_EDX[mposition];
	*reinterpret_cast<int*>(Intel_Data.Manufacturer + 8) = Intel_cpuRegister.CPU_ECX[mposition];
}

void Get_Info_For_Intel::Information_from_CPUID1()
{
	/************************************************************************/
	/*                          本段解析CPUID= 1时的含义                     */
	/************************************************************************/
	int DisplayFamily, DisplayModel;
	int DF_DM;
	mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 1 Sub Leaf 0"));
	if (mposition != -1)
	{
		Intel_Data.Family = (Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Family) >> 8;
		Intel_Data.Model = (Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Model) >> 4;
		Intel_Data.Stepping = Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Stepping;
		switch ((Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Processor_Type) >> 12)
		{
		case 0x00:	Intel_Data.CPU_Type = _T("Original OEM Processor");
			break;
		case 0x01:	Intel_Data.CPU_Type = _T("Intel OverDrive(R) Processor");
			break;
		case 0x02:	Intel_Data.CPU_Type = _T("Dual processor (not applicable to Intel486 processors)");
			break;
		case 0x03:	Intel_Data.CPU_Type = _T("Intel reserved");
			break;
		default:
			Intel_Data.CPU_Type = _T("Unknown");
			break;
		}
		Intel_Data.Ext_Family = ((Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Extended_Family) >> 20) + ((Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Family) >> 8);
		Intel_Data.Ext_Model = (((Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Extended_Model) >> 16) << 4) + ((Intel_cpuRegister.CPU_EAX[mposition] & CPUID_Model) >> 4);
		this->GET_FEATURE_INFO(Intel_cpuRegister.CPU_ECX[mposition], Intel_cpuRegister.CPU_EDX[mposition]);

		//进行DisplayFamily_DisplayModel数据处理
		if (Intel_Data.Family != 0x0F)		DisplayFamily = Intel_Data.Family;
		else	DisplayFamily = Intel_Data.Ext_Family;
		if (Intel_Data.Family == 0x06 || Intel_Data.Family == 0x0F)	DisplayModel = Intel_Data.Ext_Model;
		else	DisplayModel = Intel_Data.Model;
		DF_DM = (DisplayFamily << 8) + DisplayModel;
		this->DF_DM_info(DF_DM);
	}

}

void Get_Info_For_Intel::Information_from_CPUID2()
{
	/************************************************************************/
	/*                           本段解析CPUID= 2时的含义                    */
	/************************************************************************/

	mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 2 Sub Leaf 0"));
	if (mposition != -1)
	{
		if ((Intel_cpuRegister.CPU_EAX[mposition] & CACHE_AVAILABLE) == 0)
		{
			this->cache_Descriptors((Intel_cpuRegister.CPU_EAX[mposition] >> 24) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EAX[mposition] >> 16) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EAX[mposition] >> 8)  & CACHE_SHIELD, Intel_Data.Cache_TLB);
		}
		if ((Intel_cpuRegister.CPU_EBX[mposition] & CACHE_AVAILABLE) == 0)
		{
			this->cache_Descriptors((Intel_cpuRegister.CPU_EBX[mposition] >> 24) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EBX[mposition] >> 16) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EBX[mposition] >> 8)  & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EBX[mposition] >> 0)  & CACHE_SHIELD, Intel_Data.Cache_TLB);
		}
		if ((Intel_cpuRegister.CPU_ECX[mposition] & CACHE_AVAILABLE) == 0)
		{
			this->cache_Descriptors((Intel_cpuRegister.CPU_ECX[mposition] >> 24) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_ECX[mposition] >> 16) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_ECX[mposition] >> 8)  & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_ECX[mposition] >> 0)  & CACHE_SHIELD, Intel_Data.Cache_TLB);
		}
		if ((Intel_cpuRegister.CPU_EDX[mposition] & CACHE_AVAILABLE) == 0)
		{
			this->cache_Descriptors((Intel_cpuRegister.CPU_EDX[mposition] >> 24) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EDX[mposition] >> 16) & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EDX[mposition] >> 8)  & CACHE_SHIELD, Intel_Data.Cache_TLB);
			this->cache_Descriptors((Intel_cpuRegister.CPU_EDX[mposition] >> 0)  & CACHE_SHIELD, Intel_Data.Cache_TLB);
		}
	}
}

void Get_Info_For_Intel::Information_from_CPUID3()
{
// 	mposition = -1;
// 	try
// 	{
// 		int e = -1;
// 		mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 3 Sub Leaf 0"));
// 		throw e;
// 	}
// 	catch(int)
// 	{
// 		return;
// 	}
}

void Get_Info_For_Intel::Information_from_CPUID4()
{
	/************************************************************************/
	/*                          本段解析CPUID= 4时的含义                     */
	/************************************************************************/

	for (UINT j = 0; j < 10; ++j)
	{
		Cache_info CacheTemp;
		mposition = this->find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 4 Sub Leaf ") + to_wstring(j));
		if (mposition != -1)
		{
			CacheTemp.Cache_level = ((Intel_cpuRegister.CPU_EAX[mposition] & CACHE_LEVEL) >> 5);
			CacheTemp.Cache_Ways = (((Intel_cpuRegister.CPU_EBX[mposition] & CACHE_WAYS) >> 22) + 1);
			CacheTemp.Cache_partitions = (((Intel_cpuRegister.CPU_EBX[mposition] & CACHE_PARTITIONS) >> 12) + 1);
			CacheTemp.Cache_line_size = ((Intel_cpuRegister.CPU_EBX[mposition] & CACHE_LINE_SIZE) + 1);
			CacheTemp.Cache_Set = (Intel_cpuRegister.CPU_ECX[mposition] + 1);
			CacheTemp.Cache_Size = ((((Intel_cpuRegister.CPU_EBX[mposition] & CACHE_WAYS) >> 22) + 1) * (((Intel_cpuRegister.CPU_EBX[mposition] & CACHE_PARTITIONS) >> 12) + 1) * ((Intel_cpuRegister.CPU_EBX[mposition] & CACHE_LINE_SIZE) + 1) * (Intel_cpuRegister.CPU_ECX[mposition] + 1));
			CacheTemp.Cache_max_addr_logical = (((Intel_cpuRegister.CPU_EAX[mposition] & CACHE_MAX_ADDR_LOGICAL) >> 14) + 1);
			CacheTemp.Cache_max_addr_physical = (((Intel_cpuRegister.CPU_EAX[mposition] & CACHE_MAX_ADDR_PHYSICAL) >> 26) + 1);
			((Intel_cpuRegister.CPU_EDX[mposition] & CACHE_INVALIDATE) >> 0) ? CacheTemp.CACHE_Invalidate = FALSE : CacheTemp.CACHE_Invalidate = TRUE;
			((Intel_cpuRegister.CPU_EDX[mposition] & CACHE_INCLUSIVENESS) >> 1) ? CacheTemp.Cache_Inclusiveness = TRUE : CacheTemp.Cache_Inclusiveness = FALSE;
			((Intel_cpuRegister.CPU_EDX[mposition] & CACHE_COMPLEX_INDEXING) >> 2) ? CacheTemp.Cache_Complex_Indexing = TRUE : CacheTemp.Cache_Complex_Indexing = FALSE;
			switch (Intel_cpuRegister.CPU_EAX[mposition] & CACHE_TYPE)
			{
			case 0x00:
				break;
			case 0x01: CacheTemp.Cache_type = _T("DATA CACHE");
				break;
			case 0x02: CacheTemp.Cache_type = _T("Instruction Cache");
				break;
			case 0x03: CacheTemp.Cache_type = _T("Unified Cache");
				break;
			default:
				CacheTemp.Cache_type = _T("");
				break;
			}
			Intel_Data.Intel_cache.emplace_back(CacheTemp);
		}
		else
		{
			break;
		}
	}

}

void Get_Info_For_Intel::Information_from_CPUID5()
{
	/************************************************************************/
	/*                     本段解析CPUID= 5时的含义                          */
	/************************************************************************/

	mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 5 Sub Leaf 0"));
	if (mposition != -1)
	{
		Intel_Data.Smallest_monitor_line_size = Intel_cpuRegister.CPU_EAX[mposition] & SMALLEST_MONITOR_LINE_SIZE;
		Intel_Data.Largest_monitor_line_size = Intel_cpuRegister.CPU_EBX[mposition] & LARGEST_MONITOR_LINE_SIZE;
		(Intel_cpuRegister.CPU_ECX[mposition] & MONITOR_MWAIT_EXTENSION) ? Intel_Data.EMX = TRUE : Intel_Data.EMX = FALSE;
		(Intel_cpuRegister.CPU_ECX[mposition] & MONITOR_MWAIT_INTERRUPTS) ? Intel_Data.IBE = TRUE : Intel_Data.IBE = FALSE;
		Intel_Data.C_STATE_info[0] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_0) >> 0));
		Intel_Data.C_STATE_info[1] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_1) >> 4));
		Intel_Data.C_STATE_info[2] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_2) >> 8));
		Intel_Data.C_STATE_info[3] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_3) >> 12));
		Intel_Data.C_STATE_info[4] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_4) >> 16));
		Intel_Data.C_STATE_info[5] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_5) >> 20));
		Intel_Data.C_STATE_info[6] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_6) >> 24));
		Intel_Data.C_STATE_info[7] = (((Intel_cpuRegister.CPU_EDX[mposition] & C_STATE_7) >> 28));
	}
}

void Get_Info_For_Intel::Information_from_CPUID6()
{
	/************************************************************************/
	/*                     本段解析CPUID= 6时的含义                          */
	/************************************************************************/

	mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 6 Sub Leaf 0"));
	if (mposition != -1)
	{
		this->GET_Power_Mgmt_INFO(Intel_cpuRegister.CPU_EAX[mposition], Intel_cpuRegister.CPU_EBX[mposition], Intel_cpuRegister.CPU_ECX[mposition]);
	}

}

void Get_Info_For_Intel::Information_from_CPUID7()
{
	/************************************************************************/
	/*                     本段解析CPUID= 7时的含义                          */
	/************************************************************************/


	mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 7 Sub Leaf 0"));
	if (mposition != -1)
	{
		this->GET_EXTENED_FEATURE_INFO(Intel_cpuRegister.CPU_EBX[mposition], Intel_cpuRegister.CPU_ECX[mposition]);
	}
}

void Get_Info_For_Intel::Information_from_CPUIDA()
{
	/************************************************************************/
	/*                     本段解析CPUID= 0A时的含义                          */
	/************************************************************************/


	mposition = find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 10 Sub Leaf 0"));
	if (mposition != -1)
	{
		this->GET_ARCHITECTURAL_PERFORMANCE_MONITORING(Intel_cpuRegister.CPU_EAX[mposition], Intel_cpuRegister.CPU_EBX[mposition], Intel_cpuRegister.CPU_EDX[mposition]);
	}
}

void Get_Info_For_Intel::Information_from_CPUIDB()
{
	/************************************************************************/
	/*                     本段解析CPUID= 0B时的含义                          */
	/************************************************************************/

	for (UINT j = 0; j < 10; ++j)
	{
		mposition = this->find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 11 Sub Leaf ") + to_wstring(j));
		EXTENDED_TOPOLOGY_ENUMERATION temp;
		if (mposition != -1)
		{
			temp.Shift_X2APIC_ID = Intel_cpuRegister.CPU_EAX[mposition] & SHIFT_X2APIC_ID;
			temp.Number_of_Logical_Processor = Intel_cpuRegister.CPU_EBX[mposition] & X2APIC_NUMBER_OF_LOGICAL_PROCESSOR;
			switch ((Intel_cpuRegister.CPU_ECX[mposition] & X2APIC_LEVEL_TYPE) >> 8)
			{
			case 0:
				temp.Level_Type = _T("Invalid");
				break;
			case 1:
				temp.Level_Type = _T("SMT");
				break;
			case 2:
				temp.Level_Type = _T("Core");
				break;
			default:
				temp.Level_Type = _T("Unknown");
				break;
			}
			temp.X2APIC_ID = Intel_cpuRegister.CPU_EDX[mposition];
			Intel_Data.Extended_topology_enumeration[j] = temp;
		}
		else
		{
			break;
		}
	}
}

void Get_Info_For_Intel::Information_from_CPUIDC()
{

}

void Get_Info_For_Intel::Information_from_CPUIDD()
{
	/************************************************************************ /
	/*                     本段解析CPUID= 0D时的含义                          */
	/************************************************************************/

	mposition = this->find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 13 Sub Leaf 0"));
	if (mposition != -1)
	{
		Intel_Data.Processor_extended_state.X87_State = (Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_x87_STATE) ? TRUE : FALSE;
		Intel_Data.Processor_extended_state.SSE_State = (Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_SSE_STATE) ? TRUE : FALSE;
		Intel_Data.Processor_extended_state.AVX_State = (Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_AVX_STATE) ? TRUE : FALSE;
		Intel_Data.Processor_extended_state.MPX_State = (Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_MPX_STATE) ? TRUE : FALSE;
		Intel_Data.Processor_extended_state.AVX_512_State = (Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_AVX512_STATE) >> 5;
		Intel_Data.Processor_extended_state.IA32_XSS = (Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_IA32_XSS) ? TRUE : FALSE;
		Intel_Data.Processor_extended_state.PKRU_State = (Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_PKRU_STATE) ? TRUE : FALSE;

		Intel_Data.Processor_extended_state.MAX_XCR0 = Intel_cpuRegister.CPU_EBX[mposition];
		Intel_Data.Processor_extended_state.MAX_XSAVE = Intel_cpuRegister.CPU_ECX[mposition];
		Intel_Data.Processor_extended_state.Upper_32_XCR0 = Intel_cpuRegister.CPU_EDX[mposition];
	}

	mposition = this->find_location(Intel_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 13 Sub Leaf 1"));
	if (mposition != -1)
	{
		(Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_XSAVEOPT_STATE) ? Intel_Data.Processor_extended_state.XSAVEOPT = TRUE : Intel_Data.Processor_extended_state.XSAVEOPT = FALSE;
		(Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_XSAVEC_STATE) ? Intel_Data.Processor_extended_state.XSAVEC = TRUE : Intel_Data.Processor_extended_state.XSAVEC = FALSE;
		(Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_XGETBV_STATE) ? Intel_Data.Processor_extended_state.XGETBV = TRUE : Intel_Data.Processor_extended_state.XGETBV = FALSE;
		(Intel_cpuRegister.CPU_EAX[mposition] & EXTENDED_XSAVES_XRSTORS_STATE) ? Intel_Data.Processor_extended_state.XSAVES = TRUE : Intel_Data.Processor_extended_state.XSAVES = FALSE;

		Intel_Data.Processor_extended_state.XSAVE_Area = Intel_cpuRegister.CPU_EBX[mposition];
		Intel_Data.Processor_extended_state.Lower_32_IA32_XSS_MSR = Intel_cpuRegister.CPU_ECX[mposition];
		Intel_Data.Processor_extended_state.Upper_32_IA32_XSS_MSR = Intel_cpuRegister.CPU_EDX[mposition];
	}
}

