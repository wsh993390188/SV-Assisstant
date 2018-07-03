#include "stdafx.h"
#include "Smbios.h"

Smbios::Smbios()
{
	this->GetSmbiosinfo();
}

Smbios::~Smbios()
{
}

void Smbios::UpdateData()
{
	this->BIOSinfo.clear();
	this->Sysyteminfo.clear();
	this->BaseBroadinfo.clear();
	this->SystemEorC.clear();
	this->Processorinfo.clear();
	this->MemoryCtrlinfo.clear();
	this->MemoryModinfo.clear();
	this->Cacheinfo.clear();
	this->Portinfo.clear();
	this->SystemSlotinfo.clear();
	this->BroadDevinfo.clear();
	this->OEMString.clear();
	this->SystemConfigOption.clear();
	this->BIOSLanginfo.clear();
	this->GroupAssociations.clear();
	this->SysEventLog.clear();
	this->PhysicalMemoryArray.clear();
	this->MemoryDev.clear();
	this->MemoryErrinfo.clear();
	this->MemArrayMappedAddress.clear();
	this->MemDevMappedAddress.clear();
	this->Pointing_Dev.clear();
	this->Portable_Battery.clear();
	this->System_Reset.clear();
	this->Hardware_Security.clear();
	this->System_Power_Controls.clear();
	this->Voltage_Probe.clear();
	this->CoolingDev.clear();
	this->TemperatureProbe.clear();
	this->ElectricalCurrentProbe.clear();
	this->BandRemoteAccess.clear();
	this->Systembootstatus.clear();
	this->MemoryError64Bit.clear();
	this->ManagementDevice.clear();
	this->ManagementDeviceComponent.clear();
	this->ManagementDeviceComponentThresholdData.clear();
	this->MemoryChannel.clear();
	this->IPMIDeviceinfo.clear();
	this->SystemPowerSupply.clear();
	this->Additionalinfo.clear();
	this->BroadDevExtendedinfo.clear();
	this->ManagementControllerHostInterface.clear();
	this->TPMDevice.clear();
	this->Inactive.clear();
	this->EndofTable.clear();
	this->GetSmbiosinfo();
}

