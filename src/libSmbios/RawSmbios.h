#pragma once

namespace Smbios
{
	//�洢�����Ƶ�SMBIOS��ȫ��Ψһ
	class RawSmbios
	{
	public:
		static RawSmbios* Instance();
		void* GetPhyAdderss() const;
	private:
		RawSmbios();
		virtual ~RawSmbios();
		void* SmbiosBaseAddress;
	};

	namespace
	{
#pragma pack(push, 1)
		struct RawSMBIOSData
		{
			uint8_t    Used20CallingMethod;
			uint8_t    SMBIOSMajorVersion;
			uint8_t    SMBIOSMinorVersion;
			uint8_t    DmiRevision;
			uint32_t   Length;
			uint8_t    SMBIOSTableData[];
		};
#pragma pack(pop)
		constexpr int SmbiosEndofTable = 127;
	}
}