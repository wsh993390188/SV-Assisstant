#pragma once
#include <string>
#include <vector>

namespace SV_ASSIST
{
	namespace AUDIO
	{
		struct AudioStruct
		{
			std::wstring id;
			std::wstring state;
			std::wstring name;
			std::wstring desc;
			std::wstring audioif;
		};

		void Exec();

		void UpdateData();

		const std::vector<AudioStruct>& GetOutputAudio();
	}
}