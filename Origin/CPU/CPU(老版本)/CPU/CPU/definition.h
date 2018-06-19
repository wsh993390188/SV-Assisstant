/**

*Copyright (C), 2017-.

*@file definition.h，存储格式：UNICODE

*@brief 获取CPUID信息

*@author 王硕

*@version 1.0

*@date 2017.9.21

*@History: 修改历史记录列表，每行修改记录应包括修改日期、修改者及修改内容简述

*<p>2017.9.21 1.0 王硕 CPUID中的解析定义</p>

*/

#pragma once
#include <tchar.h>
#include <intrin.h>
#include "../WMI/SynQueryData.h"

/**

@brief READ_CPUID的返回值

*/

class UnCopyable
{
protected:
	UnCopyable() {}
	~UnCopyable() {}

private:
	UnCopyable(const UnCopyable& x);
	UnCopyable& operator=(const UnCopyable& x);
};


enum ERROR_CODE
{
	FAILED_TO_GET_INFO = 0,
	GET_CPUID_SUCCESS = 1,
	ILLEGAL_OPERAND = 2
};

/**

@brief CPU的品牌

*/
enum CPU_Brands
{
	Others = 0,
	Intel = 1,
	AMD = 2,
	ZhaoXin = 3,
	FAILED = 0xFFFF
};

/************
INTEL的信息
************/
//Instruction SIMD (x86) when CPUID = 1 
//EAX
#define CPUID_Stepping						0x0000000F
#define CPUID_Model							0x000000F0
#define CPUID_Family						0x00000F00
#define CPUID_Processor_Type				0x00003000
#define CPUID_Extended_Model				0x000F0000
#define CPUID_Extended_Family				0x0FF00000
//EBX
#define CPUID_BRANDID						0x000000FF

//ECX
#define SSE3_FLAGS							0x00000001
#define PCLMULQDQ_FLAGS						0x00000002
#define DTES64_FLAGS						0x00000004
#define MONITOR_FLAGS						0x00000008
#define DS_CPL_FLAGS						0x00000010
#define VMX_FLAGS							0x00000020
#define SMX_FLAGS							0x00000040
#define EIST_FLAGS							0x00000080
#define TM2_FLAGS							0x00000100
#define SSSE3_FLAGS							0x00000200
#define CNXT_ID_FLAGS						0x00000400
#define SDBG_FLAGS							0x00000800
#define FMA_FLAGS							0x00001000
#define CMPXCHG16B_FLAGS					0x00002000
#define xTPR_Update_Control_FLAGS			0x00004000
#define PDCM_FLAGS							0x00008000
#define PCID_FLAGS							0x00020000
#define DCA_FLAGS							0x00040000
#define SSE4_1_FLAGS						0x00080000
#define SSE4_2_FLAGS						0x00100000
#define X2APIC_FLAGS						0x00200000
#define MOVBE_FLAGS							0x00400000
#define POPCNT_FLAGS						0x00800000
#define TSC_Deadline_FLAGS					0x01000000
#define AESNI_FLAGS							0x02000000
#define XSAVE_FLAGS							0x04000000
#define OSXSAVE_FLAGS						0x08000000
#define AVX_FLAGS							0x10000000
#define F16C_FLAGS							0x20000000
#define RDRAND_FLAGS						0x40000000
//EDX
#define FPU_FLAGS							0x00000001
#define VME_FLAGS							0x00000002
#define DE_FLAGS							0x00000004
#define PSE_FLAGS							0x00000008
#define TSC_FLAGS							0x00000010
#define MSR_FLAGS							0x00000020
#define PAE_FLAGS							0x00000040
#define MCE_FLAGS							0x00000080
#define CX8_FLAGS							0x00000100
#define APIC_FLAGS							0x00000200

