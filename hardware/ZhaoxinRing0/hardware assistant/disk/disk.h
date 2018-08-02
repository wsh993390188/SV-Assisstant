#pragma once
#pragma warning(disable:4035)
#include <windows.h>
#include <winioctl.h>
#include <atacct.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <Ntddscsi.h>
#include "WMI for Disk/SynQuery.h"
/**
信息来源：
https://www.cnblogs.com/huhu0013/p/4283436.html
*/
// IOCTL控制码
#define DFP_SEND_DRIVE_COMMAND					0x0007C084
//#define DFP_SEND_DRIVE_COMMAND CTL_CODE(IOCTL_DISK_BASE, 0x0021, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define DFP_RECEIVE_DRIVE_DATA					0x0007C088
//#define DFP_RECEIVE_DRIVE_DATA CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

//From DDK

#define	FILE_DEVICE_SCSI							0x0000001b
#define	IOCTL_SCSI_MINIPORT_IDENTIFY				((FILE_DEVICE_SCSI << 16) + 0x0501)
#define	IOCTL_SCSI_MINIPORT_READ_SMART_ATTRIBS		((FILE_DEVICE_SCSI << 16) + 0x0502)
#define IOCTL_SCSI_MINIPORT_READ_SMART_THRESHOLDS	((FILE_DEVICE_SCSI << 16) + 0x0503)
#define IOCTL_SCSI_MINIPORT_ENABLE_SMART			((FILE_DEVICE_SCSI << 16) + 0x0504)
#define IOCTL_SCSI_MINIPORT_DISABLE_SMART			((FILE_DEVICE_SCSI << 16) + 0x0505)

//
// Valid values for the bCommandReg member of IDEREGS.
//

#define ATAPI_ID_CMD			  0xA1            // Returns ID sector for ATAPI.
#define IDE_ATA_IDENTIFY          0xEC            // Returns ID sector for ATA.
#define SMART_CMD				  0xB0            // Performs SMART cmd.
// Requires valid bFeaturesReg,
// bCylLowReg, and bCylHighReg

//
// Cylinder register defines for SMART command
//

#define SMART_CYL_LOW   0x4F
#define SMART_CYL_HI    0xC2

#define	MAX_ATTRIBUTE	30

typedef enum _TRANSFER_MODE
{
	TRANSFER_MODE_UNKNOWN = 0,
	TRANSFER_MODE_PIO,
	TRANSFER_MODE_PIO_DMA,
	TRANSFER_MODE_ULTRA_DMA_16,
	TRANSFER_MODE_ULTRA_DMA_25,
	TRANSFER_MODE_ULTRA_DMA_33,
	TRANSFER_MODE_ULTRA_DMA_44,
	TRANSFER_MODE_ULTRA_DMA_66,
	TRANSFER_MODE_ULTRA_DMA_100,
	TRANSFER_MODE_ULTRA_DMA_133,
	TRANSFER_MODE_SATA_150,
	TRANSFER_MODE_SATA_300,
	TRANSFER_MODE_SATA_600
}TRANSFER_MODE;

typedef enum _HOST_READS_WRITES_UNIT
{
	HOST_READS_WRITES_UNKNOWN = 0,
	HOST_READS_WRITES_512B,
	HOST_READS_WRITES_32MB,
	HOST_READS_WRITES_GB,
}HOST_READS_WRITES_UNIT;