BOOL Smbios::GetSmbiosinfo()
{
	PUCHAR SmbiosADDR = new UCHAR;
	PUCHAR SmbiosADDRtmp = SmbiosADDR;
	DWORD buffersize;
	UINT ret = 0;
	int first_flags = 1;	//第一次读Smbios Type 0 为0 是对的 之后读到的0都是错误的
	ret = GetSystemFirmwareTable('RSMB', 0, 0, 0);
	if (!ret)
	{
		printf("Function failed!\n");
		return 1;
	}
	buffersize = ret;
	PVOID buf = new char[buffersize];
	memset(buf, 0, ret);
	ret = GetSystemFirmwareTable('RSMB', 0, buf, buffersize);
	if (!ret)
	{
		printf("Function failed!\n");
		delete[]buf;
		return 1;
	}
	RawSMBIOSData* Smbios = (RawSMBIOSData *)buf;
	const string SmbiosVersion = to_string(Smbios->SMBIOSMajorVersion) + "." + to_string(Smbios->SMBIOSMinorVersion);
	SmbiosADDR = Smbios->SMBIOSTableData;
	//cout << "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" <<endl;
/*	for (size i = 0 ; i < buffersize; ++i)
	{

		cout << hex << setfill('0') << setw(2) << int(*(SmbiosADDR + i))<<" ";
		if ((i + 1) % 16 == 0)
		{
			//cout << i;
			cout <<endl;
		}
		//cout << *(p + i);
	}*/
	DMI_Header* dmi_head = NULL;
	PCHAR HextoString = new char[1024];
	PCHAR OriginHextoString = HextoString;
	for (size_t j = 0; j < Smbios->Length; ++j)
	{
		dmi_head = (DMI_Header*)SmbiosADDR;
			if (dmi_head->type == 0 && first_flags)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					BIOSinfo.insert(MapString::value_type(("BIOS Vendor"), dmi_to_string(dmi_head, SmbiosADDR[Type0_Vendor])));
					BIOSinfo.insert(MapString::value_type(("BIOS Version"), dmi_to_string(dmi_head, SmbiosADDR[Type0_BIOS_Version])));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type0_BIOS_Start_ADDR + (sizeof(WORD) - i - 1)))));
					}
					BIOSinfo.insert(MapString::value_type(("BIOS Start ADDR"), HextoString));
					BIOSinfo.insert(MapString::value_type(("BIOS Release Date"), dmi_to_string(dmi_head, SmbiosADDR[Type0_BIOS_Release_Date])));
					BIOSinfo.insert(MapString::value_type(("BIOS ROM Size"), to_string(64 * (WORD(*(SmbiosADDR + Type0_BIOS_ROM_SIZE)) + 1)) + ("KB")));
					for (size_t i = 0; i < sizeof(int64_t); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type0_BIOS_Characteristics + (sizeof(int64_t) - i - 1)))));
					}
					BIOSinfo.insert(MapString::value_type(("BIOS Characteristics"), HextoString));//可解析 未解析
					if (strcmp(("2.4"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type0_BIOS_Characteristics_EXtension + (sizeof(WORD) - i - 1)))));
						}
						BIOSinfo.insert(MapString::value_type(("BIOS Ext Characteristics"), HextoString));//可解析 未解析
						BIOSinfo.insert(MapString::value_type(("System BIOS Release"), to_string(*(SmbiosADDR + Type0_BIOS_Major_Release)) + (".") + to_string(*(SmbiosADDR + Type0_BIOS_Minor_Release))));
						BIOSinfo.insert(MapString::value_type(("EC Release"), to_string(*(SmbiosADDR + Type0_Firmware_Major_Release)) + (".") + to_string(*(SmbiosADDR + Type0_Firmware_Minor_Release))));
					}
					if (strcmp(("3.1"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type0_Extended_BIOS_ROM_SIZE + (sizeof(WORD) - i - 1)))));
						}
						BIOSinfo.insert(MapString::value_type(("Extended BIOS ROM Size"), HextoString));
					}
				}
				first_flags = 0;
			}

			if ((dmi_head->type == 1))
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					Sysyteminfo.insert(MapString::value_type(("System Manufacturer"), dmi_to_string(dmi_head, SmbiosADDR[Type1_Manufacturer])));
					Sysyteminfo.insert(MapString::value_type(("System Product Name"), dmi_to_string(dmi_head, SmbiosADDR[Type1_Product_Name])));
					Sysyteminfo.insert(MapString::value_type(("System Version"), dmi_to_string(dmi_head, SmbiosADDR[Type1_Version])));
					Sysyteminfo.insert(MapString::value_type(("System Serial Number"), dmi_to_string(dmi_head, SmbiosADDR[Type1_Serial_Number])));
					if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < 16; ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type1_UUID + i))));
						}
						Sysyteminfo.insert(MapString::value_type(("System UUID"), HextoString));
						switch (SmbiosADDR[Type1_Wake_up_Type])
						{
						case Wake_up_TYPE::Other:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("Other")));
							break;
						case Wake_up_TYPE::APM_Timer:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("APM Timer")));
							break;
						case Wake_up_TYPE::UUnknown:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("Unknown")));
							break;
						case Wake_up_TYPE::Modern_Ring:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("Modern Ring")));
						case Wake_up_TYPE::LAN_Remote:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("LAN Remote")));
							break;
						case Wake_up_TYPE::PCI_PME:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("PCI PME#")));
							break;
						case Wake_up_TYPE::Power_switch:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("Power Switch")));
							break;
						case Wake_up_TYPE::AC_Power_Restored:
							Sysyteminfo.insert(MapString::value_type(("System Wake-Up Type"), ("AC Power Restored")));
							break;
						default:
							break;
						}
					}
					if (strcmp(("2.4"), SmbiosVersion.c_str()) <= 0)
					{
						Sysyteminfo.insert(MapString::value_type(("System SKU Number"), dmi_to_string(dmi_head, SmbiosADDR[Type1_SKU_Number])));
						Sysyteminfo.insert(MapString::value_type(("System Family"), dmi_to_string(dmi_head, SmbiosADDR[Type1_Family])));
					}
				}
			}

			if (dmi_head->type == 2)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Manufacturer"), dmi_to_string(dmi_head, SmbiosADDR[Type2_Manufacturer])));
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Product"), dmi_to_string(dmi_head, SmbiosADDR[Type2_Product_Name])));
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Version"), dmi_to_string(dmi_head, SmbiosADDR[Type2_Version])));
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Serial Number"), dmi_to_string(dmi_head, SmbiosADDR[Type2_Serial_Number])));
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Asset Tag"), dmi_to_string(dmi_head, SmbiosADDR[Type2_Asset_Tag])));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type2_Feature_Flags))));
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Feature Flags"), HextoString));
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Location in Chassis"), dmi_to_string(dmi_head, SmbiosADDR[Type2_Location_in_Chassis])));
					switch (SmbiosADDR[Type2_Board_Type])
					{
					case BaseBoard_Type::BroadUnknown:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Unknown")));
						break;
					case BaseBoard_Type::BroadOther:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Other")));
						break;
					case BaseBoard_Type::Server_Blade:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Server Blade")));
						break;
					case BaseBoard_Type::Connectivity_Switch:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("ConnectivitySwitch")));
						break;
					case BaseBoard_Type::System_Management_Module:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("System Management Module")));
						break;
					case BaseBoard_Type::Processor_Module:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Processor Module")));
						break;
					case BaseBoard_Type::IO_Module:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("I/O Module")));
						break;
					case BaseBoard_Type::Memory_Module:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Memory Module")));
						break;
					case BaseBoard_Type::Daughter_board:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Daughter board")));
						break;
					case BaseBoard_Type::Motherboard:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Motherboard (includes processor, memory, and I/O)")));
						break;
					case BaseBoard_Type::Processor_Memory_Module:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Processor/Memory Module")));
						break;
					case BaseBoard_Type::Processor_IO_Module:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Processor/IO Module")));
						break;
					case BaseBoard_Type::Interconnect_board:
						BaseBroadinfo.insert(MapString::value_type(("BaseBroad Type"), ("Interconnect board")));
						break;
					default:
						break;
					}
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type2_Chassis_Handle + (sizeof(WORD) - i - 1)))));
					}
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Chassis Handle"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type2_Num_Obj_Handles))));
					BaseBroadinfo.insert(MapString::value_type(("BaseBroad Number of Contained Object Handles"), HextoString));
				}
			}

			if (dmi_head->type == 3)
			{
				static int abc = 0;
				++abc;
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Manufacturer"), dmi_to_string(dmi_head, SmbiosADDR[Type3_Manufacturer])));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type3_TypeE))));
					SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Type"), HextoString));
					if ((SmbiosADDR[Type3_TypeE] - 1) < 0x24)
					{
						SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Type Name"), System_Enclosure_or_Chassis_Types[SmbiosADDR[Type3_TypeE] - 1]));
					}
					SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Type"), HextoString));
					SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Version"), dmi_to_string(dmi_head, SmbiosADDR[Type3_Version])));
					SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Serial Number"), dmi_to_string(dmi_head, SmbiosADDR[Type3_Serial_Number])));
					SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Asset Tag"), dmi_to_string(dmi_head, SmbiosADDR[Type3_Asset_Tag])));
					if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
					{
						switch (SmbiosADDR[Type3_Boot_Up_State])
						{
						case SEC_States::SEC_States_Other:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Boot-up State"), ("Other")));
							break;
						case SEC_States::SEC_States_Unknown:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Boot-up State"), ("Unknown")));
							break;
						case SEC_States::SEC_Safe:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Boot-up State"), ("Safe")));
							break;
						case SEC_States::SEC_Warning:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Boot-up State"), ("Warning")));
							break;
						case SEC_States::SEC_Critical:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Boot-up State"), ("Critical")));
							break;
						case SEC_States::SEC_Non_recoverable:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Boot-up State"), ("Non-recoverable")));
							break;
						default:
							break;
						}
						switch (SmbiosADDR[Type3_Power_Supply_Status])
						{
						case SEC_States::SEC_States_Other:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Power Supply State"), ("Other")));
							break;
						case SEC_States::SEC_States_Unknown:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Power Supply State"), ("Unknown")));
							break;
						case SEC_States::SEC_Safe:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Power Supply State"), ("Safe")));
							break;
						case SEC_States::SEC_Warning:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Power Supply State"), ("Warning")));
							break;
						case SEC_States::SEC_Critical:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Power Supply State"), ("Critical")));
							break;
						case SEC_States::SEC_Non_recoverable:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Power Supply State"), ("Non-recoverable")));
							break;
						default:
							break;
						}
						switch (SmbiosADDR[Type3_Thermal_State])
						{
						case SEC_States::SEC_States_Other:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Thermal State"), ("Other")));
							break;
						case SEC_States::SEC_States_Unknown:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Thermal State"), ("Unknown")));
							break;
						case SEC_States::SEC_Safe:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Thermal State"), ("Safe")));
							break;
						case SEC_States::SEC_Warning:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Thermal State"), ("Warning")));
							break;
						case SEC_States::SEC_Critical:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Thermal State"), ("Critical")));
							break;
						case SEC_States::SEC_Non_recoverable:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Thermal State"), ("Non-recoverable")));
							break;
						default:
							break;
						}
						switch (SmbiosADDR[Type3_Security_Status])
						{
						case SEC_Security_States::SEC_Security_States_Other:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Security State"), ("Other")));
							break;
						case SEC_Security_States::SEC_Security_States_Unknown:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Security State"), ("Unknown")));
							break;
						case SEC_Security_States::SEC_States_None:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Security State"), ("None")));
							break;
						case SEC_Security_States::SEC_External_interface_locked_out:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Security State"), ("External interface locked out")));
							break;
						case SEC_Security_States::SEC_External_interface_enabled:
							SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Security State"), ("External interface enabled")));
							break;
						default:
							break;
						}
					}
					if (strcmp(("2.3"), SmbiosVersion.c_str()) <= 0)
					{
						size_t n = 0, m = 0;
						for (size_t i = 0; i < sizeof(DWORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type3_OEM_defined + (sizeof(DWORD) - i - 1)))));
						}
						SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis OEM-defined"), HextoString));
						sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type3_Height))));
						SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Height"), HextoString));
						sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type3_Num_of_Power_Cords))));
						SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Number of Power Cords"), HextoString));
						sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type3_Contained_Element_Count))));
						SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Contained Element Count(n)"), HextoString));
						sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type3_Contained_Element_Length))));
						SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis Contained Element Record Length(m)"), HextoString));
						n = (BYTE)SmbiosADDR[Type3_Contained_Element_Count];
						m = (BYTE)SmbiosADDR[Type3_Contained_Element_Length];
						if (n > 0 || m > 0)
						{
							for (size_t i = 0; i < n*m; ++i)
							{
								sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type3_Contained_Element + (n*m - i - 1)))));
							}
							SystemEorC.insert(MapString::value_type(("System Enclosure or Chassis Contained Elements"), HextoString));
						}
						else
						{
							sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type3_Contained_Element]);
							SystemEorC.insert(MapString::value_type(("System Enclosure or Chassis Contained Elements"), HextoString));
						}

						if (strcmp(("2.7"), SmbiosVersion.c_str()) <= 0)
						{
							if (n > 0 || m > 0)
							{
								SystemEorC.insert(MapString::value_type(("System Enclosure or  Chassis SKU Number"), dmi_to_string(dmi_head, SmbiosADDR[Type3_Contained_Element + n*m])));
							}
						}
					}
				}
			}

			if (dmi_head->type == 4)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					Processorinfo.insert(MapString::value_type(("Processor Information Socket Designation"), dmi_to_string(dmi_head, SmbiosADDR[Type4_Socket_Designation])));
					if ((SmbiosADDR[Type4_Processor_Type] - 1)< 0x06)
					{
						Processorinfo.insert(MapString::value_type(("Processor Information Type"), Processor_Type[SmbiosADDR[Type4_Processor_Type] - 1]));
					}
					if ((SmbiosADDR[Type4_Processor_Family] - 1) < sizeof(Processor_Family))
					{
						Processorinfo.insert(MapString::value_type(("Processor Information Family"), Processor_Family[SmbiosADDR[Type4_Processor_Family] - 1]));
					}
					Processorinfo.insert(MapString::value_type(("Processor Information Processor Manufacturer"), dmi_to_string(dmi_head, SmbiosADDR[Type4_Processor_Manufacturer])));
					for (size_t i = 0; i < sizeof(int64_t); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Processor_ID + (sizeof(int64_t) - i - 1)))));
					}
					Processorinfo.insert(MapString::value_type(("Processor Information Processor ID"), HextoString));
					Processorinfo.insert(MapString::value_type(("Processor Information Processor Version"), dmi_to_string(dmi_head, SmbiosADDR[Type4_Processor_Version])));
					if (SmbiosADDR[Type4_Voltage] & 0x80)
					{
						switch (SmbiosADDR[Type4_Voltage] & 0x0F)
						{
						case 0x00:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("Unknown")));
							break;
						case 0x01:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("3.3V")));
							break;
						case 0x02:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("2.9V")));
							break;
						case 0x03:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("8.3V")));
							break;
						case 0x04:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("2.9V")));
							break;
						case 0x05:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("7.9V")));
							break;
						case 0x06:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("6.2V")));
							break;
						case 0x07:
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), ("11.2V")));
							break;
						default:
							sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type4_Voltage]);
							Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), HextoString));
							break;
						}
					}
					else
					{
						double Voltage = (SmbiosADDR[Type4_Voltage] - 0x80) / 10;
						Processorinfo.insert(MapString::value_type(("Processor Information Voltage"), to_string(Voltage) + ("V")));
					}
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_External_Clock + (sizeof(WORD) - i - 1)))));
					}
					Processorinfo.insert(MapString::value_type(("Processor Information External Clock"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Max_Speed + (sizeof(WORD) - i - 1)))));
					}
					Processorinfo.insert(MapString::value_type(("Processor Information Max Speed"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Current_Speed + (sizeof(WORD) - i - 1)))));
					}
					Processorinfo.insert(MapString::value_type(("Processor Information Current Speed"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type4_Status]);
					Processorinfo.insert(MapString::value_type(("Processor Information Status"), HextoString));//可解析 未解析
					if ((SmbiosADDR[Type4_Processor_Upgrade] - 1) < sizeof(Processor_Socket))
					{
						Processorinfo.insert(MapString::value_type(("Processor Information Family"), Processor_Socket[SmbiosADDR[Type4_Processor_Upgrade] - 1]));
					}
					if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_L1_Cache_Handle + (sizeof(WORD) - i - 1)))));
						}
						Processorinfo.insert(MapString::value_type(("Processor Information L1 Cache Handle"), HextoString));
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_L2_Cache_Handle + (sizeof(WORD) - i - 1)))));
						}
						Processorinfo.insert(MapString::value_type(("Processor Information L2 Cache Handle"), HextoString));
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_L3_Cache_Handle + (sizeof(WORD) - i - 1)))));
						}
						Processorinfo.insert(MapString::value_type(("Processor Information L3 Cache Handle"), HextoString));
					}
					if (strcmp(("2.3"), SmbiosVersion.c_str()) <= 0)
					{
						Processorinfo.insert(MapString::value_type(("Processor Information Serial Number"), dmi_to_string(dmi_head, SmbiosADDR[Type4_Serial_Number])));
						Processorinfo.insert(MapString::value_type(("Processor Information Asset Tag"), dmi_to_string(dmi_head, SmbiosADDR[Type4_Asset_Tag])));
						Processorinfo.insert(MapString::value_type(("Processor Information Part Number"), dmi_to_string(dmi_head, SmbiosADDR[Type4_Part_Number])));
					}
					if (strcmp(("2.5"), SmbiosVersion.c_str()) <= 0)
					{
						sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Processor_Characteristics))));
						Processorinfo.insert(MapString::value_type(("Processor Information Processor Characteristics"), HextoString));//可解析 未解析 懒得解析
						if (strcmp(("3.0"), SmbiosVersion.c_str()) <= 0)
						{
							if (SmbiosADDR[Type4_Core_Count] < 0xFF)
							{
								if (SmbiosADDR[Type4_Core_Count] == 0)
								{
									Processorinfo.insert(MapString::value_type(("Processor Information Core Count"), ("Unknown")));
								}
								sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Count))));
								Processorinfo.insert(MapString::value_type(("Processor Information Core Count"), HextoString));
								sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Enable))));
								Processorinfo.insert(MapString::value_type(("Processor Information Core Enable"), HextoString));
								sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Thread_Count))));
								Processorinfo.insert(MapString::value_type(("Processor Information Thread Count"), HextoString));
							}
							else if (SmbiosADDR[Type4_Core_Count] == 0xFF)
							{
								if (SmbiosADDR[Type4_Core_Count2] > 0xFF)
								{
									sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Count2))));
									Processorinfo.insert(MapString::value_type(("Processor Information Core Count"), HextoString));
									sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Enable2))));
									Processorinfo.insert(MapString::value_type(("Processor Information Core Enable"), HextoString));
									sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Thread_Count2))));
									Processorinfo.insert(MapString::value_type(("Processor Information Thread Count"), HextoString));
								}
								else
								{
									sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Count))));
									Processorinfo.insert(MapString::value_type(("Processor Information Core Count"), HextoString));
									sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Enable))));
									Processorinfo.insert(MapString::value_type(("Processor Information Core Enable"), HextoString));
									sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Thread_Count))));
									Processorinfo.insert(MapString::value_type(("Processor Information Thread Count"), HextoString));
								}
							}
						}
						else
						{
							sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Count))));
							Processorinfo.insert(MapString::value_type(("Processor Information Core Count"), HextoString));
							sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Core_Enable))));
							Processorinfo.insert(MapString::value_type(("Processor Information Core Enable"), HextoString));
							sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type4_Thread_Count))));
							Processorinfo.insert(MapString::value_type(("Processor Information Thread Count"), HextoString));
						}
					}
					if (strcmp(("2.6"), SmbiosVersion.c_str()) <= 0)
					{
						if ((SmbiosADDR[Type4_Processor_NFamily2] - 1) < sizeof(Processor_Family))
						{
							Processorinfo.insert(MapString::value_type(("Processor Information Family2"), Processor_Family[SmbiosADDR[Type4_Processor_NFamily2] - 1]));
						}
					}
				}
			}

			if (dmi_head->type == 5)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					if ((SmbiosADDR[Type5_Error_Detecting_Method] - 1) < sizeof(Error_Detected_Method))
					{
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Detecting Method"), Error_Detected_Method[SmbiosADDR[Type5_Error_Detecting_Method] - 1]));
					}
					switch (SmbiosADDR[Type5_Error_COrrecting_Capability] & 0x3F)
					{
					case 0x01:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Correcting Capability"), Error_Correcting_Capability[0]));
						break;
					case 0x02:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Correcting Capability"), Error_Correcting_Capability[1]));
						break;
					case 0x04:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Correcting Capability"), Error_Correcting_Capability[2]));
						break;
					case 0x08:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Correcting Capability"), Error_Correcting_Capability[3]));
						break;
					case 0x10:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Correcting Capability"), Error_Correcting_Capability[4]));
						break;
					case 0x20:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Correcting Capability"), Error_Correcting_Capability[5]));
						break;
					default:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Correcting Capability"), Error_Correcting_Capability[6]));
						break;
					}
					if ((SmbiosADDR[Type5_Supported_Interleave] - 1) < sizeof(Interleave_Support_field))
					{
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Supported Interleave"), Interleave_Support_field[SmbiosADDR[Type5_Supported_Interleave] - 1]));
					}
					if ((SmbiosADDR[Type5_Current_Interleave] - 1) < sizeof(Interleave_Support_field))
					{
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Family Error Current Interleave"), Interleave_Support_field[SmbiosADDR[Type5_Current_Interleave] - 1]));
					}
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type5_Maximum_Memory_Moudule_size]);
					MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Maximum Memory Module Size"), HextoString));
					switch (SmbiosADDR[Type5_Error_COrrecting_Capability] & 0x1F)
					{
					case 0x01:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Supported Memory Speed"), Memory_Speed_field[0]));
						break;
					case 0x02:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Supported Memory Speed"), Memory_Speed_field[1]));
						break;
					case 0x04:
						break;
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Supported Memory Speed"), Memory_Speed_field[2]));
					case 0x08:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Supported Memory Speed"), Memory_Speed_field[3]));
						break;
					case 0x10:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Supported Memory Speed"), Memory_Speed_field[4]));
						break;
					default:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Supported Memory Speed"), Memory_Speed_field[5]));
						break;
					}
					switch ((SmbiosADDR[Type5_Supported_Memory_Types + 1] << 8) + SmbiosADDR[Type5_Supported_Memory_Types])
					{
					case 0x01:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[0]));
						break;
					case 0x02:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[1]));
						break;
					case 0x04:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[2]));
						break;
					case 0x08:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[3]));
						break;
					case 0x10:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[4]));
						break;
					case 0x20:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[5]));
						break;
					case 0x40:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[6]));
						break;
					case 0x80:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[7]));
						break;
					case 0x100:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[8]));
						break;
					case 0x200:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[9]));
						break;
					case 0x400:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[10]));
						break;
					default:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Type"), Memory_Types[11]));
						break;
					}
					switch (SmbiosADDR[Type5_Memory_Module_Voltage] & 0x0F)
					{
					case 0x01:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Module Voltage"), ("5V")));
						break;
					case 0x02:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Module Voltage"), ("3.3V")));
						break;
					case 0x04:
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Module Voltage"), ("2.9V")));
						break;
					default:
						sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type5_Memory_Module_Voltage]);
						MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Memory Module Voltage"), HextoString));
						break;
					}
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type5_Num_of_Memory_Slots]);
					MemoryCtrlinfo.insert(MapString::value_type(("Memory Controller Information Number of Associated Memory Slots"), HextoString));

					for (size_t i = 0; i < SmbiosADDR[Type5_Num_of_Memory_Slots] * sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type5_Num_of_Memory_Slots + 1 + (SmbiosADDR[Type5_Num_of_Memory_Slots] * sizeof(WORD) - i - 1)))));
					}
					Processorinfo.insert(MapString::value_type(("Processor Information External Clock"), HextoString));
				}
			}

			if (dmi_head->type == 6)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					MemoryModinfo.insert(MapString::value_type(("Memory Module Information Socket Designation"), dmi_to_string(dmi_head, SmbiosADDR[Type6_Socket_Designation])));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type6_Bank_Connections))));
					MemoryModinfo.insert(MapString::value_type(("Memory Module Information Bank Connections"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type6_Current_Speed))));
					MemoryModinfo.insert(MapString::value_type(("Memory Module Information Current Speed"), HextoString));
					switch ((SmbiosADDR[Type6_Current_Memory_Types + 1] << 8) + SmbiosADDR[Type6_Current_Memory_Types])
					{
					case 0x01:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[0]));
						break;
					case 0x02:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[1]));
						break;
					case 0x04:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[2]));
						break;
					case 0x08:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[3]));
						break;
					case 0x10:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[4]));
						break;
					case 0x20:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[5]));
						break;
					case 0x40:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[6]));
						break;
					case 0x80:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[7]));
						break;
					case 0x100:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[8]));
						break;
					case 0x200:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[9]));
						break;
					case 0x400:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[10]));
						break;
					default:
						MemoryModinfo.insert(MapString::value_type(("Memory Module Information Memory Type"), Memory_Types[11]));
						break;
					}
					/*	if (SmbiosADDR[Type6_Installed_Size])
					{
					string temp;
					if (SmbiosADDR[Type6_Installed_Size] == 0x7D)
					{
					HextoString = ("Not determinable (Installed Size only)");
					break;
					}
					if (SmbiosADDR[Type6_Installed_Size] == 0x7E)
					{
					HextoString = ("Module is installed, but no memory has been enabled");
					break;
					}
					if (SmbiosADDR[Type6_Installed_Size] == 0x7F)
					{
					HextoString = ("Not installed");
					break;
					}
					if (SmbiosADDR[Type6_Installed_Size] & 0x80)
					{
					temp = ("Double Bank ") + to_string((int)pow((int)2, SmbiosADDR[Type6_Installed_Size] & 0x7F)) + ("MB");
					strcpy_s(HextoString, sizeof(temp.c_str()) + 1, temp.c_str());
					}
					else
					{
					temp = ("Double Bank ") + to_string((int)pow((int)2, SmbiosADDR[Type6_Installed_Size] & 0x7F)) + ("MB");
					strcpy_s(HextoString, sizeof(temp.c_str()) + 1, temp.c_str());
					}
					}*/
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type6_Installed_Size]);
					MemoryModinfo.insert(MapString::value_type(("Memory Module Information Installed Size"), HextoString));
					/*if (SmbiosADDR[Type6_Enabled_Size])
					{
					string temp;
					if (SmbiosADDR[Type6_Enabled_Size] == 0x7D)
					{
					HextoString = ("Not determinable (Installed Size only)");
					break;
					}
					if (SmbiosADDR[Type6_Enabled_Size] == 0x7E)
					{
					HextoString = ("Module is installed, but no memory has been enabled");
					break;
					}
					if (SmbiosADDR[Type6_Enabled_Size] == 0x7F)
					{
					HextoString = ("Not installed");
					break;
					}
					if (SmbiosADDR[Type6_Enabled_Size] & 0x80)
					{
					temp = ("Double Bank ") + to_string((int)pow((int)2, SmbiosADDR[Type6_Enabled_Size] & 0x7F)) + ("MB");
					strcpy_s(HextoString, sizeof(temp.c_str()) + 1, temp.c_str());
					}
					else
					{
					temp = ("Double Bank ") + to_string((int)pow((int)2, SmbiosADDR[Type6_Enabled_Size] & 0x7F)) + ("MB");
					strcpy_s(HextoString, sizeof(temp.c_str()) + 1, temp.c_str());
					}
					}*/
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type6_Enabled_Size]);
					MemoryModinfo.insert(MapString::value_type(("Memory Module Information Enabled Size"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type6_Error_Status))));
					MemoryModinfo.insert(MapString::value_type(("Memory Module Information ERROR State"), HextoString));
				}
			}
			//没搞定7 一会再说
			if (dmi_head->type == 7)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type7String;
					Type7String.insert(MapString::value_type(("Cache Information Socket Designation"), dmi_to_string(dmi_head, SmbiosADDR[Type7_Socket_Designation])));
					//以下都是未解析的数据 懒的解析了
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type7_Cache_Configuration + (sizeof(WORD) - i - 1)))));
					}
					Type7String.insert(MapString::value_type(("Cache Information Cache Configuration"), HextoString));

					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type7_Maximum_Cache_Size + (sizeof(WORD) - i - 1)))));
					}
					Type7String.insert(MapString::value_type(("Cache Information Maximum Cache Size"), HextoString));

					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type7_Installed_Size + (sizeof(WORD) - i - 1)))));
					}
					Type7String.insert(MapString::value_type(("Cache Information Installed Size"), HextoString));
					string sram;
					for (size_t i = 0; i < SRAM_Type->length(); ++i)
					{

						if (*(WORD*)(SmbiosADDR + Type7_Supported_SRAM_Type) & (1 << i))
							sram += SRAM_Type[i];
					}
					Type7String.insert(MapString::value_type(("Cache Information SRAM Type"), sram));
					sram = ("");
					for (size_t i = 0; i < SRAM_Type->length(); ++i)
					{
						if (*(WORD*)(SmbiosADDR + Type7_Current_SRAM_Type) & (1 << i))
							sram += SRAM_Type[i];
					}
					Type7String.insert(MapString::value_type(("Cache Information Current SRAM Type"), sram));
					if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
					{
						sprintf_s(HextoString, sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type7_Cache_Speed))));
						Type7String.insert(MapString::value_type(("Cache Information Cache Speed"), HextoString));
						switch (SmbiosADDR[Type7_Error_Correction_Type])
						{
						case 0x01:
							Type7String.insert(MapString::value_type(("Cache Information Error Correction Type"), Error_Correction_Type[0]));
							break;
						case 0x02:
							Type7String.insert(MapString::value_type(("Cache Information Error Correction Type"), Error_Correction_Type[1]));
							break;
						case 0x03:
							Type7String.insert(MapString::value_type(("Cache Information Error Correction Type"), Error_Correction_Type[2]));
							break;
						case 0x04:
							Type7String.insert(MapString::value_type(("Cache Information Error Correction Type"), Error_Correction_Type[3]));
							break;
						case 0x05:
							Type7String.insert(MapString::value_type(("Cache Information Error Correction Type"), Error_Correction_Type[4]));
							break;
						case 0x06:
							Type7String.insert(MapString::value_type(("Cache Information Error Correction Type"), Error_Correction_Type[5]));
							break;
						default:
							Type7String.insert(MapString::value_type(("Cache Information Error Correction Type"), Error_Correction_Type[6]));
							break;
						}
						switch (SmbiosADDR[Type7_System_Cache_Type])
						{
						case 0x01:
							Type7String.insert(MapString::value_type(("Cache Information System Cache Type"), Error_Correction_Type[0]));
							break;
						case 0x02:
							Type7String.insert(MapString::value_type(("Cache Information System Cache Type"), Error_Correction_Type[1]));
							break;
						case 0x03:
							Type7String.insert(MapString::value_type(("Cache Information System Cache Type"), Error_Correction_Type[2]));
							break;
						case 0x04:
							Type7String.insert(MapString::value_type(("Cache Information System Cache Type"), Error_Correction_Type[3]));
							break;
						default:
							Type7String.insert(MapString::value_type(("Cache Information System Cache Type"), Error_Correction_Type[4]));
							break;
						}
							Type7String.insert(MapString::value_type(("Cache Information Associativity"), Cache_Associativity[SmbiosADDR[Type7_Associativity] - 1]));
					}
					if (strcmp(("3.1"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(DWORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type7_Maximum_Cache_Size2 + (sizeof(DWORD) - i - 1)))));
						}
						Type7String.insert(MapString::value_type(("Cache Information Maximum Cache Size2"), HextoString));

						for (size_t i = 0; i < sizeof(DWORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type7_Installed_Cache_Size2 + (sizeof(DWORD) - i - 1)))));
						}
						Type7String.insert(MapString::value_type(("Cache Information Maximum Installed Cache Size 2"), HextoString));
					}
					Cacheinfo.emplace_back(Type7String);
					Type7String.clear();
				}
			}

			if (dmi_head->type == 8)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type8Map;
					Type8Map.insert(MapString::value_type(("Port Connector Information Internal Reference Designator"), dmi_to_string(dmi_head, SmbiosADDR[Type8_Internal_Reference_Designator])));
					if (SmbiosADDR[Type8_Internal_Connector_Type] == 0xFF)
					{
						Type8Map.insert(MapString::value_type(("Port Connector Information Connector Types"), ("Other")));
					}
					else
					{
						Type8Map.insert(MapString::value_type(("Port Connector Information Connector Types"), Connector_Type[SmbiosADDR[Type8_Internal_Connector_Type]]));

					}
					if (SmbiosADDR[Type8_External_Connector_Type] == 0xFF)
					{
						Type8Map.insert(MapString::value_type(("Port Connector Information External Connector Types"), ("Other")));
					}
					else
					{
						Type8Map.insert(MapString::value_type(("Port Connector Information External Connector Types"), Connector_Type[SmbiosADDR[Type8_External_Connector_Type]]));
					}
					if (SmbiosADDR[Type8_Port_Type] == 0xFF)
					{
						Type8Map.insert(MapString::value_type(("Port Connector Information Port Type"), ("Other")));
					}
					else
					{
						Type8Map.insert(MapString::value_type(("Port Connector Information Port Type"), Port_Types[SmbiosADDR[Type8_Port_Type]]));
					}
					Type8Map.insert(MapString::value_type(("Port Connector Information External Reference Designator"), dmi_to_string(dmi_head, SmbiosADDR[Type8_External_Reference_Designator])));
					Portinfo.emplace_back(Type8Map);
					Type8Map.clear();
				}
			}

			if (dmi_head->type == 9)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type9String;
					Type9String.insert(MapString::value_type(("System Slots Slot Designation"), dmi_to_string(dmi_head, SmbiosADDR[Type9_Slot_Designation])));
					if (SmbiosADDR[Type9_Slot_Type] < 0x24)
					{
						Type9String.insert(MapString::value_type(("System Slots Slot Type"), System_Slots_Type_first[SmbiosADDR[Type9_Slot_Type] - 1]));
					}
					else if(SmbiosADDR[Type9_Slot_Type] >= 0xA0)
					{
						Type9String.insert(MapString::value_type(("System Slots Slot Type"), System_Slots_Type_second[SmbiosADDR[Type9_Slot_Type] - 0xA0]));
					}
					Type9String.insert(MapString::value_type(("System Slots Slot Data Bus Width"), System_Slot_Data_Bus_Width[SmbiosADDR[Type9_Slot_Data_Bus_Width] - 1]));
					Type9String.insert(MapString::value_type(("System Slots Current Usage"), System_Slot_Current_Usage[SmbiosADDR[Type9_Current_Usage] - 1]));
					Type9String.insert(MapString::value_type(("System Slots Slot Length"), System_Slot_Length[SmbiosADDR[Type9_Slot_Length] - 1]));

					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type9_Slot_ID + (sizeof(WORD) - i - 1)))));
					}
					Type9String.insert(MapString::value_type(("System Slots SlotID"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type9_Slot_Characteristics1]);
					Type9String.insert(MapString::value_type(("System Slots Slot Characteristics 1"), HextoString));
					if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
					{
						sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type9_Slot_Characteristics2]);
						Type9String.insert(MapString::value_type(("System Slots Slot Characteristics 2"), HextoString));
						if (strcmp(("2.6"), SmbiosVersion.c_str()) <= 0)
						{
							for (size_t i = 0; i < sizeof(WORD); ++i)
							{
								sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type9_Segment_Group_Num + (sizeof(WORD) - i - 1)))));
							}
							Type9String.insert(MapString::value_type(("System Slots Segment Group Number"), HextoString));
							sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type9_Bus_Number]);
							Type9String.insert(MapString::value_type(("System Slots Slot BUS Number"), HextoString));
							sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type9_Dev_Func_Num] & 0x07));
							Type9String.insert(MapString::value_type(("System Slots Slot Device Number"), HextoString));
							sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type9_Dev_Func_Num] & 0xF8));
							Type9String.insert(MapString::value_type(("System Slots Slot Function Number"), HextoString));
						}
					}
					SystemSlotinfo.emplace_back(Type9String);
					Type9String.clear();
				}
			}

			if (dmi_head->type == 10)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					for (WORD n = 1; n <= (dmi_head->length - 4) / 2; ++n)
					{
						BroadDevinfo.insert(MapString::value_type(("Board Devices ") + to_string(n) + (" Status"), (SmbiosADDR[4 + 2 * (n - 1)] & 0x80) ? ("Device Enabled") : ("Device Disabled")));
						BroadDevinfo.insert(MapString::value_type(("Board Devices ") + to_string(n) + (" Type"), Onboard_Device_Types[(SmbiosADDR[4 + 2 * (n - 1)] & 0x7F) - 1]));
						BroadDevinfo.insert(MapString::value_type(("Board Devices Information Description"), dmi_to_string(dmi_head, SmbiosADDR[5 + 2 * (n - 1)])));
					}
				}
			}

			if (dmi_head->type == 11)
			{
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type11_Count]);
				OEMString.insert(MapString::value_type(("OEM Strings Count"), HextoString));
				for (BYTE i = 1; i <= SmbiosADDR[Type11_Count]; ++i)
				{
					OEMString.insert(MapString::value_type(("OEM Strings ") + to_string(i), dmi_to_string(dmi_head, i)));
				}
			}

			if (dmi_head->type == 12)
			{
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type12_Count]);
				SystemConfigOption.insert(MapString::value_type(("System Configuration Options Count"), HextoString));
				for (BYTE i = 1; i <= SmbiosADDR[Type12_Count]; ++i)
				{
					SystemConfigOption.insert(MapString::value_type(("System Configuration Options ") + to_string(i), dmi_to_string(dmi_head, i)));
				}
			}

			if (dmi_head->type == 13)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type13_Installable_Languages]));
					BIOSLanginfo.insert(MapString::value_type(("BIOS Language Installable Languages"), HextoString));
					if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
					{
						BIOSLanginfo.insert(MapString::value_type(("BIOS Language Flags"), (SmbiosADDR[Type13_Flags] & 0x01) ? ("Abbreviated format") : ("Long format")));
						BIOSLanginfo.insert(MapString::value_type(("BIOS Language Current Language"), dmi_to_string(dmi_head, SmbiosADDR[Type13_Current_Languages])));
						if(SmbiosADDR[Type13_Installable_Languages] > 0)
						{
							for (BYTE i = 1; i <= SmbiosADDR[Type13_Installable_Languages]; ++i)
							{
								BIOSLanginfo.insert(MapString::value_type(("BIOS Language Installable Language ") + to_string(i), dmi_to_string(dmi_head, i)));
							}
						}
					}
				}
			}

			if (dmi_head->type == 14)
			{
				GroupAssociations.insert(MapString::value_type(("Group Associations Group Name"), dmi_to_string(dmi_head, SmbiosADDR[Type14_Group_Name])));
				for (WORD n = 1; n <= (dmi_head->length - 5) / 3; ++n)
				{
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type14_Item_Type + ((n - 1) * 3)]);
					SystemConfigOption.insert(MapString::value_type(("Group Associations Item Type") + to_string(n), HextoString));
					for (WORD m = 0; m < sizeof(WORD); ++m)
					{
						sprintf_s(HextoString + (m * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type14_Item_Handle + ((n - 1) * 3) + (sizeof(WORD) - m - 1)))));
						SystemConfigOption.insert(MapString::value_type(("Group Associations Item Handle") + to_string(n), HextoString));
					}
				}

			}

			if (dmi_head->type == 15)
			{
				if (strcmp(("2.0"), SmbiosVersion.c_str()) <= 0)
				{
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type15_Log_Area_Length + (sizeof(WORD) - i - 1)))));
					}
					SysEventLog.insert(MapString::value_type(("System Event Log Area Length"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type15_Log_Header_Start_Offset + (sizeof(WORD) - i - 1)))));
					}
					SysEventLog.insert(MapString::value_type(("System Event Log Header Start Offset"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type15_Log_Data_Start_Offset + (sizeof(WORD) - i - 1)))));
					}
					SysEventLog.insert(MapString::value_type(("System Event Log Data Start Offset"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type15_Access_Method]));
					SysEventLog.insert(MapString::value_type(("System Event Log Access Method"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type15_Log_Status]));
					SysEventLog.insert(MapString::value_type(("System Event Log Status"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type15_Log_Status]));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type15_Log_Change_Token + (sizeof(DWORD) - i - 1)))));
					}
					SysEventLog.insert(MapString::value_type(("System Event Log Change Token"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type15_Access_Method_Address + (sizeof(DWORD) - i - 1)))));
					}
					SysEventLog.insert(MapString::value_type(("System Event Log Access Method Address"), HextoString));
					if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
					{
						sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type15_Log_Header_Format]));
						SysEventLog.insert(MapString::value_type(("System Event Log Number of Supported Log Type Descriptors"), HextoString));
						sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type15_Num_of_Supported_Log_Types]));
						SysEventLog.insert(MapString::value_type(("System Event Log Number of Supported Log Type Descriptors"), HextoString));
						sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type15_Length_Log_Type]));
						SysEventLog.insert(MapString::value_type(("System Event Log Length of each Log Type Type Descriptors"), HextoString));
					}
				}
			}

			if (dmi_head->type == 16)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type16String;
					Type16String.insert(MapString::value_type(("Physical Memory Array Location"), MemArray_Location[SmbiosADDR[Type16_Location] - 1]));
					Type16String.insert(MapString::value_type(("Physical Memory Array Use"), MemArray_Use[SmbiosADDR[Type16_Use] - 1]));
					Type16String.insert(MapString::value_type(("Physical Memory Array Error Correction"), MemArray_Use[SmbiosADDR[Type16_Memory_Error_Correction] - 1]));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type16_Maximum_Capacity + (sizeof(DWORD) - i - 1)))));
					}
					Type16String.insert(MapString::value_type(("Physical Memory Array Maximum Capacity"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type16_Memory_Error_Information_Handle + (sizeof(WORD) - i - 1)))));
					}
					Type16String.insert(MapString::value_type(("Physical Memory Array Error Information Handle"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type16_Num_of_Memory_Devices + (sizeof(WORD) - i - 1)))));
					}
					Type16String.insert(MapString::value_type(("Physical Memory Array Number of Memory Devices"), HextoString));
					if (strcmp(("2.7"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(int64_t); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type16_Num_of_Memory_Devices + (sizeof(int64_t) - i - 1)))));
						}
						Type16String.insert(MapString::value_type(("Physical Memory Array Extended Maximum Capacity"), HextoString));
					}
					PhysicalMemoryArray.emplace_back(Type16String);
					Type16String.clear();
				}
			}

			if (dmi_head->type == 17)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type17String;
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Physical_Memory_Array_Handle + (sizeof(WORD) - i - 1)))));
					}
					Type17String.insert(MapString::value_type(("Memory Device Physical Memory Array Handle"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Memory_Error_Information_Handle + (sizeof(WORD) - i - 1)))));
					}
					Type17String.insert(MapString::value_type(("Memory Device Error Information Handle"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Memory_Total_Width + (sizeof(WORD) - i - 1)))));
					}
					Type17String.insert(MapString::value_type(("Memory Device Total Width"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Data_width + (sizeof(WORD) - i - 1)))));
					}
					Type17String.insert(MapString::value_type(("Memory Device Data Width"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Size + (sizeof(WORD) - i - 1)))));
					}
					Type17String.insert(MapString::value_type(("Memory Device Size"), HextoString));
					Type17String.insert(MapString::value_type(("Memory Device Form Factor"), FormFactor[SmbiosADDR[Type17_Form_Factor] - 1]));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type17_Device_Set]));
					Type17String.insert(MapString::value_type(("Memory Device Set"), HextoString));
					Type17String.insert(MapString::value_type(("Memory Device Locator"), dmi_to_string(dmi_head, SmbiosADDR[Type17_Device_Locator])));
					Type17String.insert(MapString::value_type(("Memory Bank Locator"), dmi_to_string(dmi_head, SmbiosADDR[Type17_Bank_Locator])));
					Type17String.insert(MapString::value_type(("Memory Device Type"), MemDeviceType[SmbiosADDR[Type17_Memory_Type] - 1]));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Type_Detail + (sizeof(WORD) - i - 1)))));
					}
					Type17String.insert(MapString::value_type(("Memory Device Type Detail"), HextoString));
					if (strcmp(("2.3"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Speed + (sizeof(WORD) - i - 1)))));
						}
						Type17String.insert(MapString::value_type(("Memory Device Speed"), HextoString));
						Type17String.insert(MapString::value_type(("Memory Device Manufacturer"), dmi_to_string(dmi_head, SmbiosADDR[Type17_Manufacturer])));
						Type17String.insert(MapString::value_type(("Memory Device Serial Number"), dmi_to_string(dmi_head, SmbiosADDR[Type17_Serial_Number])));
						Type17String.insert(MapString::value_type(("Memory Device Asset Tag"), dmi_to_string(dmi_head, SmbiosADDR[Type17_Asset_Tag])));
						Type17String.insert(MapString::value_type(("Memory Device Part Number"), dmi_to_string(dmi_head, SmbiosADDR[Type17_Part_Number])));
					}
					if (strcmp(("2.6"), SmbiosVersion.c_str()) <= 0)
					{
						sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type17_Attributes]));
						Type17String.insert(MapString::value_type(("Memory Device Attributes"), HextoString));
					}
					if (strcmp(("2.7"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(DWORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Extented_Size + (sizeof(DWORD) - i - 1)))));
						}
						Type17String.insert(MapString::value_type(("Memory Device Extended Size"), HextoString));
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Configured_Memory_Clock_Speed + (sizeof(WORD) - i - 1)))));
						}
						Type17String.insert(MapString::value_type(("Memory Device Configured Memory Clock Speed"), HextoString));
						if (strcmp(("2.8"), SmbiosVersion.c_str()) <= 0)
						{
							for (size_t i = 0; i < sizeof(WORD); ++i)
							{
								sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Minimum_Voltage + (sizeof(WORD) - i - 1)))));
							}
							Type17String.insert(MapString::value_type(("Memory Device Minimum Voltage"), HextoString));
							for (size_t i = 0; i < sizeof(WORD); ++i)
							{
								sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Maximum_Voltage + (sizeof(WORD) - i - 1)))));
							}
							Type17String.insert(MapString::value_type(("Memory Device Maximum Voltage"), HextoString));
							for (size_t i = 0; i < sizeof(WORD); ++i)
							{
								sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type17_Configured_Voltage + (sizeof(WORD) - i - 1)))));
							}
							Type17String.insert(MapString::value_type(("Memory Device Configured Voltage"), HextoString));
						}
					}
					MemoryDev.emplace_back(Type17String);
					Type17String.clear();
				}
			}

			if (dmi_head->type == 18)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type18String;
					Type18String.insert(MapString::value_type(("Memory Error Type"), Memory_Error_Type[SmbiosADDR[Type18_Error_Type] - 1]));
					Type18String.insert(MapString::value_type(("Memory Error Granularity"), Memory_Error_Granularity[SmbiosADDR[Type18_Error_Granularity] - 1]));
					Type18String.insert(MapString::value_type(("Memory Error Operation"), Memory_Error_Operation[SmbiosADDR[Type18_Error_Operation] - 1]));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type18_Vendor_Syndrome + (sizeof(DWORD) - i - 1)))));
					}
					Type18String.insert(MapString::value_type(("Memory Error Vendor Syndrome"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type18_Memory_Array_Error_Address + (sizeof(DWORD) - i - 1)))));
					}
					Type18String.insert(MapString::value_type(("Memory Error Memory Array Error Address"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type18_Device_Error_Address + (sizeof(DWORD) - i - 1)))));
					}
					Type18String.insert(MapString::value_type(("Memory Error Device Error Address"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type18_Error_Resolution + (sizeof(DWORD) - i - 1)))));
					}
					Type18String.insert(MapString::value_type(("Memory Error Error Resolution"), HextoString));
					MemoryErrinfo.emplace_back(Type18String);
					Type18String.clear();
				}
			}

			if (dmi_head->type == 19)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type19String;
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type19_Starting_Address + (sizeof(DWORD) - i - 1)))));
					}
					Type19String.insert(MapString::value_type(("Memory Array Mapped Address Starting Address"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type19_Ending_Address + (sizeof(DWORD) - i - 1)))));
					}
					Type19String.insert(MapString::value_type(("Memory Array Mapped Address Ending Address"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type19_Memory_Array_Handle + (sizeof(WORD) - i - 1)))));
					}
					Type19String.insert(MapString::value_type(("Memory Array Mapped Address Memory Array Handle"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type19_Partition_Width]);
					Type19String.insert(MapString::value_type(("Memory Array Mapped Address Partition Width"), HextoString));
					if (strcmp(("2.7"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(int64_t); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type19_Extented_Starting_Address + (sizeof(int64_t) - i - 1)))));
						}
						Type19String.insert(MapString::value_type(("Memory Array Mapped Address Extended Starting Address"), HextoString));
						for (size_t i = 0; i < sizeof(int64_t); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type19_Extented_Ending_Address + (sizeof(int64_t) - i - 1)))));
						}
						Type19String.insert(MapString::value_type(("Memory Array Mapped Address Extended Ending Address"), HextoString));
					}
					MemArrayMappedAddress.emplace_back(Type19String);
					Type19String.clear();
				}
			}

			if (dmi_head->type == 20)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type20String;
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type20_Starting_Address + (sizeof(DWORD) - i - 1)))));
					}
					Type20String.insert(MapString::value_type(("Memory Device Mapped Address Starting Address"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type20_Ending_Address + (sizeof(DWORD) - i - 1)))));
					}
					Type20String.insert(MapString::value_type(("Memory Device Mapped Address Ending Address"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type20_Memory_Device_Handle + (sizeof(WORD) - i - 1)))));
					}
					Type20String.insert(MapString::value_type(("Memory Device Mapped Address Memory Device Handle"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type20_Memory_Array_Mapped_Address_Handle + (sizeof(WORD) - i - 1)))));
					}
					Type20String.insert(MapString::value_type(("Memory Device Mapped Address Memory Array Mapped Handle"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type20_Partition_Row__Position]);
					Type20String.insert(MapString::value_type(("Memory Device Mapped Address Partition Row Width"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type20_Interleave_Position]);
					Type20String.insert(MapString::value_type(("Memory Device Mapped Address Interleave Position"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type20_Interleave_Data_Depth]);
					Type20String.insert(MapString::value_type(("Memory Device Mapped Address Interleaved Data Depth"), HextoString));
					if (strcmp(("2.7"), SmbiosVersion.c_str()) <= 0)
					{
						for (size_t i = 0; i < sizeof(int64_t); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type20_Extented_Starting_Address + (sizeof(int64_t) - i - 1)))));
						}
						Type20String.insert(MapString::value_type(("Memory Device Mapped Address Extended Starting Address"), HextoString));
						for (size_t i = 0; i < sizeof(int64_t); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type20_Extented_Ending_Address + (sizeof(int64_t) - i - 1)))));
						}
						Type20String.insert(MapString::value_type(("Memory Device Mapped Address Extended Ending Address"), HextoString));
					}
					MemDevMappedAddress.emplace_back(Type20String);
					Type20String.clear();
				}
			}

			if (dmi_head->type == 21)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					
					Pointing_Dev.insert(MapString::value_type(("Built-in Pointing Device Type"), Pointing_Device_Type[SmbiosADDR[Type21_Types] - 1]));
					Pointing_Dev.insert(MapString::value_type(("Built-in Pointing Device Interface"), Pointing_Device_Interface[SmbiosADDR[Type21_Interface] - 1]));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type21_Num_of_Buttons]);
					Pointing_Dev.insert(MapString::value_type(("Built-in Pointing Device Number of Buttons"), HextoString));
				}
			}

			if (dmi_head->type == 22)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					Portable_Battery.insert(MapString::value_type(("Portable Battery Location"), dmi_to_string(dmi_head, SmbiosADDR[Type22_Location])));
					Portable_Battery.insert(MapString::value_type(("Portable Battery Manufacturer"), dmi_to_string(dmi_head, SmbiosADDR[Type22_Manufacturer])));
					Portable_Battery.insert(MapString::value_type(("Portable Battery Serial Number"), dmi_to_string(dmi_head, SmbiosADDR[Type22_Serial_Number])));
					Portable_Battery.insert(MapString::value_type(("Portable Battery Manufacturer Date"), dmi_to_string(dmi_head, SmbiosADDR[Type22_Manufacturer_Date])));
					Portable_Battery.insert(MapString::value_type(("Portable Battery Device Name"), dmi_to_string(dmi_head, SmbiosADDR[Type22_Device_Name])));
					Portable_Battery.insert(MapString::value_type(("Portable Battery Device Name"), Portable_Battery_Device_Chemistry[SmbiosADDR[Type22_Device_Chemistry] - 1]));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type22_Design_Capacity + (sizeof(WORD) - i - 1)))));
					}
					Portable_Battery.insert(MapString::value_type(("Portable Battery Design Capacity"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type22_Design_Voltage + (sizeof(WORD) - i - 1)))));
					}
					Portable_Battery.insert(MapString::value_type(("Portable Battery Design Voltage"), HextoString));
					Portable_Battery.insert(MapString::value_type(("Portable Battery SBDS Version Number"), dmi_to_string(dmi_head, SmbiosADDR[Type22_SBDS_Version_Number])));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type22_Maximum_Error_In_Battery_Data]);
					Portable_Battery.insert(MapString::value_type(("Portable Battery Maximum Error In Battery Data"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type22_SDBS_Serial_Number + (sizeof(WORD) - i - 1)))));
					}
					Portable_Battery.insert(MapString::value_type(("Portable Battery SBDS Serial Number"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type22_SDBS_Manufacturer_Date + (sizeof(WORD) - i - 1)))));
					}
					Portable_Battery.insert(MapString::value_type(("Portable Battery SBDS Manufacture Date"), HextoString));
					Portable_Battery.insert(MapString::value_type(("Portable Battery SBDS Device Chemistry"), dmi_to_string(dmi_head, SmbiosADDR[Type22_SDBS_Device_Chemistry])));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type22_Design_Capacity_Multiplier]);
					Portable_Battery.insert(MapString::value_type(("Portable Battery Design Capacity Capacity"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type22_OEM_specific + (sizeof(DWORD) - i - 1)))));
					}
					Portable_Battery.insert(MapString::value_type(("Portable Battery OEM-specific"), HextoString));
				}
			}

			if (dmi_head->type == 23)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type23_Capabilities]);
					System_Reset.insert(MapString::value_type(("System Reset Capabilities"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type23_Reset_Count + (sizeof(WORD) - i - 1)))));
					}
					System_Reset.insert(MapString::value_type(("System Reset Count"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type23_Reset_Limit + (sizeof(WORD) - i - 1)))));
					}
					System_Reset.insert(MapString::value_type(("System Reset Limit"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type23_Timer_Interval + (sizeof(WORD) - i - 1)))));
					}
					System_Reset.insert(MapString::value_type(("System Reset Timer Interval"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type23_Timeout + (sizeof(WORD) - i - 1)))));
					}
					System_Reset.insert(MapString::value_type(("System Reset Timeout"), HextoString));
				}
			}

			if (dmi_head->type == 24)
			{
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type24_Hardware_Security_Settings]);
				Hardware_Security.insert(MapString::value_type(("Hardware Security Settings"), HextoString));
			}

			if (dmi_head->type == 25)
			{
				if (strcmp(("2.1"), SmbiosVersion.c_str()) <= 0)
				{
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type25_Next_Scheduled_Power_on_Month]);
					System_Power_Controls.insert(MapString::value_type(("System Power Controls Next Scheduled Power - on Month"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type25_Next_Scheduled_Power_on_Day]);
					System_Power_Controls.insert(MapString::value_type(("System Power Controls Next Scheduled Power - on Day-of-month"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type25_Next_Scheduled_Power_on_Hour]);
					System_Power_Controls.insert(MapString::value_type(("System Power Controls Next Scheduled Power - on Hour"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type25_Next_Scheduled_Power_on_Minute]);
					System_Power_Controls.insert(MapString::value_type(("System Power Controls Next Scheduled Power - on Minute"), HextoString));
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type25_Next_Scheduled_Power_on_Second]);
					System_Power_Controls.insert(MapString::value_type(("System Power Controls Next Scheduled Power - on Second"), HextoString));
				}
			}

			if (dmi_head->type == 26)
			{
				MapString Type26String;
				Type26String.insert(MapString::value_type(("Voltage Probe Description"), dmi_to_string(dmi_head, SmbiosADDR[Type26_Description])));
				switch (SmbiosADDR[Type26_Location_and_Status] & 0x1F)
				{
				case 0x01:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Other")));
					break;
				case 0x02:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Unknown")));
					break;
				case 0x03:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Processor")));
					break;
				case 0x04:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Disk")));
					break;
				case 0x05:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Peripheral Bay")));
					break;
				case 0x06:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("System Management Module")));
					break;
				case 0x07:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Motherboard")));
					break;
				case 0x08:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Memory Module")));
					break;
				case 0x09:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Processor Module")));
					break;
				case 0x0A:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Power Unit")));
					break;
				case 0x0B:
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), ("Add-in Card")));
					break;
				default:
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type26_Location_and_Status]);
					Type26String.insert(MapString::value_type(("Voltage Probe Location"), HextoString));
					break;
				}
				switch ((SmbiosADDR[Type26_Location_and_Status] & 0xE0) >> 5)
				{
				case 0x01:
					Type26String.insert(MapString::value_type(("Voltage Probe Status"), ("Other")));
					break;
				case 0x02:
					Type26String.insert(MapString::value_type(("Voltage Probe Status"), ("Unknown")));
					break;
				case 0x03:
					Type26String.insert(MapString::value_type(("Voltage Probe Status"), ("OK")));
					break;
				case 0x04:
					Type26String.insert(MapString::value_type(("Voltage Probe Status"), ("Non-critical")));
					break;
				case 0x05:
					Type26String.insert(MapString::value_type(("Voltage Probe Status"), ("Critical")));
					break;
				case 0x06:
					Type26String.insert(MapString::value_type(("Voltage Probe Status"), ("Non-recoverable")));
					break;
				default:
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type26_Location_and_Status]);
					Type26String.insert(MapString::value_type(("Voltage Probe Status"), HextoString));
					break;
				}

				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type26_Maximum_Value + (sizeof(WORD) - i - 1)))));
				}
				Type26String.insert(MapString::value_type(("Voltage Probe Maximum Value"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type26_Minimum_Value + (sizeof(WORD) - i - 1)))));
				}
				Type26String.insert(MapString::value_type(("Voltage Probe Minimum Value"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type26_Resolution + (sizeof(WORD) - i - 1)))));
				}
				Type26String.insert(MapString::value_type(("Voltage Probe Resolution"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type26_Tolerance + (sizeof(WORD) - i - 1)))));
				}
				Type26String.insert(MapString::value_type(("Voltage Probe Tolerance"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type26_Accuracy + (sizeof(WORD) - i - 1)))));
				}
				Type26String.insert(MapString::value_type(("Voltage Probe Accuracy"), HextoString));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type26_OEM_defined + (sizeof(DWORD) - i - 1)))));
				}
				Type26String.insert(MapString::value_type(("Voltage Probe OEM-defined"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type26_Nominal_Value + (sizeof(WORD) - i - 1)))));
				}
				Type26String.insert(MapString::value_type(("Voltage Probe Nominal Value"), HextoString));
				Voltage_Probe.emplace_back(Type26String);
				Type26String.clear();
			}

			if (dmi_head->type == 27)
			{
				if (strcmp(("2.2"), SmbiosVersion.c_str()) <= 0)
				{
					MapString Type27String;
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type27_Temperature_Probe_Handle + (sizeof(WORD) - i - 1)))));
					}
					Type27String.insert(MapString::value_type(("Cooling Device Temperature Probe Handle"), HextoString));
					switch (SmbiosADDR[Type27_Device_Type_and_Status] & 0x1F)
					{
					case 0x01:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Other")));
						break;
					case 0x02:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Unknown")));
						break;
					case 0x03:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Fan")));
						break;
					case 0x04:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Centrifugal Blower")));
						break;
					case 0x05:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Chip Fan")));
						break;
					case 0x06:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Cabinet Fan")));
						break;
					case 0x07:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Power Supply Fan")));
						break;
					case 0x08:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Heat Pipe")));
						break;
					case 0x09:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Integrated Refrigeration")));
						break;
					case 0x10:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Active Cooling")));
						break;
					case 0x11:
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), ("Passive Cooling")));
						break;
					default:
						sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type27_Device_Type_and_Status]);
						Type27String.insert(MapString::value_type(("Cooling Device Device Type"), HextoString));
						break;
					}
					switch ((SmbiosADDR[Type27_Device_Type_and_Status] & 0xE0) >> 5)
					{
					case 0x01:
						Type27String.insert(MapString::value_type(("Cooling Device Status"), ("Other")));
						break;
					case 0x02:
						Type27String.insert(MapString::value_type(("Cooling Device Status"), ("Unknown")));
						break;
					case 0x03:
						Type27String.insert(MapString::value_type(("Cooling Device Status"), ("OK")));
						break;
					case 0x04:
						Type27String.insert(MapString::value_type(("Cooling Device Status"), ("Non-critical")));
						break;
					case 0x05:
						Type27String.insert(MapString::value_type(("Cooling Device Status"), ("Critical")));
						break;
					case 0x06:
						Type27String.insert(MapString::value_type(("Cooling Device Status"), ("Non-recoverable")));
						break;
					default:
						sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type27_Device_Type_and_Status]);
						Type27String.insert(MapString::value_type(("Cooling Device Status"),HextoString));
						break;
					}
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type27_Cooling_Unit_Group]);
					Type27String.insert(MapString::value_type(("Cooling Device Cooling Unit Group"), HextoString));
					for (size_t i = 0; i < sizeof(DWORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type27_OEM_defined + (sizeof(DWORD) - i - 1)))));
					}
					Type27String.insert(MapString::value_type(("Cooling Device OEM-defined"), HextoString));
					for (size_t i = 0; i < sizeof(WORD); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type27_Nominal_Speed + (sizeof(WORD) - i - 1)))));
					}
					Type27String.insert(MapString::value_type(("Cooling Device Nominal Speed"), HextoString));
					if (strcmp(("2.7"), SmbiosVersion.c_str()) <= 0)
					{
						Type27String.insert(MapString::value_type(("Cooling Device Description"), dmi_to_string(dmi_head, SmbiosADDR[Type27_Description])));
					}
					CoolingDev.emplace_back(Type27String);
					Type27String.clear();
				}
			}

			if (dmi_head->type == 28)
			{
				MapString Type28String;
				Type28String.insert(MapString::value_type(("Temperature Probe Description"), dmi_to_string(dmi_head, SmbiosADDR[Type28_Description])));
				switch (SmbiosADDR[Type28_Location_and_Status] & 0x1F)
				{
				case 0x01:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Other")));
					break;
				case 0x02:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Unknown")));
					break;
				case 0x03:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Processor")));
					break;
				case 0x04:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Disk")));
					break;
				case 0x05:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Peripheral Bay")));
					break;
				case 0x06:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("System Management Module")));
					break;
				case 0x07:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Motherboard")));
					break;
				case 0x08:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Memory Module")));
					break;
				case 0x09:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Processor Module")));
					break;
				case 0x0A:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Power Unit")));
					break;
				case 0x0B:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Add-in Card")));
					break;
				case 0x0C:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Front Panel Board")));
					break;
				case 0x0D:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Back Panel Board")));
				case 0x0E:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Power System Board")));
					break;
				case 0x0F:
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), ("Drive Back Plane")));
					break;
				default:
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type28_Location_and_Status]);
					Type28String.insert(MapString::value_type(("Temperature Probe Location"), HextoString));
					break;
				}
				switch ((SmbiosADDR[Type28_Location_and_Status] & 0xE0) >> 5)
				{
				case 0x01:
					Type28String.insert(MapString::value_type(("Temperature Probe Status"), ("Other")));
					break;
				case 0x02:
					Type28String.insert(MapString::value_type(("Temperature Probe Status"), ("Unknown")));
					break;
				case 0x03:
					Type28String.insert(MapString::value_type(("Temperature Probe Status"), ("OK")));
					break;
				case 0x04:
					Type28String.insert(MapString::value_type(("Temperature Probe Status"), ("Non-critical")));
					break;
				case 0x05:
					Type28String.insert(MapString::value_type(("Temperature Probe Status"), ("Critical")));
					break;
				case 0x06:
					Type28String.insert(MapString::value_type(("Temperature Probe Status"), ("Non-recoverable")));
					break;
				default:
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type28_Location_and_Status]);
					Type28String.insert(MapString::value_type(("Temperature Probe Status"),HextoString));
					break;
				}
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type28_Maximum_Value + (sizeof(WORD) - i - 1)))));
				}
				Type28String.insert(MapString::value_type(("Temperature Probe Maximum Value"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type28_Minimum_Value + (sizeof(WORD) - i - 1)))));
				}
				Type28String.insert(MapString::value_type(("Temperature Probe Minimum Value"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type28_Resolution + (sizeof(WORD) - i - 1)))));
				}
				Type28String.insert(MapString::value_type(("Temperature Probe Resolution"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type28_Tolerance + (sizeof(WORD) - i - 1)))));
				}
				Type28String.insert(MapString::value_type(("Temperature Probe Tolerance"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type28_Accuracy + (sizeof(WORD) - i - 1)))));
				}
				Type28String.insert(MapString::value_type(("Temperature Probe Accuracy"), HextoString));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type28_OEM_defined + (sizeof(DWORD) - i - 1)))));
				}
				Type28String.insert(MapString::value_type(("Temperature Probe OEM-defined"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type28_Nominal_Value + (sizeof(WORD) - i - 1)))));
				}
				Type28String.insert(MapString::value_type(("Temperature Probe Nominal Value"), HextoString));
				TemperatureProbe.emplace_back(Type28String);
				Type28String.clear();

			}

			if (dmi_head->type == 29)
			{
				MapString Type29String;
				Type29String.insert(MapString::value_type(("Electrical Current Probe Description"), dmi_to_string(dmi_head, SmbiosADDR[Type29_Description])));
				switch (SmbiosADDR[Type29_Location_and_Status] & 0x1F)
				{
				case 0x01:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Other")));
					break;
				case 0x02:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Unknown")));
					break;
				case 0x03:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Processor")));
					break;
				case 0x04:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Disk")));
					break;
				case 0x05:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Peripheral Bay")));
					break;
				case 0x06:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("System Management Module")));
					break;
				case 0x07:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Motherboard")));
					break;
				case 0x08:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Memory Module")));
					break;
				case 0x09:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Processor Module")));
					break;
				case 0x0A:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Power Unit")));
					break;
				case 0x0B:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"), ("Add-in Card")));
					break;
				default:
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type29_Location_and_Status]);
					Type29String.insert(MapString::value_type(("Electrical Current Probe Location"),HextoString));
					break;
				}
				switch ((SmbiosADDR[Type29_Location_and_Status] & 0xE0) >> 5)
				{
				case 0x01:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Status"), ("Other")));
					break;
				case 0x02:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Status"), ("Unknown")));
					break;
				case 0x03:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Status"), ("OK")));
					break;
				case 0x04:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Status"), ("Non-critical")));
					break;
				case 0x05:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Status"), ("Critical")));
					break;
				case 0x06:
					Type29String.insert(MapString::value_type(("Electrical Current Probe Status"), ("Non-recoverable")));
					break;
				default:
					sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type29_Location_and_Status]);
					Type29String.insert(MapString::value_type(("Electrical Current Probe Status"), HextoString));
					break;
				}
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type29_Maximum_Value + (sizeof(WORD) - i - 1)))));
				}
				Type29String.insert(MapString::value_type(("Electrical Current Probe Maximum Value"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type29_Minimum_Value + (sizeof(WORD) - i - 1)))));
				}
				Type29String.insert(MapString::value_type(("Electrical Current Probe Minimum Value"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type29_Resolution + (sizeof(WORD) - i - 1)))));
				}
				Type29String.insert(MapString::value_type(("Electrical Current Probe Resolution"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type29_Tolerance + (sizeof(WORD) - i - 1)))));
				}
				Type29String.insert(MapString::value_type(("Electrical Current Probe Tolerance"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type29_Accuracy + (sizeof(WORD) - i - 1)))));
				}
				Type29String.insert(MapString::value_type(("Electrical Current Probe Accuracy"), HextoString));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type29_OEM_defined + (sizeof(DWORD) - i - 1)))));
				}
				Type29String.insert(MapString::value_type(("Electrical Current Probe OEM-defined"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type29_Nominal_Value + (sizeof(WORD) - i - 1)))));
				}
				Type29String.insert(MapString::value_type(("Electrical Current Probe Nominal Value"), HextoString));
				ElectricalCurrentProbe.emplace_back(Type29String);
				Type29String.clear();

			}

			if (dmi_head->type == 30)
			{
				MapString Type30String;
				Type30String.insert(MapString::value_type(("Out-of-Band Remote Access Manufacturer Name"), dmi_to_string(dmi_head, SmbiosADDR[Type30_Manufacturer_Name])));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type30_Connections]);
				Type30String.insert(MapString::value_type(("Out-of-Band Remote Access Connections"), HextoString));
				BandRemoteAccess.emplace_back(Type30String);
				Type30String.clear();
			}

			if (dmi_head->type == 32)
			{
				MapString Type32String;
				if (dmi_head->length > 10)
				{
					for (size_t i = 0; i < (size_t)(dmi_head->length - 10); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type32_Boot_Status + (dmi_head->length - 10 - i - 1)))));
					}
					Type32String.insert(MapString::value_type(("System Boot Status"), HextoString));
				}
				Systembootstatus.emplace_back(Type32String);
				Type32String.clear();
			}

			if (dmi_head->type == 33)
			{
				MapString Type33String;
				if (dmi_head->length > 10)
				{
					for (size_t i = 0; i < (size_t)(dmi_head->length - 10); ++i)
					{
						sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type32_Boot_Status + (dmi_head->length - 10 - i - 1)))));
					}
					Type33String.insert(MapString::value_type(("64-Bit Memory Error Information Error Type"), Memory_Error_Type[SmbiosADDR[Type33_Error_Type] - 1]));
				}
				Type33String.insert(MapString::value_type(("64-Bit Memory Error Information Error Granularity"), Memory_Error_Granularity[SmbiosADDR[Type33_Error_Granularity] - 1]));
				Type33String.insert(MapString::value_type(("64-Bit Memory Error Information Error Operation"), Memory_Error_Operation[SmbiosADDR[Type33_Error_Operation] - 1]));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type33_Vendor_Syndrome + (sizeof(DWORD) - i - 1)))));
				}
				Type33String.insert(MapString::value_type(("64-Bit Memory Error Information Vendor Syndrome"), HextoString));
				for (size_t i = 0; i < sizeof(int64_t); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type33_Memory_Array_Error_Address + (sizeof(int64_t) - i - 1)))));
				}
				Type33String.insert(MapString::value_type(("64-Bit Memory Error Information Memory Array Error Address"), HextoString));
				for (size_t i = 0; i < sizeof(int64_t); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type33_Device_Error_Address + (sizeof(int64_t) - i - 1)))));
				}
				Type33String.insert(MapString::value_type(("64-Bit Memory Error Information Device Error Address"), HextoString));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type33_Error_Resolution + (sizeof(DWORD) - i - 1)))));
				}
				Type33String.insert(MapString::value_type(("64-Bit Memory Error Information Error Resolution"), HextoString));
				MemoryError64Bit.emplace_back(Type33String);
				Type33String.clear();
			}

			if (dmi_head->type == 34)
			{
				MapString Type34String;
				Type34String.insert(MapString::value_type(("Management Device Description"), dmi_to_string(dmi_head, SmbiosADDR[Type34_Description])));
				Type34String.insert(MapString::value_type(("Management Device Type"), Management_Device_Type[SmbiosADDR[Type34_Types] - 1]));
				Type34String.insert(MapString::value_type(("Management Device Address Type"), Management_Device_Address_Type[SmbiosADDR[Type34_Address_Type] - 1]));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type34_Address + (sizeof(DWORD) - i - 1)))));
				}
				Type34String.insert(MapString::value_type(("Management Device Address"), HextoString));
				ManagementDevice.emplace_back(Type34String);
				Type34String.clear();
			}

			if (dmi_head->type == 35)
			{
				MapString Type35String;
				Type35String.insert(MapString::value_type(("Management Device Component Description"), dmi_to_string(dmi_head, SmbiosADDR[Type35_Description])));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type35_Management_Device_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type35String.insert(MapString::value_type(("Management Device Component Management Device Handle"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type35_Component_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type35String.insert(MapString::value_type(("Management Device Component Component Handle"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type35_Threshold_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type35String.insert(MapString::value_type(("Management Device Component Threshold Handle"), HextoString));
				ManagementDeviceComponent.emplace_back(Type35String);
				Type35String.clear();
			}

			if (dmi_head->type == 36)
			{
				MapString Type36String;
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type36_Lower_Threshold_Noncritical + (sizeof(WORD) - i - 1)))));
				}
				Type36String.insert(MapString::value_type(("Management Device Threshold Data Lower Threshold Non-critical"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type36_Upper_Threshold_Noncritical + (sizeof(WORD) - i - 1)))));
				}
				Type36String.insert(MapString::value_type(("Management Device Threshold Data Upper Threshold  Non-critical"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type36_Lower_Threshold_Critical + (sizeof(WORD) - i - 1)))));
				}
				Type36String.insert(MapString::value_type(("Management Device Threshold Data Lower Threshold Critical"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type36_Upper_Threshold_Critical + (sizeof(WORD) - i - 1)))));
				}
				Type36String.insert(MapString::value_type(("Management Device Threshold Data Upper Threshold Critical"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type36_Lower_Threshold_Nonrecoverable + (sizeof(WORD) - i - 1)))));
				}
				Type36String.insert(MapString::value_type(("Management Device Threshold Data Lower Threshold Non-recoverable"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type36_Upper_Threshold_Nonrecoverable + (sizeof(WORD) - i - 1)))));
				}
				Type36String.insert(MapString::value_type(("Management Device Threshold Data Upper Threshold Non-recoverable"), HextoString));
				ManagementDeviceComponentThresholdData.emplace_back(Type36String);
				Type36String.clear();
			}

			if (dmi_head->type == 37)
			{
				MapString Type37String;
				switch (SmbiosADDR[Type37_Channel_Type])
				{
				case 0x01:
					Type37String.insert(MapString::value_type(("Memory Channel Type"), ("Other")));
					break;
				case 0x02:
					Type37String.insert(MapString::value_type(("Memory Channel Type"), ("Unknown")));
					break;
				case 0x03:
					Type37String.insert(MapString::value_type(("Memory Channel Type"), ("RamBus")));
					break;
				case 0x04:
					Type37String.insert(MapString::value_type(("Memory Channel Type"), ("SyncLink")));
					break;
				default:
					Type37String.insert(MapString::value_type(("Memory Channel Type"), ("Reserved")));
					break;
				}
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type37_Maximum_Channel_Load]);
				Type37String.insert(MapString::value_type(("Memory Channel Maximum Channel Load"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type37_Memory_Device_Count]);
				Type37String.insert(MapString::value_type(("Memory Channel Memory Device Count"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type37_Memory1_Device_Load]);
				Type37String.insert(MapString::value_type(("Memory Channel Memory Device Load"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type37_Memory1_Device_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type37String.insert(MapString::value_type(("Memory Channel Memory Device Handle"), HextoString));
				WORD n = SmbiosADDR[Type37_Memory_Device_Count];
				if (n > 0)
				{
					for (size_t i = 1; i <= n; ++i)
					{
						sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type37_Memory1_Device_Load + 3 * (n - 1)]);
						Type37String.insert(MapString::value_type(("Memory Channel Memory Device Load") + to_string(i), HextoString));
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type37_Memory1_Device_Handle + 3 * (n - 1) + (sizeof(WORD) - i - 1)))));
						}
						Type37String.insert(MapString::value_type(("Memory Channel Memory Device Handle") + to_string(i), HextoString));
					}

				}
				MemoryChannel.emplace_back(Type37String);
				Type37String.clear();
			}

			if (dmi_head->type == 38)
			{
				MapString Type38String;
				switch (SmbiosADDR[Type38_Interface_Type])
				{
				case 0x00:
					Type38String.insert(MapString::value_type(("IPMI Device BMC Interface Type"), ("Unknown")));
					break;
				case 0x01:
					Type38String.insert(MapString::value_type(("IPMI Device BMC Interface Type"), ("KCS: Keyboard Controller Style")));
					break;
				case 0x02:
					Type38String.insert(MapString::value_type(("IPMI Device BMC Interface Type"), ("SMIC: Server Management Interface Chip")));
					break;
				case 0x03:
					Type38String.insert(MapString::value_type(("IPMI Device BMC Interface Type"), ("BT: Block Transfer")));
					break;
				default:
					Type38String.insert(MapString::value_type(("IPMI Device BMC Interface Type"), ("Reserved for future assignment by this specification")));
					break;
				}
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type38_IPMI_Spec_Rev]);
				Type38String.insert(MapString::value_type(("IPMI Device IPMI Specification Revision"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type38_I2C_SlaveAddress]);
				Type38String.insert(MapString::value_type(("IPMI Device I2C Slave Address"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type38_NV_Storage_Device_Address]);
				Type38String.insert(MapString::value_type(("IPMI Device NV Storage Device Address"), HextoString));
				for (size_t i = 0; i < sizeof(int64_t); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type38_Base_Address + (sizeof(int64_t) - i - 1)))));
				}
				Type38String.insert(MapString::value_type(("IPMI Device Base Address"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type38_Base_Address_Mod_Interrupt]);
				Type38String.insert(MapString::value_type(("IPMI Device Base Address Modifier / Interrupt Info"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type38_Interrupt_Number]);
				Type38String.insert(MapString::value_type(("IPMI Device Interrupt Number"), HextoString));
				IPMIDeviceinfo.emplace_back(Type38String);
				Type38String.clear();
			}

			if (dmi_head->type == 39)
			{
				MapString Type39String;
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type39_Power_Unit_Group]);
				Type39String.insert(MapString::value_type(("System Power Supply Power Unit Group"), HextoString));
				Type39String.insert(MapString::value_type(("System Power Supply Location"), dmi_to_string(dmi_head, SmbiosADDR[Type39_Location])));
				Type39String.insert(MapString::value_type(("System Power Supply Device Name"), dmi_to_string(dmi_head, SmbiosADDR[Type39_Device_Name])));
				Type39String.insert(MapString::value_type(("System Power Supply Manufacturer"), dmi_to_string(dmi_head, SmbiosADDR[Type39_Manufacturer])));
				Type39String.insert(MapString::value_type(("System Power Supply Serial Number"), dmi_to_string(dmi_head, SmbiosADDR[Type39_Serial_Number])));
				Type39String.insert(MapString::value_type(("System Power Supply Asset Tag Number"), dmi_to_string(dmi_head, SmbiosADDR[Type39_Asset_Tag_Number])));
				Type39String.insert(MapString::value_type(("System Power Supply Model Part Number"), dmi_to_string(dmi_head, SmbiosADDR[Type39_Model_Part_Number])));
				Type39String.insert(MapString::value_type(("System Power Supply Revision Level"), dmi_to_string(dmi_head, SmbiosADDR[Type39_Revision_Level])));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type39_Max_Power_Capacity + (sizeof(WORD) - i - 1)))));
				}
				Type39String.insert(MapString::value_type(("System Power Supply Max Power Capacity"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type39_Power_Supply_Characteristics + (sizeof(WORD) - i - 1)))));
				}
				Type39String.insert(MapString::value_type(("System Power Supply Power Supply Characteristics"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type39_Input_Voltage_Probe_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type39String.insert(MapString::value_type(("System Power Supply Input Voltage Probe Handle"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type39_Cooling_Device_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type39String.insert(MapString::value_type(("System Power Supply Cooling Device Handle"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type39_Input_Current_Probe_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type39String.insert(MapString::value_type(("System Power Supply Input Current Probe Handle"), HextoString));
				SystemPowerSupply.emplace_back(Type39String);
				Type39String.clear();
			}

			if (dmi_head->type == 40)
			{
				MapString Type40String;
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type40_Num_of_Addition_Inf_Entries]);
				Type40String.insert(MapString::value_type(("Number of Additional Information entries"), HextoString));
				if (SmbiosADDR[Type40_Num_of_Addition_Inf_Entries] > 0)
				{
					for (size_t i = 1; i <= SmbiosADDR[Type40_Num_of_Addition_Inf_Entries]; ++i)
					{
						sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type40_Addition_Inf_Entries + 4 * (i - 1)]);
						Type40String.insert(MapString::value_type(("Number of Additional Information Entry Length") + to_string(i), HextoString));
						for (size_t i = 0; i < sizeof(WORD); ++i)
						{
							sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type40_Addition_Inf_Entries + 1 + 4 * (i - 1) + (sizeof(WORD) - i - 1)))));
						}
						Type40String.insert(MapString::value_type(("Number of Additional Information Referenced Handle") + to_string(i), HextoString));
						sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type40_Addition_Inf_Entries + 3 + 4 * (i - 1)]);
						Type40String.insert(MapString::value_type(("Number of Additional Information Referenced Offset") + to_string(i), HextoString));
						Type40String.insert(MapString::value_type(("Number of Additional Information String"), dmi_to_string(dmi_head, SmbiosADDR[Type40_Addition_Inf_Entries + 4 + 4 * (i - 1)])));
					}
				}
				Additionalinfo.emplace_back(Type40String);
				Type40String.clear();
			}

			if (dmi_head->type == 41)
			{
				MapString Type41String;
				Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Reference Designation"), dmi_to_string(dmi_head, SmbiosADDR[Type41_Reference_Designation])));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type41_Device_Type_Instance]);
				Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Device Type Instance"), HextoString));
				if (SmbiosADDR[Type41_Device_Type] & 0x80)
				{
					Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Device Type Instance"), Onboard_Device_Types[(SmbiosADDR[Type41_Device_Type] & 0x7F) - 1]));
				}
				else
				{
					Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Device Type Instance"), ("Device Disable")));
				}
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%02X", BigEndian_to_LittleEndian((*(SmbiosADDR + Type41_Segment_Group_Number + (sizeof(WORD) - i - 1)))));
				}
				Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Segment Group Number"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type41_Bus_Number]);
				Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Bus Number"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type41_Device_Function_Number] & 0xF8));
				Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Device Number"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type41_Device_Function_Number] & 0x07));
				Type41String.insert(MapString::value_type(("Onboard Devices Extended Information Function Number"), HextoString));
				BroadDevExtendedinfo.emplace_back(Type41String);
				Type41String.clear();
			}

			if (dmi_head->type == 42)
			{
				MapString Type42String;
				sprintf_s(HextoString, sizeof(HextoString), "%02X", (SmbiosADDR[Type42_Interface_Type]));
				Type42String.insert(MapString::value_type(("Management Controller Host Interface Type"), HextoString));
				ManagementControllerHostInterface.emplace_back(Type42String);
				Type42String.clear();
			}

			if (dmi_head->type == 43)
			{
				MapString Type43String;
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%c", BigEndian_to_LittleEndian((*(SmbiosADDR + Type43_Vendor_ID + (sizeof(WORD) - i - 1)))));
				}
				Type43String.insert(MapString::value_type(("TMP Device Vendor ID"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type43_Major_Spec_Version]);
				Type43String.insert(MapString::value_type(("TMP Device Major Spec Version"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", SmbiosADDR[Type43_Minor_Spec_Version]);
				Type43String.insert(MapString::value_type(("TMP Device Minor Spec Version"), HextoString));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%c", BigEndian_to_LittleEndian((*(SmbiosADDR + Type43_Firmware_Version1 + (sizeof(DWORD) - i - 1)))));
				}
				Type43String.insert(MapString::value_type(("TMP Device Firmware Version 1"), HextoString));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%c", BigEndian_to_LittleEndian((*(SmbiosADDR + Type43_Firmware_Version2 + (sizeof(DWORD) - i - 1)))));
				}
				Type43String.insert(MapString::value_type(("TMP Device Firmware Version 2"), HextoString));
				Type43String.insert(MapString::value_type(("TMP Device Description"), dmi_to_string(dmi_head, SmbiosADDR[Type43_Description])));
				for (size_t i = 0; i < sizeof(int64_t); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%c", BigEndian_to_LittleEndian((*(SmbiosADDR + Type43_Characteristics + (sizeof(int64_t) - i - 1)))));
				}
				Type43String.insert(MapString::value_type(("TMP Device Characteristics"), HextoString));
				for (size_t i = 0; i < sizeof(DWORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%c", BigEndian_to_LittleEndian((*(SmbiosADDR + Type43_OEM_defined + (sizeof(DWORD) - i - 1)))));
				}
				Type43String.insert(MapString::value_type(("TMP Device OEM-defined"), HextoString));
				TPMDevice.emplace_back(Type43String);
				Type43String.clear();
			}

			if (dmi_head->type == 126)
			{
				MapString Type126String;
				sprintf_s(HextoString, sizeof(HextoString), "%02X", dmi_head->length);
				Type126String.insert(MapString::value_type(("Inactive Length"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", dmi_head->type);
				Type126String.insert(MapString::value_type(("Inactive Type"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%c", BigEndian_to_LittleEndian((*(SmbiosADDR + Type126_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type126String.insert(MapString::value_type(("Inactive Handle"), HextoString));
				Inactive.emplace_back(Type126String);
				Type126String.clear();
			}

			if (dmi_head->type == 127)
			{
				MapString Type127String;
				sprintf_s(HextoString, sizeof(HextoString), "%02X", dmi_head->length);
				Type127String.insert(MapString::value_type(("End-of-Table Length"), HextoString));
				sprintf_s(HextoString, sizeof(HextoString), "%02X", dmi_head->type);
				Type127String.insert(MapString::value_type(("End-of-Table Type"), HextoString));
				for (size_t i = 0; i < sizeof(WORD); ++i)
				{
					sprintf_s(HextoString + (i * 2), sizeof(HextoString), "%c", BigEndian_to_LittleEndian((*(SmbiosADDR + Type127_Handle + (sizeof(WORD) - i - 1)))));
				}
				Type127String.insert(MapString::value_type(("End-of-Table Handle"), HextoString));
				EndofTable.emplace_back(Type127String);
				Type127String.clear();
				break;
			}
		SmbiosADDR += dmi_head->length;//跳过结构段
		while ((*(WORD*)SmbiosADDR) != 0)
		{
/*			cout << hex << setfill('0') << setw(2) << int(*(SmbiosADDR)) << " ";*/
			++SmbiosADDR;
		}
/*		cout << endl;*/
		SmbiosADDR += 2; //跳过SMBIOS段结束符 0000
	}
	//delete p;
	//delete[]buf;
	SafeDeleteData(SmbiosADDRtmp);
	SafeDeleteArray(buf);
	SafeDeleteArray(HextoString);
	//delete[] HextoString;
	return TRUE;
}

const char* Smbios::dmi_to_string(const DMI_Header *dmi, BYTE Source_string)
{
	char* data = (char*)dmi;

	if (Source_string == 0)
	{
		return "Not Specified";
	}

	data += dmi->length;

	while (Source_string > 1 && *data)
	{
		data += strlen(data);
		Source_string--;
		++data;
	}

	if (!*data)
		return "BAD_INDEX";

	return data;
}

template< class T > 
void Smbios::SafeDeleteArray(T*& pVal)
{
	delete[] pVal;
	pVal = NULL;
}

template< class T >
void Smbios::SafeDeleteData(T*& pVal)
{
	delete pVal;
	pVal = NULL;
}

template<typename T> 
T Smbios::BigEndian_to_LittleEndian(T Data)
{
	size_t len = sizeof(Data);
	T temp;
	switch (len)
	{
	case sizeof(BYTE) :
		temp = Data;
		break;
	case sizeof(WORD) :
		temp = (Data & 0xFF) << 8;
		temp |= (Data & 0xFF00) >> 8;
		break;
	default:
		temp = 0;
		break;
	}
		return temp;		
}