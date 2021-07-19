// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <cfgmgr32.h>
#include <tchar.h>
#include <intrin.h>

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "../libSmbios/LibSMBIOS.h"
#include "../libSmbios/LenovoDisplay.h"
#include "../../Libcommon/jsoncpp/include/json/json.h"
#include "../../Libcommon/tinyxml2/tinyxml2.h"
#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>

#include "Command.h"
#include "Interface.h"
#include "Utils.h"
#include "WindowsConfig.h"