#define SEP_FLAGS							0x00000800
#define MTRR_FLAGS							0x00001000
#define PGE_FLAGS							0x00002000
#define MCA_FLAGS							0x00004000
#define CMOV_FLAGS							0x00008000
#define PAT_FLAGS							0x00010000
#define PSE_36_FLAGS						0x00020000
#define PSN_FLAGS							0x00040000
#define CLFSH_FLAGS							0x00080000

#define DS_FLAGS							0x00200000
#define ACPI_FLAGS							0x00400000
#define MMX_FLAGS							0x00800000
#define FXSR_FLAGS							0x01000000
#define SSE_FLAGS							0x02000000
#define SSE2_FLAGS							0x04000000
#define SS_FLAGS							0x08000000
#define HTT_FLAGS							0x10000000
#define TM_FLAGS							0x20000000
#define PBE_FLAGS							0x80000000

//Cache and TLB CPUID = 2
#define CACHE_AVAILABLE						0x80000000
#define	CACHE_SHIELD						0x000000FF

//Deterministic Cache Parameters CPUID = 4
//EAX
#define CACHE_TYPE							0x0000001F
#define CACHE_LEVEL							0x000000E0
#define CACHE_MAX_ADDR_LOGICAL				0x03FFC000
#define CACHE_MAX_ADDR_PHYSICAL				0xFC000000
//EBX
#define CACHE_WAYS							0xFFC00000
#define CACHE_PARTITIONS					0x003FF000
#define CACHE_LINE_SIZE						0x00000FFF
//EDX
#define CACHE_INVALIDATE					0x00000000
#define CACHE_INCLUSIVENESS					0x00000001
#define CACHE_COMPLEX_INDEXING				0x00000002

//MONITOR/MWAIT CPUID = 5
//EAX
#define SMALLEST_MONITOR_LINE_SIZE			0x0000FFFF
//EBX
#define LARGEST_MONITOR_LINE_SIZE			0x0000FFFF
//ECX
#define MONITOR_MWAIT_EXTENSION				0x00000000
#define MONITOR_MWAIT_INTERRUPTS			0x00000001
//EDX
#define C_STATE_0							0x0000000F
#define C_STATE_1							0x000000F0	
#define C_STATE_2							0x00000F00	
#define C_STATE_3							0x0000F000	
#define C_STATE_4							0x000F0000	
#define C_STATE_5							0x00F00000	
#define C_STATE_6							0x0F000000	
#define C_STATE_7							0xF0000000	

//Thermal and Power Management CPUID = 6
//EAX
#define DIGITAL_TEM_SENSOR_FLAGS			0x00000001
#define TURBO_BOOST_FLAGS					0x00000002

#define ARAT_FLAGS							0x00000008
#define PLN_FLAGS							0x00000010
#define ECMD_FLAGS							0x00000020
#define PTM_FLAGS							0x00000040
#define HWP_FLAGS							0x00000080
#define HWP_NOTIFICATION_FLAGS				0x00000100
#define HWP_ACTIVITY_WINDOW_FLAGS			0x00000200
#define HWP_ENERGY_PERFORMANCE_FLAGS		0x00000400
#define HWP_PACKAGE_LEVEL_REQUEST_FLAGS		0x00000800

#define HDC_FLAGS							0x00002000
#define TURBO_BOOST_MAX_3_FLAGS				0x00004000
//EBX
#define DIGITAL_THERMAL_SENSOR_NUM			0x0000000F
//ECX
#define HW_COORDINATION_FEEDBACK			0x00000000
#define PERFORMANCE_ENERGY_BIAS				0x00000008

//E-Feature info when CPUID=7
//EBX
#define FSGSBASE_FLAGS						0x00000001
#define IA32_TSC_ADJUST_MSR_FLAGS			0x00000002
#define SGX_FLAGS							0x00000004
#define BMI1_FLAGS							0x00000008
#define HLE_FLAGS							0x00000010
#define AVX2_FLAGS							0x00000020
#define FDP_EXCPTN_ONLY_FLAGS				0x00000040
#define SMEP_FLAGS							0x00000080
#define BMI2_FLAGS							0x00000100
#define Enhanced_REP_MOVSB_OR_STOSB_FLAGS	0x00000200
#define CNXT_ID_FLAGS						0x00000400
#define INVPCID_FLAGS						0x00000800
#define RTM_FLAGS							0x00001000
#define RDT_M_FLAGS							0x00002000
#define FPU_CS_DS_FLAGS						0x00004000
#define MPX_FLAGS							0x00008000

