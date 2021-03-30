#pragma once
#include "memorydefination.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief 内存实现基类
		/// 目前只有一个需要用到，一个是SMBIOS子类，一个是SMBUS
		class GenericMemory
		{
		protected:
			/// @brief 内存信息，First是内存标号（自定义），Second是内存通用信息
			std::map<USHORT, MemoryCommonInformation> MemoryInformation;
			/// @brief 本机物理内存的大小
			uint64_t PhysicalMemoryTotalSize;
			/// @brief 本机可用的内存数量（包括插了内存和没插内存）
			uint32_t MaxNumberOfMemory;
			/// @brief 本机已经插入的内存标号
			std::set<USHORT> AvailableMemoryIds;
		public:
			/// @brief 构造函数
			GenericMemory();

			/// @brief 析构函数
			virtual ~GenericMemory() = default;

			/// @brief 初始化内存数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize() = 0;

			/// @brief 更新内存数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update() = 0;

			/// @brief 构建初始化JSON信息
			/// @param[out] response Utf-8格式JSON字符串
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType BuildInitializeJson(std::string& response);

			/// @brief 解析获取元素的Json
			/// @param[in] root Json对象
			/// @param[out] DimmIds 内存DimmIds
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType ParserDimmJson(const Json::Value& root, USHORT& DimmIds);

			/// @brief 根据DIMM Id构造Json
			/// @param[in] DIMMIds 内存DimmIds
			/// @param[out] response Utf-8格式JSON字符串
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType BuildDIMMJson(const USHORT& DIMMIds, std::string& response);

			/// @brief 序列化SPD的数据
			/// @return @ref Data::ErrorType 默认不支持
			virtual Data::ErrorType SerializationSPD()
			{
				return Data::ErrorType::NOTSUPPORT;
			}

			/// @brief 获取内存信息
			/// @return @ref MemoryInformation
			std::add_const_t<decltype(Hardware::Memory::GenericMemory::MemoryInformation)>& GetMemoryInformation() const;

			/// @brief 获取本机内存大小
			/// @return 本机内存大小
			std::add_const_t<decltype(PhysicalMemoryTotalSize)> GetMemoryTotalSize() const
			{
				return PhysicalMemoryTotalSize;
			}
		private:
			/// @brief 从SMBIOS中获取SPD的信息,并存入结构中
			/// @param[in] SMBIOSMemoryInfo Type17中内存的信息
			/// @param[in] DIMMId 自定义的DIMMId
			/// @return @ref Data::ErrorType
			Data::ErrorType GetSPDInformationFromSMBIOS(const Smbios::Type17& SMBIOSMemoryInfo, const USHORT& DIMMId);

			/// @brief 从SMBIOS中获取SPD的信息,并存入结构中
			/// @param[out] MemoryInfo SPD解析后得内存信息
			/// @param[in] SMBIOSMemoryInfo Type17中内存的信息
			/// @return @ref Data::ErrorType
			Data::ErrorType GetSPDInformationFromSMBIOS(MemoryCommonInformation& MemoryInfo, const Smbios::Type17& SMBIOSMemoryInfo);

			/// @brief 根据内存信息构造JSON对象
			/// @param[in] Info 内存信息
			/// @return Utf-8格式JSON字符串
			std::string BuildMemoryCommonInformationToJson(const MemoryCommonInformation& Info);
		};
	}
}
