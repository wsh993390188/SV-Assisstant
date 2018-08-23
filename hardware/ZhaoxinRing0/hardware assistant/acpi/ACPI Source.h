#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include "ACPI Defination.h"

using namespace std;

class UnCopyable
{
protected:
	UnCopyable() {}
	~UnCopyable() {}

private:
	explicit UnCopyable(const UnCopyable& x);
	UnCopyable& operator=(const UnCopyable& x);
};


class CACPI : private UnCopyable
{
public:
	CACPI();
	~CACPI();

	/****************************************************
	*@Function				Excute
	*@brief					计算ACPI Table值
	*@author				王硕
	*@param					无
		*@Table_Type		ACPI表头名
	*@return				无
	****************************************************/
	void Excute();	

private:
	/****************************************************
	*@Function				GetACPIAddress
	*@brief					获取ACPI表在内存中的地址
	*@author				王硕
	*@param
	*@other
		*@isSupportACPI		会设置该变量判断执行是否成功
	****************************************************/
	void GetACPIAddress();

	/****************************************************
	*@Function				GetXSDT_RSDT
	*@brief					获取ACPI表的RSDP RSDT XSDT
	*@author				王硕
	*@param
	*@other
		*@isSupportACPI		会设置该变量判断执行是否成功
	****************************************************/
	void GetXSDT_RSDT();


	void GetOtherTabels();
public:
	ACPI_RSDP_STRUCTURE						acpi_rsdp;
	ACPI_RSDT_STRUCTURE						acpi_rsdt;
	ACPI_XSDT_STRUCTURE						acpi_xsdt;
	ACPI_MCFG_STRUCTURE						acpi_mcfg;
	ACPI_FADT_STRUCTURE						acpi_facp;
	std::shared_ptr<ACPI_FPDT_STRUCTURE>	acpi_fpdt;
	std::shared_ptr<ACPI_FACS_STRUCTURE>	acpi_facs;
	ACPI_HPET_STRUCTURE						acpi_hpet;
	std::shared_ptr<ACPI_SBST_STRUCTURE>	acpi_sbst;
	std::shared_ptr<ACPI_BGRT_STRUCTURE>	acpi_bgrt;
	bool isSupportACPI;	//是否支持获取ACPI表
private:
	bool IsUEFI;
	size_t ACPIAddress;

};