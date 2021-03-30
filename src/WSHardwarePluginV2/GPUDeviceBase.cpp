#include "stdafx.h"
#include "GPUDeviceBase.h"

Hardware::GPU::GPUDecorator::GPUDecorator(const uint64_t MemoryBase, const std::string& Name) :
	MemoryBase(MemoryBase), m_Name(Name), IsUpdate(false)
{
}

const std::string Hardware::GPU::GPUDecorator::GetDecoratorName() const
{
	return m_Name;
}