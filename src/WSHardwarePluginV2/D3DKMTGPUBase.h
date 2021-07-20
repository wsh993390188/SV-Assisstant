#pragma once
#include "GenericGPU.h"
#include "PCIGPUDetect.h"
#include "D3DKMTHelper.h"

namespace Hardware
{
	namespace GPU
	{
		/// @brief ͨ��D3D�ӿڻ�ȡGPU����Ϣ
		class D3DKMTGPUBase final : public GenericGPU, PciGpuDetect
		{
		public:
			/// @brief ��ʼ��GPU
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override;
			/// @brief ����GPU��Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override;

			/// @brief ��ȡGPU��������
			/// Json��ʽ {"SocketId": 0}
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override;
		private:
			/// @brief ��ʼ��D3DKMT
			/// @return �Ƿ��ʼ���ɹ�
			bool InitializeD3DKMT();

			/// @brief ����һ��GPU��������
			/// @return @ref GPUAdapter
			std::unique_ptr<GPUAdapter> CreateAdapter(_In_ PWSTR DeviceInterface, _In_ D3DKMT_HANDLE AdapterHandle, _In_ const LUID& AdapterLuid, _In_ ULONG NumberOfSegments, _In_ ULONG NumberOfNodes);

			/// @brief GPU��������������һЩGPU��ص�����
			std::map<uint64_t, std::shared_ptr<GPUAdapter>> GPUAdapters;
		};
	}
}
