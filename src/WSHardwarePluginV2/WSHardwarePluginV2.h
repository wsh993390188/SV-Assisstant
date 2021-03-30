/*!
* @file WSHardwarePluginV2.h
* @brief 硬件模块导出接口
*
*
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月2日
*/

#ifdef _WINDLL
#ifdef WSHARDWAREPLUGINV2_EXPORTS
/// @brief DLL 导入or导出接口定义符号
#define WSHARDWAREPLUGINV2_API __declspec(dllexport)
#else
#define WSHARDWAREPLUGINV2_API __declspec(dllimport)
#endif // WSHARDWAREPLUGINV2_EXPORTS
#else
#define WSHARDWAREPLUGINV2_API
#endif

/// @brief 回调函数类型
using PluginCallback = int(*)(LPCSTR, const char*);

#ifdef __cplusplus
extern "C" {
#endif // __

	/// @brief 硬件的调用动作接口
	///			对外
	/// @param[in] action 本次的动作信息
	/// @param[in] paramter 本次的参数JSON格式信息
	/// @param[out] response 本次操作完成的回应Json信息
	/// @return
	///			0 操作成功
	///			其他值 操作失败
	WSHARDWAREPLUGINV2_API int PcmHardwareAction(LPCSTR action, LPCSTR paramter, BSTR* response);

	/// @brief 初始化插件接口
	///			注册所需要的回调函数以供通知用
	/// @param[in] paramter 本次的参数JSON格式信息
	/// @param[in] callback 本次的回调函数指针
	/// @return
	///			S_OK 操作成功
	WSHARDWAREPLUGINV2_API int PcmHardwareInitialize(LPCSTR paramter, PluginCallback callback);

#ifdef __cplusplus
}
#endif
/// @brief 初始化硬盘信息
constexpr auto PCM_HARDWARE_ACTION_DISK_INIT = "PCM_HARDWARE_ACTION_DISK_INIT";
/// @brief 更新硬盘信息
constexpr auto PCM_HARDWARE_ACTION_DISK_UPDATE = "PCM_HARDWARE_ACTION_DISK_UPDATE";
/// @brief 获取硬盘信息
constexpr auto PCM_HARDWARE_ACTION_DISK_GET = "PCM_HARDWARE_ACTION_DISK_GET";

/// @brief 初始化CPU信息
constexpr auto PCM_HARDWARE_ACTION_CPU_INIT = "PCM_HARDWARE_ACTION_CPU_INIT";
/// @brief 更新CPU信息
constexpr auto PCM_HARDWARE_ACTION_CPU_UPDATE = "PCM_HARDWARE_ACTION_CPU_UPDATE";
/// @brief 获取CPU信息
constexpr auto PCM_HARDWARE_ACTION_CPU_GET = "PCM_HARDWARE_ACTION_CPU_GET";

/// @brief 初始化内存信息
constexpr auto PCM_HARDWARE_ACTION_MEMORY_INIT = "PCM_HARDWARE_ACTION_MEMORY_INIT";
/// @brief 更新内存信息
constexpr auto PCM_HARDWARE_ACTION_MEMORY_UPDATE = "PCM_HARDWARE_ACTION_MEMORY_UPDATE";
/// @brief 获取内存信息
constexpr auto PCM_HARDWARE_ACTION_MEMORY_GET = "PCM_HARDWARE_ACTION_MEMORY_GET";

/// @brief 初始化GPU信息
constexpr auto PCM_HARDWARE_ACTION_GPU_INIT = "PCM_HARDWARE_ACTION_GPU_INIT";
/// @brief 更新GPU信息
constexpr auto PCM_HARDWARE_ACTION_GPU_UPDATE = "PCM_HARDWARE_ACTION_GPU_UPDATE";
/// @brief 获取GPU信息
constexpr auto PCM_HARDWARE_ACTION_GPU_GET = "PCM_HARDWARE_ACTION_GPU_GET";

/// @brief 初始化主板及BIOS信息
constexpr auto PCM_HARDWARE_ACTION_BROAD_INIT = "PCM_HARDWARE_ACTION_BROAD_INIT";
/// @brief 更新主板及BIOS信息
constexpr auto PCM_HARDWARE_ACTION_BROAD_UPDATE = "PCM_HARDWARE_ACTION_BROAD_UPDATE";
/// @brief 获取主板及BIOS信息
constexpr auto PCM_HARDWARE_ACTION_BROAD_GET = "PCM_HARDWARE_ACTION_BROAD_GET";

/// @brief 初始化声卡信息
constexpr auto PCM_HARDWARE_ACTION_AUDIO_INIT = "PCM_HARDWARE_ACTION_AUDIO_INIT";
/// @brief 更新声卡信息
constexpr auto PCM_HARDWARE_ACTION_AUDIO_UPDATE = "PCM_HARDWARE_ACTION_AUDIO_UPDATE";
/// @brief 获取声卡信息
constexpr auto PCM_HARDWARE_ACTION_AUDIO_GET = "PCM_HARDWARE_ACTION_AUDIO_GET";

/// @brief 初始化网卡信息
constexpr auto PCM_HARDWARE_ACTION_NIC_INIT = "PCM_HARDWARE_ACTION_NIC_INIT";
/// @brief 更新网卡信息
constexpr auto PCM_HARDWARE_ACTION_NIC_UPDATE = "PCM_HARDWARE_ACTION_NIC_UPDATE";
/// @brief 获取网卡信息
constexpr auto PCM_HARDWARE_ACTION_NIC_GET = "PCM_HARDWARE_ACTION_NIC_GET";

/// @brief 初始化电池信息
constexpr auto PCM_HARDWARE_ACTION_BATTERY_INIT = "PCM_HARDWARE_ACTION_BATTERY_INIT";
/// @brief 更新电池信息
constexpr auto PCM_HARDWARE_ACTION_BATTERY_UPDATE = "PCM_HARDWARE_ACTION_BATTERY_UPDATE";
/// @brief 获取电池信息
constexpr auto PCM_HARDWARE_ACTION_BATTERY_GET = "PCM_HARDWARE_ACTION_BATTERY_GET";

/// @brief 初始化显示器信息
constexpr auto PCM_HARDWARE_ACTION_MONITOR_INIT = "PCM_HARDWARE_ACTION_MONITOR_INIT";
/// @brief 更新显示器信息
constexpr auto PCM_HARDWARE_ACTION_MONITOR_UPDATE = "PCM_HARDWARE_ACTION_MONITOR_UPDATE";
/// @brief 获取显示器信息
constexpr auto PCM_HARDWARE_ACTION_MONITOR_GET = "PCM_HARDWARE_ACTION_MONITOR_GET";

/// @brief 初始化Win10生物传感器信息
constexpr auto PCM_HARDWARE_ACTION_WINBIO_INIT = "PCM_HARDWARE_ACTION_WINBIO_INIT";
/// @brief 更新Win10生物传感器信息
constexpr auto PCM_HARDWARE_ACTION_WINBIO_UPDATE = "PCM_HARDWARE_ACTION_WINBIO_UPDATE";
/// @brief 获取Win10生物传感器信息
constexpr auto PCM_HARDWARE_ACTION_WINBIO_GET = "PCM_HARDWARE_ACTION_WINBIO_GET";