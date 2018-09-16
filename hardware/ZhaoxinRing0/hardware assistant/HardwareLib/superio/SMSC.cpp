#include "stdafx.h"
#include "SioChip.h"
#include "..\driver\Driverdll.h"

#pragma region SMSC
SMSCXX::SMSCXX(std::pair<USHORT, std::string> chip, USHORT LPC_Address)
{
	this->sensor.ChipName = chip.second;
	this->LPC_Base = LPC_Address;
#ifdef ZX_OutputLog
	std::string ttt = boost::str(boost::format("LPC Address: 0x%lx") % LPC_Address);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
#endif // ZX_OutputLog
}
void SMSCXX::UpdateData()
{
}
#pragma endregion