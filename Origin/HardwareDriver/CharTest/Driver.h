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
#include <intrin.h>

#include "ioctl.h"
#include "device.h"
#include "queue.h"
#include "trace.h"
#include "Function.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD CharTestEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP CharTestEvtDriverContextCleanup;
EVT_WDF_DRIVER_UNLOAD CharTestEvtDriverUnload;

EXTERN_C_END
