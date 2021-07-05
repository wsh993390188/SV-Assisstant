#include "stdafx.h"
#include "XMLConfig.h"

extern HMODULE g_hModule;

bool Hardware::XMLConfig::GetConfigXmlFromResource(std::string& configcontent, const size_t ResID, const std::wstring& ResourceType)
{
	HGLOBAL hGlobal = NULL;
	bool ret = false;
	do
	{
		HRSRC hRsrc = FindResourceEx(g_hModule, ResourceType.c_str(), MAKEINTRESOURCE(ResID), 0);
		if (NULL == hRsrc)
			break;
		DWORD dwSize = SizeofResource(g_hModule, hRsrc);
		if (0 == dwSize)
			break;
		hGlobal = LoadResource(g_hModule, hRsrc);
		if (NULL == hGlobal)
			break;
		LPVOID pBuffer = LockResource(hGlobal);
		if (NULL == pBuffer)
			break;
		configcontent.resize(dwSize);
		memcpy_s(configcontent.data(), configcontent.size(), pBuffer, dwSize);
		ret = true;
	} while (false);

	if (hGlobal)
		FreeResource(hGlobal);
	return ret;
}

Hardware::XMLConfig::QueryInfo::operator bool() const
{
	return !(Manufacture.empty() || Model.empty() || Family.empty());
}

Hardware::XMLConfig::ExtendedInfo::operator bool() const
{
	return !Element.empty();
}