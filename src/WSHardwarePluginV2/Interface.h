/*!
* @file Interface.h
* @brief Ӳ���ӿ�
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��3��
*/

#pragma once

#include "Data.h"
/// @brief �ص���������
using PluginCallback = int(*)(LPCSTR, const char*);

namespace Hardware
{
	/// @brief Ӳ���ӿ��࣬�����������Ӳ��ʵ��
	namespace Interface
	{
		/// @brief �����ߵĽӿ�
		class Manager
		{
		public:
			/// @brief ��ʼ���ص�����Ϊ��
			/// @return  ��
			Manager() noexcept : m_CallBack(nullptr) {}

			/// @brief ��������
			/// @return ��
			virtual ~Manager() = default;

			/// @brief ִ�лص�����
			///			��H5����or���÷����ݻص���������ģ����ִ��֪ͨ�������޻ص�������ִ���κβ���
			/// @param[in] category ����֪ͨ������ ����ģ�鶨�壩
			/// @param[in] message ����֪ͨ����Ϣ��JSON��ʽ��
			/// @return
			///			0 ��������ִ�����
			///			����ֵ ִ�г���
			virtual int DoCallback(LPCSTR category, const char* message);

			/// @brief ע��ص�����
			///			���Զ��ע�ᣬ��ֻ�������µĻص�����
			///
			/// @param[in] CallBack �ص�����
			/// @return
			///			<em>SUCCESS</em> ��������ִ�����
			virtual void InitManager(PluginCallback CallBack);

			/// @brief ��������ִ����Ӧ����
			/// @param[in] command ��������
			/// @param[in] paramter JSON����
			/// @param[out] response ��Ӧ��JSONֵ
			/// @return @ref Data::ErrorType
			Data::ErrorType DoAction(const CommandType& command, LPCSTR paramter, std::string& response);
		private:
			/// @brief ��ʼ���������
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Init(LPCSTR paramter, std::string& response) = 0;

			/// @brief ���������������
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(LPCSTR paramter, std::string& response) = 0;

			/// @brief ��ȡ�������
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType GetElements(LPCSTR paramter, std::string& response) = 0;
		protected:
			/// @brief Ҫִ�еĻص�����
			PluginCallback m_CallBack;
		};
	}

	/// @brief �����ߴ�������
	class ManagerFactory final
	{
	public:
		/// @brief �����߹����ĵ����ӿ�
		/// @return �����߹����ĵ���
		static ManagerFactory* Instance();

		/// @brief ��ʼ�������߹���
		/// @return ��
		ManagerFactory();

		/// @brief ���ٹ����߹���
		/// @return ��
		~ManagerFactory();

		/// @brief ���ݹ��������ͻ�ȡ�Ĺ����߶���
		/// @param[in] Type ����������
		/// @return	�����߶���
		std::shared_ptr<Interface::Manager> GetManager(const ManagerType& Type);
	private:
		/// @brief �ڲ��洢�Ĺ�����
		std::map<ManagerType, std::shared_ptr<Interface::Manager>> m_Managers;
	};
}