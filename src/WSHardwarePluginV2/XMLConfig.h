#pragma once
namespace Hardware
{
	class XMLConfig
	{
	public:
		/// @brief ���õ���չ��Ϣ
		struct ExtendedInfo
		{
			std::string Element;///<��Ϣ����
			bool Priority = true;///< �Ƿ�����ʹ��,��������ʹ�ô����б������,Ĭ������ʹ��
			operator bool() const;
		};

		/// @brief �����ݿ��в�ѯ��Ϣ
		struct QueryInfo
		{
			std::string Manufacture; ///<����
			std::string Family; ///<����
			std::string Model; ///<����
			explicit operator bool() const;
		};

	protected:
		/// @brief ����Դ�ж�ȡXML�����ļ��е���Ϣ
		/// @param configcontent XML�����ļ��ж���������
		/// @param ResID ��ԴID
		/// @return ��ȡ�����ļ��Ƿ�ɹ�
		bool GetConfigXmlFromResource(std::string& configcontent, const size_t ResID);
	};
}
