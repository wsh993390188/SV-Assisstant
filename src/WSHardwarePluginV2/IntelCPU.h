/*!
* @file IntelCPU.h
* @brief 获取Intel CPU信息
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
		class IntelTemperature final : public CPUDecorator
		{
		public:
			explicit IntelTemperature(const uint64_t MsrRegistry, const std::string& Name, const int64_t TjMax);
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		private:
			/// @brief 温度最大值
			const int64_t m_Tjmax;
			/// @brief 最新温度
			int64_t m_TjCurrent;
		};

		class IntelPower final : public CPUDecorator
		{
		public:
			explicit IntelPower(const uint64_t MsrRegistry, const std::string& Name, const double energyUnitMultiplier, const uint32_t lastEnergyConsumed);
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief 最新功率
			double m_CurrentPower;
			const double m_EnergyUnitMultiplier;
			uint32_t m_LastEnergyConsumed;
			decltype(std::chrono::high_resolution_clock::now()) m_LastEnergyTime;
		};

		class IntelVoltage final : public CPUDecorator
		{
		public:
			explicit IntelVoltage(const uint64_t MsrRegistry, const std::string& Name);
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief 最新电压
			double m_CurrentVoltage;
		};

		/// @brief intel的CPU频率信息
		class IntelFrequency final : public CPUDecorator
		{
		public:
			explicit IntelFrequency(const uint64_t MsrRegistry, const std::string& Name, const uint64_t TscFrequencyMhz);
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief 从TSC获取BusSpeed
			/// @param[in] coreid 需要的coreId
			/// @return Bus Speed
			double GetBusSpeedByTsc(const int32_t& coreid);
		private:
			/// @brief 最新频率
			double m_CurrentFrequency;
			/// @brief 设定的TSC固定频率
			double m_TscFrequencyMhz;
		};

		/// @brief IntelCPU信息获取
		class IntelCPU final : public GenericCPU
		{
		public:
			/// @brief 配置IntelCPU必须参数
			/// @return
			IntelCPU() noexcept;

			/// @brief 释放IntelCPU占用的资源
			~IntelCPU() noexcept;

			Data::ErrorType Initialize(std::string& response) override final;
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			/// @brief 从CPUID中初始化Socket资源，支持Dual Socket
			/// @return true 操作成功
			bool InitializeSocket();

			/// @brief 从WMI中初始化Socket资源,仅支持Single Socket
			/// @return true 操作成功
			bool InitializeSocketFromWMI();

			/// @brief 解析Intel CPUBrand信息
			/// @param[in,out] soc cpu信息
			/// @return @ref Hardware::XMLConfig::QueryInfo
			std::unique_ptr<Hardware::XMLConfig::QueryInfo> ParserCPUBrandName(Socket& soc);

			/// @brief 从CPUID中获取CPU信息
			void GetInfoFromCPUID();

			/// @brief 从CPUID1和7中获取CPU信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUID1_7(Socket& soc);

			/// @brief 从CPUID4中获取Cache信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUID4(Socket& soc);

			/// @brief 从CPUID 80000002中获取CPU信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUID80000002(Socket& soc);

			/// @brief 从CPUDB中获取CPU信息
			/// @param[in,out] soc cpu信息
			void GetInfoFromCPUDB(Socket& soc);

			/// @brief 根据CPU的FMS获取架构信息
			/// @param[in,out] soc cpu信息
			void GetCodeName(Socket& soc);

			/// @brief 根据CPU的FMS获取工艺信息
			/// @param[in,out] soc cpu信息
			void GetTechnology(Socket& soc);

			/// @brief 根据CPU MSR获取Spec TDP
			/// @param[in,out] soc cpu信息
			void GetSpecPower(Socket& soc);

			/// @brief 从SMBIOS中获取cpu的封装信息
			/// @param[in,out] soc cpu信息
			void GetSocketPackageFromSmbios(Socket& soc);

			/// @brief 获取CPU的最大温度阈值
			/// @param[in] soc cpu信息
			/// @param[in] msr Msr接口指针
			/// @return 温度最大值
			uint64_t GetTjMax(const Socket& soc, const std::shared_ptr<Hardware::Utils::Ring0::SafeMsrHandle>& msr);

			/// @brief 从Msr中获取温度最大值,获取不到时默认为100
			/// @param[in] msr Msr接口指针
			/// @return 温度最大值
			uint64_t GetTjMaxFromMsr(const std::shared_ptr<Hardware::Utils::Ring0::SafeMsrHandle>& msr);

			/// @brief 加入动态信息
			void AddDynamicInfo();

			/// @brief 针对Socket加入动态信息
			void AddDynamicInfoForSocket();

			/// @brief 针对超线程加入动态信息
			void AddDynamicInfoForHyperThead();

			/// @brief 为Socket增加封装温度信息
			/// @param[in] soc Socket
			void AddPackageTemperature(Socket& soc);

			/// @brief 对每个超线程增加核温度信息
			/// @param[in] soc Socket
			/// @param[in] thread 超线程
			void AddCoreTemperature(const Socket& soc, std::weak_ptr<HyperThread> thread);
			/// @brief 对每个超线程增加核电压信息
			/// @param[in] thread 超线程
			void AddCoreVoltage(std::weak_ptr<HyperThread> thread);

			/// @brief 对每个超线程增加核频率信息
			/// @param[in] thread 超线程
			void AddCoreFrequency(std::weak_ptr<HyperThread> thread);

			/// @brief 对Socket增加功率信息
			/// @param[in] soc Socket
			/// @param[in] MsrRegistry 功率对应的MSR寄存器地址
			/// @param[in] PowerName 功率名称
			void AddPower(Socket& soc, const uint64_t MsrRegistry, const std::string& PowerName);

		private:
			/// @brief 查找CPU核心or线程的辅助数据结构
			std::vector<TopologyEntry> topology;
			/// @brief 修订后主CPU的Family
			uint32_t DisplayFamily;
			/// @brief 修订后主CPU的Model
			uint32_t DisplayModel;
		};
	}
}
