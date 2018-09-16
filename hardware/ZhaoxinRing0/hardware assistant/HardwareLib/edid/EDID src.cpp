#include "stdafx.h"
#include "EDID src.h"

namespace SV_ASSIST
{
	namespace Display
	{
		_EDID_INFO::_EDID_INFO() : Driver{}, Model{}
		{
		}
		_EDID_INFO::~_EDID_INFO()
		{
			// 	vector<DISPLAY_DEVICE>().swap(Monitor);
			// 	vector<DISPLAY_DEVICE>().swap(Adapter);
			// 	vector<EDID>().swap(Edid);
			// 	wstring().swap(Model);
			// 	wstring().swap(Driver);
		}
		const vector<EDID> & _EDID_INFO::ReturnEDID() const
		{
			return Edid;
		}
		BOOL _EDID_INFO::GetCurrentMonitor(OUT vector<DISPLAY_DEVICE>& Monitor)
		{
			DWORD MointorIndex = 0, AdapterIndex = 0;
			DISPLAY_DEVICE Adapter;//当前显示设备信息
			DISPLAY_DEVICE Monitortmp;//当前显示设备临时存储信息
			//初始化变量信息
			Monitor.clear();
			ZeroMemory(&Adapter, sizeof(Adapter));
			Adapter.cb = sizeof(DISPLAY_DEVICE);
			while (
				EnumDisplayDevices(
					NULL,
					AdapterIndex,
					&Adapter,
					0
				))
			{
				ZeroMemory(&Monitortmp, sizeof(Monitortmp));
				Monitortmp.cb = sizeof(DISPLAY_DEVICE);
				DWORD MointorIndex = 0;
				while (
					EnumDisplayDevices(
						Adapter.DeviceName,
						MointorIndex,
						&Monitortmp,
						NULL
					))
				{
					if (
						((Monitortmp.StateFlags & DISPLAY_DEVICE_ACTIVE) == DISPLAY_DEVICE_ACTIVE) &&
						((Monitortmp.StateFlags & DISPLAY_DEVICE_ATTACHED) == DISPLAY_DEVICE_ATTACHED)
						)
					{
						this->Monitor.emplace_back(Monitortmp);
						this->Adapter.emplace_back(Adapter);
					}
					++MointorIndex;
					ZeroMemory(&Monitortmp, sizeof(Monitortmp));
					Monitortmp.cb = sizeof(DISPLAY_DEVICE);
				}
				++AdapterIndex;
				ZeroMemory(&Adapter, sizeof(Adapter));
				Adapter.cb = sizeof(DISPLAY_DEVICE);
			}
			if (Monitor.size())
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		BOOL _EDID_INFO::GetModelandDriver(IN const wstring& lpDeviceid, OUT wstring& Model, OUT wstring& Driver)
		{
			Model = _T("");
			Driver = _T("");
			if (lpDeviceid.empty())
			{
				return FALSE;
			}
			size_t offset = 0;
			size_t lpbegin = lpDeviceid.find(_T("\\"), offset);
			if (lpbegin == NULL)
			{
				return FALSE;
			}
			offset = lpbegin + 1;
			size_t lpend = lpDeviceid.find(_T("\\"), offset);
			if (lpend == NULL)
			{
				return FALSE;
			}
			Model.assign(lpDeviceid, lpbegin + 1, lpend - lpbegin - 1);
			lpbegin = lpend;
			Driver.assign(lpDeviceid, lpbegin + 1, lpDeviceid.size());
			return TRUE;
		}

		BOOL _EDID_INFO::GetEDID(
			_In_  const wstring Model,
			_In_  const wstring Driver,
			_Out_ PVOID buffer,		//输出缓冲区地址
			_Inout_  PDWORD bufffersize //输出缓冲区大小
		)
		{
			wstring Subkey = _T("SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\");
			int ret = 0;
			if ((bufffersize == 0) || (buffer == NULL))
			{
				return INVALID_PARAMETER;
			}
			HKEY hsubkey;
			ret = RegOpenKeyEx(
				HKEY_LOCAL_MACHINE,
				Subkey.c_str(),
				NULL,
				KEY_READ,
				&hsubkey
			);
			if (ret != ERROR_SUCCESS)
			{
				cout << "OPEN FAILED" << endl;
				return FALSE;
			}
			Subkey.append(Model);
			shared_ptr<EDID> EDIDbuf(new EDID);
			bool GetEDID = FALSE;
			DWORD dwSubKeyCnt;          // 子键的数量  
			DWORD dwSubKeyNameMaxLen;   // 子键名称的最大长度(不包含结尾的null字符)  
			DWORD dwKeyValueCnt;        // 键值项的数量  
			DWORD dwKeyValueNameMaxLen; // 键值项名称的最大长度(不包含结尾的null字符)  
			DWORD dwKeyValueDataMaxLen; // 键值项数据的最大长度(in bytes)  
			DWORD SubkeyCount = 0;
			DWORD SubKeyNameMaxLen = 0;
			ret = RegQueryInfoKey(hsubkey, NULL, NULL, NULL, &dwSubKeyCnt, &dwSubKeyNameMaxLen, NULL, &dwKeyValueCnt, &dwKeyValueNameMaxLen, &dwKeyValueDataMaxLen, NULL, NULL);
			if (ret != ERROR_SUCCESS) // Error  
			{
				cout << "RegQueryInfoKey FAILED" << endl;
				return FALSE;
			}
			SubKeyNameMaxLen = dwSubKeyNameMaxLen + 1;
			SubkeyCount = dwSubKeyCnt;
			unique_ptr<TCHAR[]> lpSubkeyName(new TCHAR[SubKeyNameMaxLen]);
			//LPWSTR lpSubkeyName = new TCHAR[SubKeyNameMaxLen];
			DWORD lpNamelen = SubKeyNameMaxLen;
			for (UINT Index = 0; Index < SubkeyCount; ++Index)
			{
				if (GetEDID)
				{
					break;
				}
				HKEY EnumSbukey;
				::ZeroMemory(lpSubkeyName.get(), lpNamelen);
				lpNamelen = SubKeyNameMaxLen;
				if (RegEnumKeyEx(hsubkey, Index, lpSubkeyName.get(), &lpNamelen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
				{
					if ((RegOpenKeyEx(hsubkey, lpSubkeyName.get(), NULL, KEY_READ, &EnumSbukey) == ERROR_SUCCESS) && (wcscmp(lpSubkeyName.get(), Model.c_str()) == 0))
					{
						ret = RegQueryInfoKey(EnumSbukey, NULL, NULL, NULL, &dwSubKeyCnt, &dwSubKeyNameMaxLen, NULL, &dwKeyValueCnt, &dwKeyValueNameMaxLen, &dwKeyValueDataMaxLen, NULL, NULL);
						if (ret != ERROR_SUCCESS) // Error  
						{
#ifdef ZX_OutputLog
							Logger::Instance()->OutputLogInfo(el::Level::Debug, "RegQueryInfoKey FAILED");
#endif
							return FALSE;
						}
						unique_ptr<TCHAR[]> lpDisplayKeyName(new TCHAR[dwSubKeyNameMaxLen + 1]);
						//LPWSTR lpDisplayKeyName = new TCHAR[dwSubKeyNameMaxLen + 1];
						DWORD DisplayNamelen = dwSubKeyNameMaxLen + 1;
						HKEY hdevicekey;
						for (UINT DisplayIndex = 0; DisplayIndex < dwSubKeyCnt; ++DisplayIndex)
						{
							if (RegEnumKeyEx(EnumSbukey, DisplayIndex, lpDisplayKeyName.get(), &DisplayNamelen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
							{
								if (RegOpenKeyEx(EnumSbukey, lpDisplayKeyName.get(), NULL, KEY_READ, &hdevicekey) == ERROR_SUCCESS)
								{
									DWORD Driversize;
									if (RegQueryValueEx(hdevicekey, _T("Driver"), NULL, NULL, NULL, &Driversize) == ERROR_SUCCESS)
									{
										LPBYTE Driverstr = new BYTE[Driversize];
										if ((RegQueryValueEx(hdevicekey, _T("Driver"), NULL, NULL, Driverstr, &Driversize) == ERROR_SUCCESS) && (wcscmp((LPCTSTR)Driverstr, Driver.c_str()) == 0))
										{
											HKEY EDIDKey;
											if (RegOpenKeyEx(hdevicekey, _T("Device Parameters"), NULL, KEY_READ, &EDIDKey) == ERROR_SUCCESS)
											{
												::ZeroMemory(EDIDbuf.get(), *bufffersize);
												if (RegQueryValueEx(EDIDKey, _T("EDID"), NULL, NULL, (LPBYTE)EDIDbuf.get(), bufffersize) == ERROR_SUCCESS)
												{
#ifdef ZX_OutputLog
													Logger::Instance()->OutputLogInfo(el::Level::Debug, "Get Edid Info");
#endif
													memcpy_s(buffer, *bufffersize, EDIDbuf.get(), sizeof(*EDIDbuf));
													GetEDID = TRUE;
												}
												else if (RegQueryValueEx(EDIDKey, _T("BAD_EDID"), NULL, NULL, (LPBYTE)EDIDbuf.get(), bufffersize) == ERROR_SUCCESS)
												{
#ifdef ZX_OutputLog
													Logger::Instance()->OutputLogInfo(el::Level::Debug, "Get Bad Edid Info");
#endif
													::ZeroMemory(buffer, *bufffersize);
												}
											}
										}
										SafeDeleteArray(Driverstr);
									}
									RegCloseKey(hdevicekey);
									::ZeroMemory(lpDisplayKeyName.get(), sizeof(lpDisplayKeyName));
									DisplayNamelen = dwSubKeyNameMaxLen + 1;
								}
							}
						}
						RegCloseKey(EnumSbukey);
					}
				}
			}
			RegCloseKey(hsubkey);
			return GetEDID;
		}


		template<class T>
		inline void _EDID_INFO::SafeDeleteArray(T *& Array)
		{
			if (Array)
			{
				delete[] Array;
				Array = NULL;
			}
		}

		template<class T>
		inline void _EDID_INFO::SafeDeleteData(T *& Data)
		{
			if (Data)
			{
				delete Data;
				Data = NULL;
			}
		}

		template<class T>
		void _EDID_INFO::LittleToBig(T & buf)
		{
			switch (sizeof(T))
			{
			case 2:
				buf = ((buf & 0xFF) << 8) | ((buf & 0xFF00) >> 8);
				break;
			case 4:
				buf = ((buf & 0xFF) << 24) | (((buf & 0xFF00) >> 8)) | (((buf & 0xFF00) >> 8)) | ((buf & 0xFF00) >> 24);
				break;
			case 1:
			default:
				break;
			}
		}

		VOID _EDID_INFO::UpdateData()
		{
			EDID buffer = {};
			Edid.clear();
			DWORD ByteRead = sizeof(EDID);
			Model = _T("");
			Driver = _T("");
#ifdef ZX_OutputLog
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "********** Edid info **********");
#endif
			GetCurrentMonitor(Monitor);
			for (const auto& i : Monitor)
			{
				GetModelandDriver(i.DeviceID, Model, Driver);
#ifdef ZX_OutputLog
				std::wstring temp = L"Search Model: " + Model + L" Driver:" + Driver;
				Logger::Instance()->OutputLogInfo(el::Level::Debug, temp);
#endif
				if (GetEDID(Model, Driver, &buffer, &ByteRead))
					Edid.emplace_back(buffer);
#ifdef ZX_OutputLog
				Logger::Instance()->OutputLogInfo(el::Level::Debug, L"End" + temp);
#endif
				Model.clear();
				Driver.clear();
			}
#ifdef ZX_OutputLog
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "********** End Edid info **********\n");
#endif
		}
	}
}