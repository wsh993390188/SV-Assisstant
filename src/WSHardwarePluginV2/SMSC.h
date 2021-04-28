#pragma once
#include "SIOBase.h"
namespace Hardware
{
	namespace SIO
	{
		class SMSC :
			public SIOBase
		{
			// Í¨¹ý SIOBase ¼Ì³Ð
			virtual bool InitializeSIO(const USHORT& port) override;
			virtual bool DeInitializeSIO(const USHORT& port) override;
		};
	}
}
