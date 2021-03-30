#pragma once
#include "SIOBase.h"
namespace Hardware
{
	namespace SIO
	{
		/// @brief Nuvoton SIO
		class Nuvoton : public SIOBase
		{
		public:
			/// @brief 初始化Novoton
			Nuvoton();

			/// @brief 初始化SIO
			/// @param port SIO的地址
			/// @return 是否成功
			bool InitializeSIO(const USHORT& port) override;

			/// @brief 反初始化SIO，用完就要销毁，不可一直占用
			/// @param port SIO的地址
			/// @return 是否成功
			bool DeInitializeSIO(const USHORT& port) override;
		private:
			/// @brief 禁用IO的锁
			/// @param port SIO的接口地址
			/// @param data SIO的数据地址
			void NuvotonDisableIOSpaceLock(const USHORT& port, const USHORT& data);
		};
	}
}
