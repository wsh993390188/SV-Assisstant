#pragma once
#include <mutex>

namespace Hardware
{
	namespace Utils
	{
		namespace Ring0
		{
			/// @brief ���̰߳�ȫ��MsrHandle
			class MsrHandle final
			{
				/// @brief ִ��Msr��CPUId
				uint32_t cpu_id;
				MsrHandle() = delete;
				MsrHandle(const MsrHandle&) = delete;
				MsrHandle& operator = (const MsrHandle&) = delete;
			public:
				/// @brief ��ʼ��Msr
				/// @param[in] cpu CPU Id
				MsrHandle(uint32_t cpu);

				/// @brief ��Msr
				/// @param[in] msr_number Msr�Ĵ�����ַ
				/// @param[out] value Msr����ֵ
				/// @return �Ƿ�ɹ�
				bool read(uint64_t msr_number, uint64_t& value);

				/// @brief дMsr
				/// @param[in] msr_number Msr�Ĵ�����ַ
				/// @param[in] value Msrд��ֵ
				/// @return �Ƿ�ɹ�
				bool write(uint64_t msr_number, uint64_t value);

				/// @brief ��ȡCPU Id
				/// @return CPUid
				int32_t getCoreId()
				{
					return (int32_t)cpu_id;
				}
				~MsrHandle() = default;
			};

			/// @brief �̰߳�ȫ��MsrHandle
			class SafeMsrHandle final
			{
				/// @brief Msr Handle
				std::shared_ptr<MsrHandle> pHandle;
				/// @brief �̰߳�ȫ����������
				std::mutex mutex;

				SafeMsrHandle(const SafeMsrHandle&) = delete;               // forbidden
				SafeMsrHandle& operator = (const SafeMsrHandle&) = delete; // forbidden

			public:
				/// @brief ��ʼ��MsrΪ��
				/// @return
				SafeMsrHandle() :pHandle{} { }

				/// @brief ������ʼ��Msr
				/// @param[in] core_id Cpu Id
				SafeMsrHandle(uint32_t core_id) : pHandle(new MsrHandle(core_id))
				{ }

				/// @brief ��Msr
				/// @param[in] msr_number Msr�Ĵ�����ַ
				/// @param[out] value Msr����ֵ
				/// @return �Ƿ�ɹ�
				bool read(uint64_t msr_number, uint64_t& value)
				{
					if (pHandle)
					{
						std::lock_guard<std::mutex> lock(mutex);
						return pHandle->read(msr_number, value);
					}

					value = 0;

					return false;
				}

				/// @brief дMsr
				/// @param[in] msr_number Msr�Ĵ�����ַ
				/// @param[in] value Msrд��ֵ
				/// @return �Ƿ�ɹ�
				bool write(uint64_t msr_number, uint64_t value)
				{
					if (pHandle)
					{
						std::lock_guard<std::mutex> lock(mutex);
						return pHandle->write(msr_number, value);
					}

					return false;
				}

				/// @brief ��ȡCPU Id
				/// @return CPUid
				int32_t getCoreId()
				{
					if (pHandle)
						return pHandle->getCoreId();

					return -1;
				}
				~SafeMsrHandle() = default;
			};
		}
	}
}
