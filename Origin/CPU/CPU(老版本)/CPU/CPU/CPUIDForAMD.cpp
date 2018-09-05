#include "../stdafx.h"
#include "CPUIDForAMD.h"

Get_Info_For_AMD::Get_Info_For_AMD()
{
	this->UPdate();
}

Get_Info_For_AMD::~Get_Info_For_AMD()
{
}
CPUID_Data Get_Info_For_AMD::Return_info() const
{
	return AMD_Data;
}

CPU_RES Get_Info_For_AMD::Return_CPUID() const
{
	return AMD_cpuRegister;
}

void Get_Info_For_AMD::UPdate()
{
	AMD_cpuRegister.erase();
	AMD_Data.erase();
	this->Get_ALL_CPUID();
	this->excute_CPUID_FOR_AMD();
}

bool Get_Info_For_AMD::Get_ALL_CPUID()
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
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{
				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[0] & 0xf) == 0)
					break;
				AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		case 0x7:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			Cpuid_sub_leaf = buffer[0];
			for (size_t j = 1; j <= Cpuid_sub_leaf; ++j)
			{
				this->Read_CPUID(buffer, i, j);
				AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(j);
				AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
				Cpuid_sub_leaf = buffer[0];
			}
			break;
		case 0x0B:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{
				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[2] & 0xff00) == 0)
					break;
				AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		case 0x0D:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{

				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((0 == buffer[0]) && (0 == buffer[1]) && (0 == buffer[2]) && (0 == buffer[3]))
					break;
				AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		case  0x0F:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			this->Read_CPUID(buffer, i, 1);
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(1);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			break;
		case 0x10:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			for (size_t j = 1; j < 4; ++j)
			{
				AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(j);
				AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			break;
		case 0x12:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			this->Read_CPUID(buffer, i, 1);
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(1);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			Cpuid_sub_leaf = 2;
			while (true)
			{
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[0] & 0xf) == 0x001b)
				{
					AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
					AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
					AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
					AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
					Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
					Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
					AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
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
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			this->Read_CPUID(buffer, i, 1);
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(1);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			break;
		case 0x17:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			Cpuid_sub_leaf = buffer[0];
			for (size_t j = 1; j <= Cpuid_sub_leaf; ++j)
			{
				this->Read_CPUID(buffer, i, j);
				AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(j);
				AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			break;
		case 0x18:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			while (true)
			{
				Cpuid_sub_leaf += 1;
				this->Read_CPUID(buffer, i, Cpuid_sub_leaf);
				if ((buffer[0]) || (buffer[3] & 0xf))
				{
					break;
				}
				AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
				AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
				AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
				AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
				Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
				Subleaf = _T(" Sub Leaf ") + std::to_wstring(Cpuid_sub_leaf);
				AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
			}
			Cpuid_sub_leaf = 0;
			break;
		default:
			AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
			AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
			AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
			AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
			Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
			Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
			AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
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
		AMD_cpuRegister.CPU_EAX.emplace_back(buffer[0]);
		AMD_cpuRegister.CPU_EBX.emplace_back(buffer[1]);
		AMD_cpuRegister.CPU_ECX.emplace_back(buffer[2]);
		AMD_cpuRegister.CPU_EDX.emplace_back(buffer[3]);
		Mainleaf = _T("Main Leaf ") + std::to_wstring(i);
		Subleaf = _T(" Sub Leaf ") + std::to_wstring(0);
		AMD_cpuRegister.CPU_LEAF_NAME.emplace_back(Mainleaf + Subleaf);
	}

	return true;
}

bool Get_Info_For_AMD::excute_CPUID_FOR_AMD()
{
	//从WMI中获取信息
	CSynQueryData recvnotify(L"root\\CIMV2", L"SELECT Name,CurrentClockSpeed ,MaxClockSpeed,ExtClock,Manufacturer,MaxClockSpeed,NumberOfCores,ProcessorId,Revision,SocketDesignation,UpgradeMethod,NumberOfLogicalProcessors FROM Win32_Processor");
	recvnotify.ExcuteFun();
	int mposition = -1;
	vector<string> out_put_data = recvnotify.ClassData();
	vector<string> out_put_name = recvnotify.ClassName();
	mposition = this->find_location(out_put_name, _T("Name"));
	if (mposition != -1)	AMD_Data.Name = out_put_data[mposition];
	else AMD_Data.Name = _T("Unknown");
	mposition = this->find_location(out_put_name, _T("CurrentClockSpeed"));
	if (mposition != -1)	AMD_Data.CurrentClockSpeed = stoi(out_put_data[mposition]);
	else AMD_Data.CurrentClockSpeed = 0;
	mposition = this->find_location(out_put_name, _T("ExtClock"));
	if (mposition != -1)	AMD_Data.ExtClock = stoi(out_put_data[mposition]);
	else AMD_Data.ExtClock = 0;
	mposition = this->find_location(out_put_name, _T("NumberOfCores"));
	if (mposition != -1)	AMD_Data.Core = stoi(out_put_data[mposition]);
	else AMD_Data.Core = 0;
	mposition = this->find_location(out_put_name, _T("ProcessorId"));
	if (mposition != -1)	AMD_Data.ProcessorID = out_put_data[mposition];
	else AMD_Data.ProcessorID = _T("Unknown");
	mposition = this->find_location(out_put_name, _T("Revision"));
	if (mposition != -1)	AMD_Data.Revision = stoi(out_put_data[mposition]);
	else AMD_Data.Revision = 0;
	mposition = this->find_location(out_put_name, _T("SocketDesignation"));
	if (mposition != -1)	AMD_Data.SocketDesignation = out_put_data[mposition];
	else AMD_Data.SocketDesignation = _T("Unknown");
	mposition = this->find_location(out_put_name, _T("UpgradeMethod"));
	if (mposition != -1)	AMD_Data.UpgradeMethod = stoi(out_put_data[mposition]);
	else AMD_Data.UpgradeMethod = 0;
	mposition = this->find_location(out_put_name, _T("MaxClockSpeed"));
	if (mposition != -1)	AMD_Data.MaxClockSpeed = stoi(out_put_data[mposition]);
	else AMD_Data.MaxClockSpeed = 0;
	mposition = this->find_location(out_put_name, _T("NumberOfLogicalProcessors"));
	if (mposition != -1)	AMD_Data.Thread = stoi(out_put_data[mposition]);
	else AMD_Data.Thread = 0;

	/************************************************************************/
	/*                          本段解析CPUID = 0时的含义                     */
	/************************************************************************/

	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 0 Sub Leaf 0"));

	*reinterpret_cast<int*>(AMD_Data.Manufacturer) = AMD_cpuRegister.CPU_EBX[mposition];
	*reinterpret_cast<int*>(AMD_Data.Manufacturer + 4) = AMD_cpuRegister.CPU_EDX[mposition];
	*reinterpret_cast<int*>(AMD_Data.Manufacturer + 8) = AMD_cpuRegister.CPU_ECX[mposition];

	/************************************************************************/
	/*                          本段解析CPUID = 1时的含义                     */
	/************************************************************************/

	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 1 Sub Leaf 0"));
	if (mposition != -1)
	{
		AMD_Data.Family = (AMD_cpuRegister.CPU_EAX[mposition] & CPUID_Family) >> 8;
		AMD_Data.Model = (AMD_cpuRegister.CPU_EAX[mposition] & CPUID_Model) >> 4;
		AMD_Data.Stepping = AMD_cpuRegister.CPU_EAX[mposition] & CPUID_Stepping;
		AMD_Data.CPU_Type = _T("AMD not Support");
		AMD_Data.Ext_Family = ((AMD_cpuRegister.CPU_EAX[mposition] & CPUID_Extended_Family) >> 20) + ((AMD_cpuRegister.CPU_EAX[mposition] & CPUID_Family) >> 8);
		AMD_Data.Ext_Model = (((AMD_cpuRegister.CPU_EAX[mposition] & CPUID_Extended_Model) >> 16) << 4) + ((AMD_cpuRegister.CPU_EAX[mposition] & CPUID_Model) >> 4);
		this->AMD_Get_Standard_Feature(AMD_cpuRegister.CPU_ECX[mposition], AMD_cpuRegister.CPU_EDX[mposition]);
	}

	/************************************************************************/
	/*                          本段解析CPUID = 5时的含义                     */
	/************************************************************************/

	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 5 Sub Leaf 0"));
	if (mposition != -1)
	{
		AMD_Data.Smallest_monitor_line_size = AMD_cpuRegister.CPU_EAX[mposition] & SMALLEST_MONITOR_LINE_SIZE;
		AMD_Data.Largest_monitor_line_size = AMD_cpuRegister.CPU_EBX[mposition] & LARGEST_MONITOR_LINE_SIZE;
		(AMD_cpuRegister.CPU_ECX[mposition] & MONITOR_MWAIT_EXTENSION) ? AMD_Data.EMX = TRUE : AMD_Data.EMX = FALSE;
		(AMD_cpuRegister.CPU_ECX[mposition] & MONITOR_MWAIT_INTERRUPTS) ? AMD_Data.IBE = TRUE : AMD_Data.IBE = FALSE;
	}

	/************************************************************************/
	/*                          本段解析CPUID = 6时的含义                     */
	/************************************************************************/

	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 6 Sub Leaf 0"));
	if (mposition != -1)
	{
		(AMD_cpuRegister.CPU_ECX[mposition] & 0x1) ? AMD_Data.EffFreq = TRUE : AMD_Data.EffFreq = FALSE;
	}

	/************************************************************************/
	/*                          本段解析CPUID = 7时的含义                     */
	/************************************************************************/

	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 7 Sub Leaf 0"));
	if (mposition != -1)
	{
		(AMD_cpuRegister.CPU_EBX[mposition] & FSGSBASE_FLAGS) ? AMD_Data.Extened_Feature.AMD_Extened_Fearture.FSGSBASE = TRUE : AMD_Data.Extened_Feature.AMD_Extened_Fearture.FSGSBASE = FALSE;
		(AMD_cpuRegister.CPU_EBX[mposition] & BMI1_FLAGS) ? AMD_Data.Extened_Feature.AMD_Extened_Fearture.BMI1 = TRUE : AMD_Data.Extened_Feature.AMD_Extened_Fearture.BMI1 = FALSE;
		(AMD_cpuRegister.CPU_EBX[mposition] & AVX2_FLAGS) ? AMD_Data.Extened_Feature.AMD_Extened_Fearture.AVX2 = TRUE : AMD_Data.Extened_Feature.AMD_Extened_Fearture.AVX2 = FALSE;
		(AMD_cpuRegister.CPU_EBX[mposition] & SMEP_FLAGS) ? AMD_Data.Extened_Feature.AMD_Extened_Fearture.SMEP = TRUE : AMD_Data.Extened_Feature.AMD_Extened_Fearture.SMEP = FALSE;
		(AMD_cpuRegister.CPU_EBX[mposition] & BMI2_FLAGS) ? AMD_Data.Extened_Feature.AMD_Extened_Fearture.BMI2 = TRUE : AMD_Data.Extened_Feature.AMD_Extened_Fearture.BMI2 = FALSE;
	}
	
	/************************************************************************/
	/*                  本段解析CPUID = 0x80000001时的含义                   */
	/************************************************************************/
	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 2147483649 Sub Leaf 0"));
	if (mposition != -1)
	{
		this->AMD_Get_Extended_Feature(AMD_cpuRegister.CPU_ECX[mposition], AMD_cpuRegister.CPU_EDX[mposition]);
	}


	/************************************************************************/
	/*                  本段解析CPUID = 0x80000005时的含义                   */
	/************************************************************************/
	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 2147483653 Sub Leaf 0"));
	if (mposition != -1)
	{
		AMD_Cache_info cache_temp;
		if (!(AMD_cpuRegister.CPU_EAX[mposition] & INSTRUCTION_TLB_ASSOC))
		{
			AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB and 4MB pages " +
				to_wstring((AMD_cpuRegister.CPU_EAX[mposition] & INSTRUCTION_TLB_ASSOC) >> 8) + 
				"-way associative" + 
				to_wstring((AMD_cpuRegister.CPU_EAX[mposition] & INSTRUCTION_TLB_SIZE) >> 0) +
				"BYTES");
		}
		if (!(AMD_cpuRegister.CPU_EAX[mposition] & DATA_TLB_ASSOC))
		{
			AMD_Data.Cache_TLB.emplace_back("L1 Data TLB:2MB and 4MB pages " +
				to_wstring((AMD_cpuRegister.CPU_EAX[mposition] & DATA_TLB_ASSOC) >> 24) +
				"-way associative" +
				to_wstring((AMD_cpuRegister.CPU_EAX[mposition] & DATA_TLB_SIZE) >> 16) +
				"BYTES");
		}
		if (!(AMD_cpuRegister.CPU_EBX[mposition] & INSTRUCTION_TLB_ASSOC))
		{
			AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages " +
				to_wstring((AMD_cpuRegister.CPU_EBX[mposition] & INSTRUCTION_TLB_ASSOC) >> 8)+
				"-way associative" +
				to_wstring((AMD_cpuRegister.CPU_EBX[mposition] & INSTRUCTION_TLB_SIZE) >> 0) +
				"BYTES");
		}
		if (!(AMD_cpuRegister.CPU_EBX[mposition] & DATA_TLB_ASSOC))
		{
			AMD_Data.Cache_TLB.emplace_back("L1 Data TLB: 4KB pages " +
				to_wstring((AMD_cpuRegister.CPU_EBX[mposition] & DATA_TLB_ASSOC) >> 24) +
				"-way associative" +
				to_wstring((AMD_cpuRegister.CPU_EBX[mposition] & DATA_TLB_SIZE) >> 16) +
				"BYTES");
		}
		cache_temp.Cache_level = 1;
		cache_temp.Cache_type = _T("Data");
		cache_temp.Cache_line_size = ((AMD_cpuRegister.CPU_ECX[mposition] & L1_DATA_CACHE_LINE_SIZE) >> 0);
		cache_temp.Cache_line_per_tag = ((AMD_cpuRegister.CPU_ECX[mposition] & L1_DATA_CACHE_LINE_PER_TAG) >> 8);
		cache_temp.Cache_Ways = ((AMD_cpuRegister.CPU_ECX[mposition] & L1_DATA_CACHE_ASSOC) >> 16);
		cache_temp.Cache_Size = ((AMD_cpuRegister.CPU_ECX[mposition] & L1_DATA_CACHE_SIZE) >> 24);
		cache_temp.Cache_level = 1;
		AMD_Data.AMD_cache.emplace_back(cache_temp);
		cache_temp.Cache_type = _T("Instruction");
		cache_temp.Cache_line_size = ((AMD_cpuRegister.CPU_EDX[mposition] & L1_DATA_CACHE_LINE_SIZE) >> 0);
		cache_temp.Cache_line_per_tag = ((AMD_cpuRegister.CPU_EDX[mposition] & L1_DATA_CACHE_LINE_PER_TAG) >> 8);
		cache_temp.Cache_Ways = ((AMD_cpuRegister.CPU_EDX[mposition] & L1_DATA_CACHE_ASSOC) >> 16);
		cache_temp.Cache_Size = ((AMD_cpuRegister.CPU_EDX[mposition] & L1_DATA_CACHE_SIZE) >> 24);
		AMD_Data.AMD_cache.emplace_back(cache_temp);
	}

	/************************************************************************/
	/*                  本段解析CPUID = 0x80000006时的含义                   */
	/************************************************************************/
	mposition = find_location(AMD_cpuRegister.CPU_LEAF_NAME, _T("Main Leaf 2147483654 Sub Leaf 0"));
	if (mposition != -1)
	{
		AMD_Cache_info cache_temp;
		if (!(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_ASSOC))
		{
			switch ((AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_ASSOC) >> 12)
			{
			case 0x01:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages Direct mapped" +
									to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
									"BYTES");
				break;
			case 0x02:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 2-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0x04:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 4-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0x06:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 8-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0x08:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 16-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xA0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 32-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xB0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 48-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xC0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 64-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xD0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 96-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xE0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 128-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xF0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages Fully associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			}
		}
		if (!((AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_ASSOC) >> 28))
		{
			switch ((AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_ASSOC) >> 28)
			{
			case 0x01:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages Direct mapped" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x02:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 2-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x04:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 4-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x06:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 8-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x08:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 16-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xA0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 32-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xB0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 48-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xC0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 64-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xD0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 96-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xE0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages 128-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xF0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 2MB - 4MB pages Fully associative" +
					to_wstring(AMD_cpuRegister.CPU_EAX[mposition] & L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			}
		}
		if (!(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_ASSOC))
		{
			switch ((AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_ASSOC) >> 12)
			{
			case 0x01:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages Direct mapped" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0x02:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 2-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0x04:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 4-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0x06:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 8-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0x08:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 16-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xA0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 32-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xB0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 48-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xC0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 64-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xD0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 96-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xE0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 128-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			case 0xF0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages Fully associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_INSTRUCTION_TLB_SIZE) +
					"BYTES");
				break;
			}
		}
		if (!((AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_ASSOC) >> 12))
		{
			switch ((AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_ASSOC) >> 12)
			{
			case 0x01:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages Direct mapped" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x02:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 2-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x04:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 4-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x06:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 8-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0x08:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 16-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xA0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 32-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xB0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 48-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xC0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 64-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xD0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 96-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xE0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages 128-way associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			case 0xF0:
				AMD_Data.Cache_TLB.emplace_back("L1 Instruction TLB: 4KB pages Fully associative" +
					to_wstring(AMD_cpuRegister.CPU_EBX[mposition] & KB4_L2_DATA_TLB_SIZE) +
					"BYTES");
				break;
			}
		}
		cache_temp.Cache_level = 2;
		cache_temp.Cache_type = _T("Data");
		cache_temp.Cache_line_size = ((AMD_cpuRegister.CPU_ECX[mposition] & L2_DATA_CACHE_LINE_SIZE) >> 0);
		cache_temp.Cache_line_per_tag = ((AMD_cpuRegister.CPU_ECX[mposition] & L2_DATA_CACHE_LINE_PER_TAG) >> 8);
		cache_temp.Cache_Ways = ((AMD_cpuRegister.CPU_ECX[mposition] & L2_DATA_CACHE_ASSOC) >> 12);
		cache_temp.Cache_Size = ((AMD_cpuRegister.CPU_ECX[mposition] & L2_DATA_CACHE_SIZE) >> 16);
		cache_temp.Cache_level = 3;
		cache_temp.Cache_type = _T("Data");
		cache_temp.Cache_line_size = ((AMD_cpuRegister.CPU_EDX[mposition] & L3_INSTRUCTION_CACHE_LINE_SIZE) >> 0);
		cache_temp.Cache_line_per_tag = ((AMD_cpuRegister.CPU_EDX[mposition] & L3_INSTRUCTION_CACHE_LINE_PER_TAG) >> 8);
		cache_temp.Cache_Ways = ((AMD_cpuRegister.CPU_EDX[mposition] & L3_INSTRUCTION_CACHE_ASSOC) >> 12);
		cache_temp.Cache_Size = ((AMD_cpuRegister.CPU_EDX[mposition] & L3_INSTRUCTION_CACHE_SIZE) >> 18);
		AMD_Data.AMD_cache.emplace_back(cache_temp);
	}
	return true;
}

