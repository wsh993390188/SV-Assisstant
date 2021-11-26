#pragma once
#include <mutex>

namespace Hardware
{
	namespace Utils
	{
		namespace Ring0
		{
			/// @brief 非线程安全的获取1ms的Tsc间隔
			class TscIntervalHandle final
			{
				/// @brief 执行Msr的CPUId
				uint32_t cpu_id;
				TscIntervalHandle() = delete;
				TscIntervalHandle(const TscIntervalHandle&) = delete;
				TscIntervalHandle& operator = (const TscIntervalHandle&) = delete;
			public:
				/// @brief 初始化Msr
				/// @param[in] cpu CPU Id
				TscIntervalHandle(uint32_t cpu);

				/// @brief 读Tsc
				/// @param[out] value Tsc返回值
				/// @return 是否成功
				bool read(double& value);

				/// @brief 获取CPU Id
				/// @return CPUid
				int32_t getCoreId()
				{
					return (int32_t)cpu_id;
				}
				~TscIntervalHandle() = default;
			};

			/// @brief 线程安全的Tsc间隔
			class SafeTscIntervalHandle final
			{
				/// @brief Tsc Handle
				std::shared_ptr<TscIntervalHandle> pHandle;
				/// @brief 线程安全保护互斥量
				mutable std::mutex mutex;

				SafeTscIntervalHandle(const SafeTscIntervalHandle&) = delete;               // forbidden
				SafeTscIntervalHandle& operator = (const SafeTscIntervalHandle&) = delete; // forbidden

			public:
				/// @brief 初始化Tsc为空
				/// @return
				SafeTscIntervalHandle() :pHandle{} { }

				/// @brief 正常初始化Tsc
				/// @param[in] core_id Cpu Id
				SafeTscIntervalHandle(uint32_t core_id) : pHandle(std::make_shared<TscIntervalHandle>(core_id))
				{ }

				/// @brief 读Tsc
				/// @param[out] value Tsc返回值
				/// @return 是否成功
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

				/// @brief 获取CPU Id
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
