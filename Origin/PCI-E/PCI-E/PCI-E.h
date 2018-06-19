#pragma once
#include "../../ACPI/ACPI/ACPI.h"

BOOL Initialize(HMODULE& hMoudle);

void Free(HMODULE& hMoudle);

BOOL GetPCIEBaseAddress(_Out_ ULONGLONG& PCIE_BaseAddress);