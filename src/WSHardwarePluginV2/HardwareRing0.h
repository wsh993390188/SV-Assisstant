#pragma once
#ifndef _PCI_X_
#define _PCI_X_
#define PCI_TYPE0_ADDRESSES             6
#define PCI_TYPE1_ADDRESSES             2
#define PCI_TYPE2_ADDRESSES             5
#pragma pack(push,1)
typedef struct _PCI_COMMON_HEADER {
	USHORT  VendorID;                   // (ro)
	USHORT  DeviceID;                   // (ro)
	USHORT  Command;                    // Device control
	USHORT  Status;
	UCHAR   RevisionID;                 // (ro)
	UCHAR   ProgIf;                     // (ro)
	UCHAR   SubClass;                   // (ro)
	UCHAR   BaseClass;                  // (ro)
	UCHAR   CacheLineSize;              // (ro+)
	UCHAR   LatencyTimer;               // (ro+)
	UCHAR   HeaderType;                 // (ro)
	UCHAR   BIST;                       // Built in self test

	union {
		struct _PCI_HEADER_TYPE_0 {
			ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];
			ULONG   CIS;
			USHORT  SubVendorID;
			USHORT  SubSystemID;
			ULONG   ROMBaseAddress;
			UCHAR   CapabilitiesPtr;
			UCHAR   Reserved1[3];
			ULONG   Reserved2;
			UCHAR   InterruptLine;      //
			UCHAR   InterruptPin;       // (ro)
			UCHAR   MinimumGrant;       // (ro)
			UCHAR   MaximumLatency;     // (ro)
		} type0;

		//
		// PCI to PCI Bridge
		//

		struct _PCI_HEADER_TYPE_1 {
			ULONG   BaseAddresses[PCI_TYPE1_ADDRESSES];
			UCHAR   PrimaryBus;
			UCHAR   SecondaryBus;
			UCHAR   SubordinateBus;
			UCHAR   SecondaryLatency;
			UCHAR   IOBase;
			UCHAR   IOLimit;
			USHORT  SecondaryStatus;
			USHORT  MemoryBase;
			USHORT  MemoryLimit;
			USHORT  PrefetchBase;
			USHORT  PrefetchLimit;
			ULONG   PrefetchBaseUpper32;
			ULONG   PrefetchLimitUpper32;
			USHORT  IOBaseUpper16;
			USHORT  IOLimitUpper16;
			UCHAR   CapabilitiesPtr;
			UCHAR   Reserved1[3];
			ULONG   ROMBaseAddress;
			UCHAR   InterruptLine;
			UCHAR   InterruptPin;
			USHORT  BridgeControl;
		} type1;

		//
		// PCI to CARDBUS Bridge
		//

		struct _PCI_HEADER_TYPE_2 {
			ULONG   SocketRegistersBaseAddress;
			UCHAR   CapabilitiesPtr;
			UCHAR   Reserved;
			USHORT  SecondaryStatus;
			UCHAR   PrimaryBus;
			UCHAR   SecondaryBus;
			UCHAR   SubordinateBus;
			UCHAR   SecondaryLatency;
			struct {
				ULONG   Base;
				ULONG   Limit;
			}       Range[PCI_TYPE2_ADDRESSES - 1];
			UCHAR   InterruptLine;
			UCHAR   InterruptPin;
			USHORT  BridgeControl;
		} type2;
	} u;
	UCHAR   DeviceSpecific[192];
} PCI_COMMON_CONFIG, * PPCI_COMMON_CONFIG;

typedef struct _PCI_SLOT_NUMBER {
	union {
		struct {
			ULONG   FunctionNumber : 3;
			ULONG   DeviceNumber : 5;
			ULONG   Reserved : 24;
		} bits;
		ULONG   AsULONG;
	} u;
} PCI_SLOT_NUMBER, * PPCI_SLOT_NUMBER;
#pragma pack(pop)
#endif // !_PCI_X_
namespace Hardware
{
	namespace Utils
	{
		/// @brief Ring0级权限获取系统底层信息
		namespace Ring0
		{
			/// @brief WinRing0帮助函数
			class WinRing0 final
			{
			public:
				/// @brief WinRing0单例创建者
				/// @return WinRing0单例
				static WinRing0& Instance();

