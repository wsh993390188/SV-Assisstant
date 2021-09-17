#pragma once
#include "XMLConfig.h"
namespace Hardware
{
	namespace Utils
	{
		/// @brief ��ȡPNPId������ģ��
		class PNPIDUtils final : public XMLConfig
		{
			/// @brief ���캯��
			PNPIDUtils();

			/// @brief XMLĬ�ϰ汾��
			std::string m_DefaultVersion;

			/// @brief �����ļ����µ���
			mutable std::mutex m_Mutex;

			/// @brief PNPIDs�����ݿ�
			std::map<std::string, XMLConfig::ExtendedInfo> m_PNPIDs;
		public:
			/// @brief ����ģʽ
			/// @return @ref PNPIDUtils
			static PNPIDUtils& Instance();

			/// @brief ͨ��PNPID��ѯ����
			/// @param PnpId PNPID
			/// @return ����
			std::string QueryNameByPnpId(const std::string& PnpId);
		private:
			/// @brief PNPID���л��������
			using PNPIDDBType = decltype(m_PNPIDs);

			/// @brief ��ʼ��PNP ID�����ļ�
			void Initialize();

			/// @brief ����PNPID�ֶ�
			/// @param PNPIDElement �ֶ�Ԫ��
			/// @param PnpidDB ���л���
			void ParserPNPID(tinyxml2::XMLElement const* const PNPIDElement, PNPIDDBType& PnpidDB);
		};
	}
}
