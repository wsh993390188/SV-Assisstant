/*!
* @file AmdCPU.h
* @brief 获取AMD CPU信息
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月5日
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
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
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
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
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
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		private:
			std::function<double(const uint32_t, const uint32_t)> m_DealValueFn;
			const uint32_t m_Family;
			double m_CurrentVoltage;
		};

		/// @brief AMD CPU信息获取
		class AmdCPU final : public GenericCPU
		{
		public:
			AmdCPU();
			Data::ErrorType Initialize(std::string& response) override final;
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			bool InitializeSocketFromAPIC();
			bool InitializeSocketFromWMI();
			/// @brief 从CPUID中获取CPU信息
			void GetInfoFromCPUID();
			/// @brief 从CPUID1和80000001中获取CPU信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUID1_80000001(Socket& soc);

			/// @brief 从CPUID80000005 80000006中获取Cache信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUID80000005_6(Socket& soc);

			/// @brief 从CPUID 80000002中获取CPU信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUID80000002(Socket& soc);

			/// @brief 从CPUDB中获取CPU信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUDB(Socket& soc);

			/// @brief 加入动态信息
			void AddDynamicInfo();

			/// @brief 针对Socket加入动态信息
			void AddDynamicInfoForSocket();

			/// @brief 针对超线程加入动态信息
			void AddDynamicInfoForHyperThead();

			/// @brief 为Socket增加封装温度信息
			/// @param[in] soc Socket
			void AddPackageTemperature(Socket& soc);

			/// @brief 对每个超线程增加核电压信息
			/// @param[in] thread 超线程
			/// @param[in] Family Family代号
			/// @param[in] Model Model代号
			void AddCoreVoltage(std::weak_ptr<HyperThread> thread, const uint32_t Family, const uint32_t Model);

			/// @brief 对每个超线程增加核频率信息
			/// @param[in] thread 超线程
			/// @param[in] Family Family代号
			/// @param[in] Model Model代号
			void AddCoreFrequency(std::weak_ptr<HyperThread> thread, const uint32_t Family, const uint32_t Model);
		private:
			std::vector<TopologyEntry> topology;
		};
	}
}
