/*!
* @file Data.h
* @brief 通用数据接口
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月3日
*/
#pragma once

namespace Hardware
{
	/// @brief 通用数据
	namespace Data
	{
		/// @brief 返回值类型
		enum class ErrorType : int
		{
			SUCCESS = 0,		///< 操作成功
			UNKNOWN,			///< 未知的错误
			UNKNOWNCOMMAND,		///< 未知的命令
			UNKNOWNJSON,		///<不识别的JSON数据
			UNKNOWNCPU,			///<未知的CPU
			OVERBOUND,			///<索引越界
			NOTINITIALIZE,		///<未进行初始化就执行本函数
			NOTIMPLEMENTED,		///<未实现的功能
			NOTSUPPORT,			///<不支持的功能
			XMLERROR,			///<XML配置文件错误
			DATAEMPTY,			///<数据为空
			COMCREATEERROR,		///<创建Com数据时出错
		};
	}
}
