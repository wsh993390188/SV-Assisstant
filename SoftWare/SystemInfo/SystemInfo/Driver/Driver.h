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
#include <initguid.h>

#include "../ioctl.h"
#include "device.h"
#include "func.h"
#include "defination.h"
#include "../Procssor/processor.h"
#include "../DriverModule/module.h"
#include "../Kernel/Callback/callback.h"
#include "../Kernel/Filter.h"
#include "../Kernel/DPCTimer.h"
#include "../Kernel/GDTABLE.h"
#include "../Ring0Hook/SSDTHook.h"
#include "../Ring0Hook/FSDHOOK.h"
#include "../Ring0Hook/KeybroadHook.h"
#include "../Ring0Hook/MouseHook.h"
#include "../Ring0Hook/AcpiHook.h"
#include "../Ring0Hook/AtapiHook.h"
#include "../Ring0Hook/DiskHook.h"
#include "../Ring0Hook/I8042prtHook.h"
#include "../Ring0Hook/PartmgrHook.h"
#include "../Ring0Hook/ScsiHook.h"
#include "../Ring0Hook/IDTABLE.h"
#include "../Ring3Hook/MsgHook.h"
#include "../NetWork/Tcpip.h"
#include "../NetWork/nsiproxy.h"
#include "../NetWork/Tdx.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_OBJECT_CONTEXT_CLEANUP SystemInfoEvtDriverContextCleanup;
EVT_WDF_DRIVER_UNLOAD  SystemInfoEvtDriverUnload;
EXTERN_C_END
