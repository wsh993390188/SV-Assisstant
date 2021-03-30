/*!
* @file WSHardwarePluginV2.h
* @brief Ӳ��ģ�鵼���ӿ�
*
*
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��2��
*/

#ifdef _WINDLL
#ifdef WSHARDWAREPLUGINV2_EXPORTS
/// @brief DLL ����or�����ӿڶ������
#define WSHARDWAREPLUGINV2_API __declspec(dllexport)
#else
#define WSHARDWAREPLUGINV2_API __declspec(dllimport)
#endif // WSHARDWAREPLUGINV2_EXPORTS
#else
#define WSHARDWAREPLUGINV2_API
#endif

/// @brief �ص���������
using PluginCallback = int(*)(LPCSTR, const char*);

#ifdef __cplusplus
extern "C" {
#endif // __

	/// @brief Ӳ���ĵ��ö����ӿ�
	///			����
	/// @param[in] action ���εĶ�����Ϣ
	/// @param[in] paramter ���εĲ���JSON��ʽ��Ϣ
	/// @param[out] response ���β�����ɵĻ�ӦJson��Ϣ
	/// @return
	///			0 �����ɹ�
	///			����ֵ ����ʧ��
	WSHARDWAREPLUGINV2_API int PcmHardwareAction(LPCSTR action, LPCSTR paramter, BSTR* response);

	/// @brief ��ʼ������ӿ�
	///			ע������Ҫ�Ļص������Թ�֪ͨ��
	/// @param[in] paramter ���εĲ���JSON��ʽ��Ϣ
	/// @param[in] callback ���εĻص�����ָ��
	/// @return
	///			S_OK �����ɹ�
	WSHARDWAREPLUGINV2_API int PcmHardwareInitialize(LPCSTR paramter, PluginCallback callback);

#ifdef __cplusplus
}
#endif
/// @brief ��ʼ��Ӳ����Ϣ
constexpr auto PCM_HARDWARE_ACTION_DISK_INIT = "PCM_HARDWARE_ACTION_DISK_INIT";
/// @brief ����Ӳ����Ϣ
constexpr auto PCM_HARDWARE_ACTION_DISK_UPDATE = "PCM_HARDWARE_ACTION_DISK_UPDATE";
/// @brief ��ȡӲ����Ϣ
constexpr auto PCM_HARDWARE_ACTION_DISK_GET = "PCM_HARDWARE_ACTION_DISK_GET";

/// @brief ��ʼ��CPU��Ϣ
constexpr auto PCM_HARDWARE_ACTION_CPU_INIT = "PCM_HARDWARE_ACTION_CPU_INIT";
/// @brief ����CPU��Ϣ
constexpr auto PCM_HARDWARE_ACTION_CPU_UPDATE = "PCM_HARDWARE_ACTION_CPU_UPDATE";
/// @brief ��ȡCPU��Ϣ
constexpr auto PCM_HARDWARE_ACTION_CPU_GET = "PCM_HARDWARE_ACTION_CPU_GET";

/// @brief ��ʼ���ڴ���Ϣ
constexpr auto PCM_HARDWARE_ACTION_MEMORY_INIT = "PCM_HARDWARE_ACTION_MEMORY_INIT";
/// @brief �����ڴ���Ϣ
constexpr auto PCM_HARDWARE_ACTION_MEMORY_UPDATE = "PCM_HARDWARE_ACTION_MEMORY_UPDATE";
/// @brief ��ȡ�ڴ���Ϣ
constexpr auto PCM_HARDWARE_ACTION_MEMORY_GET = "PCM_HARDWARE_ACTION_MEMORY_GET";

/// @brief ��ʼ��GPU��Ϣ
constexpr auto PCM_HARDWARE_ACTION_GPU_INIT = "PCM_HARDWARE_ACTION_GPU_INIT";
/// @brief ����GPU��Ϣ
constexpr auto PCM_HARDWARE_ACTION_GPU_UPDATE = "PCM_HARDWARE_ACTION_GPU_UPDATE";
/// @brief ��ȡGPU��Ϣ
constexpr auto PCM_HARDWARE_ACTION_GPU_GET = "PCM_HARDWARE_ACTION_GPU_GET";

