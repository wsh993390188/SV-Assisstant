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
		class NetWorkInterface
		{
		public:
			virtual ~NetWorkInterface();
			virtual void Exec() = 0;
			virtual void Update() = 0;
			virtual const std::vector<NetStruct> & GetData() = 0;
		protected:
			NetWorkInterface();
		private:
			explicit NetWorkInterface(const NetWorkInterface& x);
			NetWorkInterface& operator=(const NetWorkInterface& x);
		};

		class NetWork : public NetWorkInterface
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


