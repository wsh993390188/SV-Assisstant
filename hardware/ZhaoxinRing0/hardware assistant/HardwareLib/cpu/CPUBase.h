#pragma once
#include <intrin.h>
#include <windows.h>
#include <array>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include "defination.h"
#include "CPUWMI.h"
/**

*Copyright (C), 2017-.

*@file CPUBase.h，存储格式：UNICODE

*@brief 获取CPUID信息 基类

*@author 王硕

*@version 2.0

*@date 2018.9.21

*@History: 修改历史记录列表，每行修改记录应包括修改日期、修改者及修改内容简述

*<p>2017.9.21 1.0 王硕 CPUID中的解析定义</p>

*/

/**

@brief CPU的品牌

*/

//Instruction SIMD (x86) when CPUID = 1 
//EAX
#define CPUID_Stepping						0x0000000F
#define CPUID_Model							0x000000F0
#define CPUID_Family						0x00000F00
#define CPUID_Processor_Type				0x00003000
#define CPUID_Extended_Model				0x000F0000
#define CPUID_Extended_Family				0x0FF00000

enum CPU_Brands
{
	OTHERS_= 0,
	INTEL_ = 1,
	AMD_ = 2,
	ZHAOXIN_ = 3,
	FAILED_ = 0xFFFF
};

class CPUBASE
{
public:
	virtual void Init(void) = 0;
	virtual void UpDateData(void) = 0;
	virtual ~CPUBASE();
	static CPU_Brands GetCPUBrands();
	//CPUID解析后数据
	//0
	std::string Name;
	std::string Manufacturer;
	std::string microarchitecture;
	std::string Brand;
	unsigned int Technology;
	unsigned int MaxTDP;
	std::string SocketDesignation;

	//1
	short Family;
	short Model;
	short Stepping;
	short ExtFamily;
	short ExtModel;
	CPUFeature Feature;

	std::string ProcessorId;
	UINT ExtClock;
	UINT Core;
	UINT Thread;
	UINT Revision;
	UINT UpgradeMethod;
	//cache
	Cache_info Cache[4];

	//SouthBridge
	std::string SouthBridge;

	//Temperature
	std::vector<double> Temperature;
	double PackageTemperature;

	//Frequence
	double BusSpeed;
	std::vector<double> CurrentClockSpeed;
	std::vector<double> CoreVID;

	void GetCpuid(int (&cpuid)[4], const int eax, const int ecx);
protected:
	CPUBASE();
	size_t findcpuid(int value);
	std::vector<std::array<int, 4>> data__;
	std::vector<std::array<int, 4>> extdata__;
	std::vector<int> nIdsLeaf;	//0xFFFF|FFFF 前半段为main leaf（0x0001 Normal 0x8000 Ext） 后半段为sub leaf
	std::map<std::string, std::map<std::string, std::vector<std::string>>> CPUDB; //cpu数据库
	int nIds_;
	int nExIds_;
private:
	void InitCPUDB();
	void split(const std::string& s,
		const std::string& delim,
		std::vector<std::string>& ret);
	explicit CPUBASE(const CPUBASE& x);
	CPUBASE& operator=(const CPUBASE& x);
};
