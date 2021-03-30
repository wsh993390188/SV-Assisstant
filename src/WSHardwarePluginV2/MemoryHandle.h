#pragma once
namespace Hardware
{
	namespace Utils
	{
		namespace Ring0
		{
			/// @brief PCI ����İ�����
			class MemoryHandle final
			{
				MemoryHandle(const MemoryHandle&) = delete;
				MemoryHandle& operator = (const MemoryHandle&) = delete;
			public:
				/// @brief ��ʼ��PCI
				MemoryHandle() = default;

				bool ReadByte(const DWORD MemoryAddress, DWORD& value);
				bool ReadWORD(const DWORD MemoryAddress, DWORD& value);
				bool ReadDWORD(const DWORD MemoryAddress, DWORD& value);

				bool WriteByte(const DWORD MemoryAddress, const DWORD value);
				bool WriteWORD(const DWORD MemoryAddress, const DWORD value);
				bool WriteDWORD(const DWORD MemoryAddress, const DWORD value);

				~MemoryHandle() = default;
			};

			/// @brief �̰߳�ȫ��PCI ����İ�����
			class SafeMemoryHandle final
			{
				std::shared_ptr<MemoryHandle> pHandle;
			public:
				/// @brief ��ʼ��PCI
				SafeMemoryHandle() : pHandle(std::make_shared<MemoryHandle>()) {}

				bool ReadByte(const DWORD MemoryAddress, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadByte(MemoryAddress, value);
					}
					return false;
				}
				bool ReadWORD(const DWORD MemoryAddress, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadWORD(MemoryAddress, value);
					}
					return false;
				}
				bool ReadDWORD(const DWORD MemoryAddress, DWORD& value)
				{
					if (pHandle)
					{
						return pHandle->ReadDWORD(MemoryAddress, value);
					}
					return false;
				}
				bool WriteByte(const DWORD MemoryAddress, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteByte(MemoryAddress, value);
					}
					return false;
				}
				bool WriteWORD(const DWORD MemoryAddress, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteWORD(MemoryAddress, value);
					}
					return false;
				}
				bool WriteDWORD(const DWORD MemoryAddress, const DWORD value)
				{
					if (pHandle)
					{
						return pHandle->WriteDWORD(MemoryAddress, value);
					}
					return false;
				}
			};
		}
	}
}
