#pragma once

#define DEVICE_NAME_STRING			L"\\Device\\{8EB9AAE8-AC93-4AB3-ACC1-275CB500E346}"
#define SYMBOLIC_NAME_STRING		L"\\DosDevices\\{8EB9AAE8-AC93-4AB3-ACC1-275CB500E346}"

// PCI Device Address to Bus Number
#define PciGetBus(address)				((address>>8) & 0xFF)
// PCI Device Address to Device Number
#define PciGetDev(address)				((address>>3) & 0x1F)
// PCI Device Address to Function Number
#define PciGetFunc(address)				(address&7)

// 直接采用WinIO的方式 避免Win10 1809使用MM出错的问题
struct WinIoPhysStruct
{
	DWORD64 dwPhysMemSizeInBytes;
	DWORD64 pvPhysAddress;
	DWORD64 PhysicalMemoryHandle;
	DWORD64 pvPhysMemLin;
	DWORD64 pvPhysSection;
};
