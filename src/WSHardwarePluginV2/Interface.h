/*!
* @file Interface.h
* @brief 硬件接口
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月3日
*/

#pragma once

#include "Data.h"
/// @brief 回调函数类型
using PluginCallback = int(*)(LPCSTR, const char*);

namespace Hardware
{
	/// @brief 硬件接口类，用来管理多种硬件实现
	namespace Interface
	{
		/// @brief 管理者的接口
		class Manager
		{
		public:
			/// @brief 初始化回调函数为空
			/// @return  空
			Manager() noexcept : m_CallBack(nullptr) {}

			/// @brief 析构函数
			/// @return 空
			virtual ~Manager() = default;

			/// @brief 执行回调函数
			///			由H5界面or调用方传递回调函数并由模块内执行通知操作，无回调函数则不执行任何操作
			/// @param[in] category 本次通知的类型 （由模块定义）
			/// @param[in] message 本次通知的消息（JSON格式）
			/// @return
			///			0 正常流程执行完成
			///			其他值 执行出错
			virtual int DoCallback(LPCSTR category, const char* message);

			/// @brief 注册回调函数
			///			可以多次注册，但只保留最新的回调函数
			///
			/// @param[in] CallBack 回调函数
			/// @return
			///			<em>SUCCESS</em> 正常流程执行完成
			virtual void InitManager(PluginCallback CallBack);

			/// @brief 依据命令执行相应动作
			/// @param[in] command 命令类型
			/// @param[in] paramter JSON参数
			/// @param[out] response 回应的JSON值
			/// @return @ref Data::ErrorType
			Data::ErrorType DoAction(const CommandType& command, LPCSTR paramter, std::string& response);
		private:
			/// @brief 初始化命令操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Init(LPCSTR paramter, std::string& response) = 0;

			/// @brief 更新数据命令操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(LPCSTR paramter, std::string& response) = 0;

			/// @brief 获取命令操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType GetElements(LPCSTR paramter, std::string& response) = 0;
		protected:
			/// @brief 要执行的回调函数
			PluginCallback m_CallBack;
		};
	}

	/// @brief 管理者创建工厂
	class ManagerFactory final
	{
	public:
		/// @brief 管理者工厂的单例接口
		/// @return 管理者工厂的单例
		static ManagerFactory* Instance();

		/// @brief 初始化管理者工厂
		/// @return 空
		ManagerFactory();

		/// @brief 销毁管理者工厂
		/// @return 空
		~ManagerFactory();

		/// @brief 根据管理者类型获取的管理者对象
		/// @param[in] Type 管理者类型
		/// @return	管理者对象
		std::shared_ptr<Interface::Manager> GetManager(const ManagerType& Type);
	private:
		/// @brief 内部存储的管理者
		std::map<ManagerType, std::shared_ptr<Interface::Manager>> m_Managers;
	};
}