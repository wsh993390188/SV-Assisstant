#define GAC_HEADER_USE_NAMESPACE
#include "UI\Source\Smbios.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;

void GuiMain()
{
	{
		FileStream fileStream(L"UI\\Smbios.bin", FileStream::ReadOnly);
		GetResourceManager()->LoadResourceOrPending(fileStream);
	}
	Smbios::MainWindow window;
	window.MoveToScreenCenter();

	GetApplication()->Run(&window);
}