typedef	enum _VENDOR_ID
{
	HDD_GENERAL = 0,
	SSD_GENERAL = 1,
	SSD_VENDOR_MTRON = 2,
	SSD_VENDOR_INDILINX = 3,
	SSD_VENDOR_JMICRON = 4,
	SSD_VENDOR_INTEL = 5,
	SSD_VENDOR_SAMSUNG = 6,
	SSD_VENDOR_SANDFORCE = 7,
	SSD_VENDOR_MICRON = 8,
	SSD_VENDOR_OCZ = 9,
	HDD_SSD_VENDOR_SEAGATE = 10,
	HDD_VENDOR_WESTERN_DIGITAL = 11,
	SSD_VENDOR_PLEXTOR = 12,
	SSD_VENDOR_SANDISK = 13,
	SSD_VENDOR_OCZ_VECTOR = 14,
	HDD_SSD_VENDOR_TOSHIBA = 15,
	SSD_VENDOR_CORSAIR = 16,
	SSD_VENDOR_KINGSTON = 17,
	SSD_VENDOR_MICRON_MU02 = 18,
	SSD_VENDOR_NVME = 19,
	SSD_VENDOR_MAX = 99,

	VENDOR_UNKNOWN = 0x0000,
	USB_VENDOR_BUFFALO = 0x0411,
	USB_VENDOR_IO_DATA = 0x04BB,
	USB_VENDOR_LOGITEC = 0x0789,
	USB_VENDOR_INITIO = 0x13FD,
	USB_VENDOR_SUNPLUS = 0x04FC,
	USB_VENDOR_JMICRON = 0x152D,
	USB_VENDOR_CYPRESS = 0x04B4,
	USB_VENDOR_OXFORD = 0x0928,
	USB_VENDOR_PROLIFIC = 0x067B,
	USB_VENDOR_ALL = 0xFFFF,
}VENDOR_ID;

typedef	enum _POWER_ON_HOURS_UNIT
{
	POWER_ON_UNKNOWN = 0,
	POWER_ON_HOURS,
	POWER_ON_MINUTES,
	POWER_ON_HALF_MINUTES,
	POWER_ON_SECONDS,
	POWER_ON_10_MINUTES,
	POWER_ON_MILLI_SECONDS,
}POWER_ON_HOURS_UNIT;

static const TCHAR *ssdVendorString[] =
{
	_T(""),
	_T(""),
	_T("mt"), // MTron
	_T("ix"), // Indilinx
	_T("jm"), // JMicron
	_T("il"), // Intel
	_T("sg"), // SAMSUNG
	_T("sf"), // SandForce
	_T("mi"), // Micron
	_T("oz"), // OCZ
	_T("st"), // SEAGATE
	_T("wd"), // WDC
	_T("px"), // PLEXTOR
	_T("sd"), // SanDisk
	_T("oz"), // OCZ Vector
	_T("to"), // TOSHIABA
	_T("co"), // Corsair
	_T("ki"), // Kingston
	_T("m2"), // Micron MU02
	_T("nv"), // NVMe
};

static const TCHAR *deviceFormFactorString[] =
{
	_T(""),
	_T("5.25 inch"),
	_T("3.5 inch"),
	_T("2.5 inch"),
	_T("1.8 inch"),
	_T("< 1.8 inch")
};

