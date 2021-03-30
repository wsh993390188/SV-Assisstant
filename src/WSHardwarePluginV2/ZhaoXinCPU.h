/*!
* @file ZhaoXinCPU.h
* @brief 获取兆芯CPU信息
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
		class ZhaoxinTemperature final : public CPUDecorator
		{
		public:
			explicit ZhaoxinTemperature(const uint64_t MsrRegistry, const std::string& Name);
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief 最新温度
			double m_CurrentTemperature;
		};
		class ZhaoxinVoltage final : public CPUDecorator
		{
		public:
			explicit ZhaoxinVoltage(const uint64_t MsrRegistry, const std::string& Name);
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
		class ZhaoxinFrequency final : public CPUDecorator
		{
		public:
			explicit ZhaoxinFrequency(const uint64_t MsrRegistry, const std::string& Name, const double BusSpeed);
			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override;

			/// @brief 更新信息
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief 最新频率
			double m_CurrentFrequency;
			double m_BusSpeed;
		};
		/// @brief 兆芯CPU信息获取
		class ZhaoXinCPU final : public GenericCPU
		{
		public:
			ZhaoXinCPU();
			Data::ErrorType Initialize(std::string& response) override final;
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			bool InitializeSocketFromWMI();
			void GetInfoFromCPUID();
			void GetInfoFromCPUID1(Socket& soc);
			void GetInfoFromCPUID80000005_6(Socket& soc);
			void GetInfoFromCPUID80000002(Socket& soc);

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
			void AddCoreVoltage(std::weak_ptr<HyperThread> thread);

			/// @brief 对每个超线程增加核频率信息
			/// @param[in] thread 超线程
			void AddCoreFrequency(std::weak_ptr<HyperThread> thread);
		private:
			/// @brief 查找CPU核心or线程的辅助数据结构
			std::vector<TopologyEntry> topology;
		};
	}
}
