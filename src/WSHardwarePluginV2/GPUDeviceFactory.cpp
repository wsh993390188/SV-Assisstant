#include "stdafx.h"
#include "GPUDeviceFactory.h"

Hardware::GPU::GPUDeviceFactory& Hardware::GPU::GPUDeviceFactory::Instance()
{
	static GPUDeviceFactory inst;
	return inst;
}