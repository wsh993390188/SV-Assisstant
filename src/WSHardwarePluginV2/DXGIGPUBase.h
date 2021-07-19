#pragma once
#include "GenericGPU.h"
#include <dxgi.h>

namespace Hardware
{
	namespace GPU
	{
		/// @brief 根据DX返回的GPU基础数据
		struct GPUDXBaseData
		{
			std::wstring Description;
			uint32_t SubSysId;
			uint32_t Revision;
			size_t DedicatedVideoMemory;
			size_t DedicatedSystemMemory;
			size_t SharedSystemMemory;
			LUID AdapterLuid;
		};

		class DXGIGPUBase final
		{
		public:
			/// @brief @ref DXGIGPUBase的单例模式
			/// @return DXGIGPUBase的单例
			static DXGIGPUBase& Instance();

			/// @brief 根据VID和DID查询GPU的信息
			/// @param query GPU的查询数据 @ref GPUQuery
			/// @return @ref Data::ErrorType
			Data::ErrorType QueryGPUInfo(const GPUQuery& query, GPUDXBaseData& DxData);
		private:
			/// @brief 初始化设备
			/// @return @ref Data::ErrorType
			Data::ErrorType InitializeDevice();

			/// @brief 构造函数
			DXGIGPUBase();

			/// @brief 析构函数
			~DXGIGPUBase();

			/// @brief 初始化DXGI中GPU信息
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize();
		private:
			/// @brief DXGI模块指针
			HMODULE hDxgi;
			/// @brief CreateDXGIFactory函数指针
			using LPCREATEDXGIFACTORY = HRESULT(WINAPI*)(REFIID riid, void** ppFactory);
			/// @brief CreateDXGIFactory函数对象
			LPCREATEDXGIFACTORY pCreateDXGIFactory;
			/// @brief GPU从DX中获取的数据
			/// First:@ref GPUQuery
			/// Second:@ref GPUDXBaseData
			std::map < GPUQuery, GPUDXBaseData> GPUDatas;
		};
	}
}
