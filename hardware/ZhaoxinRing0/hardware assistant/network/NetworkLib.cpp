#include "stdafx.h"
#include "NetWork.h"
#include "NetworkLib.h"

namespace SV_ASSIST
{
	namespace Net
	{
		class NetInterface
		{
		public:
			static NetInterface* Instance()
			{
				if (!NetInstance.get())
					NetInstance = std::make_shared<NetInterface>();
				return NetInstance.get();
			}
			void Update()
			{
				temp->Update();
			}
			const std::vector<NetStruct> & GetData()
			{
				return temp->GetData();
			}
			NetInterface() : temp(std::make_shared<NetWork>()) {}
		private:
			explicit NetInterface(const NetInterface& x);
			NetInterface& operator=(const NetInterface& x);
			std::shared_ptr<NetWork> temp;
			static std::shared_ptr<NetInterface> NetInstance;
		};

		std::shared_ptr<NetInterface> NetInterface::NetInstance = nullptr;
		
 		void Update()
 		{
			NetInterface::Instance()->Update();
		}

		const std::vector<NetStruct>& GetData()
		{
			return NetInterface::Instance()->GetData();
		}
	}
}