// IDE的ID命令返回的数据
// 共512字节 (256个WORD)，这里仅定义了一些感兴趣的项(基本上依据ATA/ATAPI-4)
#pragma pack(push,1)
struct IDINFO
{
	USHORT wGenConfig; // WORD 0: 基本信息字
	USHORT wNumCyls; // WORD 1: 柱面数
	USHORT wReserved2; // WORD 2: 保留
	USHORT wNumHeads; // WORD 3: 磁头数　
	USHORT wReserved4; // WORD 4: 保留
	USHORT wReserved5; // WORD 5: 保留
	USHORT wNumSectorsPerTrack;// WORD 6: 每磁 道扇区数
	USHORT wVendorUnique[3]; // WORD 7-9: 厂家设定值
	CHAR sSerialNumber[20]; // WORD 10-19:序列号
	USHORT wBufferType;// WORD 20: 缓冲类 型
	USHORT wBufferSize;// WORD 21: 缓冲大小
	USHORT wECCSize; // WORD 22: ECC校验大小
	CHAR sFirmwareRev[8]; // WORD 23-26: 固件版本
	CHAR sModelNumber[40]; // WORD 27-46: 内部型号
	USHORT wMoreVendorUnique; // WORD 47: 厂家设定值
	USHORT wReserved48;// WORD 48: 保留
	union 
	{
		struct {
			USHORT reserved1 : 8;
			USHORT DMA : 1; // 1=支持DMA
			USHORT LBA : 1; // 1=支持 LBA
			USHORT DisIORDY : 1;// 1=可不使用IORDY
			USHORT IORDY : 1;// 1=支持 IORDY
			USHORT SoftReset : 1; // 1=需要ATA软启动
			USHORT Overlap : 1;// 1= 支持重叠操作
			USHORT Queue : 1;// 1=支持命令队列
			USHORT InlDMA : 1;// 1=支持交叉存取DMA
		}bits;
		USHORT Capabilities;
	}wCapabilities; // WORD 49: 一般能力
	USHORT wReserved1;  // WORD 50: 保留
	USHORT wPIOTiming; // WORD 51: PIO时序
	USHORT wDMATiming; // WORD 52: DMA时序
	union 
	{
		struct {
			USHORT CHSNumber : 1; // 1=WORD 54-58有效
			USHORT CycleNumber : 1; // 1=WORD 64-70有效
			USHORT UnltraDMA : 1;  // 1=WORD 88有效
			USHORT reserved : 13;
		}bits;
		USHORT FieldValidity;
	}wFieldValidity; // WORD 53: 后 续字段有效性标志
	USHORT wNumCurCyls;// WORD 54: CHS可寻址的柱面数
	USHORT wNumCurHeads;// WORD 55: CHS可寻址的磁头数
	USHORT wNumCurSectorsPerTrack; // WORD 56: CHS可寻址每磁道扇区数
	USHORT wCurSectorsLow;// WORD 57: CHS可寻址的扇区 数低位字
	USHORT wCurSectorsHigh; // WORD 58: CHS可寻址的扇区数高位字
	union 
	{
		struct {
			USHORT CurNumber : 8; // 当前一次性可读写扇区数
			USHORT Multi : 1;// 1=已选择多扇区读写
			USHORT reserved1 : 7;
		}bits;
		USHORT MultSectorStuff;
	}wMultSectorStuff; // WORD 59: 多 扇区读写设定
	ULONG dwTotalSectors;// WORD 60-61: LBA可寻址的扇区数
	USHORT wSingleWordDMA;// WORD 62: 单字节DMA支持能力　
	union 
	{
		struct {
			USHORT Mode0 : 1;// 1=支持模式0 (4.17Mb/s)
			USHORT Mode1 : 1;// 1=支持模式1 (13.3Mb/s)
			USHORT  Mode2 : 1;// 1=支持模式2 (16.7Mb/s)
			USHORT Reserved1 : 5;
			USHORT Mode0Sel : 1;// 1=已选择模式0
			USHORT Mode1Sel : 1;// 1=已选择模式1
			USHORT Mode2Sel : 1;// 1=已选择模式2
			USHORT Reserved2 : 5;
		}bits;
		USHORT MultiWordDMA;
	}wMultiWordDMA; // WORD 63: 多字节DMA支持能力
	struct {
		USHORT AdvPOIModes : 8; // 支持高 级POI模式数
		USHORT reserved : 8;
	} wPIOCapacity; // WORD 64: 高级PIO支持能 力
	USHORT wMinMultiWordDMACycle; // WORD 65: 多字节DMA传输周期的最小值
	USHORT  wRecMultiWordDMACycle; // WORD 66: 多字节DMA传输周期的建议值
	USHORT wMinPIONoFlowCycle; // WORD 67: 无流控制时PIO传输周期的最小值
	USHORT wMinPOIFlowCycle; // WORD 68: 有流控制时PIO传输周期的最小值
	USHORT wReserved69[11]; // WORD 69-79: 保留
	union 
	{
		struct {
			USHORT Reserved1 : 1;
			USHORT ATA1 : 1; // 1=支持ATA-1
			USHORT ATA2 : 1; // 1=支持ATA-2
			USHORT ATA3 : 1; // 1=支持ATA-3
			USHORT ATA4 : 1; // 1=支持ATA/ATAPI-4
			USHORT ATA5 : 1; // 1=支持ATA/ATAPI-5
			USHORT ATA6 : 1; // 1=支持ATA/ATAPI-6
			USHORT ATA7 : 1; // 1=支持ATA/ATAPI-7
			USHORT ATA8 : 1; // 1=支持ATA/ATAPI- 8
			USHORT ATA9 : 1; // 1=支持ATA/ATAPI-9
			USHORT ATA10 : 1; // 1=支持 ATA/ATAPI-10
			USHORT ATA11 : 1; // 1=支持ATA/ATAPI-11
			USHORT ATA12 : 1; // 1=支持ATA/ATAPI-12
			USHORT ATA13 : 1;// 1=支持ATA/ATAPI-13
			USHORT ATA14 : 1;// 1=支持ATA/ATAPI-14
			USHORT Reserved2 : 1;
		}bits;
		USHORT MajorVersion;
	}wMajorVersion; // WORD 80: 主版本
	USHORT wMinorVersion;// WORD 81: 副版本
	USHORT wReserved82[6];// WORD 82-87: 保留
	union 
	{
		struct {
			USHORT Mode0 : 1;// 1=支持 模式0 (16.7Mb/s)
			USHORT Mode1 : 1;// 1=支持模式1 (25Mb/s)
			USHORT Mode2 : 1;// 1=支持模式2 (33Mb/s)
			USHORT Mode3 : 1;// 1=支持模式3 (44Mb/s) USHORT Mode4:1;// 1=支持模式4 (66Mb/s)
			USHORT Mode5 : 1;// 1=支持模式5 (100Mb/s)
			USHORT Mode6 : 1;// 1=支持模式6 (133Mb/s)
			USHORT Mode7 : 1; // 1=支持模式7 (166Mb/s) ???
			USHORT Mode0Sel : 1;// 1=已选择模式0
			USHORT  Mode1Sel : 1;// 1=已选择模式1
			USHORT Mode2Sel : 1;// 1=已选择模式2
			USHORT Mode3Sel : 1;// 1=已选择模式3
			USHORT Mode4Sel : 1;// 1=已选择模式4
			USHORT Mode5Sel : 1;// 1=已选择模式5
			USHORT Mode6Sel : 1;// 1=已选择模式 6
			USHORT Mode7Sel : 1;// 1=已选择模式7
		}bits;
		USHORT UltraDMA;
	}wUltraDMA; // WORD 88: Ultra DMA支持能力;
	USHORT wReserved89[167]; // WORD 89-255
};

