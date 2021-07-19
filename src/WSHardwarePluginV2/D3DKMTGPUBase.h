#pragma once
#include "GenericGPU.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief 通过D3D接口获取GPU的信息
		class D3DKMTGPUBase final : public GenericGPU
		{
		public:
			/// @brief 初始化GPU
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override;
			/// @brief 更新GPU信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override;

			/// @brief 获取GPU基础操作
			/// Json格式 {"SocketId": 0}
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override;
		private:
			/// @brief 初始化D3DKMT
			/// @return 是否初始化成功
			bool InitializeD3DKMT();
		};
	}
}
