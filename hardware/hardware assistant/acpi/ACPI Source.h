#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include "ACPI Defination.h"

using namespace std;

//ACPI 表的名字
typedef enum _ACPI_Head 
{
	RSDP = 'PDSR',
	XSDT = 'TDSX',
	RSDT = 'TDSR',
	FACP = 'PCAF',
	FADT = 'TDAF',
	APIC = 'CIPA',
	FPDT = 'TDPF',
	MCFG = 'GFCM',
	HPET = 'TEPH',
	BGRT = 'TRGB',
	SSDT = 'TDSS',
	FACS = 'SCAF',
	DSDT = 'TDSD'
}ACPI_Head;

//
typedef struct _ACPI_TABLE
{
	PUCHAR FireWareTable;
	size_t FireWareTableSize;
}ACPI_TABLE;

class UnCopyable
{
protected:
	UnCopyable() {}
	~UnCopyable() {}

private:
	UnCopyable(const UnCopyable& x);
	UnCopyable& operator=(const UnCopyable& x);
};


class CACPI : private UnCopyable
{
public:
	CACPI();
	virtual ~CACPI();
	/****************************************************
	*@Function				Get
	*@brief					获取ACPI Table值
	*@author				王硕
	*@param					无
	*@return				无
	****************************************************/
	void Get();

	/****************************************************
	*@Function				Excute
	*@brief					计算ACPI Table值
	*@author				王硕
	*@param					无
		*@Table_Type		ACPI表头名
	*@return				无
	****************************************************/
	void Excute(const DWORD Table_Type);	

private:
	/****************************************************
	*@Function				SafeDeleteArray
	*@brief					安全释放数组
	*@author				王硕
	*@param					无
		*@Array				数组
	*@return				无
	****************************************************/
	template<class T>
	void SafeDeleteArray(T*& Array);

	/****************************************************
	*@Function				SafeDeletePoint
	*@brief					安全释放指针
	*@author				王硕
	*@param					
		*@Point				指针
	*@return				无
	****************************************************/
	template<class T>
	void SafeDeletePoint(T*& Point);

	/****************************************************
	*@Function				Get_FirmWare_Enum
	*@brief					枚举ACPI表的数量以及表的名称 存在类中变量FirmwareTableID
	*@author				王硕
	*@param					无
	*@return				
		*@bool				执行是否成功
	****************************************************/
	bool Get_FirmWare_Enum();

	/****************************************************
	*@Function				PrintACPITable
	*@brief					打印表的名字
	*@author				王硕
	*@param					
		*@TableName			表的名字（数字形式）
	*@return
		*@bool				执行是否成功
	****************************************************/
	void PrintACPITable(DWORD TableName);

	/****************************************************
	*@Function				Get_ACPI_Table
	*@brief					获取ACPI表的数据 存放在类中FirewareTable中
	*@author				王硕
	*@param
		*@Table_Type		表的名字（数字形式）
		*@Num				存放数组的位置
	*@return
		*@BOOL				执行是否成功
	****************************************************/
	BOOL Get_ACPI_Table(const DWORD Table_Type, const size_t Num);
public:
	ACPI_FADT_STRUCTURE				facp;
	ACPI_HPET_STRUCTURE				hpet;
	ACPI_MCFG_STRUCTURE				mcfg;
	PACPI_FPDT_STRUCTURE			fpdt;
	ACPI_BGRT_STRUCTURE				bgrt;
	bool isSupportACPI;	//是否支持获取ACPI表
private:
	vector<DWORD> FirmwareTableID;	//可获取的ACPI表头的名称
	ACPI_TABLE FirewareTable[30];	//ACPI表的数据

};
