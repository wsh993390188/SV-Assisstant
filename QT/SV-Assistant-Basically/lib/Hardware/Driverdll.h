#include <windows.h>
#include "Ring0Defination.h"

namespace SV_ASSIST
{
    namespace Ring0
    {
        /**********************************************************************
        *@Function				RdIOPort
        *@brief					读取IO Port的值
        *@author				王硕
        *@param
            *@IO_Port_Addr		IN IO Port的地址
            *@IO_DataSize		IN IO 输出数据的大小 1、2、4（8、16、32bits）
            *@IO_Data			OUT 输出的IO Port中的值
        *@return
			*@BOOL				是否成功 0成功 其他值 失败
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
        *@Function				RdMsrTx
        *@brief					获取MSR的值
        *@author				王硕
        *@param
            *@Index				IN 输入MSR的索引
            *@threadAffinityMask设置线程所运行的核心
            *@Data				OUT 输出MSR的值，64位
        *@return
            *@BOOL				是否成功 0成功 其他值 失败
        ***************************************************/
        BOOL RdMsrTx(IN DWORD Index, OUT DWORD64& Data, IN DWORD threadAffinityMask);
        /**************************************************
        *@Function				WrMsrTx
        *@brief					写MSR的值
        *@author				王硕
        *@param
            *@Index				IN 输入MSR的索引
            *@threadAffinityMask设置线程所运行的核心
            *@Data				IN 需要写入MSR的值，64位
        *@return
            *@BOOL				是否成功 0成功 其他值 失败
        ***************************************************/
        BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD threadAffinityMask);
		/**************************************************
		*@Function				RdTscTx
		*@brief					写MSR的值
		*@author				王硕
		*@param
			*@threadAffinityMask设置线程所运行的核心
			*@Data				OUT 输出MSR的值，64位
		*@return
			*@BOOL				是否成功 0成功 其他值 失败
		***************************************************/
		BOOL RdTscTx(OUT DWORD64 & Data, IN DWORD threadAffinityMask);
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
		*@Function				RdTsc
		*@brief					获取TSC的值
		*@author				王硕
		*@param
			*@Data				OUT 输出TSC的值，64位
		*@return
			*@BOOL				是否成功 0成功 其他值 失败
		***************************************************/
		BOOL RdTsc(OUT DWORD64 & Data);
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
        BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);

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
        BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);

        /**************************************************
        *@Function					GetECData
        *@brief						获取EC的信息
        *@author					王硕
        *@param
			*@Embedded_Controller_Data	OUT EC 256字节的信息
        *@return
			*@BOOL					读取是否成功
        ***************************************************/
        BOOL GetECData(Embedded_Controller_Data& EC_Data);

        /***************************************************
        *@Function					SetECData
        *@brief						设置EC的信息
        *@author					王硕
        *@param
			*@EC_Addr				IN 要设置的EC的地址
			*@EC_Write_Data			IN 要设置的EC的数据
        *@return
			*@BOOL					设置是否成功
        ****************************************************/
        BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);

		/***************************************************
		*@Function					ReadSMbusByByte
		*@brief						以Byte读取SMBUS上Device设备信息
		*@author					王硕
		*@param
			*@SmbusBase				IN SMBUS设备地址，由GetSMbusBaseAddr函数所确定
			*@SlaveAddress			IN 从设备地址
			*@offset				IN 获取信息的偏移量
			*@data					OUT 一个Byte的数据
		*@return
			*@BOOL					设置是否成功
		****************************************************/
		BOOL ReadSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, BYTE & data);

		/***************************************************
		*@Function					ReadSMbusByWord
		*@brief						以WORD读取SMBUS上Device设备信息
		*@author					王硕
		*@param
			*@SmbusBase				IN SMBUS设备地址，由GetSMbusBaseAddr函数所确定
			*@SlaveAddress			IN 从设备地址
			*@offset				IN 获取信息的偏移量
			*@data					OUT 两个Bytes的数据
		*@return
			*@BOOL					设置是否成功
		****************************************************/
		BOOL ReadSMbusByWord(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, WORD & data);

		/***************************************************
		*@Function					WriteSMbusByByte
		*@brief						以Byte写入SMBUS上Device设备
		*@author					王硕
		*@param
			*@SmbusBase				IN SMBUS设备地址，由GetSMbusBaseAddr函数所确定
			*@SlaveAddress			IN 从设备地址
			*@offset				IN 获取信息的偏移量
			*@data					IN 要写入的一个B的数据
		*@return
		****************************************************/
		void WriteSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, const DWORD data);

		/***************************************************
		*@Function					Scan_SMBUS_DEVICE
		*@brief						扫描SMBUS上可用设备
		*@author					王硕
		*@param
			*@SmbusBase				IN SMBUS设备地址，由GetSMbusBaseAddr函数所确定
		*@return
			*@vector<USHORT>		从设备信息的数组
		****************************************************/
		const std::vector<USHORT> Scan_SMBUS_DEVICE(const USHORT SmbusBase);

        /***************************************************
        *@Function					GetSMbusBaseAddr
        *@brief						获取SMBUS基地址
        *@author					王硕
        *@param
			*@VendorID				IN Chip的厂商名
			*@SMbusBaseAddress		OUT SMBUS基地址
		*@return
			*@BOOL					设置是否成功
        ****************************************************/
        BOOL GetSMbusBaseAddr(const USHORT VendorID, USHORT& SMbusBaseAddress);

        /***************************************************
        *@Function					GetPCIVendorID
        *@brief						获取Chip厂商名
        *@author					王硕
        *@param
        *@return
        *@USHORT					厂商pci代号，如1022、8086、1106
        ****************************************************/
        const USHORT GetPCIVendorID();

		/***************************************************
		*@Function					GetAllPciInfo
		*@brief						获取全部的PCI配置空间信息
		*@author					王硕
		*@param
		*@return
			*@Pci_All_Config_Space	Pci设备
		****************************************************/
		const Pci_All_Config_Space& GetAllPciInfo();
    }
}
