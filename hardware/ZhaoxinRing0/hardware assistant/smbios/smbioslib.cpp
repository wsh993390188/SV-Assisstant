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
			static execSMBIOS* Instance() 
			{
				if (!sm.get())
					sm = std::make_shared<execSMBIOS>();
				return sm.get();
			}
			execSMBIOS() :data(std::make_shared<Smbios>()) {}
			void outputfile();
			const SMBIOS_Struct& GetSMBIOSinfo()
			{
				return data->GetSmbiosInfo();
			}
		protected:
		private:
			std::shared_ptr<Smbios> data;
			static std::shared_ptr<execSMBIOS> sm;
		};

		std::shared_ptr<execSMBIOS> execSMBIOS::sm = nullptr;

		void execSMBIOS::outputfile()
		{
			TCHAR   szCurDir[MAX_PATH] = {};
			if (GetModuleFileName(NULL, szCurDir, MAX_PATH) == 0) {

				printf("GetCurrentDirectory failed!  Error = %d \n", GetLastError());
				return;

			}
			(_tcsrchr(szCurDir, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
			std::wstring dir(szCurDir);
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
			std::ofstream outfile;
			outfile.open(dir + _T("\\smbios.txt"));
			outfile << "				Desktop Management Interface Information" << std::endl;
			auto smbios_temp = data->GetSmbiosInfo();
			if (!smbios_temp.BIOSinfo.empty())
			{
				outfile << "[BIOS Information]" << std::endl;
				for (const auto& i : smbios_temp.BIOSinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.Sysyteminfo.empty())
			{
				outfile << "[System Information]" << std::endl;
				for (const auto& i : smbios_temp.Sysyteminfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.BaseBroadinfo.empty())
			{
				outfile << "[Base Board Information]" << std::endl;
				for (const auto& i : smbios_temp.BaseBroadinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.SystemEorC.empty())
			{
				outfile << "[System Enclosure or Chassis Information]" << std::endl;
				for (const auto& i : smbios_temp.SystemEorC)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.Processorinfo.empty())
			{
				outfile << "[Processor Information]" << std::endl;
				for (const auto& i : smbios_temp.Processorinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.MemoryCtrlinfo.empty())
			{
				outfile << "[Memory Controller Information]" << std::endl;
				for (const auto& i : smbios_temp.MemoryCtrlinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.MemoryModinfo.empty())
			{
				outfile << "[Memory Module Information]" << std::endl;
				for (const auto& i : smbios_temp.MemoryModinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.Cacheinfo.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.Cacheinfo)
				{
					outfile << "[Cache Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.Portinfo.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.Portinfo)
				{
					outfile << "[Port Connector Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.SystemSlotinfo.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.SystemSlotinfo)
				{
					outfile << "[System Slots Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.BroadDevinfo.empty())
			{
				outfile << "[On Board Devices Information]" << std::endl;
				for (const auto& i : smbios_temp.BroadDevinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}


			if (!smbios_temp.OEMString.empty())
			{
				outfile << "[OEM Strings Information]" << std::endl;
				for (const auto& i : smbios_temp.OEMString)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}


			if (!smbios_temp.SystemConfigOption.empty())
			{
				outfile << "[System Configuration Information]" << std::endl;
				for (const auto& i : smbios_temp.SystemConfigOption)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}


			if (!smbios_temp.BIOSLanginfo.empty())
			{
				outfile << "[BIOS Language Information]" << std::endl;
				for (const auto& i : smbios_temp.BIOSLanginfo)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.GroupAssociations.empty())
			{
				outfile << "[Group Associations Information]" << std::endl;
				for (const auto& i : smbios_temp.GroupAssociations)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}


			if (!smbios_temp.SysEventLog.empty())
			{
				outfile << "[System Event Log Information]" << std::endl;
				for (const auto& i : smbios_temp.SysEventLog)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.PhysicalMemoryArray.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.PhysicalMemoryArray)
				{
					outfile << "[Physical Memory Array Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.MemoryDev.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.MemoryDev)
				{
					outfile << "[Memory Device Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.MemoryErrinfo.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.MemoryErrinfo)
				{
					outfile << "[32-Bit Memory Error Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.MemArrayMappedAddress.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.MemArrayMappedAddress)
				{
					outfile << "[Memory Array Mapped Address Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.MemDevMappedAddress.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.MemDevMappedAddress)
				{
					outfile << "[Memory Device Mapped Address Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.Pointing_Dev.empty())
			{
				outfile << "[Built-in Pointing Device Information]" << std::endl;
				for (const auto& i : smbios_temp.Pointing_Dev)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}


			if (!smbios_temp.Portable_Battery.empty())
			{
				outfile << "[Portable Battery Information]" << std::endl;
				for (const auto& i : smbios_temp.Portable_Battery)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}


			if (!smbios_temp.System_Reset.empty())
			{
				outfile << "[System Reset Information]" << std::endl;
				for (const auto& i : smbios_temp.System_Reset)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.Hardware_Security.empty())
			{
				outfile << "[Hardware Security Information]" << std::endl;
				for (const auto& i : smbios_temp.Hardware_Security)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}


			if (!smbios_temp.System_Power_Controls.empty())
			{
				outfile << "[System Power Controls Information]" << std::endl;
				for (const auto& i : smbios_temp.System_Power_Controls)
				{
					outfile << "	" << i.first << ":		" << i.second << std::endl;
				}
			}

			if (!smbios_temp.Voltage_Probe.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.Voltage_Probe)
				{
					outfile << "[Voltage Probe Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.CoolingDev.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.CoolingDev)
				{
					outfile << "[Cooling Device Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.TemperatureProbe.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.TemperatureProbe)
				{
					outfile << "[Temperature Probe Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.ElectricalCurrentProbe.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.ElectricalCurrentProbe)
				{
					outfile << "[Electrical Current Probe Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.BandRemoteAccess.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.BandRemoteAccess)
				{
					outfile << "[Out-of-Band Remote Access Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.Systembootstatus.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.Systembootstatus)
				{
					outfile << "[System Boot Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.MemoryError64Bit.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.MemoryError64Bit)
				{
					outfile << "[64-Bit Memory Error Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.ManagementDevice.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.ManagementDevice)
				{
					outfile << "[Management Device Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.ManagementDeviceComponent.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.ManagementDeviceComponent)
				{
					outfile << "[Management Device Component Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.ManagementDeviceComponentThresholdData.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.ManagementDeviceComponentThresholdData)
				{
					outfile << "[Management Device Threshold Data Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.MemoryChannel.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.MemoryChannel)
				{
					outfile << "[Memory Channel Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.IPMIDeviceinfo.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.IPMIDeviceinfo)
				{
					outfile << "[IPMI Device Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.SystemPowerSupply.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.SystemPowerSupply)
				{
					outfile << "[System Power Supply Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.Additionalinfo.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.Additionalinfo)
				{
					outfile << "[Additional Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.BroadDevExtendedinfo.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.BroadDevExtendedinfo)
				{
					outfile << "[On board Devices Extended Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.ManagementControllerHostInterface.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.ManagementControllerHostInterface)
				{
					outfile << "[Management Controller Host Interface Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.TPMDevice.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.TPMDevice)
				{
					outfile << "[TPM Device Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.Inactive.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.Inactive)
				{
					outfile << "[Inactive Information #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}

			if (!smbios_temp.EndofTable.empty())
			{
				int k = 1;
				for (const auto& i : smbios_temp.EndofTable)
				{
					outfile << "[End-of-Table #" << k++ << "]" << std::endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << std::endl;
					}
				}
			}


			outfile.close();
		}

		void OutputFile()
		{
			execSMBIOS::Instance()->outputfile();
		}

		const SMBIOS_Struct& GetSMBIOSinfo()
		{
			return execSMBIOS::Instance()->GetSMBIOSinfo();
		}
	}
}