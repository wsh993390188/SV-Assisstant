#include "stdafx.h"
#include "GPUDeviceBase.h"
#include "PCIUtils.h"
#include "D3DKMTHelper.h"

Hardware::GPU::GPUDecorator::GPUDecorator(const uint64_t MemoryBase, const std::string& Name) :
	MemoryBase(MemoryBase), m_Name(Name), IsUpdate(false)
{
}

const std::string Hardware::GPU::GPUDecorator::GetDecoratorName() const
{
	return m_Name;
}

Hardware::GPU::GPUDeviceBase::GPUDeviceBase(const GPUDevice& GpuData, std::unique_ptr<GPUAdapter>&& Adapter) : GPUBaseData(GpuData), m_Adapter(std::move(Adapter))
{
	SubVendor = Utils::PCIUtils::Instance().GetVendorName(GpuData.SubVendorId);
	Utils::trim(SubVendor);

	for (auto& Node : m_Adapter->Nodes)
	{
		UpdateNode(Node);
	}
}

bool Hardware::GPU::GPUDeviceBase::UpdateNode(GPUNode& Node)
{
	if (!m_Adapter)
		return false;
	uint64_t runningTime;
	if (D3DKMTHelper::UpdateNodeInformation(m_Adapter->AdapterLuid, Node.Index, runningTime))
	{
		Node.Delta = runningTime - Node.Value;
		Node.Value = runningTime;
		return true;
	}
	return false;
}