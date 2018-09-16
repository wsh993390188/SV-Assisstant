#pragma once
#include "SuperIOStruct.h"

namespace SV_ASSIST
{
	namespace SUPERIO
	{
		/***************************************************
		*@Function					GetSuperIoChipId
		*@brief						返回Sio芯片代号
		*@author					王硕
		*@param
		*@return
		*@SIO_Model				Sio芯片代号，0为无Sio
		****************************************************/
		SIO_Model GetSuperIoChipId();

		/***************************************************
		*@Function					ReadSioPage
		*@brief						获取SuperIo的所有Page信息(SuperIoChipId=0时不可调用)
		*@author					王硕
		*@param
		*@Siodata				输入用于获取信息的数组
		*@Pages					输入由GetSuperIoChipId所确定芯片的Pages
		*@return
		*@BOOL						是否成功
		****************************************************/
		BOOL ReadSioPage(std::vector<Super_IO_Data>& Siodata, const std::vector<USHORT>& Pages);


		/***************************************************
		*@Function					GetSioSensor
		*@brief						获取Sio的Sensor信息(SuperIoChipId=0时不可调用)
		*@author					王硕
		*@param
		*@return
			*@SioSensor				Sio的Sensor信息
		****************************************************/
		const SioSensor& GetSioSensor();

		/***************************************************
		*@Function					GetSioPage
		*@brief						获取Sio的Page信息(SuperIoChipId=0时不可调用)
		*@author					王硕
		*@param
		*@return
			*@std::vector<USHORT>		Sio的Page信息
		****************************************************/
		const std::vector<USHORT>& GetSioPage(IN int Chipid);

		/***************************************************
		*@Function					UpdateData
		*@brief						更新Sio信息 每次更新有关Sio信息前需调用本函数刷新Sio信息
		*@author					王硕
		*@param
		*@return
			*@null
		****************************************************/
		void UpdateData();
	}

	namespace CPU
	{
		/***************************************************
		*@Function					GetCpuVCore
		*@brief						获取cpu的核心电压 从Sio处获取信息
		*@author					王硕
		*@param
			*@VendorID				输入PCI定义的VendorId 0x8080 0x1106 0x1022等
		*@return
			*@float					cpu核心电压值 单位V
		****************************************************/
		float GetCpuVCore(IN DWORD VendorID);
	}
}


