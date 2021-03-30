#pragma once
#include "GenericMotherBroad.h"
namespace Hardware
{
	namespace MotherBroad
	{
		class MotherBroadFromSMBIOS : public GenericMotherBroad
		{
		public:
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
		};
	}
}
