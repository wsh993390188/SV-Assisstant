/*!
* @file Comand.h
* @brief ��������ӿ�
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��3��
*/
#pragma once
namespace Hardware
{
	/// @brief ����������
	enum class ManagerType : uint32_t
	{
		None = 0, ///< δ֪������
		Disk = 1, ///< Ӳ�����
		CPU = 2, ///<CPU���
		Memory, ///<�ڴ����
		Broad, ///<���弰Bios���
		GPU, ///<�Կ����
		Audio, ///<�������
		Net, ///<�������
		WinBio, ///<Win10���ﴫ�������
		Battery, ///<������
		Monitor, ///<��ʾ�����
	};

	/// @brief ��������
	enum class CommandType : uint32_t
	{
		None = 0,	///< δ֪������
		Init,		///<��ʼ������(or ����ȫ������)
		Update,		///<������������
		Get,			///<��ȡ��������
	};

	/// @brief ִ�е�����
	class Command
	{
	public:
		/// @brief ��ȡ����������
		/// @return
		///			@ref ManagerType
		//////////////////////////////////////////////////////////////////////////
		virtual ManagerType GetManagerType() = 0;

		/// @brief ��ȡ��������
		/// @return
		///			@ref CommandType
		virtual CommandType GetCommandType() = 0;
	};

	/// @brief ����ģʽ���ݿ⣬������ַ���ת������
	class CommandDataBase
	{
	public:
		/// @brief �������ݿⵥ�����
		/// @return ����ĵ���
		static CommandDataBase* Instance();

		/// @brief ��ʼ������ģʽ���ݿ�
		/// @return ��
		CommandDataBase();

		/// @brief �����ַ���ת��������
		/// @param[in] Action ����Ķ���
		/// @return ��������
		std::shared_ptr<Command> SwitchToCommand(const std::string& Action);

	private:
		/// @brief ��ʼ���������
		/// @tparam ManagerCommand ����������
		/// @return �������ݿ�
		template<class ManagerCommand>
		std::map<CommandType, std::shared_ptr<Command>> InitCommand();
	private:
		/// @brief �������ַ���ת����
		/// @param[in] ManagerString
		/// @return @ref ManagerType
		ManagerType StringToMangerType(const std::string& ManagerString);

		/// @brief �����ַ���ת����
		/// @param[in] OperatorString
		/// @return @ref CommandType
		CommandType StringToOperatorType(const std::string& OperatorString);
		/// @brief ���ݿ�
		///		1.����������,2.����������,3.ʵ�ʶ�Ӧ���������
		std::map<ManagerType, std::map<CommandType, std::shared_ptr<Command>>> m_CommandDatabase;
	};
}
