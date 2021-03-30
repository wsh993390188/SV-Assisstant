#pragma once
#include "SIOBase.h"
namespace Hardware
{
	namespace SIO
	{
		/// @brief ITE SIO探测器
		class ITE : public SIOBase
		{
		public:
			/// @brief 初始化ITE SIO
			ITE();

			/// @brief 初始化SIO
			/// @param port SIO的地址
			/// @return 是否成功
			bool InitializeSIO(const USHORT& port) override;

			/// @brief 反初始化SIO，用完就要销毁，不可一直占用
			/// @param port SIO的地址
			/// @return 是否成功
			bool DeInitializeSIO(const USHORT& port) override;
		private:
			/// @brief ITE Enrironment register
			const UCHAR IT87_ENVIRONMENT_CONTROLLER;
			/// @brief ITE Chip version register
			const UCHAR IT87_CHIP_VERSION_REGISTER;
		};
	}
}