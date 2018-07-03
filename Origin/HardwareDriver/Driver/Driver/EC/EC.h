#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取Embedded Controller的信息
**************************************************/
#include "../MyDriverClass.h"
#define		EC_SC					0x66
#define		EC_DATA					0x62
#define		RD_EC					0x80
#define		WR_EC					0x81
#define		BE_EC					0x82
#define		BD_EC					0x83
#define		QR_EC					0x84

//EC Data 共256个字节 具体每一位代表什么需要相应的spec来确定
typedef struct _Embedded_Controller_Data
{
	BYTE	Data[256];
}Embedded_Controller_Data;

/**************************************************
*@Function					GetECData
*@brief						获取EC的信息
*@author					王硕
*@param
	*@Embedded_Controller_Data	OUT EC 256字节的信息
*@return
	*@BOOL						读取是否成功
***************************************************/
BOOL GetECData(Embedded_Controller_Data& EC_Data);

/***************************************************
*@Function					SetECData
*@brief						设置EC的信息
*@author					王硕
*@param
	*@EC_Addr					IN 要设置的EC的地址
	*@EC_Write_Data				IN 要设置的EC的数据
*@return
*@BOOL						设置是否成功
****************************************************/
BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);