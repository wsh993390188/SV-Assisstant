#pragma once
#include "BaseSMBUS.h"
#include "IoHandle.h"

namespace Hardware
{
	namespace Memory
	{
		/// @brief ͨ�õ�SMBUS��ȡ�ڴ���Ϣ
		class CommonSMBUS final : public SMBUSControllerBase
		{
		public:
			/// @brief ���캯��
			CommonSMBUS();
			/// @brief �ж�DIMM��Ӧ�ڴ��Ƿ����
			/// @param[in] DIMMId DIMMId
			/// @return �Ƿ�����ڴ��豸
			bool IsDeviceOnline(const USHORT& DIMMId) override;

		private:
			/// @brief ��0-7ת��SMBUS��ʶ���DIMMId,������Public������DIMMId
			/// @param[in] id id��
			/// @return DIMMId
			USHORT ConverterIdToDIMMId(const USHORT& id);

			/// @brief ��ȡSMBUS��IO��ַ
			/// @param[in] VendorID Vendor��
			/// @param[out] SMbusBaseAddress IO��ַ
			/// @return �Ƿ�ɹ�
			bool GetSMbusBaseAddr(const USHORT& VendorID, USHORT& SMbusBaseAddress);

			/// @brief Ѱ��SMBUS��PCI�豸����Intel����
			/// @param[in] bus bus��
			/// @param[in] dev dev��
			/// @param[out] func func��
			/// @return �����Ƿ�ɹ�
			bool FindIntelSmbusControl(const USHORT& bus, const USHORT& dev, USHORT& func);

			/// @brief ��ȡ������SPD����Ϣ
			/// @param DIMMId DIMM Id
			/// @param MemoryInfo SPDͨ����Ϣ
			/// @return �����Ƿ�ɹ�
			bool ParserSPD(const USHORT& DIMMId, MemoryCommonInformation& MemoryInfo)override final;

			/// @brief ��ȡSPD��Ϣ
			/// @param DIMMId DIMM id
			/// @param data SPDͨ����Ϣ
			/// @return ��ȡ�Ƿ�ɹ�
			bool ReadSPD(const USHORT& DIMMId, MemoryCommonInformation& data);

		private:

			/// @brief ��ȡDDR3��SPD
			/// @param DIMMId DIMM id
			/// @return ��ȡ�Ƿ�ɹ�
			bool ReadDDR3SPD(const USHORT& DIMMId);

			/// @brief ��ȡDDR4��SPD
			/// @param DIMMId DIMM id
			/// @return ��ȡ�Ƿ�ɹ�
			bool ReadDDR4SPD(const USHORT& DIMMId);

			/// @brief ��ȡDDR5��SPD
			/// @param DIMMId DIMM id
			/// @return ��ȡ�Ƿ�ɹ�
			bool ReadDDR5SPD(const USHORT& DIMMId);

			/// @brief ���ֽڷ�ʽ��ȡSPD�е���Ϣ
			/// @param[in] Offset DIMM�ı���
			/// @param[out] val ��ȡ���ֵ
			bool ReadSPDByte(const uint8_t& Offset, DWORD& val);

			/// @brief ����SPD���ڴ�ռ�
			/// @param DDRSize DDR�Ĵ�С
			/// @param DDRType DDR������
			void AllocateSPDSize(const uint8_t& DDRSize, const uint8_t& DDRType);
		private:
			/// @brief �ȴ�Smbus Ready
			/// @return �Ƿ�Ready
			bool smbus_wait_until_ready();

			/// @brief �ȴ�Smbus�����Ƿ����
			/// @return 0Ϊ�ɹ�����ֵΪʧ��
			int smbus_wait_until_done();

			/// @brief Ԥ��ʼ��SMBUS
			/// @return �Ƿ�ɹ���ʼ��SMBUS
			bool PrepareSmbus();

			/// @brief д��Pm�Ĵ���
			/// @param reg �Ĵ�����ַ
			/// @param data �Ĵ�������
			void writePmReg(const UINT8& reg, const UINT8& data);

			/// @brief ����ͬ��
			void setupFch();

			/// @brief �л�ҳ��
			/// @param data �Ĵ�������
			/// @return �Ƿ�ɹ��л�SPD
			bool SwitchToPage(const UINT8& data);
		private:
			/// @brief SMbus����ַ
			USHORT SmbusBase;
			/// @brief IO����ָ��
			Utils::Ring0::SafeIoHandle IoPtr;
			/// @brief SPD������
			std::vector<uint8_t> SPDData;
		};
	}
}
