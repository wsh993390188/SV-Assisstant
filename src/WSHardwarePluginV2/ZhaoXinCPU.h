/*!
* @file ZhaoXinCPU.h
* @brief ��ȡ��оCPU��Ϣ
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
		class ZhaoxinTemperature final : public CPUDecorator
		{
		public:
			explicit ZhaoxinTemperature(const uint64_t MsrRegistry, const std::string& Name);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief �����¶�
			double m_CurrentTemperature;
		};
		class ZhaoxinVoltage final : public CPUDecorator
		{
		public:
			explicit ZhaoxinVoltage(const uint64_t MsrRegistry, const std::string& Name);
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
		class ZhaoxinFrequency final : public CPUDecorator
		{
		public:
			explicit ZhaoxinFrequency(const uint64_t MsrRegistry, const std::string& Name, const double BusSpeed);
			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override;

			/// @brief ������Ϣ
			void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) override;
		protected:
		private:
			/// @brief ����Ƶ��
			double m_CurrentFrequency;
			double m_BusSpeed;
		};
		/// @brief ��оCPU��Ϣ��ȡ
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
			void AddCoreVoltage(std::weak_ptr<HyperThread> thread);

			/// @brief ��ÿ�����߳����Ӻ�Ƶ����Ϣ
			/// @param[in] thread ���߳�
			void AddCoreFrequency(std::weak_ptr<HyperThread> thread);
		private:
			/// @brief ����CPU����or�̵߳ĸ������ݽṹ
			std::vector<TopologyEntry> topology;
		};
	}
}
