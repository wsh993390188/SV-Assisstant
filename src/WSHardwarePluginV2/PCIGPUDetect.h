#pragma once
#include "GPUDeviceBase.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief ͨ��PCIE����̽��GPU�豸
		class PciGpuDetect
		{
		public:
			/// @brief ������PCI�����ϵ�����GPU�豸
			/// @return PCI��BDFֵ����
			bool FindGPUDeviceOnPCI();

			/// @brief ����PCI�ϵ�GPU�豸
			/// @param VendorId VendorID
			/// @param DeviceId DeviceID
			/// @param count ���ֵĴ���
			/// @param Device ���ҵ����豸
			/// @return �����Ƿ�ɹ�
			bool FindDevice(const uint32_t VendorId, const uint32_t DeviceId, const uint8_t count, GPUDevice& Device);
		private:
			/// @brief Ѱ��Intel MCHBAR
			/// @param bus Pci��bus��
			/// @return MCHBAR�ĵ�ַ
			std::uint64_t FindIntelMCHBAR(const uint32_t& bus);

			/// @brief PCI�ϵ�GPU�豸
			std::vector<GPUDevice> PciDevice;
		};
	}
}
