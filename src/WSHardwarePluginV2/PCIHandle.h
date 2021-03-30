#pragma once
namespace Hardware
{
	namespace Utils
	{
		namespace Ring0
		{
			/// @brief PCI ����İ�����
			class PCIHandle final
			{
				PCIHandle(const PCIHandle&) = delete;
				PCIHandle& operator = (const PCIHandle&) = delete;
			public:
				/// @brief ��ʼ��PCI
				PCIHandle() = default;

				bool ReadByte(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value);
				bool ReadWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value);
				bool ReadDWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value);

				bool WriteByte(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value);
				bool WriteWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value);
				bool WriteDWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value);

				~PCIHandle() = default;
			};

			/// @brief �̰߳�ȫ��PCI ����İ�����
			class SafePCIHandle final
			{
				std::shared_ptr<PCIHandle> pHandle;
			public:
				/// @brief ��ʼ��PCI
				SafePCIHandle() : pHandle(std::make_shared<PCIHandle>()) {}

				bool ReadByte(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadByte(bus, dev, func, offset, value);
					}
					return false;
				}
				bool ReadWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadWORD(bus, dev, func, offset, value);
					}
					return false;
				}
				bool ReadDWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadDWORD(bus, dev, func, offset, value);
					}
					return false;
				}
				bool WriteByte(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteByte(bus, dev, func, offset, value);
					}
					return false;
				}
				bool WriteWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteWORD(bus, dev, func, offset, value);
					}
					return false;
				}
				bool WriteDWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteDWORD(bus, dev, func, offset, value);
					}
					return false;
				}
			};
		}
	}
}
