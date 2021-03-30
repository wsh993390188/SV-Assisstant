#pragma once
#include "Interface.h"
#include "GenericGPU.h"
#include <string>
namespace Hardware
{
	/// @brief �Կ����Ƚӿ���
	class GPUManager : public Interface::Manager
	{
	public:
		/// @brief �Կ����Ƚӿڹ��캯��
		GPUManager();
		/// @brief ��ʼ���������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType Init(LPCSTR paramter, std::string& response) override final;

		/// @brief ���������������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType Update(LPCSTR paramter, std::string& response) override final;

		/// @brief ��ȡ�������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override final;
	private:
		std::unique_ptr<GPU::GenericGPU> GPUController;
	};
}
