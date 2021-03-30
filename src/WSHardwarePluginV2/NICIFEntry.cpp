#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "NICIFEntry.h"

bool Hardware::NIC::NICIFEnry::QueryPackages(const uint32_t IFIndex, uint64_t& InOctets, uint64_t& OutOctets)
{
	MIB_IF_ROW2 IfRow{};
	IfRow.InterfaceIndex = IFIndex;
	auto retVal = GetIfEntry2(&IfRow);
	if (retVal == NO_ERROR)
	{
		InOctets = IfRow.InOctets;
		OutOctets = IfRow.OutOctets;
		return true;
	}
	else
	{
		Utils::ErrorCode("GetIfEntry2 error: ", retVal);
	}
	return false;
}

bool Hardware::NIC::NICIFEnry::QueryGUID(const uint32_t IFIndex, GUID& guid)
{
	MIB_IF_ROW2 IfRow{};
	IfRow.InterfaceIndex = IFIndex;
	auto retVal = GetIfEntry2(&IfRow);
	if (retVal == NO_ERROR)
	{
		guid = IfRow.InterfaceGuid;
		return true;
	}
	else
	{
		Utils::ErrorCode("GetIfEntry2 error: ", retVal);
	}
	return false;
}