#define RDSEED_FLAGS						0x00040000
#define ADX_FLAGS							0x00080000
#define SMAP_FLAGS							0x00100000

#define CLFLUSHOPT_FLAGS					0x00800000
#define CLWB_FLAGS							0x01000000
#define Intel_Processor_Trace_FLAGS			0x02000000

#define SHA_FLAGS							0x20000000
//ECX
#define PREFETCHWT1_FLAGS					0x00000001
#define UMIP_FLAGS							0x00000004
#define PKU_FLAGS							0x00000008
#define OSPKE_FLAGS							0x00000010
#define MAWAU_VALUE_BY_BNDLDX				0x003E0000
#define RDPID_FLAGS							0x00400000
#define SGX_LC_FLAGS						0x40000000

//Architectural Performance Monitoring CPUID = 0A
//EAX
#define VERSION_ID_OF_P_MONITOR				0x000000FF
//Number of general-purpose performance monitoring counter per logical processor.
#define NUMBER_GENERAL_PURPOSE				0x0000FF00
#define BITWIDTH_GENERAL_PURPOSE			0x00FF0000
#define LENGTH_OF_EBX_BIT					0xFF000000
//EBX
#define CORE_CYCLE_EVENT					0x00000001
#define INSTRUCTION_RETIRED_EVENT			0x00000002
#define REFERENCE_CYCLES_EVENT				0x00000004
#define LAST_LEVEL_CACHE_REFERENCE_EVENT	0x00000008
#define LAST_LEVEL_CACHE_MISSES_EVENT		0x00000010
#define BRANCH_INSTRUCTION_RETIRED_EVENT	0x00000020
#define BRANCH_MISPREDICT_RETIRED_EVENT		0x00000040
//EDX
#define NUMBER_OF_FIXED_FUNCTION			0x0000001F
#define BITWIDTH_OF_FIXED_FUNCTION			0x00001FE0
#define ANY_THREAD_DEPRECATION				0x00008000

//Extended Topology Enumeration CPUID = 0B
//EAX
#define SHIFT_X2APIC_ID						0x0000001F
//EBX 
#define X2APIC_NUMBER_OF_LOGICAL_PROCESSOR	0x0000FFFF
//ECX
#define X2APIC_LEVEL_TYPE					0x0000FF00

//Processor Extended State Enumeration Main Leaf CPUID = 0D ECX = 0
//EAX
#define EXTENDED_x87_STATE					0x00000001
#define EXTENDED_SSE_STATE					0x00000002
#define EXTENDED_AVX_STATE					0x00000004
#define EXTENDED_MPX_STATE					0x00000018
#define EXTENDED_AVX512_STATE				0x000000E0
#define EXTENDED_IA32_XSS					0x00000100
#define EXTENDED_PKRU_STATE					0x00000200
//Processor Extended State Enumeration Sub Leaf CPUID = 0D ECX = 1
//EAX
#define EXTENDED_XSAVEOPT_STATE				0x00000001
#define EXTENDED_XSAVEC_STATE				0x00000002
#define EXTENDED_XGETBV_STATE				0x00000004
#define EXTENDED_XSAVES_XRSTORS_STATE		0x00000008

