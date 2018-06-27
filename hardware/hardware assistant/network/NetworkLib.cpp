#include "stdafx.h"
#include "NetWork.h"
#include "NetworkLib.h"

namespace SV_ASSIST
{
	namespace Net
	{
		static std::shared_ptr<NetWorkInterface> NetInstance = std::make_shared<NetWork>();

		void Exec()
		{
			NetInstance->Exec();
		}
		
 		void Update()
 		{
 			NetInstance->Update();
		}

		const std::vector<NetStruct>& GetData()
		{
			return NetInstance->GetData();
		}
	}
}