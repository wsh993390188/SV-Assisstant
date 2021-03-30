/*!
* @file CPUManager.h
* @brief CPU������
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��3��
*/
#pragma once
#include "Interface.h"
#include "GenericCPU.h"

namespace Hardware
{
	/// @brief CPU��������
	class CPUManager : public Interface::Manager
	{
	public:
		/// @brief ��ʼ���������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType Init(LPCSTR paramter, std::string& response) override;

		/// @brief ���������������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType Update(LPCSTR paramter, std::string& response) override;

		/// @brief ��ȡ�������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override;
	private:
		/// @brief CPU����
		/// @TODO ARM CPUӦ��δ���
		enum class CPU_Brands
		{
			/// @brief δ֪��CPU
			Unknown,
			/// @brief IntelCPU
			Intel,
			/// @brief AmdCPU
			Amd,
			/// @brief ��оCPU
			Zhaoxin
		};
		/// @brief ��CPUID�л�ȡCPUƷ��
		/// @return @ref CPU_Brands
		static const CPU_Brands GetCPUBrandsFromCPUID() noexcept;
	private:
		/// @brief CPU����ʵ�ʴ洢λ��
		std::unique_ptr<CPU::GenericCPU> m_CPUs;
	};
}
