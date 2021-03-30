#pragma once
#include "GenericMemory.h"
#include "BaseSMBUS.h"

namespace Hardware
{
	namespace Memory
	{
		class SPDMemoryController : public GenericMemory
		{
		public:
			/// @brief 初始化内存数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize() override final;

			/// @brief 更新内存数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update() override final;

			/// @brief 序列化SPD的数据
			/// @return @ref Data::ErrorType 默认不支持
			Data::ErrorType SerializationSPD() override final;
		private:
			/// @brief 获取SPD的信息
			/// @return @ref Data::ErrorType
			Data::ErrorType GetSPDInformation();

			/// @brief SMBUS读取控制器
			std::unique_ptr<SMBUSControllerBase> SMBUSController;
		};
	}
}
