// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <windows.h>
#include <stdio.h>
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <Taskschd.h>
#include <tchar.h>
#include "Softwaredefination.h"
#include <iphlpapi.h>


// TODO: 在此处引用程序需要的其他头文件

#define TCP IPPROTO_TCP
#define UDP IPPROTO_UDP

void OutputDebugPrintf(const char* strOutputString, ...);
// TODO: 在此处引用程序需要的其他头文件
