#pragma once
#include <string>

namespace Smbios
{
	namespace LenovoDisplay
	{
		/// <summary>
		///	��ȡ������SN��
		/// </summary>
		/// <returns>SN��</returns>
		std::wstring GetSNFromSMbios();

		/// <summary>
		/// ��ȡ����������
		/// </summary>
		/// <returns>������ʾ��</returns>
		std::wstring GetMachineTypeFromSMbios();

		/// <summary>
		/// ��ȡ������MT
		/// </summary>
		/// <returns>MT</returns>
		std::wstring GetMachineType();

		/// <summary>
		/// ��ȡ������MTM
		/// </summary>
		/// <returns>MTM</returns>
		std::wstring GetMachineTypeModel();

		/// <summary>
		/// ��ȡ������UUID
		/// </summary>
		/// <returns>UUID</returns>
		std::wstring GetMachineUUID();

		/// <summary>
		/// �ж����Ƿ��������BIOS
		/// </summary>
		/// <returns></returns>
		bool IsLenovo();

		/// <summary>
		/// �ж��Ƿ���Thinkϵ�л���
		/// </summary>
		/// <returns></returns>
		bool IsThink();
	};
}