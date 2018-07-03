#include "stdafx.h"
#include "disk.h"
#include "diskdefination.h"

namespace SV_ASSIST
{
	namespace Storage
	{
		void updatediskinfo()
		{
			Diskinfo::Instance()->updatedisk();
		}
	}
}