/*********
AMD的信息
**********/
//Instruction SIMD (x86) when CPUID = 1 different from Intel
//ECX
#define CMPXCHG16B_FLAGS					0x00002000
//Instruction SIMD (x86) when EAX=0x80000001 CPUID(Only AMD)
//EDX
#define SYSCALLSYSRET_FLAGS					0x00000800
#define NX_FLAGS							0x00100000
#define MMXEXT_FLAGS						0x00400000
#define FFXSR_FLAGS							0x02000000
#define PAGE1GB_FLAGS						0x04000000
#define RDTSCP_FLAGS						0x08000000
#define LM_FLAGS							0x20000000
#define E_3DNOW_FLAGS						0x40000000
#define _3DNOW_FLAGS						0x80000000
//ECX
#define LAHFSAHF_FLAGS						0x00000001
#define CMPLEGACY_FLAGS						0x00000002
#define SVM_FLAGS							0x00000004
#define EXTAPICSPACE_FLAGS					0x00000008
#define ALTMOVCR8_FLAGS						0x00000010
#define ABM_FLAGS							0x00000020
#define SSE4A_FLAGS							0x00000040
#define MISALIGNSSE_FLAGS					0x00000080
#define _3DNOWPREFETCH_FLAGS				0x00000100
#define OSVW_FLAGS							0x00000200
#define IBS_FLAGS							0x00000400
#define XOP_FLAGS							0x00000800
#define SKINIT_FLAGS						0x00001000
#define WDT_FLAGS							0x00002000
#define LWP_FLAGS							0x00008000
#define FMA4_FLAGS							0x00010000
#define TBM_FLAGS							0x00200000
#define TOPOLOGYEXTENSIONS_FLAGS			0x00400000
#define PERFCTREXTCORE_FLAGS				0x00800000
#define PERFCTREXTNB_FLAGS					0x01000000
#define DATABREAKPOINTEXTENSION_FLAGS		0x04000000
#define PERFTSC_FLAGS						0x08000000

//L1 Cache and TLB Information CPUID = 0x80000005
//EAX
#define INSTRUCTION_TLB_SIZE				0x000000FF
#define INSTRUCTION_TLB_ASSOC				0x0000FF00
#define DATA_TLB_SIZE						0x00FF0000
#define DATA_TLB_ASSOC						0xFF000000
//EBX
#define KB4_INSTRUCTION_TLB_SIZE			0x000000FF
#define KB4_INSTRUCTION_TLB_ASSOC			0x0000FF00
#define KB4_DATA_TLB_SIZE					0x00FF0000
#define KB4_DATA_TLB_ASSOC					0xFF000000
//ECX
#define L1_DATA_CACHE_SIZE					0xFF000000
#define L1_DATA_CACHE_ASSOC					0x00FF0000
#define L1_DATA_CACHE_LINE_PER_TAG			0x0000FF00
#define L1_DATA_CACHE_LINE_SIZE				0x000000FF
//EDX
#define L1_INSTRUCTION_CACHE_SIZE			0xFF000000
#define L1_INSTRUCTION_CACHE_ASSOC			0x00FF0000
#define L1_INSTRUCTION_CACHE_LINE_PER_TAG	0x0000FF00
#define L1_INSTRUCTION_CACHE_LINE_SIZE		0x000000FF

//L2 Cache and TLB and L3 Cache Information CPUID = 0x80000006
//EAX
#define L2_INSTRUCTION_TLB_ASSOC			0x0000F000
#define L2_INSTRUCTION_TLB_SIZE				0x00000FFF
#define L2_DATA_TLB_SIZE					0x0FFF0000
#define L2_DATA_TLB_ASSOC					0xF0000000
//EBX
#define KB4_L2_INSTRUCTION_TLB_SIZE			0x00000FFF
#define KB4_L2_INSTRUCTION_TLB_ASSOC		0x0000F000
#define KB4_L2_DATA_TLB_SIZE				0x0FFF0000
#define KB4_L2_DATA_TLB_ASSOC				0xF0000000
//ECX
#define L2_DATA_CACHE_SIZE					0xFFFF0000
#define L2_DATA_CACHE_ASSOC					0x0000F000
#define L2_DATA_CACHE_LINE_PER_TAG			0x00000F00
#define L2_DATA_CACHE_LINE_SIZE				0x000000FF
//EDX
#define L3_INSTRUCTION_CACHE_SIZE			0xFFFC0000
#define L3_INSTRUCTION_CACHE_ASSOC			0x0000F000
#define L3_INSTRUCTION_CACHE_LINE_PER_TAG	0x00000F00
#define L3_INSTRUCTION_CACHE_LINE_SIZE		0x000000FF

