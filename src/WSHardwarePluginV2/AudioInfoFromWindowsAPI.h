#pragma once
#include "GenericAudio.h"
namespace Hardware
{
	namespace Audio
	{
		/// @brief 依据Windows Audio Core API获取声卡信息
		class AudioInfoFromWindowsAPI final : public GenericAudio
		{
		public:
			/// @brief 初始化Audio Core函数
			AudioInfoFromWindowsAPI();
			/// @brief 初始化主板及BIOS信息
			/// @param[out] response 回应的Json数据
			/// 目前来说无返回数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief 更新主板信息
			/// @param[in] Args Json数据 暂不适用
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		};
	}
}
