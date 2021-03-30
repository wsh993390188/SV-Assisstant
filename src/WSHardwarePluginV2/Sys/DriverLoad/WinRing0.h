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

		/**************************************************
		*@Function				RdMsr
		*@brief					获取MSR的值
		*@author				王硕
		*@param
			*@Index				IN 输入MSR的索引
			*@Data				OUT 输出MSR的值，64位
		*@return
			*@BOOL				是否成功 0成功 其他值 失败
		***************************************************/
		BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data);

		/**************************************************
		*@Function				WrMsr
		*@brief					写MSR的值
		*@author				王硕
		*@param
			*@Index				IN 输入MSR的索引
			*@Data				IN 需要写入MSR的值，64位
		*@return
			*@BOOL				是否成功 0成功 其他值 失败
		***************************************************/
		BOOL WrMsr(IN DWORD Index, IN DWORD64 Data);

		/**************************************************
		*@Function				RdTsc
		*@brief					获取TSC的值
		*@author				王硕
		*@param
			*@Data				OUT 输出TSC的值，64位
		*@return
			*@BOOL				是否成功 0成功 其他值 失败
		***************************************************/
		BOOL RdTsc(OUT DWORD64& Data);
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
	private:
		HANDLE& hdevice;
	};
}
