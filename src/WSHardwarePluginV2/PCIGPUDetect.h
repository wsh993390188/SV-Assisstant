#pragma once
#include "GenericGPU.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief 通过PCIE总线探测是否为GPU设备
		class PciGpuDetect final : public GenericGPU
		{
		public:
			/// @brief 构造函数
			PciGpuDetect();

			/// @brief 析构函数
			~PciGpuDetect();

			/// @brief 初始化GPU
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief 更新GPU信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;

			/// @brief 获取GPU基础操作
			/// Json格式 {"SocketId": 0}
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		private:
			/// @brief 初始化设备
			/// @return @ref Data::ErrorType
			Data::ErrorType InitializeDevice();

			/// @brief 查找在PCI总线上的所有GPU设备
			/// @return PCI的BDF值数组
			std::vector<GPUDevice> FindGPUDeviceOnPCI();

			/// @brief 寻找Intel MCHBAR
			/// @param bus Pci的bus号
			/// @return MCHBAR的地址
			std::uint64_t FindIntelMCHBAR(const uint32_t& bus);
		};
	}
}
