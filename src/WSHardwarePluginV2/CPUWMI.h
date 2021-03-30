#pragma once
#include <string>
#include <windows.h>

#include "WMI\WMIExtern.h"

namespace Hardware
{
	namespace CPU
	{
		class CPU_WMI : public CWMI
		{
		public:
			CPU_WMI();
		private:
			HRESULT Excute(CComPtr<IWbemServices> pSvc) override;
			explicit CPU_WMI(const CPU_WMI&) = delete;
			CPU_WMI& operator=(const CPU_WMI&) = delete;
		public:
			UINT ExtClock;
			UINT Core;
			UINT Thread;
			UINT Revision;
			UINT MaxClockSpeed;
		};
	}
}