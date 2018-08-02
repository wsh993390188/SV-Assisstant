#pragma once
#include "WMIExtern.h"

using namespace std;

struct WMI_DISK_INFO
{
	WMI_DISK_INFO();
	CString model;
	CString deviceid;
	CString diskSize;
	CString mediaType;
	CString interfaceTypeWmi;
	CString pnpDeviceId;
	CString firmware;
	INT physicalDriveId;
	INT scsiPort; 
	INT scsiTargetId; 
	INT scsiBus;
	
};

class CDiskQuery : public CDISKWMI
{
public:
    CDiskQuery(const wstring& wszNamespace, const wstring& wszWQLQuery);
    ~CDiskQuery(void);
private:
    HRESULT Excute(CComPtr<IWbemServices> pSvc);
	template<typename T>
	void SafeDeleteArray(T*& Array);
	template<typename T>
	void SafeDeleteData(T*& Data);
public:
	vector<CString> Disk_DeviceID;
	vector<CString> Disk_Name;
	vector<WMI_DISK_INFO> wmi_info;
private:
     wstring m_wszWQLQuery;
};


