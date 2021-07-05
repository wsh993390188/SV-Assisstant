#pragma once
#include <mutex>
#include "XMLConfig.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief �ڴ泧�̵����ñ�
		class JEDECConfig final : public XMLConfig
		{
			/// @brief �ڴ泧�̵�Banks
			std::vector<std::map<size_t, std::string>> m_Banks;

			/// @brief Bank�Ĺ�����֯
			using BanksType = decltype(m_Banks);

			/// @brief Bank������
			using BankType = BanksType::value_type;

			/// @brief XMLĬ�ϰ汾��
			std::string m_DefaultVersion;
			/// @brief �����ļ����µ���
			mutable std::mutex m_Mutex;
		public:
			/// @brief �ڴ泧�̵����ñ����л�����
			/// @return �������� @ref JEDECConfig
			static JEDECConfig& Instance();

			/// @brief ��Bank�л�ȡ������Ϣ
			/// @param BankCode Bank Code
			/// @param ContinuationCode Continuation Code
			/// @return ���̵�U8��
			std::string GetManufacture(const size_t BankCode, const size_t ContinuationCode) const;
		private:
			/// @brief Ĭ�Ϲ��죬��ֹ�ⲿ���ʹ��캯��
			JEDECConfig();

			/// @brief ��ʼ���ڴ�������ļ�
			void Initialize();

			/// @brief ����Bank�ֶ�
			/// @param BankElement �ֶ�Ԫ��
			/// @param BanksDB ���л���
			void ParserBank(tinyxml2::XMLElement const* const BankElement, BanksType& BanksDB);

			/// @brief ����Manufacture�ֶ�
			/// @param ManufactureElement �ֶ�Ԫ��
			/// @param BankDB ���л���
			void ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, BankType& BankDB);
		};
	}
}
