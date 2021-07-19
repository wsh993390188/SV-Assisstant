/// @bug 目前都是用DeviceID查找GPU设备的，如果遇到相同的DID就瞎了
#pragma once
#include "GPUDeviceBase.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief GPU的查询结构
		struct GPUQuery
		{
			std::uint32_t VID;///<VendorId
			std::uint32_t DID;///<DeviceId
			/// @brief 初始化结构
			/// @param vid VendorId
			/// @param did DeviceId
			GPUQuery(const uint32_t& vid, const uint32_t& did) : VID(vid), DID(did) {}

			friend bool operator<(const GPUQuery& This, const GPUQuery& Other)
			{
				if (Other.VID != This.VID)
					return Other.VID < This.VID;
				else
					return Other.DID < This.DID;
			}
		};

		/// @brief GPU通用初始化函数
		class GenericGPU
		{
		public:
			/// @brief 构造函数，用于初始化GPU信息
			GenericGPU();

			/// @brief 虚析构函数
			virtual ~GenericGPU() = default;

			/// @brief 初始化GPU
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新GPU信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取GPU基础操作
			/// Json格式 {"SocketId": 0}
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType GetElements(LPCSTR paramter, std::string& response) = 0;

		protected:
			/// @brief GPU的实例类
			std::map<uint32_t, std::shared_ptr<GPUDeviceBase>> GPUInfos;
		protected:
			/// @brief 构建初始化JSON数据
			/// @return JSON Utf-8字符
			std::string BuildInitializeJson();

			/// @brief 构建显卡信息的JSON字符
			/// @param[in] GPUId 显卡ID
			/// @return JSON字符
			bool ParserJson(const std::string& JsonString, uint32_t& GPUId);
		};

		/// @brief 显卡控制对象工厂类
		class GPUInstanceFactory final
		{
			/// @brief 默认构造函数
			GPUInstanceFactory() = default;
		public:
			/// @brief 单例函数
			/// @return 单例对象
			static GPUInstanceFactory& Instance();
			/// @brief 创建电池
			/// @param[in] DevicePath 电池的设备路径
			/// @return 电池元素 @ref BatteryElement
			std::unique_ptr<GenericGPU> CreateGPU();
		};
	}
}
