#pragma once
#include "../MyDriverClass.h"
#include <fstream>

using namespace std;
//打开文件

/**************************************************************
*@Function			openpci
*@brief				打开文件获取PCI Vendor Device string信息
*@author			王硕
*@param
	*@filename			IN 要文件名打开的
	*@mode				打开得方式 ios::in 只读
	*@VendorID			IN 输入目标Vendor
	*@VendorString		OUT 输出VendorID的string类型
*@return
	*@BOOL				读取是否成功
****************************************************************/
BOOL openpci(const TCHAR * filename, ios_base::openmode mode, const USHORT VendorID, string& VendorString);