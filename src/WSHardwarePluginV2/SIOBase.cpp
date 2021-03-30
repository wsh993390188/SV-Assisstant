#include "stdafx.h"
#include "SIOBase.h"

namespace
{
	constexpr auto DEVCIE_SELECT_REGISTER = 0x07u;
}

bool Hardware::SIO::SIOBase::SelectPage(const USHORT& port, const USHORT& data, const UCHAR& logicalDeviceNumber)
{
	if (IoPtr.WriteByte(port, DEVCIE_SELECT_REGISTER))
	{
		return IoPtr.WriteByte(data, logicalDeviceNumber);
	}
	return false;
}

bool Hardware::SIO::SIOBase::SIOReadWord(const USHORT& port, const USHORT& data, const DWORD& offset, DWORD& output)
{
	DWORD temp1 = 0, temp2 = 0;
	if (SIOReadByte(port, data, offset, temp1))
	{
		if (SIOReadByte(port, data, offset + 1, temp2))
		{
			output = temp1 << 8 | temp2;
			return true;
		}
	}
	return false;
}

bool Hardware::SIO::SIOBase::SIOReadByte(const USHORT& port, const USHORT& data, const DWORD& offset, DWORD& output)
{
	if (IoPtr.WriteByte(port, offset))
	{
		return IoPtr.ReadByte(data, output);
	}
	return false;
}

Hardware::SIO::SIOBase::SIOBase() : SIOPort{ std::make_pair(0x2Eu, 0x2fu), std::make_pair(0x4Eu, 0x4fu) }, active(false),
CHIP_ID_REGISTER{ 0x20u }, CHIP_REVISION_REGISTER{ 0x21u }, BASE_ADDRESS_REGISTER{ 0x60u }
{
}

bool Hardware::SIO::SIOBase::IsActive() const
{
	return active;
}