//Intel MSR Resgister

#define IA32_THERM_STATUS_MSR				0x019C
#define IA32_TEMPERATURE_TARGET				0x01A2
#define IA32_PERF_STATUS					0x0198
#define MSR_PLATFORM_INFO					0xCE
#define IA32_PACKAGE_THERM_STATUS			0x1B1
#define MSR_RAPL_POWER_UNIT					0x606
#define MSR_PKG_ENERY_STATUS				0x611
#define MSR_DRAM_ENERGY_STATUS				0x619
#define MSR_PP0_ENERY_STATUS				0x639
#define MSR_PP1_ENERY_STATUS				0x641
/***********************说明********************

			Intel CPUID的查询结构

*************************************************/

/**

@brief CPUID的枚举信息

@CPU_MAIN_LEAF_NAME 查询的主叶号

@CPU_SUB_LEAF_NAME 查询的子叶号

@CPU_EAX 返回的EAX中信息

@CPU_EBX 返回的EBX中信息

@CPU_ECX 返回的ECX中信息

@CPU_EDX 返回的EDX中信息

*/
class CPU_RES
{
public:
	//CPU_RES();
	//CPU_RES(const CPU_RES&x);
	vector<string> CPU_LEAF_NAME;
	vector<int> CPU_EAX;
	vector<int> CPU_EBX;
	vector<int> CPU_ECX;
	vector<int> CPU_EDX;
public:
	void erase();
};
//CPU支持的各种功能
struct Intel_FEATURE_INFO
{
	bool SSE3;
	bool PCLMULQDQ;
	bool DTES64;
	bool MONITOR;
	bool DS_CPL;
	bool VMX;
	bool SMX;
	bool EIST;
	bool TM2;
	bool SSSE3;
	bool CNXT_ID;
	bool SDBG;
	bool FMA;
	bool CMPXCHG16B;
	bool xTPR_Update_Control;
	bool PDCM;
	bool PCID;
	bool DCA;
	bool SSE4_1;
	bool SSE4_2;
	bool x2APIC;
	bool MOVBE;
	bool POPCNT;
	bool TSC_Deadline;
	bool AESNI;
	bool XSAVE;
	bool OSXSAVE;
	bool AVX;
	bool F16C;
	bool RDRAND;

	bool FPU;
	bool VME;
	bool DE;
	bool PSE;
	bool TSC;
	bool MSR;
	bool PAE;
	bool MCE;
	bool CX8;
	bool APIC;
	bool SEP;
	bool MTRR;
	bool PGE;
	bool MCA;
	bool CMOV;
	bool PAT;
	bool PSE_36;
	bool PSN;
	bool CLFSH;
	bool DS;
	bool ACPI;
	bool MMX;
	bool FXSR;
	bool SSE;
	bool SSE2;
	bool SS;
	bool HTT;
	bool TM;
	bool PBE;
};
//Cache信息结构
struct Cache_info
{
	string Cache_type;
	UINT Cache_level;
	UINT Cache_Ways;
	UINT Cache_line_size;
	UINT Cache_partitions;
	UINT Cache_Set;
	UINT Cache_Size;
	ULONG Cache_max_addr_logical;
	ULONG Cache_max_addr_physical;
	bool CACHE_Invalidate;
	bool Cache_Inclusiveness;
	bool Cache_Complex_Indexing;
	void erase();
};
//CPU的电源管理所支持的功能
struct POEWR_MGMT_REGISTER
{
	bool Digital_tem_sensor;
	bool Turbo_boost;
	bool ARAT;
	bool PLN;
	bool ECMD;
	bool PTM;
	bool HWP;
	bool HWP_notification;
	bool HWP_Activity_Window;
	bool HWP_Energy_Performance;
	bool HWP_Package_Level_Request;
	bool HDC;
	bool Turbo_boost_3;
	bool Hardware_Coordination_Feedback;
	bool performance_energy_bias_preference;
	UINT Digital_thermal_sensor_num;
};
//CPU支持的扩展功能
struct Intel_EXTENED_FEARTURE_INFO
{
	bool FSGSBASE;
	bool IA32_TSC_ADJUST_MSR;
	bool SGX;
	bool BMI1;
	bool HLE;
	bool AVX2;
	bool FDP_EXCPTN_ONLY;
	bool SMEP;
	bool BMI2;
	bool Enhanced_REP_MOVSB_OR_STOSB;
	bool INVPCID;
	bool RTM;
	bool RDT_M;
	bool FPU_CS_DS;
	bool MPX;
	bool RDSEED;
	bool ADX;
	bool SMAP;
	bool CLFLUSHOPT;
	bool CLWB;
	bool Intel_Processor_Trace;
	bool SHA;

