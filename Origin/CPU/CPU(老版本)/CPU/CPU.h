// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CPU_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CPU_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef CPU_EXPORTS
#define CPU_API __declspec(dllexport)
#else
#define CPU_API __declspec(dllimport)
#endif

#include <windows.h>
#include "CPU/definition.h"

// 此类是从 CPU.dll 导出的
class CPU_API CCPU {
public:
	CCPU(void);
	// TODO:  在此添加您的方法。	
	virtual void Excute(CPUID_Data* buffer, ULONG& buffer_size) = 0;
	virtual void Get_CPUID(CPU_RES* Data) = 0;
	virtual CPUID_Data Excute() = 0;

};
extern "C"
{
	CPU_API CCPU* Instantiate(void);
	CPU_API void Free(CCPU* pBase);
}





