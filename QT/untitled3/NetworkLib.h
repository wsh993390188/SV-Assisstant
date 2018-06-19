#pragma once

#include <vector>
#include <string>

namespace SV_ASSIST
{
	namespace Net
	{
		void Exec();

		void Update();

		const std::vector<std::wstring>& GetData();
	}
}