bool Get_Info_For_AMD::AMD_Get_Standard_Feature(int ECX, int EDX)
{
	//ECX
	(ECX & SSE3_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SSE3 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SSE3 = FALSE;
	(ECX & PCLMULQDQ_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PCLMULQDQ = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PCLMULQDQ = FALSE;
	(ECX & MONITOR_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MONITOR = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MONITOR = FALSE;
	(ECX & SSSE3_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SSSE3 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SSSE3 = FALSE;
	(ECX & FMA_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.FMA = TRUE : AMD_Data.Feature_info.AMD_Feature_info.FMA = FALSE;
	(ECX & CMPXCHG16B_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.CMPXCHG16B = TRUE : AMD_Data.Feature_info.AMD_Feature_info.CMPXCHG16B = FALSE;
	(ECX & SSE4_1_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SSE41 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SSE41 = FALSE;
	(ECX & SSE4_2_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SSE42 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SSE42 = FALSE;
	(ECX & POPCNT_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.POPCNT = TRUE : AMD_Data.Feature_info.AMD_Feature_info.POPCNT = FALSE;
	(ECX & AESNI_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.AES = TRUE : AMD_Data.Feature_info.AMD_Feature_info.AES = FALSE;
	(ECX & XSAVE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.XSAVE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.XSAVE = FALSE;
	(ECX & OSXSAVE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.OSXSAVE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.OSXSAVE = FALSE;
	(ECX & AVX_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.AVX = TRUE : AMD_Data.Feature_info.AMD_Feature_info.AVX = FALSE;
	(ECX & F16C_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.F16C = TRUE : AMD_Data.Feature_info.AMD_Feature_info.F16C = FALSE;
	(ECX & RDRAND_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.RDRAND = TRUE : AMD_Data.Feature_info.AMD_Feature_info.RDRAND = FALSE;
	//EDX
	(EDX & FPU_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.FPU = TRUE : AMD_Data.Feature_info.AMD_Feature_info.FPU = FALSE;
	(EDX & VME_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.VME = TRUE : AMD_Data.Feature_info.AMD_Feature_info.VME = FALSE;
	(EDX & DE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.DE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.DE = FALSE;
	(EDX & PSE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PSE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PSE = FALSE;
	(EDX & TSC_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.TSC = TRUE : AMD_Data.Feature_info.AMD_Feature_info.TSC = FALSE;
	(EDX & MSR_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MSR = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MSR = FALSE;
	(EDX & PAE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PAE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PAE = FALSE;
	(EDX & MCE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MCE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MCE = FALSE;
	(EDX & CX8_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.CMPXCHG8B = TRUE : AMD_Data.Feature_info.AMD_Feature_info.CMPXCHG8B = FALSE;
	(EDX & APIC_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.APIC = TRUE : AMD_Data.Feature_info.AMD_Feature_info.APIC = FALSE;
	(EDX & SEP_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SysEnterSysExit = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SysEnterSysExit = FALSE;
	(EDX & MTRR_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MTRR = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MTRR = FALSE;
	(EDX & PGE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PGE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PGE = FALSE;
	(EDX & MCA_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MCA = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MCA = FALSE;
	(EDX & CMOV_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.CMOV = TRUE : AMD_Data.Feature_info.AMD_Feature_info.CMOV = FALSE;
	(EDX & PAT_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PAT = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PAT = FALSE;
	(EDX & PSE_36_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PSE36 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PSE36 = FALSE;
	(EDX & CLFSH_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.CLFSH = TRUE : AMD_Data.Feature_info.AMD_Feature_info.CLFSH = FALSE;
	(EDX & MMX_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MMX = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MMX = FALSE;
	(EDX & FXSR_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.FXSR = TRUE : AMD_Data.Feature_info.AMD_Feature_info.FXSR = FALSE;
	(EDX & SSE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SSE = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SSE = FALSE;
	(EDX & SSE2_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SSE2 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SSE2 = FALSE;
	(EDX & HTT_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.HTT = TRUE : AMD_Data.Feature_info.AMD_Feature_info.HTT = FALSE;

	return TRUE;
}

bool Get_Info_For_AMD::AMD_Get_Extended_Feature(int ECX, int EDX)
{
	//ECX
	(ECX & LAHFSAHF_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.LahfSahf = TRUE : AMD_Data.Feature_info.AMD_Feature_info.LahfSahf = FALSE;
	(ECX & CMPLEGACY_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.CmpLegacy = TRUE : AMD_Data.Feature_info.AMD_Feature_info.CmpLegacy = FALSE;
	(ECX & SVM_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SVM = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SVM = FALSE;
	(ECX & EXTAPICSPACE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.ExtApicSpace = TRUE : AMD_Data.Feature_info.AMD_Feature_info.ExtApicSpace = FALSE;
	(ECX & ALTMOVCR8_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.AltMovCr8 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.AltMovCr8 = FALSE;
	(ECX & ABM_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.ABM = TRUE : AMD_Data.Feature_info.AMD_Feature_info.ABM = FALSE;
	(ECX & SSE4A_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SSE4A = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SSE4A = FALSE;
	(ECX & MISALIGNSSE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MisAlignSse = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MisAlignSse = FALSE;
	(ECX & _3DNOWPREFETCH_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info._3DNowPrefetch = TRUE : AMD_Data.Feature_info.AMD_Feature_info._3DNowPrefetch = FALSE;
	(ECX & OSVW_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.OSVW = TRUE : AMD_Data.Feature_info.AMD_Feature_info.OSVW = FALSE;
	(ECX & IBS_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.IBS = TRUE : AMD_Data.Feature_info.AMD_Feature_info.IBS = FALSE;
	(ECX & XOP_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.XOP = TRUE : AMD_Data.Feature_info.AMD_Feature_info.XOP = FALSE;
	(ECX & SKINIT_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SKINIT = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SKINIT = FALSE;
	(ECX & WDT_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.WDT = TRUE : AMD_Data.Feature_info.AMD_Feature_info.WDT = FALSE;
	(ECX & LWP_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.LWP = TRUE : AMD_Data.Feature_info.AMD_Feature_info.LWP = FALSE;
	(ECX & FMA4_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.FMA4 = TRUE : AMD_Data.Feature_info.AMD_Feature_info.FMA4 = FALSE;
	(ECX & TBM_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.TBM = TRUE : AMD_Data.Feature_info.AMD_Feature_info.TBM = FALSE;
	(ECX & TOPOLOGYEXTENSIONS_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.TopologyExtensions = TRUE : AMD_Data.Feature_info.AMD_Feature_info.TopologyExtensions = FALSE;
	(ECX & PERFCTREXTCORE_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PerfCtrExtCore = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PerfCtrExtCore = FALSE;
	(ECX & PERFCTREXTNB_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PerfCtrExtNB = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PerfCtrExtNB = FALSE;
	(ECX & DATABREAKPOINTEXTENSION_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.DataBreakpointExtension = TRUE : AMD_Data.Feature_info.AMD_Feature_info.DataBreakpointExtension = FALSE;
	(ECX & PERFTSC_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.PerfTsc = TRUE : AMD_Data.Feature_info.AMD_Feature_info.PerfTsc = FALSE;
	//EDX
	(EDX & SYSCALLSYSRET_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.SysCallSysRet = TRUE : AMD_Data.Feature_info.AMD_Feature_info.SysCallSysRet = FALSE;
	(EDX & NX_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.NX = TRUE : AMD_Data.Feature_info.AMD_Feature_info.NX = FALSE;
	(EDX & MMXEXT_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.MmxExt = TRUE : AMD_Data.Feature_info.AMD_Feature_info.MmxExt = FALSE;
	(EDX & FFXSR_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.FFXSR = TRUE : AMD_Data.Feature_info.AMD_Feature_info.FFXSR = FALSE;
	(EDX & PAGE1GB_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.Page1GB = TRUE : AMD_Data.Feature_info.AMD_Feature_info.Page1GB = FALSE;
	(EDX & RDTSCP_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.RDTSCP = TRUE : AMD_Data.Feature_info.AMD_Feature_info.RDTSCP = FALSE;
	(EDX & LM_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info.LM = TRUE : AMD_Data.Feature_info.AMD_Feature_info.LM = FALSE;
	(EDX & E_3DNOW_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info._3DNowExt = TRUE : AMD_Data.Feature_info.AMD_Feature_info._3DNowExt = FALSE;
	(EDX & _3DNOW_FLAGS) ? AMD_Data.Feature_info.AMD_Feature_info._3DNow = TRUE : AMD_Data.Feature_info.AMD_Feature_info._3DNow = FALSE;
	return true;
}

int Get_Info_For_AMD::find_location(vector<wstring> vecIntegerArray, wstring search_string)
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