	bool PREFETCHWT1;
	bool UMIP;
	bool PKU;
	bool OSPKE;
	bool RDPID;
	bool SGX_LC;

	UINT MAWAU_Value;
};
//性能检测的架构
struct ARCHITECTURAL_PERFORMANCE_MONITORING
{
	UINT Version_ID;
	UINT Number_of_General_purpose;
	UINT BitWidth_of_General_purpose;
	UINT Length_of_EBX;

	bool Core_Cycles;
	bool Instruction_retired_event;
	bool Reference_cycles_event;
	bool Last_level_cache_reference_event;
	bool Last_level_cache_misses_event;
	bool Branch_instruction_retired_event;
	bool Branch_mispredict_retired_event;

	UINT Number_of_fixed_function;
	ULONG Bitwidth_of_fixed_function;
	UINT Any_thread_deprecation;
};
//枚举扩展拓扑
struct EXTENDED_TOPOLOGY_ENUMERATION 
{
	UINT Shift_X2APIC_ID; //得到独一无二的topology ID
	UINT Number_of_Logical_Processor;//at this level type
	string Level_Type;
	UINT X2APIC_ID;

	void erase();
};
//枚举CPU的扩展状态
struct PROCESSOR_EXTENDED_STATE_ENUMERATION
{
	bool XSAVEOPT;
	bool XSAVEC;
	bool XGETBV;
	bool XSAVES;

	bool X87_State;
	bool SSE_State;
	bool AVX_State;
	bool MPX_State;

	bool IA32_XSS;
	bool PKRU_State;
	UINT AVX_512_State;

	UINT MAX_XCR0;
	UINT MAX_XSAVE;
	UINT Upper_32_XCR0;



	UINT XSAVE_Area;
	UINT Lower_32_IA32_XSS_MSR;
	UINT Upper_32_IA32_XSS_MSR;
};

/************************************************************************/
/*                       AMD 与Intel不同的查询结构                       */
/************************************************************************/
//CPU支持的各种功能
struct AMD_FEATURE_INFO
{
	//ECX
	bool SSE3;
	bool PCLMULQDQ;
	bool MONITOR;
	bool SSSE3;
	bool FMA;
	bool CMPXCHG16B;
	bool SSE41;
	bool SSE42;
	bool POPCNT;
	bool AES;
	bool XSAVE;
	bool OSXSAVE;
	bool AVX;
	bool F16C;
	bool RDRAND;

	//EDX
	bool FPU;
	bool VME;
	bool DE;
	bool PSE;
	bool TSC;
	bool MSR;
	bool PAE;
	bool MCE;
	bool CMPXCHG8B;
	bool APIC;
	bool SysEnterSysExit;
	bool MTRR;
	bool PGE;
	bool MCA;
	bool CMOV;
	bool PAT;
	bool PSE36;
	bool CLFSH;
	bool MMX;
	bool FXSR;
	bool SSE;
	bool SSE2;
	bool HTT;