typedef IDINFO *PIDINFO;

struct SMART_THRESHOLD
{
	BYTE	Id;
	BYTE	ThresholdValue;
	BYTE	Reserved[10];
};

struct SMART_ATTRIBUTE
{
	BYTE	Id;
	WORD	StatusFlags;
	BYTE	CurrentValue;
	BYTE	WorstValue;
	BYTE	RawValue[6];
	BYTE	Reserved;
};

//需要引起注意的是IDINFO第57 - 58 WORD(CHS可寻址的扇区数)，因为不满足32位对齐的要求，不可定义为一个ULONG字段。Lynn McGuire的程序里正是由于定义为一个ULONG字段，导致该结构不可用。
#pragma pack(pop)
// SCSI驱动所需的输入输出共用的结构
/*typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength; // 头长度
	UCHAR Signature[8]; // 特征名称
	ULONG Timeout; // 超时时间
	ULONG ControlCode; // 控制码
	ULONG ReturnCode; // 返回码
	ULONG Length; // 缓冲区长度
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;
// IDE命令 寄存器
typedef struct _IDEREGS
{
BYTE bFeaturesReg;// 特征寄存器 (用于SMART 命令)　 BYTE bSectorCountReg;　 // 扇区数目寄存器
BYTE bSectorNumberReg;// 开始扇区寄存器
BYTE bCylLowReg; // 开始柱面低字节 寄存器
BYTE bCylHighReg; // 开始柱面高字节寄存器
BYTE bDriveHeadReg; // 驱动器/磁头寄存器
BYTE bCommandReg; // 指令寄存器
BYTE bReserved; // 保留
} IDEREGS, *PIDEREGS, *LPIDEREGS;
// 从驱动程序返回的状态
typedef struct _DRIVERSTATUS
{
BYTE bDriverError;  // 错误码
BYTE bIDEStatus; // IDE状态寄存器
BYTE bReserved[2]; // 保留
DWORD dwReserved[2]; // 保留
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;
// IDE设备IOCTL输入数据结构
typedef struct _SENDCMDINPARAMS
{
DWORD cBufferSize;// 缓冲区字节数
IDEREGS irDriveRegs;// IDE寄存器组
BYTE bDriveNumber;// 驱动器号
BYTE bReserved[3];//保留
DWORD dwReserved[4]; //保留
BYTE bBuffer[1]; //输入缓冲区(此处象征性地包含1字节)
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
// IDE设备IOCTL输出数据结构
typedef struct _SENDCMDOUTPARAMS
{
DWORD cBufferSize;// 缓冲区 字节数
DRIVERSTATUS DriverStatus; // 驱动程序返回状态
BYTE bBuffer[1]; // 输入缓冲区(此处象征性地包含1字节)
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;*/

