#pragma once
#include "diskdefination.h"

namespace SV_ASSIST
{
	namespace Storage
	{
		struct  DISK_SMART_INFO
		{
			SMART_ATTRIBUTE		Attribute[30];
			SMART_THRESHOLD		Threshold[30];

			bool				IsSsd;
			std::wstring		NominalMediaRotationRate;
			std::wstring		BufferSize;
			std::wstring		NvCacheSize;
			std::wstring		LogicalDrive;
			std::wstring		PowerOnCount;
			std::wstring		Temperature;
			std::wstring		HostWrites;
			std::wstring		HostReads;
			std::wstring		GBytesErased;
			std::wstring		NandWrites;
			std::wstring		PowerOnHours;
			std::wstring		TotalDiskSize;
			std::wstring		SerialNumber;
			std::wstring		FirmwareRev;
			std::wstring		Model;
			std::wstring		MaxTransferMode;
			std::wstring		CurrentTransferMode;
			std::wstring		MajorVersion;
			std::wstring		MinorVersion;
			std::wstring		Interface;
			std::wstring		Enclosure;
			std::wstring		DeviceNominalFormFactor;
			std::wstring		Feature;
			std::wstring		DiskState;
		};
		
		const std::vector<DISK_SMART_INFO>& GetDiskSMARTInfo();

		void UpdateData();
	}
}
