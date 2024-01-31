#pragma once
#include "GenericWinBio.h"
namespace Hardware
{
	namespace WinBio
	{
		/// @brief WinBio的构造实现函数
		class WinBioImpl :
			public GenericWinBio
		{
		public:
			/// @brief 构造函数
			WinBioImpl();

			/// @brief 初始化Win10的生物传感器
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief 更新Win10的生物传感器信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			/// @brief WinBio设备序号
			uint32_t Number;

			/// @brief 枚举Winbio可用的设备
			/// @param[in] Type 设备类型
			/// @return 结果
			bool EnumerateSensors(const uint32_t& Type);
		};
	}
}
