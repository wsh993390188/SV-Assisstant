#pragma once

#include <atlbase.h>
#include <vector>
#include <string>
#include <NetCon.h>
#include <locale>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>
#include "NetworkLib.h"

namespace SV_ASSIST
{
	namespace Net
	{
		class NetWork
		{
		public:
			NetWork();
			~NetWork();
			void Exec();
			BOOL EnumWlan();
			void Update();
			const std::vector<NetStruct> & GetData();

		private:
			std::wstring GetDeviceState(IN DWORD state);
			std::wstring GetDeviceMediaType(IN DWORD state);
			explicit NetWork(const NetWork& x);
			NetWork& operator=(const NetWork& x);
			HRESULT hr;
			std::vector<NetStruct> NetInfo;
			CComPtr<INetConnectionManager>  pManager;
			CComPtr<INetConnection> pConnection;
			CComPtr<IEnumNetConnection> pEnum;
			ULONG           celt;
			CComPtr<INetSharingManager> pNetSharingManager;
			CComPtr<INetConnectionProps> pProps;
			CComPtr<INetSharingConfiguration> pConfiguration;
			NETCON_PROPERTIES* NNproperties;
		};
	}
}


