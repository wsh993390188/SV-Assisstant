// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <windows.h>

// TODO:  在此处引用程序需要的其他头文件

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DRIVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DRIVERDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifdef DRIVERDLL_EXPORTS
#define DRIVERDLL_API __declspec(dllexport)
#else
#define DRIVERDLL_API __declspec(dllimport)
#endif

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <memory>
#include <tchar.h>
#include <vector>
#include <map>
#include <string>
#include <array>
#include <bitset>
#include "Defination\Ring0Defination.h"

void OutputDebugPrintf(const TCHAR* strOutputString, ...);





