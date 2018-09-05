#include "stdafx.h"
#include "Iusb.h"
#include "UsbHub.h"

namespace SV_ASSIST
{
    namespace USB
    {
		class USBLib
		{
		public:
			static USBLib* Instance()
			{
				if (!m_usb.get())
					m_usb = std::make_shared<USBLib>();
				return m_usb.get();
			}
			USBLib() : m_usbinfo(std::make_shared<_EnumUsbHub>())
			{}
			const vector<USB_ROOT_HUB>& GetAllUsbinfo() const
			{
				return m_usbinfo->GetAllUsbinfo();
			}
			const std::vector<DEVICE_INFO_NODE>& GetAllUsbDeviceInfo() const
			{
				return m_usbinfo->GetAllUsbDeviceInfo();
			}
			const std::vector<DEVICE_INFO_NODE>& GetAllUsbHubInfo() const
			{
				return m_usbinfo->GetAllUsbHubInfo();
			}
			void UpdateData()
			{
				m_usbinfo->Updatedata();
			}
		private:
			USBLib(const USBLib& that);
			USBLib operator=(const USBLib& that) {}
		private:
			std::shared_ptr<_EnumUsbHub> m_usbinfo;
			static std::shared_ptr<USBLib> m_usb;
		};
		std::shared_ptr<USBLib> USBLib::m_usb = nullptr;
		const vector<USB_ROOT_HUB>& GetAllUsbinfo()
		{
			return USBLib::Instance()->GetAllUsbinfo();
		}

		const std::vector<DEVICE_INFO_NODE>& GetAllUsbDeviceInfo()
		{
			return USBLib::Instance()->GetAllUsbDeviceInfo();
		}

		const std::vector<DEVICE_INFO_NODE>& GetAllUsbHubInfo()
		{
			return USBLib::Instance()->GetAllUsbHubInfo();
		}

		void UpdateData()
        {
			USBLib::Instance()->UpdateData();
        }
    }
}