struct SMART_READ_DATA_OUTDATA
{
	SENDCMDOUTPARAMS	SendCmdOutParam;
	BYTE				Data[READ_ATTRIBUTE_BUFFER_SIZE - 1];
};

struct ATA_SMART_INFO
{
	ATA_SMART_INFO();
	IDINFO				Identify;
	BYTE				SmartReadData[512];
	BYTE				SmartReadThreshold[512];
	SMART_ATTRIBUTE		Attribute[MAX_ATTRIBUTE];
	SMART_THRESHOLD		Threshold[MAX_ATTRIBUTE];
	wstring				model;
	wstring				deviceid;
	wstring				diskSize;
	wstring				mediaType;
	wstring				interfaceTypeWmi;
	wstring				pnpDeviceId;
	wstring				firmware;
	wstring				MajorVersion;
	wstring				MinorVersion;
	wstring				MaxTransferMode;
	wstring				CurrentTransferMode;
	wstring				Interface;
	wstring				SmartKeyName;
	wstring				SsdVendorString;
	INT					physicalDriveId;
	INT					scsiPort;
	INT					scsiTargetId;
	INT					scsiBus;
	DWORD				AttributeCount;
	DWORD				DiskVendorId;
	DWORD				Major;
	INT					HostWrites;
	INT					HostReads;
	INT					GBytesErased;
	INT					NandWrites;
	INT					WearLevelingCount;
	INT					Life;
	HOST_READS_WRITES_UNIT HostReadsWritesUnit;
	INT					PowerOnRawValue;
	INT					PowerOnStartRawValue;
	DWORD				PowerOnCount;
	INT					Temperature;
	double				TemperatureMultiplier;
	INT					DetectedPowerOnHours;
	INT					MeasuredPowerOnHours;
	DWORD				TransferModeType;
	DWORD				DetectedTimeUnitType;
	DWORD				MeasuredTimeUnitType;
	BOOL				IsSsd;
	BOOL				IsRawValues8;
	BOOL				IsRawValues7;
};

typedef ATA_SMART_INFO *PATA_SMART_INFO;

class UnCopyable
{
protected:
	UnCopyable() {}
	~UnCopyable() {}

private:
	explicit UnCopyable(const UnCopyable& x);
	UnCopyable& operator=(const UnCopyable& x);
};

class Disk : private UnCopyable
{
public:
	Disk();
	~Disk();

	VOID UpdateDate();

	vector<ATA_SMART_INFO> SMARTinfo;

private:
	// 读取IDE硬盘的设备信息，必须有 足够权限
	// nDrive: 驱动器号(0=第一个硬盘，1=0=第二个硬盘，......)
	// pIdInfo: 设备信息结构指针
	BOOL GetPhysicalDriveInfoInNT(int nDrive, PIDINFO pIdInfo);

	// 用SCSI驱动读取IDE硬盘的设备信息，不受权限制约
	// nDrive: 驱动器号(0=Primary Master, 1=Promary Slave, 2=Secondary master, 3=Secondary slave)
	// pIdInfo: 设备信息结 构指针
	BOOL GetIdeDriveAsScsiInfoInNT(int nDrive, PIDINFO pIdInfo);

