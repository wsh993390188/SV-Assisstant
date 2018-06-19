#include "stdafx.h"
#include "UsbHub.h"
#include "Iusb.h"
#include <memory>

namespace SV_ASSIST
{
    namespace USB
    {
        static std::shared_ptr<_EnumUsbHub> m_usb = std::make_shared<_EnumUsbHub>();

		const vector<USB_ROOT_HUB>& GetUsbInfo()
		{
			return m_usb->Usb_Tree;
		}

		void UpdateData()
        {
            m_usb->Updatedata();
        }
    }
}