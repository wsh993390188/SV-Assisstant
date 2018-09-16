#pragma once
#include <map>

class Option_Rom
{
public:
	Option_Rom();
	const std::vector<std::pair<PCI_OPTION_ROM_HEADER, PCI_OPTION_ROM_Data_Structure>>& GetPCIOptionRom() const;
	void UpdateData();
	~Option_Rom();
private:
	explicit Option_Rom(const Option_Rom& that);
	Option_Rom& operator=(const Option_Rom& that) {}
	void Scan_Option_Rom();
	void Get_Option_Rom();
private:
	std::vector<std::pair<PCI_OPTION_ROM_HEADER, PCI_OPTION_ROM_Data_Structure>> OPDB;
	std::vector<size_t> OptionRomHeader;
};

