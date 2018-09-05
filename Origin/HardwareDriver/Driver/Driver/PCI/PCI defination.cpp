#include "PCI defination.h"

BOOL openpci(const TCHAR * filename, ios_base::openmode mode, const USHORT VendorID, string& VendorString)
{
	PTCHAR tmp = new TCHAR[1024];
	ULONG tmpVendorId = 0;
	size_t ipos = 0;
	string tmpVendorString = _T("");
	string FileString = _T("");
	::ZeroMemory(tmp, 1024);
	ifstream in;
	in.open(filename, mode);
	if (!in.is_open())
	{
		OutputDebugString("打开文件失败");
		return FALSE;
	}
	while (in.getline(tmp, 1024))
	{
		if (*tmp == ';' || *tmp == '[' || *tmp == '#' || *tmp == NULL)		continue;
		FileString = tmp;
		if (*tmp == '\t')
		{
			continue;
			// Its DeviceID or SubVendorID
		}
		sscanf_s(tmp, "%x  ", &tmpVendorId);
		ipos = FileString.find("  ");
		tmpVendorString = FileString.substr(ipos + 1, FileString.size());
		if (tmpVendorId == VendorID)
		{
			VendorString = tmpVendorString;
			goto _exit;
		}
		tmpVendorString = _T("");
		FileString = _T("");
		::ZeroMemory(tmp, 1024);
	}
_exit:
	in.close();
	if (tmp)
	{
		delete[] tmp;
		tmp = NULL;
	}
	if (VendorString == _T(""))		return FALSE;
	return TRUE;
}



