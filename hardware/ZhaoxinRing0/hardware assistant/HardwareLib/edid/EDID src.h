#pragma once
#include <algorithm>  
#include <iterator> 
#include <fstream>
#include "EDIDdefination.h"
using namespace std;

//GetEDID 错误码
#define INVALID_PARAMETER	0x02

namespace SV_ASSIST
{
	namespace Display
	{
		class _EDID_INFO
		{
		public:
			_EDID_INFO();
			~_EDID_INFO();
			/****************************************************
			*@Function				ReturnEDID
			*@brief					返回EDID的值
			*@author				王硕
			*@param
			*@return				EDID的数组
			****************************************************/
			const vector<EDID>& ReturnEDID() const;

			/****************************************************
			*@Function				UpdateData
			*@brief					更新EDID
			*@author				王硕
			*@param
			 	*@null
			*@return				无
			****************************************************/
			 VOID UpdateData();

		private:
			/****************************************************
			*@Function				GetCurrentMonitor
			*@brief					获取当前显示器的信息
			*@author				王硕
			*@param
				*@Monitor			输出显示器在电脑端对应的Device等信息
			*@return				是否成功
			****************************************************/
			BOOL GetCurrentMonitor(OUT vector<DISPLAY_DEVICE>& Monitor);

			/****************************************************
			*@Function				GetModelandDriver
			*@brief					获取显示器的Model和Driver信息
			*@author				王硕
			*@param
				*@lpDeviceid		显示器的DeviceID
				*@Model				显示器的Model
				*@Driver			显示器的Driver
			*@return				是否成功
			****************************************************/
			BOOL GetModelandDriver(
				IN  const wstring& lpDeviceid,
				OUT wstring& Model,
				OUT wstring& Driver
			);
			/****************************************************
			*@Function				GetEDID
			*@brief					根据Model和Driver定位注册表 获取当前显示器的EDID
			*@author				王硕
			*@param
				*@Model				显示器的Model
				*@Driver			显示器的Driver
				*@Model				输出缓冲区地址
				*@Driver			输出缓冲区大小
			*@return				是否成功
			****************************************************/
			BOOL GetEDID(
				_In_  const wstring Model,
				_In_  const wstring Driver,
				_Out_ PVOID buffer,
				_Inout_  PDWORD bufffersize
			);

			//释放指针
			template<class T>
			void SafeDeleteArray(T*& Array);
			template<class T>
			void SafeDeleteData(T*& Data);

			//小端大端转换
			template<class T>
			void LittleToBig(T& buf);

			explicit _EDID_INFO(const _EDID_INFO& that);
			_EDID_INFO operator=(const _EDID_INFO& that);

		private:
			vector<DISPLAY_DEVICE> Monitor;	//显示屏
			vector<DISPLAY_DEVICE> Adapter;	//适配器
			vector<EDID> Edid;
			wstring Model, Driver;
		};
	}
}
