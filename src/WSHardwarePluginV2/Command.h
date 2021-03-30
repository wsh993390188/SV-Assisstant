/*!
* @file Comand.h
* @brief 调用命令接口
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月3日
*/
#pragma once
namespace Hardware
{
	/// @brief 管理者类型
	enum class ManagerType : uint32_t
	{
		None = 0, ///< 未知的类型
		Disk = 1, ///< 硬盘相关
		CPU = 2, ///<CPU相关
		Memory, ///<内存相关
		Broad, ///<主板及Bios相关
		GPU, ///<显卡相关
		Audio, ///<声卡相关
		Net, ///<网卡相关
		WinBio, ///<Win10生物传感器相关
		Battery, ///<电池相关
		Monitor, ///<显示器相关
	};

	/// @brief 命令类型
	enum class CommandType : uint32_t
	{
		None = 0,	///< 未知的命令
		Init,		///<初始化操作(or 更新全部数据)
		Update,		///<更新现有数据
		Get,			///<获取现有数据
	};

	/// @brief 执行的命令
	class Command
	{
	public:
		/// @brief 获取管理者类型
		/// @return
		///			@ref ManagerType
		//////////////////////////////////////////////////////////////////////////
		virtual ManagerType GetManagerType() = 0;

		/// @brief 获取命令类型
		/// @return
		///			@ref CommandType
		virtual CommandType GetCommandType() = 0;
	};

	/// @brief 命令模式数据库，负责从字符串转成命令
	class CommandDataBase
	{
	public:
		/// @brief 命令数据库单例入口
		/// @return 命令的单例
		static CommandDataBase* Instance();

		/// @brief 初始化命令模式数据库
		/// @return 空
		CommandDataBase();

		/// @brief 动作字符串转命令类型
		/// @param[in] Action 传入的动作
		/// @return 命令类型
		std::shared_ptr<Command> SwitchToCommand(const std::string& Action);

	private:
		/// @brief 初始化相关命令
		/// @tparam ManagerCommand 管理者命令
		/// @return 命令数据库
		template<class ManagerCommand>
		std::map<CommandType, std::shared_ptr<Command>> InitCommand();
	private:
		/// @brief 管理者字符串转类型
		/// @param[in] ManagerString
		/// @return @ref ManagerType
		ManagerType StringToMangerType(const std::string& ManagerString);

		/// @brief 命令字符串转类型
		/// @param[in] OperatorString
		/// @return @ref CommandType
		CommandType StringToOperatorType(const std::string& OperatorString);
		/// @brief 数据库
		///		1.管理者类型,2.存命令类型,3.实际对应的命令对象
		std::map<ManagerType, std::map<CommandType, std::shared_ptr<Command>>> m_CommandDatabase;
	};
}
