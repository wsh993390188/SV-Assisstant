#include "stdafx.h"
#include "DriverOrigin.h"

std::unique_ptr<ZhaoxinDriver> ZhaoxinDriver::ring0_temp = nullptr;

ZhaoxinDriver::ZhaoxinDriver() : ring(std::make_shared<CRing0>()), ring0_mutex{}
{}

ZhaoxinDriver * ZhaoxinDriver::Instance()
{
	if (!ring0_temp.get())
	{
		ring0_temp = std::make_unique<ZhaoxinDriver>();
	}
	return ring0_temp.get();
}

BOOL ZhaoxinDriver::RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD & IO_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
}

BOOL ZhaoxinDriver::WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WrIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
}

BOOL ZhaoxinDriver::RdMsr(IN DWORD Index, OUT DWORD64 & Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdMsr(Index, Data);
}

BOOL ZhaoxinDriver::WrMsr(IN DWORD Index, IN DWORD64 Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WrMsr(Index, Data);
}

BOOL ZhaoxinDriver::RdMsrTx(IN DWORD Index, OUT DWORD64 & Data, IN DWORD threadAffinityMask)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdMsrTx(Index, threadAffinityMask, Data);
}

BOOL ZhaoxinDriver::WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD threadAffinityMask)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WrMsrTx(Index, threadAffinityMask, Data);
}

BOOL ZhaoxinDriver::RdMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT DWORD & Memory_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdMemory(Memory_Addr, Mem_DataSize, Memory_Data);
}

BOOL ZhaoxinDriver::WrMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	auto state = ring->WrMemory(Memory_Addr, Mem_DataSize, Memory_Data);
	Sleep(1);
	return state;
}

BOOL ZhaoxinDriver::ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG & pci_config)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->ReadPci(bus, dev, func, pci_config);
}

BOOL ZhaoxinDriver::WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WritePci(bus, dev, func, offset, Data);
}

BOOL ZhaoxinDriver::GetECData(Embedded_Controller_Data & EC_Data)
{
	return ring->GetECData(EC_Data);
}

BOOL ZhaoxinDriver::SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
{
	return ring->SetECData(EC_Addr, EC_Write_Data);
}

BOOL ZhaoxinDriver::ReadSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, BYTE & data)
{
	if (smbus_wait_until_ready(SmbusBase))
	{
		WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, SlaveAddress | 1);
		WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, offset);
		WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, SMBUS_READ_BYTE_COMMAND);
		if (smbus_wait_until_done(SmbusBase) < 0)
			return false;
		DWORD val = 0;
		RdIOPort(SmbusBase + SMBUS_DATA0_REG, 1, val);
		memcpy(&data, &val, 1);
	}
	else
		return FALSE;
	return TRUE;
}

BOOL ZhaoxinDriver::ReadSMbusByWord(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, WORD & data)
{
	if (smbus_wait_until_ready(SmbusBase))
	{
		WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, SlaveAddress | 1);
		WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, offset);
		WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, SMBUS_READ_WORD_COMMAND);
		if (smbus_wait_until_done(SmbusBase) < 0)
			return false;
		DWORD val = 0;
		RdIOPort(SmbusBase + SMBUS_DATA0_REG, 2, val);
		memcpy(&data, &val, 2);
	}
	else
		return FALSE;
	return TRUE;
}

bool ZhaoxinDriver::smbus_wait_until_ready(const USHORT SmbusBase)
{
	ULONG loops = SMBUS_TIMEOUT;
	do {
		DWORD val = {};
		RdIOPort(SmbusBase + SMBUS_STATUS_REG, 1, val);
		val &= 0x1f;
		if (val == 0) {	/* ready now */
			return true;
		}
		WrIOPort(SmbusBase + SMBUS_STATUS_REG, 1, 0xFE);
	} while (--loops);
	return false;		/* time out */
}

int ZhaoxinDriver::smbus_wait_until_done(const USHORT SmbusBase)
{
	DWORD loops = SMBUS_TIMEOUT;
	do {
		DWORD val = {};

		RdIOPort(SmbusBase + SMBUS_STATUS_REG, 1, val);
		val &= 0x1f;	/* mask off reserved bits */
		if (val & 0x1c) {
			if ((val & SMBHSTSTS_DEV_ERR) == SMBHSTSTS_DEV_ERR)
				return -2;
			if ((val & SMBHSTSTS_BUS_ERR) == SMBHSTSTS_BUS_ERR)
				return -3;
			if ((val & SMBHSTSTS_FAILED) == SMBHSTSTS_FAILED)
				return -4;
			return -5;	/* error */
		}
		if ((val & 0x03) == 0x02) {
			return 0;
		}
	} while (--loops);
	return -1;		/* timeout */
}

void ZhaoxinDriver::WriteSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, const DWORD data)
{
	if (smbus_wait_until_ready(SmbusBase))
	{
		WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, SlaveAddress & 0xFE);
		if (!smbus_wait_until_ready(SmbusBase))
			return;
		WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, offset);
		WrIOPort(SmbusBase + SMBUS_DATA0_REG, 1, data);
		WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, SMBUS_READ_BYTE_COMMAND);
		if (smbus_wait_until_done(SmbusBase) < 0)
			return;
	}
}

USHORT ZhaoxinDriver::GetPCIVendorID()
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->VendorID;
}

const Pci_All_Config_Space& ZhaoxinDriver::GetAllPciInfo()
{
	return ring->ReturnPCIConfigSpace();
}