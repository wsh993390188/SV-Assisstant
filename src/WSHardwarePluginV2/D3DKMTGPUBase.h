#pragma once
#include "GenericGPU.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief ͨ��D3D�ӿڻ�ȡGPU����Ϣ
		class D3DKMTGPUBase final : public GenericGPU
		{
		public:
			/// @brief ��ʼ��GPU
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override;
			/// @brief ����GPU��Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override;

			/// @brief ��ȡGPU��������
			/// Json��ʽ {"SocketId": 0}
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override;
		private:
			/// @brief ��ʼ��D3DKMT
			/// @return �Ƿ��ʼ���ɹ�
			bool InitializeD3DKMT();
		};
	}
}
