#include "stdafx.h"
#include "Utils.h"
#include <strsafe.h>

using namespace Hardware::Utils;

ThreadGroupTempAffinity::ThreadGroupTempAffinity(uint32_t core_id, bool checkStatus)
{
	GROUP_AFFINITY NewGroupAffinity;
	memset(&NewGroupAffinity, 0, sizeof(GROUP_AFFINITY));
	memset(&PreviousGroupAffinity, 0, sizeof(GROUP_AFFINITY));
	DWORD currentGroupSize = 0;

	while (static_cast<DWORD>(core_id) >= (currentGroupSize = GetActiveProcessorCount(NewGroupAffinity.Group)))
	{
		core_id -= static_cast<uint32_t>(currentGroupSize);
		++NewGroupAffinity.Group;
	}
	NewGroupAffinity.Mask = 1ULL << core_id;
	const auto res = SetThreadGroupAffinity(GetCurrentThread(), &NewGroupAffinity, &PreviousGroupAffinity);
	if (res == FALSE && checkStatus)
	{
		spdlog::info("ERROR: SetThreadGroupAffinity for core {} failed with error {}", core_id, GetLastError());
		throw std::exception();
	}
}
ThreadGroupTempAffinity::~ThreadGroupTempAffinity()
{
	SetThreadGroupAffinity(GetCurrentThread(), &PreviousGroupAffinity, NULL);
}

void Hardware::Utils::GetCpuid(const int leaf, CPUID_INFO& info)
{
	__cpuid(info.array, leaf);
}

void Hardware::Utils::GetCpuid(const unsigned leaf, const unsigned subleaf, CPUID_INFO& info)
{
	__cpuidex(info.array, leaf, subleaf);
}

uint32_t Hardware::Utils::build_bit_ui(uint32_t beg, uint32_t end)
{
	uint32_t myll = 0;
	if (end == 31)
	{
		myll = (uint32_t)(-1);
	}
	else
	{
		myll = (1 << (end + 1)) - 1;
	}
	myll = myll >> beg;
	return myll;
}

uint32_t Hardware::Utils::extract_bits_ui(uint32_t myin, uint32_t beg, uint32_t end)
{
	uint32_t myll = 0;
	uint32_t beg1, end1;

	// Let the user reverse the order of beg & end.
	if (beg <= end)
	{
		beg1 = beg;
		end1 = end;
	}
	else
	{
		beg1 = end;
		end1 = beg;
	}
	myll = myin >> beg1;
	myll = myll & build_bit_ui(beg1, end1);
	return myll;
}

uint64_t Hardware::Utils::build_bit(uint32_t beg, uint32_t end)
{
	uint64_t myll = 0;
	if (end == 63)
	{
		myll = static_cast<uint64_t>(-1);
	}
	else
	{
		myll = (1LL << (end + 1)) - 1;
	}
	myll = myll >> beg;
	return myll;
}

uint64_t Hardware::Utils::extract_bits(uint64_t myin, uint32_t beg, uint32_t end)
{
	uint64_t myll = 0;
	uint32_t beg1, end1;

	// Let the user reverse the order of beg & end.
	if (beg <= end)
	{
		beg1 = beg;
		end1 = end;
	}
	else
	{
		beg1 = end;
		end1 = beg;
	}
	myll = myin >> beg1;
	myll = myll & build_bit(beg1, end1);
	return myll;
}

HANDLE_unique_ptr Hardware::Utils::make_HANDLE_unique(HANDLE handle)
{
	if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
	{
		return nullptr;
	}

	return HANDLE_unique_ptr(handle);
}

void Hardware::Utils::ErrorCode(LPCSTR lpszFunction)
{
	ErrorCode(lpszFunction, GetLastError());
}

// Format a readable error message, display a message box,
// and exit from the application.
void Hardware::Utils::ErrorCode(LPCSTR lpszFunction, const DWORD dw)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(strlen((LPCSTR)lpMsgBuf) + strlen((LPCSTR)lpszFunction) + 40));
	StringCchPrintfA((LPSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf),
		"%s failed with error %d: %s",
		lpszFunction, dw, lpMsgBuf);
	spdlog::error((LPCSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

std::string Hardware::Utils::GUIDToString(const GUID& guid)
{
	CHAR guidString[40] = { 0 };
	sprintf_s(guidString, _countof(guidString), "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return guidString;
}