	//0x80000001时 ECX、EDX
	//ECX
	bool LahfSahf;
	bool CmpLegacy;
	bool SVM;
	bool ExtApicSpace;
	bool AltMovCr8;
	bool ABM;
	bool SSE4A;
	bool MisAlignSse;
	bool _3DNowPrefetch;
	bool OSVW;
	bool IBS;
	bool XOP;
	bool SKINIT;
	bool WDT;
	bool LWP;
	bool FMA4;
	bool TBM;
	bool TopologyExtensions;
	bool PerfCtrExtCore;
	bool PerfCtrExtNB;
	bool DataBreakpointExtension;
	bool PerfTsc;
	//EDX
	bool SysCallSysRet;
	bool NX;
	bool MmxExt;
	bool FFXSR;
	bool Page1GB;
	bool RDTSCP;
	bool LM;
	bool _3DNowExt;
	bool _3DNow;

};
//Cache信息结构
struct AMD_Cache_info {
	string Cache_type;
	UINT Cache_level;
	UINT Cache_Ways;
	UINT Cache_line_size;
	UINT Cache_line_per_tag;
	UINT Cache_Size;

	void erase();
};
//CPU支持的扩展功能
struct AMD_EXTENED_FEARTURE_INFO
{
	bool FSGSBASE;
	bool BMI1;
	bool AVX2;
	bool SMEP;
	bool BMI2;
};

/************************************************************************/
/*                       CPU解析后的信息结构 未完成……                     */
/************************************************************************/

class CPUID_Data {
public:
	UINT CurrentClockSpeed;
	UINT ExtClock;
	UINT Core;
	UINT Thread;
	UINT Revision;
	UINT MaxClockSpeed;
	UINT UpgradeMethod;
	USHORT Technology;
	USHORT MAXTDP;
	//PCHAR Socket_Packeage;

	//CPUID = 0
	char Manufacturer[0x20];

	//CPUID = 1
	UINT Family;
	UINT Model;
	UINT Stepping;
	UINT Ext_Family;
	UINT Ext_Model;

	union FEATURE
	{
		Intel_FEATURE_INFO Intel_Feature_info;
		AMD_FEATURE_INFO AMD_Feature_info;
	}Feature_info;


	//CPUID = 5
	bool EMX;
	bool IBE;
	UINT Largest_monitor_line_size;
	UINT Smallest_monitor_line_size;
	UINT C_STATE_info[7];	//Intel 特有

	//CPUID = 6
	POEWR_MGMT_REGISTER Power_Mgmt;
	bool EffFreq;

	//CPUID = 7
	union EXTENED_FEARTURE
	{
		Intel_EXTENED_FEARTURE_INFO Intel_Extened_Fearture;
		AMD_EXTENED_FEARTURE_INFO AMD_Extened_Fearture;
	}Extened_Feature;
			
	//CPUID = 0A
	ARCHITECTURAL_PERFORMANCE_MONITORING Architectural_performance_monitoring;

	//CPUID = 0B
	EXTENDED_TOPOLOGY_ENUMERATION Extended_topology_enumeration[4];

	//CPUID = 0D
	PROCESSOR_EXTENDED_STATE_ENUMERATION Processor_extended_state;
	wstring SocketDesignation;
	wstring Architecture;
	wstring Code_Name;
	wstring Name;
	wstring ProcessorID;
	wstring CPU_Type;
	//CPUID = 2
	vector<string> Cache_TLB;

	//CPUID = 4
	vector<Cache_info> Intel_cache;
	vector<AMD_Cache_info> AMD_cache;

public:
	void erase();
// 	CPUID_Data& operator=(const CPUID_Data& x);
// protected:
// 	void swap(const CPUID_Data& x);
};

//这是需要重复刷新数据的数据结构。