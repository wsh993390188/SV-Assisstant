#pragma once
#include "GenericMotherBroad.h"
#include "SIOBase.h"
namespace Hardware
{
	namespace MotherBroad
	{
		class MotherBroadFromSMBIOS final : public GenericMotherBroad
		{
		public:
			/// @brief 主板的构造函数
			MotherBroadFromSMBIOS();
			/// @brief 初始化主板及BIOS信息
			/// @param[out] response 回应的Json数据
			/// 目前来说无返回数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief 更新主板信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			std::unique_ptr<SIO::SIOBase> SuperIO;
		};
	}
}
