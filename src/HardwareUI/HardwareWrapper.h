#pragma once
#include <string>
#include <map>

namespace Hardware
{
	namespace HardwareWrapper
	{
		namespace CPU
		{
			struct CPUCore
			{
				uint32_t CoreId;
				std::vector<size_t> ThreadIds;
			};

			struct CPUSocket
			{
				uint32_t SocketID;
				std::vector<CPUCore> Cores;
			};

			std::vector<CPUSocket> InitializeCPU();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& SocketId);

			std::vector<std::pair<std::wstring, std::wstring>> UpdateElements(const size_t& SocketId);

			std::vector<std::pair<std::wstring, std::wstring>> UpdateElements(const size_t& SocketId, const size_t& CoreId, const size_t& ThreadId);
		}

		namespace GPU
		{
			struct GPUCore
			{
				std::uint32_t GPUId;
				explicit GPUCore(const uint32_t& Id) : GPUId(Id) {}
			};

			std::vector<GPUCore> InitializeGPU();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& GpuId);

			std::vector<std::pair<std::wstring, std::wstring>> UpdateElements(const size_t& GpuId);
		}

		namespace Memory
		{
			std::vector<size_t> InitializeMemory();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& MemoryId);

			std::vector<std::pair<std::wstring, std::wstring>> UpdateElements(const size_t& MemoryId);
		}

		namespace Disk
		{
			struct DiskQueryInfo
			{
				std::uint32_t DiskId;
				explicit DiskQueryInfo(const uint32_t& Id) : DiskId(Id) {}
				std::vector<std::string> UpdateElements;
				std::vector<std::string> StaticElements;
			};

			std::vector<DiskQueryInfo> InitializeDisk();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const DiskQueryInfo& DiskInfo);

			std::vector<std::pair<std::wstring, std::wstring>> UpdateElements(const DiskQueryInfo& DiskInfo);
		}

		namespace Battery
		{
			std::vector<size_t> InitializeBattery();

			std::vector<std::pair<std::wstring, std::wstring>> UpdateElements(const size_t& MemoryId);
		}

		namespace Monitor
		{
			std::vector<std::string> InitializeMonitor();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const std::string& MemoryId);
		}

		namespace Motherbroad
		{
			std::vector<size_t> InitializeMotherbroad();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& MemoryId);
		}

		namespace Bios
		{
			std::vector<size_t> InitializeBios();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& MemoryId);
		}

		namespace Net
		{
			std::vector<size_t> InitializeNet();

			std::vector<std::pair<std::wstring, std::wstring>> UpdateElements(const size_t& MemoryId);

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& MemoryId);
		}

		namespace Audio
		{
			std::vector<size_t> InitializeAudio();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& MemoryId);
		}

		namespace WinBio
		{
			std::vector<size_t> InitializeWinBio();

			std::vector<std::pair<std::wstring, std::wstring>> GetElements(const size_t& MemoryId);
		}
	}
}
