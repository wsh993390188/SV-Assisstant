#pragma once
#include "BaseSMBUS.h"
#include "IoHandle.h"

namespace Hardware
{
	namespace Memory
	{
		/// @brief 通用的SMBUS读取内存信息
		class CommonSMBUS final : public SMBUSControllerBase
		{
		public:
			/// @brief 构造函数
			CommonSMBUS();
			/// @brief 判断DIMM对应内存是否存在
			/// @param[in] DIMMId DIMMId
			/// @return 是否存在内存设备
			bool IsDeviceOnline(const USHORT& DIMMId) override;

		private:
			/// @brief 将0-7转成SMBUS可识别的DIMMId,仅用于Public函数的DIMMId
			/// @param[in] id id号
			/// @return DIMMId
			USHORT ConverterIdToDIMMId(const USHORT& id);

			/// @brief 获取SMBUS的IO地址
			/// @param[in] VendorID Vendor名
			/// @param[out] SMbusBaseAddress IO地址
			/// @return 是否成功
			bool GetSMbusBaseAddr(const USHORT& VendorID, USHORT& SMbusBaseAddress);

			/// @brief 寻找SMBUS的PCI设备，仅Intel可用
			/// @param[in] bus bus号
			/// @param[in] dev dev号
			/// @param[out] func func号
			/// @return 查找是否成功
			bool FindIntelSmbusControl(const USHORT& bus, const USHORT& dev, USHORT& func);

			/// @brief 读取并解析SPD的信息
			/// @param DIMMId DIMM Id
			/// @param MemoryInfo SPD通用信息
			/// @return 解析是否成功
			bool ParserSPD(const USHORT& DIMMId, MemoryCommonInformation& MemoryInfo)override final;

			/// @brief 读取SPD信息
			/// @param DIMMId DIMM id
			/// @param data SPD通用信息
			/// @return 读取是否成功
			bool ReadSPD(const USHORT& DIMMId, MemoryCommonInformation& data);

		private:

			/// @brief 读取DDR3的SPD
			/// @param DIMMId DIMM id
			/// @return 读取是否成功
			bool ReadDDR3SPD(const USHORT& DIMMId);

			/// @brief 读取DDR4的SPD
			/// @param DIMMId DIMM id
			/// @return 读取是否成功
			bool ReadDDR4SPD(const USHORT& DIMMId);

			/// @brief 读取DDR5的SPD
			/// @param DIMMId DIMM id
			/// @return 读取是否成功
			bool ReadDDR5SPD(const USHORT& DIMMId);

			/// @brief 用字节方式读取SPD中的信息
			/// @param[in] Offset DIMM的便宜
			/// @param[out] val 读取后的值
			bool ReadSPDByte(const uint8_t& Offset, DWORD& val);

			/// @brief 分配SPD的内存空间
			/// @param DDRSize DDR的大小
			/// @param DDRType DDR的类型
			void AllocateSPDSize(const uint8_t& DDRSize, const uint8_t& DDRType);
		private:
			/// @brief 等待Smbus Ready
			/// @return 是否Ready
			bool smbus_wait_until_ready();

			/// @brief 等待Smbus操作是否完成
			/// @return 0为成功，负值为失败
			int smbus_wait_until_done();

			/// @brief 预初始化SMBUS
			/// @return 是否成功初始化SMBUS
			bool PrepareSmbus();

			/// @brief 写入Pm寄存器
			/// @param reg 寄存器地址
			/// @param data 寄存器数据
			void writePmReg(const UINT8& reg, const UINT8& data);

			/// @brief 设置同步
			void setupFch();

			/// @brief 切换页表
			/// @param data 寄存器数据
			/// @return 是否成功切换SPD
			bool SwitchToPage(const UINT8& data);
		private:
			/// @brief SMbus基地址
			USHORT SmbusBase;
			/// @brief IO操作指针
			Utils::Ring0::SafeIoHandle IoPtr;
			/// @brief SPD的数据
			std::vector<uint8_t> SPDData;
		};
	}
}