/// @brief ��ʼ�����弰BIOS��Ϣ
constexpr auto PCM_HARDWARE_ACTION_BROAD_INIT = "PCM_HARDWARE_ACTION_BROAD_INIT";
/// @brief �������弰BIOS��Ϣ
constexpr auto PCM_HARDWARE_ACTION_BROAD_UPDATE = "PCM_HARDWARE_ACTION_BROAD_UPDATE";
/// @brief ��ȡ���弰BIOS��Ϣ
constexpr auto PCM_HARDWARE_ACTION_BROAD_GET = "PCM_HARDWARE_ACTION_BROAD_GET";

/// @brief ��ʼ��������Ϣ
constexpr auto PCM_HARDWARE_ACTION_AUDIO_INIT = "PCM_HARDWARE_ACTION_AUDIO_INIT";
/// @brief ����������Ϣ
constexpr auto PCM_HARDWARE_ACTION_AUDIO_UPDATE = "PCM_HARDWARE_ACTION_AUDIO_UPDATE";
/// @brief ��ȡ������Ϣ
constexpr auto PCM_HARDWARE_ACTION_AUDIO_GET = "PCM_HARDWARE_ACTION_AUDIO_GET";

/// @brief ��ʼ��������Ϣ
constexpr auto PCM_HARDWARE_ACTION_NIC_INIT = "PCM_HARDWARE_ACTION_NIC_INIT";
/// @brief ����������Ϣ
constexpr auto PCM_HARDWARE_ACTION_NIC_UPDATE = "PCM_HARDWARE_ACTION_NIC_UPDATE";
/// @brief ��ȡ������Ϣ
constexpr auto PCM_HARDWARE_ACTION_NIC_GET = "PCM_HARDWARE_ACTION_NIC_GET";

/// @brief ��ʼ�������Ϣ
constexpr auto PCM_HARDWARE_ACTION_BATTERY_INIT = "PCM_HARDWARE_ACTION_BATTERY_INIT";
/// @brief ���µ����Ϣ
constexpr auto PCM_HARDWARE_ACTION_BATTERY_UPDATE = "PCM_HARDWARE_ACTION_BATTERY_UPDATE";
/// @brief ��ȡ�����Ϣ
constexpr auto PCM_HARDWARE_ACTION_BATTERY_GET = "PCM_HARDWARE_ACTION_BATTERY_GET";

/// @brief ��ʼ����ʾ����Ϣ
constexpr auto PCM_HARDWARE_ACTION_MONITOR_INIT = "PCM_HARDWARE_ACTION_MONITOR_INIT";
/// @brief ������ʾ����Ϣ
constexpr auto PCM_HARDWARE_ACTION_MONITOR_UPDATE = "PCM_HARDWARE_ACTION_MONITOR_UPDATE";
/// @brief ��ȡ��ʾ����Ϣ
constexpr auto PCM_HARDWARE_ACTION_MONITOR_GET = "PCM_HARDWARE_ACTION_MONITOR_GET";

/// @brief ��ʼ��Win10���ﴫ������Ϣ
constexpr auto PCM_HARDWARE_ACTION_WINBIO_INIT = "PCM_HARDWARE_ACTION_WINBIO_INIT";
/// @brief ����Win10���ﴫ������Ϣ
constexpr auto PCM_HARDWARE_ACTION_WINBIO_UPDATE = "PCM_HARDWARE_ACTION_WINBIO_UPDATE";
/// @brief ��ȡWin10���ﴫ������Ϣ
constexpr auto PCM_HARDWARE_ACTION_WINBIO_GET = "PCM_HARDWARE_ACTION_WINBIO_GET";