#pragma once
#include <string>

namespace Smbios
{
	namespace LenovoDisplay
	{
		/// <summary>
		///	获取机器的SN码
		/// </summary>
		/// <returns>SN码</returns>
		std::wstring GetSNFromSMbios();

		/// <summary>
		/// 获取机器的名称
		/// </summary>
		/// <returns>机器显示名</returns>
		std::wstring GetMachineTypeFromSMbios();

		/// <summary>
		/// 获取机器的MT
		/// </summary>
		/// <returns>MT</returns>
		std::wstring GetMachineType();

		/// <summary>
		/// 获取机器的MTM
		/// </summary>
		/// <returns>MTM</returns>
		std::wstring GetMachineTypeModel();

		/// <summary>
		/// 获取机器的UUID
		/// </summary>
		/// <returns>UUID</returns>
		std::wstring GetMachineUUID();

		/// <summary>
		/// 判断你是否是联想的BIOS
		/// </summary>
		/// <returns></returns>
		bool IsLenovo();

		/// <summary>
		/// 判断是否是Think系列机型
		/// </summary>
		/// <returns></returns>
		bool IsThink();
	};
}