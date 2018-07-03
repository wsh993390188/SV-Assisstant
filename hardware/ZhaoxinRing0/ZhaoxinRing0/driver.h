/*++

Module Name:

driver.h

Abstract:

This file contains the driver definitions.

Environment:

Kernel-mode Driver Framework

--*/

#include <ntddk.h>
#include <wdf.h>

#include "ioctl.h"
#include "device.h"
#include "Function.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_OBJECT_CONTEXT_CLEANUP ZhaoxinRing0EvtDriverContextCleanup;
EVT_WDF_DRIVER_UNLOAD ZhaoxinRing0EvtDriverUnload;
EXTERN_C_END