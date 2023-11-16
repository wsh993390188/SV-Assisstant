// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

// TODO:  在此处引用程序需要的其他头文件
#include <windows.h>
#include <stdint.h>

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>

#include "char8_t-remediation.h"

#include "StringSplit.hpp"
#include "Exception.hpp"
#include "SmbiosDefination.h"