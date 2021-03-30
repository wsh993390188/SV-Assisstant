#pragma once
#include "Interface.h"
#include "GenericMemory.h"
#include <string>
namespace Hardware
{
	/// @brief �ڴ���Ƚӿ���
	class MemoryManager : public Interface::Manager
	{
	public:
		/// @brief �ڴ���Ƚӿڹ��캯��
		MemoryManager();
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
		/// @brief �ڴ���Ϣʵ����
		std::unique_ptr<Memory::GenericMemory> MemoryControl;
	private:
	};
}
