#pragma once
#include <mutex>
#include "XMLConfig.h"
namespace Hardware
{
	namespace CPU
	{
		/// @brief �������л�ȡCPU��Ϣ
		struct CPUExtendedInfoFromCPUDB
		{
			XMLConfig::ExtendedInfo CodeName; ///< cpu����
			XMLConfig::ExtendedInfo SocketPackage; ///< cpu��װleixing
			XMLConfig::ExtendedInfo MaxTDP;///<cpu�ı�׼����TDP
			XMLConfig::ExtendedInfo Technology;///<cpu�Ĺ���
			explicit operator bool() const;
		};

		/// @brief CPU�����ļ����л���
		class CPUDB final : public XMLConfig
		{
		private:
			/// @brief cpudb�е���Ϣ���л�
			struct CpuInformationFromCpudb
			{
				/// @brief ��CPUDB�л�ȡ��cpu��Ϣ, First��CPUϵ�� Second��CPU��չ��Ϣ
				std::map<std::string, CPUExtendedInfoFromCPUDB> Element;
				explicit operator bool() const;
			};

			/// @brief cpudb�е���Ϣ���л�
			struct CpuFamily
			{
				/// @brief ��CPUDB�л�ȡ��cpu��Ϣ, First��CPU�ļ��� Second��CPU��չ��Ϣ
				std::map<std::string, CpuInformationFromCpudb> Element;
				explicit operator bool() const;
			};
			/// @brief �洢�ⲿ��Ϣ����
			/// First��CPU�ĳ��̣�Second��CPU�ļ�����Ϣ
			std::map<std::string, CpuFamily> m_CPUDB;

			/// @brief XMLĬ�ϰ汾��
			std::string m_DefaultVersion;

			/// @brief �����ļ����µ���
			mutable std::mutex m_Mutex;
		public:
			/// @brief CPU���л�����
			/// @return �������� @ref CPUDB
			static CPUDB& Instance();

			/// @brief ��ʼ��CPU�����ļ�
			void Initialize();

			/// @brief ����CPU������Ѱ�Ҳ�����Ϣ
			/// @param CpuManufacture CPU�ĳ���
			/// @param CpuFamily CPU�ļ���
			/// @param CpuModel CPU���ͺ�
			/// @return Cpu������Ϣ��ָ��
			std::unique_ptr<CPUExtendedInfoFromCPUDB> FindElements(const QueryInfo& QueryInfo) const;

		private:
			/// @brief CPU���л��������
			using CPUDBType = decltype(m_CPUDB);

			/// @brief Ĭ�Ϲ��죬��ֹ�ⲿ���ʹ��캯��
			CPUDB() = default;

			/// @brief ����Manufacture�ֶ�
			/// @param ManufactureElement �ֶ�Ԫ��
			/// @param CpuDB ���л���
			void ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, CPUDBType& CpuDB);

			/// @brief ����Family�ֶ�
			/// @param FamilyElement �ֶ�Ԫ��
			/// @param FamilyData ���л���
			/// @return �������� @ref Data::ErrorType
			Data::ErrorType ParserCpuFamily(tinyxml2::XMLElement const* const FamilyElement, CpuFamily& FamilyData);

			/// @brief ����CPU�ֶ�
			/// @param CpuElement �ֶ�Ԫ��
			/// @param CpuData ���л���
			/// @return �������� @ref Data::ErrorType
			Data::ErrorType ParserCpuInformaion(tinyxml2::XMLElement const* const CpuElement, CpuInformationFromCpudb& CpuData);
		};
	}
}
