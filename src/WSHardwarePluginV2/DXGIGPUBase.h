#pragma once
#include "GenericGPU.h"
#include <dxgi.h>

namespace Hardware
{
	namespace GPU
	{
		/// @brief ����DX���ص�GPU��������
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

		struct LUIDCompare
		{
			bool operator()(const LUID& k1, const LUID& k2)const
			{
				if (k1.LowPart == k2.LowPart)
					return k1.HighPart < k2.HighPart;
				else
					return k1.LowPart < k2.LowPart;
			}
		};

		class DXGIGPUBase final
		{
		public:
			/// @brief @ref DXGIGPUBase�ĵ���ģʽ
			/// @return DXGIGPUBase�ĵ���
			static DXGIGPUBase& Instance();

			/// @brief ����VID��DID��ѯGPU����Ϣ
			/// @param query GPU�Ĳ�ѯ���� @ref LUID
			/// @return @ref Data::ErrorType
			Data::ErrorType QueryGPUInfo(const LUID& query, GPUDXBaseData& DxData);
		private:
			/// @brief ��ʼ���豸
			/// @return @ref Data::ErrorType
			Data::ErrorType InitializeDevice();

			/// @brief ���캯��
			DXGIGPUBase();

			/// @brief ��������
			~DXGIGPUBase();

			/// @brief ��ʼ��DXGI��GPU��Ϣ
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize();
		private:
			/// @brief DXGIģ��ָ��
			HMODULE hDxgi;
			/// @brief CreateDXGIFactory����ָ��
			using LPCREATEDXGIFACTORY = HRESULT(WINAPI*)(REFIID riid, void** ppFactory);
			/// @brief CreateDXGIFactory��������
			LPCREATEDXGIFACTORY pCreateDXGIFactory;
			/// @brief GPU��DX�л�ȡ������
			/// First:@ref LUID
			/// Second:@ref GPUDXBaseData
			std::map <LUID, GPUDXBaseData, LUIDCompare> GPUDatas;
		};
	}
}
