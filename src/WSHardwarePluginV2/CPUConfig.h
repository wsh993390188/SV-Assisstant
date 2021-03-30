#pragma once
#include <mutex>
namespace Hardware
{
	namespace CPU
	{
		struct CPUExtendedInfo
		{
			std::string Element;
			bool Priority = true;
		};
		/// @brief �������л�ȡCPU��Ϣ
		struct CPUExtendedInfoFromCPUDB
		{
			CPUExtendedInfo CodeName; ///< cpu����
			CPUExtendedInfo SocketPackage; ///< cpu��װleixing
			CPUExtendedInfo MaxTDP;///<cpu�ı�׼����TDP
			CPUExtendedInfo Technology;///<cpu�Ĺ���
			explicit operator bool() const;
		};

		/// @brief CPU�����ļ����л���
		class CPUDB final
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
			/// @brief CPU���л��������
			using CPUDBType = decltype(m_CPUDB);

			/// @brief ��CPUDB�в�ѯCPU��Ϣ
			struct CPUQueryInfo
			{
				std::string CpuManufacture; ///<cpu����
				std::string CpuFamily; ///<cpu����
				std::string CpuModel; ///<cpu����
				explicit operator bool() const;
			};
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
			std::unique_ptr<CPUExtendedInfoFromCPUDB> FindElements(const CPUQueryInfo& QueryInfo) const;

			/// @brief ��ȡ���л��������
			/// @return ���л��������
			const CPUDBType& GetCpuData()const;
		private:
			/// @brief Ĭ�Ϲ��죬��ֹ�ⲿ���ʹ��캯��
			CPUDB() = default;
			/// @brief ��ȡXML�����ļ��е���Ϣ
			/// @param fileName ���ܺ��ļ���
			/// @param localConfigName ����δ�����ļ���
			/// @param configcontent XML�����ļ��ж���������
			/// @return ��ȡ�����ļ��Ƿ�ɹ�
			bool GetConfigXml(const std::wstring& fileName, const std::wstring& localConfigName, std::string& configcontent);

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

		private:
			/// @brief �жϲ��������ļ��Ƿ����,�����ù��ÿⷽ��������ʵ��
			/// @return ���������ļ��Ƿ����
			bool TestConfigIsExist();
		};
	}
}
