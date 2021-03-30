#define GAC_HEADER_USE_NAMESPACE
#include "UI\Source\Hardware.h"
#include <Windows.h>
#include "framework.hpp"

#pragma comment(lib, "comsuppw.lib")//important

void GuiMain()
{
	PcmHardwareInitialize(nullptr, nullptr);
	{
		vl::stream::FileStream fileStream(LR"(UI\HardwareMain.bin)", vl::stream::FileStream::ReadOnly);
		GetResourceManager()->LoadResourceOrPending(fileStream);
	}
	Hardware::MainWindow window;
	window.MoveToScreenCenter();

	GetApplication()->Run(&window);
}