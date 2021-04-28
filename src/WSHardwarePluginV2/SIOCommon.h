#pragma once
#include "SIOBase.h"
namespace Hardware
{
	/// @brief SuperIO模块
	namespace SIO
	{
		/// @brief 通用的SuperIO的结构
		class SIOCommon : public SIOBase
		{
		public:
			// 通过 SIOBase 继承
			virtual bool InitializeSIO(const USHORT& port) override final;
			virtual bool DeInitializeSIO(const USHORT& port) override final;
			virtual ~SIOCommon() = default;
		};
	}
}
