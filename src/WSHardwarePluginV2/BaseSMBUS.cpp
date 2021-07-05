#include "stdafx.h"
#include "BaseSMBUS.h"

Hardware::Memory::DeviceType Hardware::Memory::SMBUSControllerBase::ParseDeviceType(const uint8_t& DDRTypes)
{
	return DeviceType(DDRTypes);
}

bool Hardware::Memory::SMBUSControllerBase::IsDDR3Device(const DeviceType& type)
{
	return (type == DeviceType::DDR3SDRAM) || (type == DeviceType::LPDDR3SDRAM);
}

bool Hardware::Memory::SMBUSControllerBase::IsDDR4Device(const DeviceType& type)
{
	return (type == DeviceType::DDR4SDRAM) || (type == DeviceType::LPDDR4SDRAM) || (type == DeviceType::DDR4ESDRAM) || (type == DeviceType::LPDDR4XSDRAM);
}

bool Hardware::Memory::SMBUSControllerBase::IsDDR5Device(const DeviceType& type)
{
	return (type == DeviceType::DDR5SDRAM) || (type == DeviceType::LPDDR5SDRAM);
}