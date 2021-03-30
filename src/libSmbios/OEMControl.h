#pragma once

namespace Smbios
{
	class OEMControl final
	{
	public:
		static OEMControl& Instance();
		OEMControl();
		~OEMControl() = default;

		enum class OemVendor
		{
			VENDOR_UNKNOWN,
			VENDOR_ACER,
			VENDOR_HP,
			VENDOR_LENOVO,
		};

		const OemVendor GetOemVendor() const noexcept;

	private:
		OemVendor dmi_vendor;
	};
}
