#include "stdafx.h"
#include "Power Management.h"
#include "service.h"
#include "startup.h"
#include "TaskScheduler.h"


int main()
{
	EnumFolderTaskScheduler();
	PowerManagement();
	OSVERSIONINFOEX osvi = {};
	osvi.dwOSVersionInfoSize = sizeof(osvi);
#pragma warning(disable:4996)
	if (!GetVersionEx((OSVERSIONINFO *)&osvi))
	{
		std::cout << "Failed";
	}
#pragma warning(default:4996)

	
	ChangeBlueCrash(TRUE);
	FireWall(FALSE);
	Enumservice();
	ShowAutoRunList();

	return 0;
}

