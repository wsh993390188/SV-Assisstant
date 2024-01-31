#pragma once
namespace Hardware
{
	namespace Audio
	{
		/// @brief 通用信息
		struct CommonInfo
		{
			std::wstring id; ///< 注册表设备id
			std::wstring state; ///< 设备状态
			std::wstring name;///<设备名
			std::wstring desc;///<设备描述
			std::wstring audioif;///<物理设备名称
		};

		/// @brief 设备信息
		struct DeviceInfo
		{
			std::map<uint32_t, CommonInfo> Audios;///<存在的发声设备
			std::map<uint32_t, CommonInfo> Mics;///<存在的麦克风设备
		};

		/// @brief 声卡通用初始化函数
		class GenericAudio
		{
		public:
			/// @brief 构造函数，用于初始化声卡信息
			GenericAudio() = default;

			/// @brief 初始化声卡
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新声卡信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取声卡基础数据操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief 声卡信息存储数据
			DeviceInfo AudioInfos;

			enum class DeviceType
			{
				Audio = 0,
				Mic,
			};
		protected:
			/// @brief 构建初始化Json
			/// @return Json字符
			std::string BuildInitializeJson();

			/// @brief 解析Json
			/// @param[in] JsonString Json字符
			/// @param[out] AudioId 音频Id
			/// @param[out] DeviceType 音频类型 @ref DeviceType
			/// @return 是否解析成功
			bool ParserJson(const std::string& JsonString, uint32_t& AudioId, DeviceType& DevType);

			/// @brief 构建音频元素Json字符
			/// @param[in] DeviceInfos 设备信息集合
			/// @param[in] AudioId 设备Id
			/// @return Json字符
			std::string BuildElementJson(const std::map<uint32_t, CommonInfo>& DeviceInfos, const uint32_t& AudioId);
		};
	}
}
