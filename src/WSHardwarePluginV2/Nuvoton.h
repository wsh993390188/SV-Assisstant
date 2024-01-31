#pragma once
#include "SIOCommon.h"
namespace Hardware
{
	namespace SIO
	{
		/// @brief Nuvoton SIO
		class Nuvoton : public SIOCommon
		{
		public:
			/// @brief 初始化Novoton
			Nuvoton();

		private:
			/// @brief 禁用IO的锁
			/// @param port SIO的接口地址
			/// @param data SIO的数据地址
			void NuvotonDisableIOSpaceLock(const USHORT& port, const USHORT& data);
		};
	}
}
