#pragma once
#include "GenericGPU.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief ͨ��PCIE����̽���Ƿ�ΪGPU�豸
		class PciGpuDetect final : public GenericGPU
		{
		public:
			/// @brief ���캯��
			PciGpuDetect();

			/// @brief ��������
			~PciGpuDetect();

			/// @brief ��ʼ��GPU
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief ����GPU��Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;

			/// @brief ��ȡGPU��������
			/// Json��ʽ {"SocketId": 0}
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		private:
			/// @brief ��ʼ���豸
			/// @return @ref Data::ErrorType
			Data::ErrorType InitializeDevice();

			/// @brief ������PCI�����ϵ�����GPU�豸
			/// @return PCI��BDFֵ����
			std::vector<GPUDevice> FindGPUDeviceOnPCI();

			/// @brief Ѱ��Intel MCHBAR
			/// @param bus Pci��bus��
			/// @return MCHBAR�ĵ�ַ
			std::uint64_t FindIntelMCHBAR(const uint32_t& bus);
		};
	}
}
