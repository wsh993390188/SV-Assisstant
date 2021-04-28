#pragma once
#include "SIOBase.h"
namespace Hardware
{
	/// @brief SuperIO模块
	namespace SIO
	{
		/// @brief SuperIO的生产者
		class SIOFactory final
		{
			/// @brief 构造函数
			SIOFactory();
			SIOFactory(const SIOFactory&) = delete;
			SIOFactory& operator=(const SIOFactory&) = delete;
		public:
			/// @brief 单例的入口函数
			/// @return
			static SIOFactory& Instance();

			/// @brief 探测SIO的所属厂商
			/// @return 是否存在SIO
			bool DetectSIO();
		private:
			/// @brief SIO的进入端口
			std::array<std::pair<USHORT, USHORT>, 2> SIOPort;
			std::unique_ptr<SIOBase> SuperIO;
		};
	}
}
