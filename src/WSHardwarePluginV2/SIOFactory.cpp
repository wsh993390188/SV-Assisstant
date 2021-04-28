#include "stdafx.h"
#include "SIOFactory.h"

#include "ITE.h"
#include "Winbond.h"
#include "SMSC.h"
#include "Fintek.h"
#include "Nuvoton.h"

Hardware::SIO::SIOFactory::SIOFactory() : SIOPort{ std::make_pair(0x2Eu, 0x2fu), std::make_pair(0x4Eu, 0x4fu) }
{
}

Hardware::SIO::SIOFactory& Hardware::SIO::SIOFactory::Instance()
{
	static Hardware::SIO::SIOFactory tmp;
	return tmp;
}

bool Hardware::SIO::SIOFactory::DetectSIO()
{
	for (const auto& port : SIOPort)
	{
	}
}