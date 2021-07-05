#pragma once
#include "XMLConfig.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief �������л�ȡCPU��Ϣ
		struct GPUExtendedInfoFromGPUDB
		{
			XMLConfig::ExtendedInfo ChipName; ///< оƬ��
			XMLConfig::ExtendedInfo Released; ///< Released Date
			XMLConfig::ExtendedInfo MemorySize;///< Memory Size
			XMLConfig::ExtendedInfo MemoryType;///< Memory Type
			XMLConfig::ExtendedInfo MemoryBits;///< Memory Bits
			XMLConfig::ExtendedInfo MemoryClock;///< Memory Clock
			XMLConfig::ExtendedInfo GPUClock;///< GPU Clock
			XMLConfig::ExtendedInfo Shaders;///< Shaders
			XMLConfig::ExtendedInfo TMUs;///< TMUs
			XMLConfig::ExtendedInfo ROPs;///< ROPs
			explicit operator bool() const;
		};

		/// @brief GPU�����ñ�
		class GPUConfig final : public XMLConfig
		{
		private:
			/// @brief cpudb�е���Ϣ���л�
			struct InformationFromDB
			{
				/// @brief ��CPUDB�л�ȡ��cpu��Ϣ, First��CPUϵ�� Second��CPU��չ��Ϣ
				std::map<std::string, GPUExtendedInfoFromGPUDB> Element;
				explicit operator bool() const;
			};

			/// @brief �洢�ⲿ��Ϣ����
			/// First��CPU�ĳ��̣�Second��CPU�ļ�����Ϣ
			std::map<std::string, InformationFromDB> m_GPUDB;

			/// @brief XMLĬ�ϰ汾��
			std::string m_DefaultVersion;

			/// @brief �����ļ����µ���
			mutable std::mutex m_Mutex;
		public:
			/// @brief CPU���л�����
			/// @return �������� @ref CPUDB
			static GPUConfig& Instance();

			/// @brief ��ʼ��CPU�����ļ�
			void Initialize();

			/// @brief ����CPU������Ѱ�Ҳ�����Ϣ
			/// @param CpuManufacture CPU�ĳ���
			/// @param CpuFamily CPU�ļ���
			/// @param CpuModel CPU���ͺ�
			/// @return Cpu������Ϣ��ָ��
			std::unique_ptr<GPUExtendedInfoFromGPUDB> FindElements(const QueryInfo& QueryInfo) const;

		private:
			/// @brief CPU���л��������
			using GPUDBType = decltype(m_GPUDB);

			/// @brief Ĭ�Ϲ��죬��ֹ�ⲿ���ʹ��캯��
			GPUConfig() = default;

			/// @brief ����Manufacture�ֶ�
			/// @param ManufactureElement �ֶ�Ԫ��
			/// @param CpuDB ���л���
			void ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, GPUDBType& DB);

			/// @brief ����CPU�ֶ�
			/// @param CpuElement �ֶ�Ԫ��
			/// @param CpuData ���л���
			/// @return �������� @ref Data::ErrorType
			Data::ErrorType ParserInformaion(tinyxml2::XMLElement const* const CpuElement, InformationFromDB& Data);
		};
	}
}
