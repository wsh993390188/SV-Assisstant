#pragma once
#include "memorydefination.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief ����DDR3��spd��Ϣ
		class ParserDDR3SPD
		{
		public:
			/// @brief ����ͷ128λ��SPD��Ϣ
			/// @param SpdInfo spd��Ϣ
			/// @param OutputData �����������
			void ParserFirstSPD(const DDR3_INFO_FIRST_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief ������128λ��SPD��Ϣ
			/// @param SpdInfo spd��Ϣ
			/// @param OutputData �����������
			void ParserSecondSPD(const DDR3_INFO_SECOND_128* SpdInfo, MemoryCommonInformation& OutputData);
		private:
			/// @brief ����DDR3ģ��Ƶ��
			/// @param SpdInfo SPD����Ϣ @ref Hardware::Memory::DDR3_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "�ڴ��ͨ����Ϣ"
			void CalcDDR3TimeBase(const Hardware::Memory::DDR3_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief ����DDR3ģ��Ƶ��
			/// @param SpdInfo SPD����Ϣ @ref Hardware::Memory::DDR3_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "�ڴ��ͨ����Ϣ"
			void CalcDIMMFrequency(const DDR3_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);
		};

		/// @brief ����DDR4��SPD��Ϣ
		class ParserDDR4SPD
		{
		public:
			/// @brief ������һ���SPD��Ϣ
			/// @param SpdInfo spd��Ϣ
			/// @param OutputData �����������
			void ParserFirstSPD(const DDR4_INFO_FIRST_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief �����ڶ����SPD��Ϣ
			/// @param SpdInfo spd��Ϣ
			/// @param OutputData �����������
			void ParserSecondSPD(const DDR4_INFO_SECOND_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief �����������SPD��Ϣ
			/// @param SpdInfo spd��Ϣ
			/// @param OutputData �����������
			void ParserThirdSPD(const DDR4_INFO_THIRD_128* SpdInfo, MemoryCommonInformation& OutputData);

			/// @brief �������Ŀ��SPD��Ϣ
			/// @param SpdInfo spd��Ϣ
			/// @param OutputData �����������
			void ParserFourthSPD(const DDR4_INFO_FOURTH_128* SpdInfo, MemoryCommonInformation& OutputData);

		private:
			/// @brief ����DRAM��Revision
			/// @param SpdInfo
			/// @param OutputData
			void ParseDRAMRevision(const DDR4_INFO_THIRD_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief ����DDR4ģ��Ƶ��
			/// @param SpdInfo SPD����Ϣ @ref Hardware::Memory::DDR4_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "�ڴ��ͨ����Ϣ"
			void CalcDIMMFrequency(const DDR4_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief ����DDR4��ģ��ʱ����Ϣ
			/// @param SpdInfo SPD����Ϣ @ref Hardware::Memory::DDR4_INFO_FIRST_128
			/// @param OutputData @ref Hardware::Memory::MemoryCommonInformation "�ڴ��ͨ����Ϣ"
			void CalcDDR4TimeBase(const DDR4_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData);

			/// @brief ����ʱ����Ϣ
			/// @param tXX_MTB Medium Timebase
			/// @param tXX_FTB Fine Timebase
			/// @return �������ʱ��ֵ
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
