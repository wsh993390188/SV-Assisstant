/*!
* @file IntelCPU.h
* @brief ��ȡIntel CPU��Ϣ
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��5��
*/
#pragma once
#include "GenericCPU.h"

namespace Hardware
{
	namespace CPU
	{
		class IntelTemperature final : public CPUDecorator
		{
		public:
			explicit IntelTemperature(const uint64_t MsrRegistry, const std::string& Name, const int64_t TjMax);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		private:
			/// @brief �¶����ֵ
			const int64_t m_Tjmax;
			/// @brief �����¶�
			int64_t m_TjCurrent;
		};

		class IntelPower final : public CPUDecorator
		{
		public:
			explicit IntelPower(const uint64_t MsrRegistry, const std::string& Name, const double energyUnitMultiplier, const uint32_t lastEnergyConsumed);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief ���¹���
			double m_CurrentPower;
			const double m_EnergyUnitMultiplier;
			uint32_t m_LastEnergyConsumed;
			decltype(std::chrono::high_resolution_clock::now()) m_LastEnergyTime;
		};

		class IntelVoltage final : public CPUDecorator
		{
		public:
			explicit IntelVoltage(const uint64_t MsrRegistry, const std::string& Name);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief ���µ�ѹ
			double m_CurrentVoltage;
		};

		/// @brief intel��CPUƵ����Ϣ
		class IntelFrequency final : public CPUDecorator
		{
		public:
			explicit IntelFrequency(const uint64_t MsrRegistry, const std::string& Name, const uint64_t TscFrequencyMhz);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief ��TSC��ȡBusSpeed
			/// @param[in] coreid ��Ҫ��coreId
			/// @return Bus Speed
			double GetBusSpeedByTsc(const int32_t& coreid);
		private:
			/// @brief ����Ƶ��
			double m_CurrentFrequency;
			/// @brief �趨��TSC�̶�Ƶ��
			double m_TscFrequencyMhz;
		};

		/// @brief IntelCPU��Ϣ��ȡ
		class IntelCPU final : public GenericCPU
		{
		public:
			/// @brief ����IntelCPU�������
			/// @return
			IntelCPU() noexcept;

			/// @brief �ͷ�IntelCPUռ�õ���Դ
			~IntelCPU() noexcept;

			Data::ErrorType Initialize(std::string& response) override final;
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			/// @brief ��CPUID�г�ʼ��Socket��Դ��֧��Dual Socket
			/// @return true �����ɹ�
			bool InitializeSocket();

			/// @brief ��WMI�г�ʼ��Socket��Դ,��֧��Single Socket
			/// @return true �����ɹ�
			bool InitializeSocketFromWMI();

			/// @brief ����Intel CPUBrand��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			/// @return @ref Hardware::CPU::CPUDB::CPUQueryInfo
			std::unique_ptr<Hardware::CPU::CPUDB::CPUQueryInfo> ParserCPUBrandName(Socket& soc);

			/// @brief ��CPUID�л�ȡCPU��Ϣ
			void GetInfoFromCPUID();

			/// @brief ��CPUID1��7�л�ȡCPU��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUID1_7(Socket& soc);

			/// @brief ��CPUID4�л�ȡCache��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUID4(Socket& soc);

			/// @brief ��CPUID 80000002�л�ȡCPU��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUID80000002(Socket& soc);

			/// @brief ��CPUDB�л�ȡCPU��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUDB(Socket& soc);

			/// @brief ����CPU��FMS��ȡ�ܹ���Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetCodeName(Socket& soc);

			/// @brief ����CPU��FMS��ȡ������Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetTechnology(Socket& soc);

			/// @brief ����CPU MSR��ȡSpec TDP
			/// @param[in,out] soc cpu��Ϣ
			void GetSpecPower(Socket& soc);

			/// @brief ��SMBIOS�л�ȡcpu�ķ�װ��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetSocketPackageFromSmbios(Socket& soc);

			/// @brief ��ȡCPU������¶���ֵ
			/// @param[in] soc cpu��Ϣ
			/// @param[in] msr Msr�ӿ�ָ��
			/// @return �¶����ֵ
			uint64_t GetTjMax(const Socket& soc, const std::shared_ptr<Hardware::Utils::Ring0::SafeMsrHandle>& msr);

			/// @brief ��Msr�л�ȡ�¶����ֵ,��ȡ����ʱĬ��Ϊ100
			/// @param[in] msr Msr�ӿ�ָ��
			/// @return �¶����ֵ
			uint64_t GetTjMaxFromMsr(const std::shared_ptr<Hardware::Utils::Ring0::SafeMsrHandle>& msr);

			/// @brief ���붯̬��Ϣ
			void AddDynamicInfo();

			/// @brief ���Socket���붯̬��Ϣ
			void AddDynamicInfoForSocket();

			/// @brief ��Գ��̼߳��붯̬��Ϣ
			void AddDynamicInfoForHyperThead();

			/// @brief ΪSocket���ӷ�װ�¶���Ϣ
			/// @param[in] soc Socket
			void AddPackageTemperature(Socket& soc);

			/// @brief ��ÿ�����߳����Ӻ��¶���Ϣ
			/// @param[in] soc Socket
			/// @param[in] thread ���߳�
			void AddCoreTemperature(const Socket& soc, std::weak_ptr<HyperThread> thread);
			/// @brief ��ÿ�����߳����Ӻ˵�ѹ��Ϣ
			/// @param[in] thread ���߳�
			void AddCoreVoltage(std::weak_ptr<HyperThread> thread);

			/// @brief ��ÿ�����߳����Ӻ�Ƶ����Ϣ
			/// @param[in] thread ���߳�
			void AddCoreFrequency(std::weak_ptr<HyperThread> thread);

			/// @brief ��Socket���ӹ�����Ϣ
			/// @param[in] soc Socket
			/// @param[in] MsrRegistry ���ʶ�Ӧ��MSR�Ĵ�����ַ
			/// @param[in] PowerName ��������
			void AddPower(Socket& soc, const uint64_t MsrRegistry, const std::string& PowerName);

		private:
			/// @brief ����CPU����or�̵߳ĸ������ݽṹ
			std::vector<TopologyEntry> topology;
			/// @brief �޶�����CPU��Family
			uint32_t DisplayFamily;
			/// @brief �޶�����CPU��Model
			uint32_t DisplayModel;
		};
	}
}
