#pragma once
#include "GenericWinBio.h"
namespace Hardware
{
	namespace WinBio
	{
		/// @brief WinBio�Ĺ���ʵ�ֺ���
		class WinBioImpl :
			public GenericWinBio
		{
		public:
			/// @brief ���캯��
			WinBioImpl();

			/// @brief ��ʼ��Win10�����ﴫ����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief ����Win10�����ﴫ������Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			/// @brief WinBio�豸���
			uint32_t Number;

			/// @brief ö��Winbio���õ��豸
			/// @param[in] Type �豸����
			/// @return ���
			bool EnumerateSensors(const uint32_t& Type);
		};
	}
}
