#pragma once
namespace Hardware
{
	namespace Utils
	{
		namespace Ring0
		{
			/// @brief PCI �����İ�����
			class IoHandle final
			{
				IoHandle(const IoHandle&) = delete;
				IoHandle& operator = (const IoHandle&) = delete;
			public:
				/// @brief ��ʼ��PCI
				IoHandle() = default;

				bool ReadByte(const USHORT IoAddress, DWORD& value);
				bool ReadWORD(const USHORT IoAddress, DWORD& value);
				bool ReadDWORD(const USHORT IoAddress, DWORD& value);

				bool WriteByte(const USHORT IoAddress, const DWORD value);
				bool WriteWORD(const USHORT IoAddress, const DWORD value);
				bool WriteDWORD(const USHORT IoAddress, const DWORD value);

				~IoHandle() = default;
			};

			/// @brief �̰߳�ȫ��PCI �����İ�����
			class SafeIoHandle final
			{
				std::shared_ptr<IoHandle> pHandle;
			public:
				/// @brief ��ʼ��PCI
				SafeIoHandle() : pHandle(std::make_shared<IoHandle>()) {}

				bool ReadByte(const USHORT IoAddress, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadByte(IoAddress, value);
					}
					return false;
				}
				bool ReadWORD(const USHORT IoAddress, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadWORD(IoAddress, value);
					}
					return false;
				}
				bool ReadDWORD(const USHORT IoAddress, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadDWORD(IoAddress, value);
					}
					return false;
				}
				bool WriteByte(const USHORT IoAddress, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteByte(IoAddress, value);
					}
					return false;
				}
				bool WriteWORD(const USHORT IoAddress, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteWORD(IoAddress, value);
					}
					return false;
				}
				bool WriteDWORD(const USHORT IoAddress, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteDWORD(IoAddress, value);
					}
					return false;
				}
			};
		}
	}
}