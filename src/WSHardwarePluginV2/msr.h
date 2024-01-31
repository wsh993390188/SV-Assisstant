#pragma once
#include <mutex>

namespace Hardware
{
	namespace Utils
	{
		namespace Ring0
		{
			/// @brief 非线程安全的MsrHandle
			class MsrHandle final
			{
				/// @brief 执行Msr的CPUId
				uint32_t cpu_id;
				MsrHandle() = delete;
				MsrHandle(const MsrHandle&) = delete;
				MsrHandle& operator = (const MsrHandle&) = delete;
			public:
				/// @brief 初始化Msr
				/// @param[in] cpu CPU Id
				MsrHandle(uint32_t cpu);

				/// @brief 读Msr
				/// @param[in] msr_number Msr寄存器地址
				/// @param[out] value Msr返回值
				/// @return 是否成功
				bool read(uint64_t msr_number, uint64_t& value);

				/// @brief 写Msr
				/// @param[in] msr_number Msr寄存器地址
				/// @param[in] value Msr写入值
				/// @return 是否成功
				bool write(uint64_t msr_number, uint64_t value);

				/// @brief 获取CPU Id
				/// @return CPUid
				int32_t getCoreId()
				{
					return (int32_t)cpu_id;
				}
				~MsrHandle() = default;
			};

			/// @brief 线程安全的MsrHandle
			class SafeMsrHandle final
			{
				/// @brief Msr Handle
				std::shared_ptr<MsrHandle> pHandle;
				/// @brief 线程安全保护互斥量
				std::mutex mutex;

				SafeMsrHandle(const SafeMsrHandle&) = delete;               // forbidden
				SafeMsrHandle& operator = (const SafeMsrHandle&) = delete; // forbidden

			public:
				/// @brief 初始化Msr为空
				/// @return
				SafeMsrHandle() :pHandle{} { }

				/// @brief 正常初始化Msr
				/// @param[in] core_id Cpu Id
				SafeMsrHandle(uint32_t core_id) : pHandle(new MsrHandle(core_id))
				{ }

				/// @brief 读Msr
				/// @param[in] msr_number Msr寄存器地址
				/// @param[out] value Msr返回值
				/// @return 是否成功
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

				/// @brief 写Msr
				/// @param[in] msr_number Msr寄存器地址
				/// @param[in] value Msr写入值
				/// @return 是否成功
				bool write(uint64_t msr_number, uint64_t value)
				{
					if (pHandle)
					{
						std::lock_guard<std::mutex> lock(mutex);
						return pHandle->write(msr_number, value);
					}

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
				~SafeMsrHandle() = default;
			};
		}
	}
}
