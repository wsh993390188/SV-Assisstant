#include "stdafx.h"
#include <atlbase.h>
#include <wlanapi.h>
#include <objbase.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include "NICIphlpapi.h"

namespace
{
	constexpr auto MAX_TRIES = 3;
}

using namespace Hardware;

Hardware::NIC::NICIphlpapi::NICIphlpapi() :WSAInit(false), IfQuery(std::make_unique<NICIFEnry>()), GenericNIC()
{
	WSAInit = InitWSA();
}

Hardware::NIC::NICIphlpapi::~NICIphlpapi()
{
	if (WSAInit)
	{
		WSACleanup();
	}
}

Data::ErrorType Hardware::NIC::NICIphlpapi::Initialize(std::string& response)
{
	NICInfos.clear();
	auto ret = EnumCurrentDevice();
	if (ret == Data::ErrorType::SUCCESS)
	{
		response = BuildInitializeJson();
	}
	return ret;
}

Data::ErrorType Hardware::NIC::NICIphlpapi::Update(const std::string& Args, std::string& response)
{
	auto ret = Data::ErrorType::NOTSUPPORT;
	uint32_t NICId;
	if (ParserJson(Args, NICId))
	{
		if (IfQuery)
		{
			if (auto itr = NICInfos.find(NICId); itr != NICInfos.end())
			{
				if (IfQuery->QueryPackages(NICId, itr->second.InOctets, itr->second.OutOctets))
				{
					response = BuildElementUpdateJson(itr->second);
					if (!response.empty())
						ret = Data::ErrorType::SUCCESS;
				}
			}
		}
	}
	else
	{
		ret = Data::ErrorType::UNKNOWNJSON;
	}

	return ret;
}

std::string Hardware::NIC::NICIphlpapi::ConvertDeviceIFToString(const DWORD& state)
{
	std::string ret;
	switch (state) {
	case IF_TYPE_OTHER:
		ret = ("Other");
		break;
	case IF_TYPE_ETHERNET_CSMACD:
		ret = ("Ethernet");
		break;
	case IF_TYPE_ISO88025_TOKENRING:
		ret = ("Token Ring");
		break;
	case IF_TYPE_PPP:
		ret = ("PPP");
		break;
	case IF_TYPE_SOFTWARE_LOOPBACK:
		ret = ("Software Lookback");
		break;
	case IF_TYPE_ATM:
		ret = ("ATM");
		break;
	case IF_TYPE_IEEE80211:
		ret = ("IEEE 802.11 Wireless");
		break;
	case IF_TYPE_TUNNEL:
		ret = ("Tunnel type encapsulation");
		break;
	case IF_TYPE_IEEE1394:
		ret = ("IEEE 1394 Firewire");
		break;
	default:
		ret = "Unknown type" + std::to_string(state);
		break;
	}
	return ret;
}

Data::ErrorType Hardware::NIC::NICIphlpapi::EnumCurrentDevice()
{
	// Set the flags to pass to GetAdaptersAddresses
	constexpr auto flags = GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS;
	DWORD dwRetVal = 0;
	ULONG outBufLen = 0;
	std::unique_ptr<uint8_t[]> pAddresses{};
	Data::ErrorType ret = Data::ErrorType::NOTSUPPORT;
	auto Iterations = 0;
	do
	{
		dwRetVal = GetAdaptersAddresses(AF_UNSPEC, flags, NULL, reinterpret_cast<PIP_ADAPTER_ADDRESSES>(pAddresses.get()), &outBufLen);
		if (dwRetVal == ERROR_BUFFER_OVERFLOW)
		{
			pAddresses = std::make_unique<uint8_t[]>(outBufLen);
		}
		else {
			break;
		}
		Iterations++;
	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR)
	{
		// If successful, output some information from the data we received
		auto pCurrAddresses = reinterpret_cast<const PIP_ADAPTER_ADDRESSES>(pAddresses.get());
		while (pCurrAddresses)
		{
			if (IF_TYPE_SOFTWARE_LOOPBACK != pCurrAddresses->IfType)
			{
				NetStruct temp;
				temp.If_Index = pCurrAddresses->IfIndex;
				if (pCurrAddresses->PhysicalAddressLength != 0) {
					for (auto i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
						i++)
					{
						temp.MacAddress += Utils::to_string_hex(pCurrAddresses->PhysicalAddress[i]);
						if (i != (pCurrAddresses->PhysicalAddressLength - 1))
							temp.MacAddress += "-";
					}
				}
				auto pUnicast = pCurrAddresses->FirstUnicastAddress;
				if (pUnicast != NULL)
				{
					DWORD bufflen = INET6_ADDRSTRLEN + 1;
					auto buff = std::make_unique<TCHAR[]>(bufflen);
					while (pUnicast)
					{
						INT brtVal = WSAAddressToString(pUnicast->Address.lpSockaddr,
							pUnicast->Address.iSockaddrLength, NULL, buff.get(), &bufflen);

						if (brtVal == 0)
						{
							IPAddress IpAddr;
							switch (pUnicast->Address.lpSockaddr->sa_family)
							{
							case AF_INET:
								IpAddr.Type = IPAddress::IPAddressType::IPV4;
								break;
							case AF_INET6:
								IpAddr.Type = IPAddress::IPAddressType::IPV6;
								break;
							default:
								IpAddr.Type = IPAddress::IPAddressType::UNKNOWN;
								break;
							}
							IpAddr.Address = Utils::wstringToUtf8(reinterpret_cast<LPCTSTR>(buff.get()));
							temp.UniAddress.emplace_back(std::move(IpAddr));
						}
						else
						{
							Utils::ErrorCode("WSAAddressToString Error: ", WSAGetLastError());
						}
						pUnicast = pUnicast->Next;
					}
				}

				temp.ConnectionSpeed = pCurrAddresses->TransmitLinkSpeed;
				temp.Description = Utils::wstringToUtf8(pCurrAddresses->Description);
				temp.ConnectionName = Utils::wstringToUtf8(pCurrAddresses->FriendlyName);
				temp.InterfaceType = ConvertDeviceIFToString(pCurrAddresses->IfType);
				temp.MTU = pCurrAddresses->Mtu;
				if (IfQuery)
				{
					IfQuery->QueryGUID(temp.If_Index, temp.IfGuid);
				}
				if (pCurrAddresses->IfType == IF_TYPE_IEEE80211)
				{
					temp.IsWifi = true;
				}
				else
				{
					temp.IsWifi = false;
				}
				this->NICInfos.emplace(pCurrAddresses->IfIndex, std::move(temp));
			}

			pCurrAddresses = pCurrAddresses->Next;
		}

		if (!this->NICInfos.empty())
		{
			ret = Data::ErrorType::SUCCESS;
		}
	}
	else
	{
		Utils::ErrorCode("GetAdaptersAddresses allocate memory error ");
	}
	return ret;
}

bool Hardware::NIC::NICIphlpapi::InitWSA()
{
	WSADATA wsaData;
	int err;
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		spdlog::error("WSAStartup failed with error: {}\n", err);
		return false;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.										*/
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		spdlog::error("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return false;
	}
	return true;
}