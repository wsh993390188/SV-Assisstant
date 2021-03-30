#pragma once
namespace Hardware
{
	class WinRing0
	{
	public:
		static WinRing0& Instance();
		WinRing0();
		~WinRing0();

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

		/**************************************************
		*@Function				RdMsr
		*@brief					��ȡMSR��ֵ
		*@author				��˶
		*@param
			*@Index				IN ����MSR������
			*@Data				OUT ���MSR��ֵ��64λ
		*@return
			*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
		***************************************************/
		BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data);

		/**************************************************
		*@Function				WrMsr
		*@brief					дMSR��ֵ
		*@author				��˶
		*@param
			*@Index				IN ����MSR������
			*@Data				IN ��Ҫд��MSR��ֵ��64λ
		*@return
			*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
		***************************************************/
		BOOL WrMsr(IN DWORD Index, IN DWORD64 Data);

		/**************************************************
		*@Function				RdTsc
		*@brief					��ȡTSC��ֵ
		*@author				��˶
		*@param
			*@Data				OUT ���TSC��ֵ��64λ
		*@return
			*@BOOL				�Ƿ�ɹ� 0�ɹ� ����ֵ ʧ��
		***************************************************/
		BOOL RdTsc(OUT DWORD64& Data);
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
	private:
		HANDLE& hdevice;
	};
}
