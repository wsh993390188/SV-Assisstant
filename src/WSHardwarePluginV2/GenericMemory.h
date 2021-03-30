#pragma once
#include "memorydefination.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief �ڴ�ʵ�ֻ���
		/// Ŀǰֻ��һ����Ҫ�õ���һ����SMBIOS���࣬һ����SMBUS
		class GenericMemory
		{
		protected:
			/// @brief �ڴ���Ϣ��First���ڴ��ţ��Զ��壩��Second���ڴ�ͨ����Ϣ
			std::map<USHORT, MemoryCommonInformation> MemoryInformation;
			/// @brief ���������ڴ�Ĵ�С
			uint64_t PhysicalMemoryTotalSize;
			/// @brief �������õ��ڴ����������������ڴ��û���ڴ棩
			uint32_t MaxNumberOfMemory;
			/// @brief �����Ѿ�������ڴ���
			std::set<USHORT> AvailableMemoryIds;
		public:
			/// @brief ���캯��
			GenericMemory();

			/// @brief ��������
			virtual ~GenericMemory() = default;

			/// @brief ��ʼ���ڴ�����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize() = 0;

			/// @brief �����ڴ�����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update() = 0;

			/// @brief ������ʼ��JSON��Ϣ
			/// @param[out] response Utf-8��ʽJSON�ַ���
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType BuildInitializeJson(std::string& response);

			/// @brief ������ȡԪ�ص�Json
			/// @param[in] root Json����
			/// @param[out] DimmIds �ڴ�DimmIds
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType ParserDimmJson(const Json::Value& root, USHORT& DimmIds);

			/// @brief ����DIMM Id����Json
			/// @param[in] DIMMIds �ڴ�DimmIds
			/// @param[out] response Utf-8��ʽJSON�ַ���
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType BuildDIMMJson(const USHORT& DIMMIds, std::string& response);

			/// @brief ���л�SPD������
			/// @return @ref Data::ErrorType Ĭ�ϲ�֧��
			virtual Data::ErrorType SerializationSPD()
			{
				return Data::ErrorType::NOTSUPPORT;
			}

			/// @brief ��ȡ�ڴ���Ϣ
			/// @return @ref MemoryInformation
			std::add_const_t<decltype(Hardware::Memory::GenericMemory::MemoryInformation)>& GetMemoryInformation() const;

			/// @brief ��ȡ�����ڴ��С
			/// @return �����ڴ��С
			std::add_const_t<decltype(PhysicalMemoryTotalSize)> GetMemoryTotalSize() const
			{
				return PhysicalMemoryTotalSize;
			}
		private:
			/// @brief ��SMBIOS�л�ȡSPD����Ϣ,������ṹ��
			/// @param[in] SMBIOSMemoryInfo Type17���ڴ����Ϣ
			/// @param[in] DIMMId �Զ����DIMMId
			/// @return @ref Data::ErrorType
			Data::ErrorType GetSPDInformationFromSMBIOS(const Smbios::Type17& SMBIOSMemoryInfo, const USHORT& DIMMId);

			/// @brief ��SMBIOS�л�ȡSPD����Ϣ,������ṹ��
			/// @param[out] MemoryInfo SPD��������ڴ���Ϣ
			/// @param[in] SMBIOSMemoryInfo Type17���ڴ����Ϣ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetSPDInformationFromSMBIOS(MemoryCommonInformation& MemoryInfo, const Smbios::Type17& SMBIOSMemoryInfo);

			/// @brief �����ڴ���Ϣ����JSON����
			/// @param[in] Info �ڴ���Ϣ
			/// @return Utf-8��ʽJSON�ַ���
			std::string BuildMemoryCommonInformationToJson(const MemoryCommonInformation& Info);
		};
	}
}
