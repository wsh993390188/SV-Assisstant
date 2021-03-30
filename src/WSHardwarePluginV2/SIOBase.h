#pragma once
#include <array>
#include "IoHandle.h"
namespace Hardware
{
	/// @brief SuperIO模块
	namespace SIO
	{
		/// @brief SuperIO的基类
		class SIOBase
		{
		public:
			/// @brief 初始化SIO的基础数据
			SIOBase();

			/// @brief 虚析构函数
			virtual ~SIOBase() = default;

			/// @brief 初始化SIO
			/// @param port SIO的地址
			/// @return 是否成功
			virtual bool InitializeSIO(const USHORT& port) = 0;

			/// @brief 反初始化SIO，用完就要销毁，不可一直占用
			/// @param port SIO的地址
			/// @return 是否成功
			virtual bool DeInitializeSIO(const USHORT& port) = 0;

			/// @brief 芯片是否可用
			/// @return 是否可用
			bool IsActive()const;
		protected:
			/// @brief 切换SIO的Page
			/// @param port SIO的位置接口
			/// @param data SIO的数据接口
			/// @param logicalDeviceNumber Page的Number
			/// @return 是否成功
			bool SelectPage(const USHORT& port, const USHORT& data, const UCHAR& logicalDeviceNumber);

			/// @brief SIO读取一个Word的封装
			/// @param port	  SIO的位置接口
			/// @param data	  SIO的数据接口
			/// @param offset 读取的偏移量
			/// @param output 输出的值
			/// @return 是否成功
			bool SIOReadWord(const USHORT& port, const USHORT& data, const DWORD& offset, DWORD& output);
			/// @brief SIO读取一个Byte的封装
			/// @param port	  SIO的位置接口
			/// @param data	  SIO的数据接口
			/// @param offset 读取的偏移量
			/// @param output 输出的值
			/// @return 是否成功
			bool SIOReadByte(const USHORT& port, const USHORT& data, const DWORD& offset, DWORD& output);
		protected:
			/// @brief 该芯片是否可用
			bool active;

			/// @brief SIO占用的IO读取功能
			Utils::Ring0::SafeIoHandle IoPtr;

			/// @brief SIO的进入端口
			std::array<std::pair<USHORT, USHORT>, 2> SIOPort;

			/// @brief Chip的ID
			const uint32_t CHIP_ID_REGISTER;
			/// @brief Chip的Revision
			const uint32_t CHIP_REVISION_REGISTER;
			/// @brief LPC地址
			const uint32_t BASE_ADDRESS_REGISTER;
		};
	}
}
