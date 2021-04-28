#pragma once
#include "SIOBase.h"
namespace Hardware
{
	namespace SIO
	{
		class SMSC :
			public SIOBase
		{
			// ͨ�� SIOBase �̳�
			virtual bool InitializeSIO(const USHORT& port) override;
			virtual bool DeInitializeSIO(const USHORT& port) override;
		};
	}
}