	BOOL GetPhysicalSMARTInfo(INT nDrive, BYTE target, ATA_SMART_INFO& SMART_info);
	BOOL GetPhysicalThresholdInfo(INT nDrive, BYTE target, ATA_SMART_INFO& SMART_info);
	BOOL GetScsiSMARTInfo(INT scsiPort, INT scsiTargetId, ATA_SMART_INFO& SMART_info);
	BOOL GetScsiThresholdInfo(INT scsiPort, INT scsiTargetId, ATA_SMART_INFO& SMART_info);

	// 打开设备
	// filename: 设备的“文件名”
	HANDLE OpenDevice(LPCTSTR filename);

	// 向驱动发“IDENTIFY DEVICE”命令，获得设备信息
	// hDevice: 设备句柄
	// pIdInfo:　设备信息结构指针
	BOOL IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo);

	// 向SCSI MINI-PORT 驱动发“IDENTIFY DEVICE”命令，获得设备信息
	// hDevice: 设备句柄 
	// pIdInfo:　设备信息结构指针
	BOOL IdentifyDeviceAsScsi(HANDLE hDevice, int nDrive, PIDINFO pIdInfo);

	// 将串中的字符两两颠倒
	// 原因是ATA/ATAPI中的WORD，与Windows采用的字节顺序相 反
	// 驱动程序中已经将收到的数据全部反过来，我们来个负负得正
	void AdjustString(char* str, int len);
	//获取ATA得版本号
	VOID GetAtaMinorVersion(WORD w81, wstring & minor);
	//获取运行时间
	DWORD GetPowerOnHours(DWORD rawValue, DWORD timeUnitType);
	DWORD GetTransferMode(WORD w63, WORD w76, WORD w77, WORD w88, wstring & current, wstring & max, wstring & type);
	//通过字符判断硬盘厂家 及 硬盘类型 ssd or 机械
	VOID CheckSsdSupport(ATA_SMART_INFO & asi);
	BOOL IsSsdOld(ATA_SMART_INFO & asi);
	BOOL IsSsdMtron(ATA_SMART_INFO & asi);
	BOOL IsSsdJMicron60x(ATA_SMART_INFO & asi);
	BOOL IsSsdJMicron61x(ATA_SMART_INFO & asi);
	BOOL IsSsdIndlinx(ATA_SMART_INFO & asi);
	BOOL IsSsdIntel(ATA_SMART_INFO & asi);
	BOOL IsSsdSamsung(ATA_SMART_INFO & asi);
	BOOL IsSsdSandForce(ATA_SMART_INFO & asi);
	BOOL IsSsdMicronMU02(ATA_SMART_INFO & asi);
	BOOL IsSsdMicron(ATA_SMART_INFO & asi);
	BOOL IsSsdOcz(ATA_SMART_INFO & asi);
	BOOL IsSsdOczVector(ATA_SMART_INFO & asi);
	BOOL IsSsdPlextor(ATA_SMART_INFO & asi);
	BOOL IsSsdSanDisk(ATA_SMART_INFO & asi);
	BOOL IsSsdKingston(ATA_SMART_INFO & asi);
	BOOL IsSsdToshiba(ATA_SMART_INFO & asi);
	//解析出SMART中的信息
	DWORD GetTimeUnitType(wstring Model, wstring Firmware, DWORD major, DWORD transferMode);
	BOOL FillSmartData(ATA_SMART_INFO * asi);
	VOID GetSMARTIDINFO(ATA_SMART_INFO * asi);
	BOOL FillSmartThreshold(ATA_SMART_INFO * asi);
	DWORD GetAtaMajorVersion(WORD w80, wstring & majorVersion);
};

class Diskinfo
{
public:
	static Diskinfo* Instance();
	void updatedisk();
	Diskinfo();
private:
	void ChartoWchar(std::wstring & des, const char * src);
	std::shared_ptr<Disk> m_disk;
	static std::shared_ptr<Diskinfo> m_diskinfo;
}; 