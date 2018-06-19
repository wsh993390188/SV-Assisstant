#pragma once

#include <atlbase.h>
#include <NetCon.h>
#include <locale>

#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"ole32.lib")

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
			virtual const std::vector<std::wstring> & GetData() = 0;
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

			void Update();
			const std::vector<std::wstring> & GetData();

		private:
			std::vector<std::wstring> t;
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


