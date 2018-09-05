#include "stdafx.h"
#include <sstream>
#include <memory>
#include "DiskLib.h"
#include "AtaSmart.h"

namespace SV_ASSIST
{
	namespace Storage
	{
		class DiskLibrary
		{
		public:
			static DiskLibrary* Instance() 
			{
				if (!m_disk.get())
					m_disk = std::make_shared<DiskLibrary>();
				return m_disk.get();
			}
			DiskLibrary() : m_Ata(std::make_shared<CAtaSmart>()), m_FlagChangeDisk{ FALSE }, m_FlagAdvancedDiskSearch{ FALSE }, m_FlagHideNoSmartDisk{ FALSE }, m_FlagworkaroundAdataSsd{ TRUE }, m_FlagWorkaroundHD204UI{ FALSE }
			{
				m_Ata->Init(TRUE, m_FlagAdvancedDiskSearch, &m_FlagChangeDisk, m_FlagWorkaroundHD204UI, m_FlagworkaroundAdataSsd, m_FlagHideNoSmartDisk);
				for (const auto& i : m_Ata->vars)
				{
					DISK_SMART_INFO temp = {};
					for (DWORD j =0;j < i.AttributeCount;j++)
					{
						temp.Attribute[j] = i.Attribute[j];
						temp.Threshold[j] = i.Threshold[j];
					}
					if(i.MeasuredPowerOnHours > 0)
					{
						temp.PowerOnHours = std::to_wstring(i.MeasuredPowerOnHours) + _T(" Hours");
					}
					else if (i.DetectedPowerOnHours >= 0)
					{
						temp.PowerOnHours = std::to_wstring(i.DetectedPowerOnHours) + _T(" Hours");
					}

					if (i.TotalDiskSize >= 1000)
					{
						temp.TotalDiskSize = boost::str(boost::wformat(L"%.1f GB") % (i.TotalDiskSize/1000.0f));
					}
					else
					{
						temp.TotalDiskSize = boost::str(boost::wformat(L"%.1f MB") % (i.TotalDiskSize * 1.0f));
					}

					temp.PowerOnCount = std::to_wstring(i.PowerOnCount) + _T(" Counts");
					if (i.Temperature > -300)
						temp.Temperature = std::to_wstring(i.Temperature) + _T(" ¡ãC");
					else
						temp.Temperature.clear();

					if (i.HostWrites >= 0)
					{
						if(i.HostWrites > 1024 * 1024)
							temp.HostWrites = boost::str(boost::wformat(L"%.3f PB") % (i.HostWrites / 1024.0f / 1024.0f));
						else if (i.HostWrites > 1024)
							temp.HostWrites = boost::str(boost::wformat(L"%.3f TB") % (i.HostWrites / 1024.0f));
						else
							temp.HostWrites = boost::str(boost::wformat(L"%d GB") % i.HostWrites);
					}

					if (i.HostReads >= 0)
					{
						if (i.HostReads > 1024 * 1024)
							temp.HostReads = boost::str(boost::wformat(L"%.3f PB") % (i.HostReads / 1024.0f / 1024.0f));
						else if (i.HostReads > 1024)
							temp.HostReads = boost::str(boost::wformat(L"%.3f TB") % (i.HostReads / 1024.0f));
						else
							temp.HostReads = boost::str(boost::wformat(L"%d GB") % i.HostReads);
					}

					if(i.GBytesErased >=0)
						temp.GBytesErased = boost::str(boost::wformat(L"%d GB") % (i.GBytesErased));

					if (i.NandWrites >= 0)
						temp.NandWrites = boost::str(boost::wformat(L"%d GB") % (i.NandWrites));

					if (i.BufferSize == 0xFFFF * 512)
						temp.BufferSize = _T(">= 32 MB");
					else if (i.BufferSize > 0)
						temp.BufferSize = boost::str(boost::wformat(L"%d KB") % (i.BufferSize / 1024));

					if(i.NvCacheSize > 0)
						temp.NvCacheSize = boost::str(boost::wformat(L"%d MB") % (i.NvCacheSize / 1024 / 1024));

					DWORD map = i.DriveLetterMap;
					for (int j = 0; j < 26; j++)
					{
						if (map & (1 << j))
						{
							char a = j + 'A';
							temp.LogicalDrive += boost::str(boost::wformat(L"%1%: ") % a);
						}
					}
					if (i.NominalMediaRotationRate == 1)
					{
						temp.NominalMediaRotationRate = _T("(SSD)");
					}
					else if (i.NominalMediaRotationRate > 0)
					{
						temp.NominalMediaRotationRate = std::to_wstring(i.NominalMediaRotationRate) + _T(" RPM");
					}

					if (i.IsSmartSupported)
					{
						temp.Feature += _T("S.M.A.R.T., ");
					}

					if (i.IsApmSupported)
					{
						temp.Feature += _T("APM, ");
					}

					if (i.IsAamSupported)
					{
						temp.Feature += _T("AAM, ");
					}

					
					if(i.IsLba48Supported)
					{
					temp.Feature += _T("48bit LBA, ");
					}

					if (i.IsNcqSupported)
					{
						temp.Feature += _T("NCQ, ");
					}

					if (i.IsTrimSupported)
					{
						temp.Feature += _T("TRIM, ");
					}

					if (i.IsDeviceSleepSupported)
					{
						temp.Feature += _T("DevSleep, ");
					}

					temp.Feature = temp.Feature.substr(0, temp.Feature.size() - 2);
					temp.SerialNumber = i.SerialNumber;
					temp.FirmwareRev = i.FirmwareRev;
					temp.Model = i.Model;
					temp.MaxTransferMode = i.MaxTransferMode;
					temp.CurrentTransferMode = i.CurrentTransferMode;
					temp.MajorVersion = i.MajorVersion;
					temp.MinorVersion = i.MinorVersion;
					temp.Interface = i.Interface;
					temp.Enclosure = i.Enclosure;
					temp.DeviceNominalFormFactor = i.DeviceNominalFormFactor;
					m_info.emplace_back(temp);
				}
			}
			~DiskLibrary();
			void UpdateData()
			{
				for (size_t i = 0; i < m_Ata->vars.size(); i++)
				{
					switch (m_Ata->UpdateSmartInfo(i))
					{
					case CAtaSmart::SMART_STATUS_MAJOR_CHANGE:
					case CAtaSmart::SMART_STATUS_MINOR_CHANGE:
						for (DWORD j = 0; j < m_Ata->vars[i].AttributeCount; j++)
						{
							m_info[i].Attribute[j] = m_Ata->vars[i].Attribute[j];
							m_info[i].Threshold[j] = m_Ata->vars[i].Threshold[j];
						}
						if (m_Ata->vars[i].MeasuredPowerOnHours > 0)
						{
							m_info[i].PowerOnHours = std::to_wstring(m_Ata->vars[i].MeasuredPowerOnHours) + _T(" Hours");
						}
						else if (m_Ata->vars[i].DetectedPowerOnHours >= 0)
						{
							m_info[i].PowerOnHours = std::to_wstring(m_Ata->vars[i].DetectedPowerOnHours) + _T(" Hours");
						}

						if (m_Ata->vars[i].TotalDiskSize >= 1000)
						{
							m_info[i].TotalDiskSize = boost::str(boost::wformat(L"%.1f GB") % (m_Ata->vars[i].TotalDiskSize / 1000.0f));
						}
						else
						{
							m_info[i].TotalDiskSize = boost::str(boost::wformat(L"%.1f MB") % (m_Ata->vars[i].TotalDiskSize * 1.0f));
						}

						m_info[i].PowerOnCount = std::to_wstring(m_Ata->vars[i].PowerOnCount) + _T(" Counts");
						if (m_Ata->vars[i].Temperature > -300)
							m_info[i].Temperature = std::to_wstring(m_Ata->vars[i].Temperature) + _T(" ¡ãC");
						else
							m_info[i].Temperature.clear();

						if (m_Ata->vars[i].HostWrites >= 0)
						{
							if (m_Ata->vars[i].HostWrites > 1024 * 1024)
								m_info[i].HostWrites = boost::str(boost::wformat(L"%.3f PB") % (m_Ata->vars[i].HostWrites / 1024.0f / 1024.0f));
							else if (m_Ata->vars[i].HostWrites > 1024)
								m_info[i].HostWrites = boost::str(boost::wformat(L"%.3f TB") % (m_Ata->vars[i].HostWrites / 1024.0f));
							else
								m_info[i].HostWrites = boost::str(boost::wformat(L"%d GB") % m_Ata->vars[i].HostWrites);
						}

						if (m_Ata->vars[i].HostReads >= 0)
						{
							if (m_Ata->vars[i].HostReads > 1024 * 1024)
								m_info[i].HostReads = boost::str(boost::wformat(L"%.3f PB") % (m_Ata->vars[i].HostReads / 1024.0f / 1024.0f));
							else if (m_Ata->vars[i].HostReads > 1024)
								m_info[i].HostReads = boost::str(boost::wformat(L"%.3f TB") % (m_Ata->vars[i].HostReads / 1024.0f));
							else
								m_info[i].HostReads = boost::str(boost::wformat(L"%d GB") % m_Ata->vars[i].HostReads);
						}

						if (m_Ata->vars[i].GBytesErased >= 0)
							m_info[i].GBytesErased = boost::str(boost::wformat(L"%d GB") % (m_Ata->vars[i].GBytesErased));

						if (m_Ata->vars[i].NandWrites >= 0)
							m_info[i].NandWrites = boost::str(boost::wformat(L"%d GB") % (m_Ata->vars[i].NandWrites));

						if (m_Ata->vars[i].BufferSize == 0xFFFF * 512)
							m_info[i].BufferSize = _T(">= 32 MB");
						else if (m_Ata->vars[i].BufferSize > 0)
							m_info[i].BufferSize = boost::str(boost::wformat(L"%d KB") % (m_Ata->vars[i].BufferSize / 1024));

						if (m_Ata->vars[i].NvCacheSize > 0)
							m_info[i].NvCacheSize = boost::str(boost::wformat(L"%d MB") % (m_Ata->vars[i].NvCacheSize / 1024 / 1024));
						break;
					default:
						break;
					}
				}
			}
			const std::vector<DISK_SMART_INFO>& GetDiskSMARTInfo()
			{
				return m_info;
			}
		private:
			explicit DiskLibrary(const DiskLibrary& that) {}
			DiskLibrary& operator=(const DiskLibrary& that) {}
		private:
			std::vector<DISK_SMART_INFO> m_info;
			BOOL m_FlagChangeDisk;
			BOOL m_FlagworkaroundAdataSsd;
			BOOL m_FlagAdvancedDiskSearch;
			BOOL m_FlagWorkaroundHD204UI;
			BOOL m_FlagHideNoSmartDisk;
			std::shared_ptr<CAtaSmart> m_Ata;
			static std::shared_ptr<DiskLibrary> m_disk;
		};
		std::shared_ptr<DiskLibrary> DiskLibrary::m_disk = nullptr;

		DiskLibrary::~DiskLibrary()
		{
		}
	}
}
const std::vector<SV_ASSIST::Storage::DISK_SMART_INFO>& SV_ASSIST::Storage::GetDiskSMARTInfo()
{
	return DiskLibrary::Instance()->GetDiskSMARTInfo();
}
void SV_ASSIST::Storage::UpdateData()
{
	DiskLibrary::Instance()->UpdateData();
}