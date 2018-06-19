#pragma once
#include <comdef.h>
#include <atlbase.h>

namespace SV_ASSIST
{
	namespace Software
	{
		namespace Origin
		{
			BOOLEAN EnumFolderTaskScheduler(std::list<TaskScheduler_Struct>& data);
			BOOLEAN EnumRuningTaskScheduler(std::list<TaskScheduler_Struct>& data);
		}
	}
}