// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容

// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <comutil.h>

#include <vector>

#include "../../Libcommon/jsoncpp/include/json/json.h"
#include "../../Libcommon/utfcpp/source/utf8.h"
#include "..\WSHardwarePluginV2\WSHardwarePluginV2.h"

std::string utf8_encode(const std::wstring& wstr);

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str);