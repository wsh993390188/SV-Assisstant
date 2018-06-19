#pragma once
#include <PowrProf.h>
#include <initguid.h>

#pragma comment(lib, "PowrProf.lib")

/*************************************************
Copyright:Zhaoxin
Author:王硕
Date:2017-12-26
Description:更改电源管理选项、蓝屏是否重启、防火墙是否打开
**************************************************/
namespace SV_ASSIST
{
	namespace Software
	{
		namespace Origin
		{
			// 省电器	降低性能，可能会增加节能。	a1841308 - 3541 - 4fab - bc81 - f71556f20b4a
			// 均衡	根据需求自动平衡性能和功耗。	381b4222 - f694 - 41f0 - 9685 - ff5bb260df2e
			// 高性能	以更高的功耗为代价提供最高的性能。	8c5e7fda - e8bf - 4a96 - 9a85 - a6e23a8c635c

			DEFINE_GUID(BALANCEMODE, 0x381B4222, 0xF694, 0x41F0, 0x96, 0x85, 0xFF, 0x5B, 0xB2, 0x60, 0xDF, 0x2E);
			DEFINE_GUID(HIGHPERFORMANCEMODE, 0x8C5E7FDA, 0xE8BF, 0x4A96, 0x9A, 0x85, 0xA6, 0xE2, 0x3A, 0x8C, 0x63, 0x5C);
			DEFINE_GUID(POWERSAVEMODE, 0xA1841308, 0x3541, 0x4FAB, 0xBC, 0x81, 0xF7, 0x15, 0x56, 0xF2, 0x0B, 0x4A);

			typedef struct _SYSTEM_POWER_INFORMATION {
				ULONG MaxIdlenessAllowed;
				ULONG Idleness;
				ULONG TimeRemaining;
				UCHAR CoolingMode;
			} SYSTEM_POWER_INFORMATION, *PSYSTEM_POWER_INFORMATION;

			/*************************************************
			Function:       // PowerManagement
			Description:    // 将电源策略设置为高性能、不关闭屏幕
			Input:          // 无
			Output:         // 无
			Return:         // 是否成功
			Others:         // 无
			*************************************************/
			BOOL PowerManagement();

			/*************************************************
			Function:       // ChangeBlueCrash
			Description:    // 将蓝屏重启进行设置
			Input:          // 无
			AutoReboot	//是否自重启 TRUE：重启 FALSE：不重启 Others：错误值
			Output:         // 无
			Return:         // 是否成功
			Others:         // 无
			*************************************************/
			BOOL ChangeBlueCrash(BOOL AutoReboot);

			/*************************************************
			Function:       // FireWall
			Description:    // 将防火墙进行设置
			Input:          // 无
			AutoReboot	//是否开启防火墙 TRUE：开启 FALSE：不开启 Others：错误值
			Output:         // 无
			Return:         // 是否成功
			Others:         // 无
			*************************************************/
			BOOL FireWall(BOOL EnableFireWall);
		}
	}
}
