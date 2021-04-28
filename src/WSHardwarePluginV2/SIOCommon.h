#pragma once
#include "SIOBase.h"
namespace Hardware
{
	/// @brief SuperIOģ��
	namespace SIO
	{
		/// @brief ͨ�õ�SuperIO�Ľṹ
		class SIOCommon : public SIOBase
		{
		public:
			// ͨ�� SIOBase �̳�
			virtual bool InitializeSIO(const USHORT& port) override final;
			virtual bool DeInitializeSIO(const USHORT& port) override final;
			virtual ~SIOCommon() = default;
		};
	}
}
