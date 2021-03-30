/*!
* @file AmdCPU.h
* @brief ��ȡAMD CPU��Ϣ
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
		class AMDTemperature final : public CPUDecorator
		{
		public:
			AMDTemperature() = delete;
			explicit AMDTemperature(const uint64_t MsrRegistry, const std::string& Name, std::function<double(const uint32_t, const uint32_t)> DealTemperatureFn, const uint32_t Family, const uint32_t Model);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		private:
			const uint32_t m_Family;
			const uint32_t m_Model;
			double m_CurrentTemperature;
			std::function<double(const uint32_t, const uint32_t)> m_DealTemperatureFn;
		};

		class AMDFrequency final : public CPUDecorator
		{
		public:
			explicit AMDFrequency(const uint64_t MsrRegistry, const std::string& Name, const uint32_t Family, std::function<double(const uint32_t, const uint32_t)> DealValueFn, const uint32_t MaxClockSpeed);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		private:
			double GetBusSpeedByTsc(const int32_t& coreid);
		private:
			std::function<double(const uint32_t, const uint32_t)> m_DealValueFn;
			const uint32_t m_Family;
			double m_CurrentFrequency;
			const uint32_t m_TscFrequencyMhz;
		};

		class AMDVoltage final : public CPUDecorator
		{
		public:
			explicit AMDVoltage(const uint64_t MsrRegistry, const std::string& Name, const uint32_t Family, std::function<double(const uint32_t, const uint32_t)> DealValueFn);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		private:
			std::function<double(const uint32_t, const uint32_t)> m_DealValueFn;
			const uint32_t m_Family;
			double m_CurrentVoltage;
		};

		/// @brief AMD CPU��Ϣ��ȡ
		class AmdCPU final : public GenericCPU
		{
		public:
			AmdCPU();
			Data::ErrorType Initialize(std::string& response) override final;
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			bool InitializeSocketFromAPIC();
			bool InitializeSocketFromWMI();
			/// @brief ��CPUID�л�ȡCPU��Ϣ
			void GetInfoFromCPUID();
			/// @brief ��CPUID1��80000001�л�ȡCPU��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUID1_80000001(Socket& soc);

			/// @brief ��CPUID80000005 80000006�л�ȡCache��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUID80000005_6(Socket& soc);

			/// @brief ��CPUID 80000002�л�ȡCPU��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUID80000002(Socket& soc);

			/// @brief ��CPUDB�л�ȡCPU��Ϣ
			/// @param[in,out] soc cpu��Ϣ
			void GetInfoFromCPUDB(Socket& soc);

			/// @brief ���붯̬��Ϣ
			void AddDynamicInfo();

			/// @brief ���Socket���붯̬��Ϣ
			void AddDynamicInfoForSocket();

			/// @brief ��Գ��̼߳��붯̬��Ϣ
			void AddDynamicInfoForHyperThead();

			/// @brief ΪSocket���ӷ�װ�¶���Ϣ
			/// @param[in] soc Socket
			void AddPackageTemperature(Socket& soc);

			/// @brief ��ÿ�����߳����Ӻ˵�ѹ��Ϣ
			/// @param[in] thread ���߳�
			/// @param[in] Family Family����
			/// @param[in] Model Model����
			void AddCoreVoltage(std::weak_ptr<HyperThread> thread, const uint32_t Family, const uint32_t Model);

			/// @brief ��ÿ�����߳����Ӻ�Ƶ����Ϣ
			/// @param[in] thread ���߳�
			/// @param[in] Family Family����
			/// @param[in] Model Model����
			void AddCoreFrequency(std::weak_ptr<HyperThread> thread, const uint32_t Family, const uint32_t Model);
		private:
			std::vector<TopologyEntry> topology;
		};
	}
}
