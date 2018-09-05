#pragma once
#include <memory>
#include <mutex>
// #include <thread>
#include "MyDriverClass.h"

class ZhaoxinDriver
{
public:
	static ZhaoxinDriver* Instance();
	BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD& IO_Data);
	BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);
	BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data);
	BOOL WrMsr(IN DWORD Index, IN DWORD64 Data);
	BOOL RdMsrTx(IN DWORD Index, OUT DWORD64& Data, IN DWORD threadAffinityMask);
	BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD threadAffinityMask);
	BOOL RdMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT DWORD& Memory_Data);
	BOOL WrMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
	BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);
	BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);
	BOOL GetECData(Embedded_Controller_Data& EC_Data);
	BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);
	BOOL ReadSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, BYTE& data);
	BOOL ReadSMbusByWord(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, WORD& data);
	void WriteSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, const DWORD data);
	USHORT GetPCIVendorID();
	const Pci_All_Config_Space& GetAllPciInfo();
	ZhaoxinDriver();
private:
	bool smbus_wait_until_ready(const USHORT SmbusBase);
	int smbus_wait_until_done(const USHORT SmbusBase);
private:

#pragma region SMBUS Register
#define PMIO_INDEX_REG			0xCD6
#define PMIO_DATA_REG			0xCD7

#define SMBUS_BAR_LOW_BYTE		0x2C
#define SMBUS_BAR_HIGH_BYTE		0x2D

#define SMBUS_STATUS_REG		0x00
#define SMBUS_SLAVE_STATUS_REG	0x01
#define SMBUS_COMMAND_REG		0x02
#define SMBUS_CONTROL_REG		0x03
#define SMBUS_HOST_CMD_REG		0x04
#define SMBUS_DATA0_REG			0x05
#define SMBUS_CLOCK_REG			0x0E

#define SMBUS_FREQUENCY_CONST	66000000 / 4

	/* Between 1-10 seconds, We should never timeout normally
	* Longer than this is just painful when a timeout condition occurs.
	*/
#define SMBUS_TIMEOUT (100*1000*10)
#pragma endregion

#pragma region SMBUS Command
#define SMBUS_READ_WORD_COMMAND	0x4C
#define SMBUS_READ_BYTE_COMMAND	0x48
#define SMBUS_READ_COMMAND		0x44
#pragma endregion

#pragma region SMBUS Status
#define SMBHSTSTS_BYTE_DONE	(1 << 7)
#define SMBHSTSTS_INUSE_STS	(1 << 6)
#define SMBHSTSTS_SMBALERT_STS	(1 << 5)
#define SMBHSTSTS_FAILED	(1 << 4)
#define SMBHSTSTS_BUS_ERR	(1 << 3)
#define SMBHSTSTS_DEV_ERR	(1 << 2)
#define SMBHSTSTS_INTR		(1 << 1)
#define SMBHSTSTS_HOST_BUSY	(1 << 0)
#pragma endregion

	static std::shared_ptr<ZhaoxinDriver> ring0_temp;
	std::mutex ring0_mutex;
	std::shared_ptr<CRing0> ring;
};
//mutex ZhaoxinDriver::ring0_mutex;

