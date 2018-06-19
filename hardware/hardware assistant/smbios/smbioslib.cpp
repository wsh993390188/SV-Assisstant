// smbiosdll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "Smbios.h"
#include "smbioslib.h"

namespace SV_ASSIST
{
	namespace DMI_SMBIOS
	{
		class execSMBIOS
		{
		public:
			static void outtofile();
		protected:
		private:
			const static Smbios sm;
		};

		const Smbios execSMBIOS::sm;

		void execSMBIOS::outtofile()
		{
			TCHAR   szCurDir[MAX_PATH];
			if (GetModuleFileName(NULL, szCurDir, MAX_PATH) == 0) {

				printf("GetCurrentDirectory failed!  Error = %d \n", GetLastError());
				return;

			}
			(_tcsrchr(szCurDir, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
			wstring dir(szCurDir);
			dir.append(L"\\HardWare\\DMI\\");
			if (_waccess(dir.c_str(), 0) == -1)
			{
				OutputDebugString(dir.c_str());
				OutputDebugString(L" is not existing;\n now make it");
				for (int i = 0; i < dir.size(); i++)
				{
					if (dir[i] == '\\')
					{
						dir[i] = '\0';
						if (_waccess(dir.c_str(), 0) != 0)
						{
							int flag = _wmkdir(dir.c_str());
							if (flag == 0)
							{
								OutputDebugString(L"make successfully");
							}
							else {
								OutputDebugString(L"make errorly");
								return;
							}
						}
						dir[i] = '\\';
					}
				}
			}
			ofstream outfile;
			outfile.open(dir + _T("\\smbios.txt"));
			outfile << "				Desktop Management Interface Information" << endl;
			if (!sm.BIOSinfo.empty())
			{
				outfile << "[BIOS Information]" << endl;
				for (const auto& i : sm.BIOSinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.Sysyteminfo.empty())
			{
				outfile << "[System Information]" << endl;
				for (const auto& i : sm.Sysyteminfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.BaseBroadinfo.empty())
			{
				outfile << "[Base Board Information]" << endl;
				for (const auto& i : sm.BaseBroadinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.SystemEorC.empty())
			{
				outfile << "[System Enclosure or Chassis Information]" << endl;
				for (const auto& i : sm.SystemEorC)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.Processorinfo.empty())
			{
				outfile << "[Processor Information]" << endl;
				for (const auto& i : sm.Processorinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.MemoryCtrlinfo.empty())
			{
				outfile << "[Memory Controller Information]" << endl;
				for (const auto& i : sm.MemoryCtrlinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.MemoryModinfo.empty())
			{
				outfile << "[Memory Module Information]" << endl;
				for (const auto& i : sm.MemoryModinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.Cacheinfo.empty())
			{
				int k = 1;
				for (const auto& i : sm.Cacheinfo)
				{
					outfile << "[Cache Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.Portinfo.empty())
			{
				int k = 1;
				for (const auto& i : sm.Portinfo)
				{
					outfile << "[Port Connector Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.SystemSlotinfo.empty())
			{
				int k = 1;
				for (const auto& i : sm.SystemSlotinfo)
				{
					outfile << "[System Slots Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.BroadDevinfo.empty())
			{
				outfile << "[On Board Devices Information]" << endl;
				for (const auto& i : sm.BroadDevinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!sm.OEMString.empty())
			{
				outfile << "[OEM Strings Information]" << endl;
				for (const auto& i : sm.OEMString)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!sm.SystemConfigOption.empty())
			{
				outfile << "[System Configuration Information]" << endl;
				for (const auto& i : sm.SystemConfigOption)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!sm.BIOSLanginfo.empty())
			{
				outfile << "[BIOS Language Information]" << endl;
				for (const auto& i : sm.BIOSLanginfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.GroupAssociations.empty())
			{
				outfile << "[Group Associations Information]" << endl;
				for (const auto& i : sm.GroupAssociations)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!sm.SysEventLog.empty())
			{
				outfile << "[System Event Log Information]" << endl;
				for (const auto& i : sm.SysEventLog)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.PhysicalMemoryArray.empty())
			{
				int k = 1;
				for (const auto& i : sm.PhysicalMemoryArray)
				{
					outfile << "[Physical Memory Array Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.MemoryDev.empty())
			{
				int k = 1;
				for (const auto& i : sm.MemoryDev)
				{
					outfile << "[Memory Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.MemoryErrinfo.empty())
			{
				int k = 1;
				for (const auto& i : sm.MemoryErrinfo)
				{
					outfile << "[32-Bit Memory Error Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.MemArrayMappedAddress.empty())
			{
				int k = 1;
				for (const auto& i : sm.MemArrayMappedAddress)
				{
					outfile << "[Memory Array Mapped Address Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.MemDevMappedAddress.empty())
			{
				int k = 1;
				for (const auto& i : sm.MemDevMappedAddress)
				{
					outfile << "[Memory Device Mapped Address Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.Pointing_Dev.empty())
			{
				outfile << "[Built-in Pointing Device Information]" << endl;
				for (const auto& i : sm.Pointing_Dev)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!sm.Portable_Battery.empty())
			{
				outfile << "[Portable Battery Information]" << endl;
				for (const auto& i : sm.Portable_Battery)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!sm.System_Reset.empty())
			{
				outfile << "[System Reset Information]" << endl;
				for (const auto& i : sm.System_Reset)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.Hardware_Security.empty())
			{
				outfile << "[Hardware Security Information]" << endl;
				for (const auto& i : sm.Hardware_Security)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!sm.System_Power_Controls.empty())
			{
				outfile << "[System Power Controls Information]" << endl;
				for (const auto& i : sm.System_Power_Controls)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!sm.Voltage_Probe.empty())
			{
				int k = 1;
				for (const auto& i : sm.Voltage_Probe)
				{
					outfile << "[Voltage Probe Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.CoolingDev.empty())
			{
				int k = 1;
				for (const auto& i : sm.CoolingDev)
				{
					outfile << "[Cooling Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.TemperatureProbe.empty())
			{
				int k = 1;
				for (const auto& i : sm.TemperatureProbe)
				{
					outfile << "[Temperature Probe Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.ElectricalCurrentProbe.empty())
			{
				int k = 1;
				for (const auto& i : sm.ElectricalCurrentProbe)
				{
					outfile << "[Electrical Current Probe Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.BandRemoteAccess.empty())
			{
				int k = 1;
				for (const auto& i : sm.BandRemoteAccess)
				{
					outfile << "[Out-of-Band Remote Access Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.Systembootstatus.empty())
			{
				int k = 1;
				for (const auto& i : sm.Systembootstatus)
				{
					outfile << "[System Boot Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.MemoryError64Bit.empty())
			{
				int k = 1;
				for (const auto& i : sm.MemoryError64Bit)
				{
					outfile << "[64-Bit Memory Error Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.ManagementDevice.empty())
			{
				int k = 1;
				for (const auto& i : sm.ManagementDevice)
				{
					outfile << "[Management Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.ManagementDeviceComponent.empty())
			{
				int k = 1;
				for (const auto& i : sm.ManagementDeviceComponent)
				{
					outfile << "[Management Device Component Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.ManagementDeviceComponentThresholdData.empty())
			{
				int k = 1;
				for (const auto& i : sm.ManagementDeviceComponentThresholdData)
				{
					outfile << "[Management Device Threshold Data Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.MemoryChannel.empty())
			{
				int k = 1;
				for (const auto& i : sm.MemoryChannel)
				{
					outfile << "[Memory Channel Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.IPMIDeviceinfo.empty())
			{
				int k = 1;
				for (const auto& i : sm.IPMIDeviceinfo)
				{
					outfile << "[IPMI Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.SystemPowerSupply.empty())
			{
				int k = 1;
				for (const auto& i : sm.SystemPowerSupply)
				{
					outfile << "[System Power Supply Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.Additionalinfo.empty())
			{
				int k = 1;
				for (const auto& i : sm.Additionalinfo)
				{
					outfile << "[Additional Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.BroadDevExtendedinfo.empty())
			{
				int k = 1;
				for (const auto& i : sm.BroadDevExtendedinfo)
				{
					outfile << "[On board Devices Extended Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.ManagementControllerHostInterface.empty())
			{
				int k = 1;
				for (const auto& i : sm.ManagementControllerHostInterface)
				{
					outfile << "[Management Controller Host Interface Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.TPMDevice.empty())
			{
				int k = 1;
				for (const auto& i : sm.TPMDevice)
				{
					outfile << "[TPM Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.Inactive.empty())
			{
				int k = 1;
				for (const auto& i : sm.Inactive)
				{
					outfile << "[Inactive Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!sm.EndofTable.empty())
			{
				int k = 1;
				for (const auto& i : sm.EndofTable)
				{
					outfile << "[End-of-Table #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}


			outfile.close();
		}

		void Updatesmbios()
		{
			execSMBIOS::outtofile();
		}
	}
}