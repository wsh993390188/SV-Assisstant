#pragma once
namespace SV_ASSIST
{
	namespace AUDIO
	{
		struct AudioStruct
		{
			AudioStruct();
			explicit AudioStruct(std::wstring id,
				std::wstring state,
				std::wstring name,
				std::wstring desc,
				std::wstring audioif);
			explicit AudioStruct(const AudioStruct& that);
			AudioStruct& operator=(const AudioStruct& that);

			std::wstring id;
			std::wstring state;
			std::wstring name;
			std::wstring desc;
			std::wstring audioif;
		};
		
		void UpdateData();

		const std::vector<AudioStruct>& GetOutputAudio();
	}
}