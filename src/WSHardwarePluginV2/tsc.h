#pragma once
#include <mutex>

namespace Hardware
{
	namespace Utils
	{
		namespace Ring0
		{
			/// @brief ���̰߳�ȫ�Ļ�ȡ1ms��Tsc���
			class TscIntervalHandle final
			{
				/// @brief ִ��Msr��CPUId
				uint32_t cpu_id;
				TscIntervalHandle() = delete;
				TscIntervalHandle(const TscIntervalHandle&) = delete;
				TscIntervalHandle& operator = (const TscIntervalHandle&) = delete;
			public:
				/// @brief ��ʼ��Msr
				/// @param[in] cpu CPU Id
				TscIntervalHandle(uint32_t cpu);

				/// @brief ��Tsc
				/// @param[out] value Tsc����ֵ
				/// @return �Ƿ�ɹ�
				bool read(double& value);

				/// @brief ��ȡCPU Id
				/// @return CPUid
				int32_t getCoreId()
				{
					return (int32_t)cpu_id;
				}
				~TscIntervalHandle() = default;
			};

			/// @brief �̰߳�ȫ��Tsc���
			class SafeTscIntervalHandle final
			{
				/// @brief Tsc Handle
				std::shared_ptr<TscIntervalHandle> pHandle;
				/// @brief �̰߳�ȫ����������
				mutable std::mutex mutex;

				SafeTscIntervalHandle(const SafeTscIntervalHandle&) = delete;               // forbidden
				SafeTscIntervalHandle& operator = (const SafeTscIntervalHandle&) = delete; // forbidden

			public:
				/// @brief ��ʼ��TscΪ��
				/// @return
				SafeTscIntervalHandle() :pHandle{} { }

				/// @brief ������ʼ��Tsc
				/// @param[in] core_id Cpu Id
				SafeTscIntervalHandle(uint32_t core_id) : pHandle(std::make_shared<TscIntervalHandle>(core_id))
				{ }

				/// @brief ��Tsc
				/// @param[out] value Tsc����ֵ
				/// @return �Ƿ�ɹ�
				bool read(double& value) const
				{
					if (pHandle)
					{
						std::lock_guard<std::mutex> lock(mutex);
						return pHandle->read(value);
					}

					value = 0;

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
				~SafeTscIntervalHandle() = default;
			};
		}
	}
}
