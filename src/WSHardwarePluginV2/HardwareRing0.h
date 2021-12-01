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
		/// @brief Ring0��Ȩ�޻�ȡϵͳ�ײ���Ϣ
		namespace Ring0
		{
			/// @brief WinRing0��������
			class WinRing0 final
			{
			public:
				/// @brief WinRing0����������
				/// @return WinRing0����
				static WinRing0& Instance();

				/// @brief WinRing0��ʼ��
				WinRing0();
				/// @brief WinRing0�ͷ�
				~WinRing0();

				/// @brief ��ȡMsr
				/// @param[in] index �Ĵ�����ַ
				/// @param[out] eax
				/// @param[out] edx
				/// @return �Ƿ�ɹ�
				BOOL RdMsr(DWORD index, PDWORD eax, PDWORD edx);

				/// @brief дMsr
				/// @param[in] index �Ĵ�����ַ
				/// @param[in] eax
				/// @param[in] edx
				/// @return �Ƿ�ɹ�
				BOOL WrMsr(DWORD index, DWORD eax, DWORD edx);

				/*************************************************
				*@Function				ReadPci
				*@brief					��PCI���ÿռ������
				*@author				��˶
				*@param
					*@bus				IN ����bus��ֵ
					*@dev				IN ����dev��ֵ
					*@func				IN ����func��ֵ
					*@pci_config		OUT ���Pci���ÿռ������
				*@return
					*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
				**************************************************/
				BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN USHORT offset, IN USHORT DataSize, OUT DWORD& pci_config);

				/*************************************************
				*@Function				WritePci
				*@brief					дPCI���ÿռ������
				*@author				��˶
				*@param
					*@bus				IN ����bus��ֵ
					*@dev				IN ����dev��ֵ
					*@func				IN ����func��ֵ
					*@offset			IN ����Ҫд��ƫ������ֵ
					*@Data				IN ����Ҫд�����ݵ�ֵ
				*@return
					*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
				**************************************************/
				BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN USHORT DataSize, IN ULONG Data);

				/**********************************************************************
				*@Function				RdIOPort
				*@brief					��ȡIO Port��ֵ
				*@author				��˶
				*@param
					*@IO_Port_Addr		IN IO Port�ĵ�ַ
					*@IO_DataSize		IN IO ������ݵĴ�С 1��2��4��8��16��32bits��
					*@IO_Data			OUT �����IO Port�е�ֵ
				*@return
				*@BOOL				�Ƿ�ɹ�
				**************************************************************************/
				BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD& IO_Data);

				/**********************************************************************
				*@Function				WrIOPort
				*@brief				д��IO Port��ֵ
				*@author			��˶
				*@param
					*@IO_Data			IN ����IO Port��ֵ
					*@IO_DataSize		IN IO ������ݵĴ�С 1��2��4��8��16��32bits��
				*@IO_Port_Addr		IN ����IO Port�ĵ�ַ
				*@return
					*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
				**************************************************************************/
				BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);

				/**************************************************************************
				*@Function				RdMemory
				*@brief					��ȡ�ڴ��ֵ
				*@author				��˶
				*@param
					*@IO_Port_Addr		IN ����Memory�ĵ�ַ
					*@IO_DataSize		IN Memory ������ݵĴ�С 1��2��4��8��16��32bits)
					*@IO_Data			OUT �����Memory�е�ֵ
				*@return
					*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
				***************************************************************************/
				BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);

				/***************************************************************************
				*@Function				WrMemory
				*@brief					д���ڴ��ֵ
				*@author				��˶
				*@param
					*@IO_Data			IN ����Memory��ֵ
					*@IO_DataSize		IN Memory ������ݵĴ�С 1��2��4��8��16��32bits��
					*@IO_Port_Addr		IN ����Memory�ĵ�ַ
				*@return
					*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
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