				/// @brief WinRing0初始化
				WinRing0();
				/// @brief WinRing0释放
				~WinRing0();

				/// @brief 读取Msr
				/// @param[in] index 寄存器地址
				/// @param[out] eax
				/// @param[out] edx
				/// @return 是否成功
				BOOL RdMsr(DWORD index, PDWORD eax, PDWORD edx);

				/// @brief 写Msr
				/// @param[in] index 寄存器地址
				/// @param[in] eax
				/// @param[in] edx
				/// @return 是否成功
				BOOL WrMsr(DWORD index, DWORD eax, DWORD edx);

				/*************************************************
				*@Function				ReadPci
				*@brief					读PCI配置空间的数据
				*@author				王硕
				*@param
					*@bus				IN 输入bus的值
					*@dev				IN 输入dev的值
					*@func				IN 输入func的值
					*@pci_config		OUT 输出Pci配置空间的数据
				*@return
					*@BOOL				是否成功 0成功 其他值 失败
				**************************************************/
				BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN USHORT offset, IN USHORT DataSize, OUT DWORD& pci_config);

				/*************************************************
				*@Function				WritePci
				*@brief					写PCI配置空间的数据
				*@author				王硕
				*@param
					*@bus				IN 输入bus的值
					*@dev				IN 输入dev的值
					*@func				IN 输入func的值
					*@offset			IN 输入要写入偏移量的值
					*@Data				IN 输入要写入数据的值
				*@return
					*@BOOL				是否成功 0成功 其他值 失败
				**************************************************/
				BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN USHORT DataSize, IN ULONG Data);

				/**********************************************************************
				*@Function				RdIOPort
				*@brief					读取IO Port的值
				*@author				王硕
				*@param
					*@IO_Port_Addr		IN IO Port的地址
					*@IO_DataSize		IN IO 输出数据的大小 1、2、4（8、16、32bits）
					*@IO_Data			OUT 输出的IO Port中的值
				*@return
				*@BOOL				是否成功
				**************************************************************************/
				BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD& IO_Data);

				/**********************************************************************
				*@Function				WrIOPort
				*@brief				写入IO Port的值
				*@author			王硕
				*@param
					*@IO_Data			IN 输入IO Port的值
					*@IO_DataSize		IN IO 输出数据的大小 1、2、4（8、16、32bits）
				*@IO_Port_Addr		IN 输入IO Port的地址
				*@return
					*@BOOL				是否成功 0成功 其他值 失败
				**************************************************************************/
				BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);

				/**************************************************************************
				*@Function				RdMemory
				*@brief					读取内存的值
				*@author				王硕
				*@param
					*@IO_Port_Addr		IN 输入Memory的地址
					*@IO_DataSize		IN Memory 输出数据的大小 1、2、4（8、16、32bits)
					*@IO_Data			OUT 输出的Memory中的值
				*@return
					*@BOOL				是否成功 0成功 其他值 失败
				***************************************************************************/
				BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);

				/***************************************************************************
				*@Function				WrMemory
				*@brief					写入内存的值
				*@author				王硕
				*@param
					*@IO_Data			IN 输入Memory的值
					*@IO_DataSize		IN Memory 输出数据的大小 1、2、4（8、16、32bits）
					*@IO_Port_Addr		IN 输入Memory的地址
				*@return
					*@BOOL				是否成功 0成功 其他值 失败
				******************************************************************************/
				BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);

				BOOL RdTsc(OUT DWORD64& Data);

				double GetTscFrequency();
			private:
#ifdef RUN_TIME_DYNAMIC_LINKING
				HMODULE Hmodule;
#endif
				LARGE_INTEGER nFreq;
			};
		}
	}
}
