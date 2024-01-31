#pragma once
#include "memorydefination.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief 解析DDR3的spd信息
		class ParserDDR3SPD
		{
		public:
			/// @brief 解析头128位的SPD信息
			/// @param SpdInfo spd信息
			/// @param OutputData 解析后的数据
			void ParserFirstSPD(const DDR3_INFO_FIRST_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief 解析后128位的SPD信息
			/// @param SpdInfo spd信息
			/// @param OutputData 解析后的数据
			void ParserSecondSPD(const DDR3_INFO_SECOND_128* SpdInfo, MemoryCommonInformation& OutputData);
		private:
			/// @brief 计算DDR3模块频率
			/// @param SpdInfo SPD的信息 @ref Hardware::Memory::DDR3_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "内存的通用信息"
			void CalcDDR3TimeBase(const Hardware::Memory::DDR3_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief 计算DDR3模块频率
			/// @param SpdInfo SPD的信息 @ref Hardware::Memory::DDR3_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "内存的通用信息"
			void CalcDIMMFrequency(const DDR3_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);
		};

		/// @brief 解析DDR4的SPD信息
		class ParserDDR4SPD
		{
		public:
			/// @brief 解析第一块的SPD信息
			/// @param SpdInfo spd信息
			/// @param OutputData 解析后的数据
			void ParserFirstSPD(const DDR4_INFO_FIRST_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief 解析第二块的SPD信息
			/// @param SpdInfo spd信息
			/// @param OutputData 解析后的数据
			void ParserSecondSPD(const DDR4_INFO_SECOND_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief 解析第三块的SPD信息
			/// @param SpdInfo spd信息
			/// @param OutputData 解析后的数据
			void ParserThirdSPD(const DDR4_INFO_THIRD_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief 解析第四块的SPD信息
			/// @param SpdInfo spd信息
			/// @param OutputData 解析后的数据
			void ParserFourthSPD(const DDR4_INFO_FOURTH_128* SpdInfo, MemoryCommonInformation& OutputData);

		private:
			/// @brief 解析DRAM的Revision
			/// @param SpdInfo
			/// @param OutputData
			void ParseDRAMRevision(const DDR4_INFO_THIRD_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief 计算DDR4模块频率
			/// @param SpdInfo SPD的信息 @ref Hardware::Memory::DDR4_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "内存的通用信息"
			void CalcDIMMFrequency(const DDR4_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief 计算DDR4的模块时序信息
			/// @param SpdInfo SPD的信息 @ref Hardware::Memory::DDR4_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "内存的通用信息"
			void CalcDDR4TimeBase(const DDR4_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief 解析时序信息
			/// @param tXX_MTB Medium Timebase
			/// @param tXX_FTB Fine Timebase
			/// @return 计算出的时序值
			template<typename T>
			std::enable_if_t<std::is_integral_v<T>, double> GetTimeBase(const T tXX_MTB, const T tXX_FTB) const
			{
				return (tXX_MTB * MTB) + (tXX_FTB * FTB);
			}

			const double MTB = 0.125; ///< Medium Timebase
			const double FTB = 0.001; ///< Fine Timebase
		};
	}
}
