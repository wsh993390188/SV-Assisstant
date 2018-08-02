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
					sm = make_shared<execSMBIOS>();
				return sm.get();
			}
			execSMBIOS() :data(make_shared<Smbios>()) {}
			void outtofile();
			void Updatedata();
		protected:
		private:
			std::shared_ptr<Smbios> data;
			static std::shared_ptr<execSMBIOS> sm;
		};

		std::shared_ptr<execSMBIOS> execSMBIOS::sm = nullptr;

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
			if (!data->BIOSinfo.empty())
			{
				outfile << "[BIOS Information]" << endl;
				for (const auto& i : data->BIOSinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->Sysyteminfo.empty())
			{
				outfile << "[System Information]" << endl;
				for (const auto& i : data->Sysyteminfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->BaseBroadinfo.empty())
			{
				outfile << "[Base Board Information]" << endl;
				for (const auto& i : data->BaseBroadinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->SystemEorC.empty())
			{
				outfile << "[System Enclosure or Chassis Information]" << endl;
				for (const auto& i : data->SystemEorC)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->Processorinfo.empty())
			{
				outfile << "[Processor Information]" << endl;
				for (const auto& i : data->Processorinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->MemoryCtrlinfo.empty())
			{
				outfile << "[Memory Controller Information]" << endl;
				for (const auto& i : data->MemoryCtrlinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->MemoryModinfo.empty())
			{
				outfile << "[Memory Module Information]" << endl;
				for (const auto& i : data->MemoryModinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->Cacheinfo.empty())
			{
				int k = 1;
				for (const auto& i : data->Cacheinfo)
				{
					outfile << "[Cache Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->Portinfo.empty())
			{
				int k = 1;
				for (const auto& i : data->Portinfo)
				{
					outfile << "[Port Connector Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->SystemSlotinfo.empty())
			{
				int k = 1;
				for (const auto& i : data->SystemSlotinfo)
				{
					outfile << "[System Slots Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->BroadDevinfo.empty())
			{
				outfile << "[On Board Devices Information]" << endl;
				for (const auto& i : data->BroadDevinfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!data->OEMString.empty())
			{
				outfile << "[OEM Strings Information]" << endl;
				for (const auto& i : data->OEMString)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!data->SystemConfigOption.empty())
			{
				outfile << "[System Configuration Information]" << endl;
				for (const auto& i : data->SystemConfigOption)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!data->BIOSLanginfo.empty())
			{
				outfile << "[BIOS Language Information]" << endl;
				for (const auto& i : data->BIOSLanginfo)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->GroupAssociations.empty())
			{
				outfile << "[Group Associations Information]" << endl;
				for (const auto& i : data->GroupAssociations)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!data->SysEventLog.empty())
			{
				outfile << "[System Event Log Information]" << endl;
				for (const auto& i : data->SysEventLog)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->PhysicalMemoryArray.empty())
			{
				int k = 1;
				for (const auto& i : data->PhysicalMemoryArray)
				{
					outfile << "[Physical Memory Array Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->MemoryDev.empty())
			{
				int k = 1;
				for (const auto& i : data->MemoryDev)
				{
					outfile << "[Memory Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->MemoryErrinfo.empty())
			{
				int k = 1;
				for (const auto& i : data->MemoryErrinfo)
				{
					outfile << "[32-Bit Memory Error Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->MemArrayMappedAddress.empty())
			{
				int k = 1;
				for (const auto& i : data->MemArrayMappedAddress)
				{
					outfile << "[Memory Array Mapped Address Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->MemDevMappedAddress.empty())
			{
				int k = 1;
				for (const auto& i : data->MemDevMappedAddress)
				{
					outfile << "[Memory Device Mapped Address Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->Pointing_Dev.empty())
			{
				outfile << "[Built-in Pointing Device Information]" << endl;
				for (const auto& i : data->Pointing_Dev)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!data->Portable_Battery.empty())
			{
				outfile << "[Portable Battery Information]" << endl;
				for (const auto& i : data->Portable_Battery)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!data->System_Reset.empty())
			{
				outfile << "[System Reset Information]" << endl;
				for (const auto& i : data->System_Reset)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->Hardware_Security.empty())
			{
				outfile << "[Hardware Security Information]" << endl;
				for (const auto& i : data->Hardware_Security)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}


			if (!data->System_Power_Controls.empty())
			{
				outfile << "[System Power Controls Information]" << endl;
				for (const auto& i : data->System_Power_Controls)
				{
					outfile << "	" << i.first << ":		" << i.second << endl;
				}
			}

			if (!data->Voltage_Probe.empty())
			{
				int k = 1;
				for (const auto& i : data->Voltage_Probe)
				{
					outfile << "[Voltage Probe Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->CoolingDev.empty())
			{
				int k = 1;
				for (const auto& i : data->CoolingDev)
				{
					outfile << "[Cooling Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->TemperatureProbe.empty())
			{
				int k = 1;
				for (const auto& i : data->TemperatureProbe)
				{
					outfile << "[Temperature Probe Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->ElectricalCurrentProbe.empty())
			{
				int k = 1;
				for (const auto& i : data->ElectricalCurrentProbe)
				{
					outfile << "[Electrical Current Probe Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->BandRemoteAccess.empty())
			{
				int k = 1;
				for (const auto& i : data->BandRemoteAccess)
				{
					outfile << "[Out-of-Band Remote Access Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->Systembootstatus.empty())
			{
				int k = 1;
				for (const auto& i : data->Systembootstatus)
				{
					outfile << "[System Boot Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->MemoryError64Bit.empty())
			{
				int k = 1;
				for (const auto& i : data->MemoryError64Bit)
				{
					outfile << "[64-Bit Memory Error Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->ManagementDevice.empty())
			{
				int k = 1;
				for (const auto& i : data->ManagementDevice)
				{
					outfile << "[Management Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->ManagementDeviceComponent.empty())
			{
				int k = 1;
				for (const auto& i : data->ManagementDeviceComponent)
				{
					outfile << "[Management Device Component Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->ManagementDeviceComponentThresholdData.empty())
			{
				int k = 1;
				for (const auto& i : data->ManagementDeviceComponentThresholdData)
				{
					outfile << "[Management Device Threshold Data Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->MemoryChannel.empty())
			{
				int k = 1;
				for (const auto& i : data->MemoryChannel)
				{
					outfile << "[Memory Channel Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->IPMIDeviceinfo.empty())
			{
				int k = 1;
				for (const auto& i : data->IPMIDeviceinfo)
				{
					outfile << "[IPMI Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->SystemPowerSupply.empty())
			{
				int k = 1;
				for (const auto& i : data->SystemPowerSupply)
				{
					outfile << "[System Power Supply Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->Additionalinfo.empty())
			{
				int k = 1;
				for (const auto& i : data->Additionalinfo)
				{
					outfile << "[Additional Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->BroadDevExtendedinfo.empty())
			{
				int k = 1;
				for (const auto& i : data->BroadDevExtendedinfo)
				{
					outfile << "[On board Devices Extended Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->ManagementControllerHostInterface.empty())
			{
				int k = 1;
				for (const auto& i : data->ManagementControllerHostInterface)
				{
					outfile << "[Management Controller Host Interface Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->TPMDevice.empty())
			{
				int k = 1;
				for (const auto& i : data->TPMDevice)
				{
					outfile << "[TPM Device Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->Inactive.empty())
			{
				int k = 1;
				for (const auto& i : data->Inactive)
				{
					outfile << "[Inactive Information #" << k++ << "]" << endl;
					for (const auto&j : i)
					{
						outfile << "	" << j.first << ":		" << j.second << endl;
					}
				}
			}

			if (!data->EndofTable.empty())
			{
				int k = 1;
				for (const auto& i : data->EndofTable)
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

		void execSMBIOS::Updatedata()
		{
			data->UpdateData();
		}

		void Updatesmbios()
		{
			execSMBIOS::Instance()->Updatedata();
			execSMBIOS::Instance()->outtofile();
		}
	}
}