/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include "..\public.h"

EXTERN_C_START

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
	ULONG PrivateDeviceData;  // just a placeholder

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

// 
// Following request context is used only for the method-neither ioctl case.
// 
// typedef struct _REQUEST_CONTEXT {
// 
// 	WDFMEMORY InputMemoryBuffer;
// 	WDFMEMORY OutputMemoryBuffer;
// 
// } REQUEST_CONTEXT, *PREQUEST_CONTEXT;
// 
// WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(REQUEST_CONTEXT, GetRequestContext)


//
// Function to initialize the device and its callbacks
//

/*EVT_WDF_IO_IN_CALLER_CONTEXT ZhaoxinRing0EvtDeviceIoInCallerContext;*/

EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL SystemInfoEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP SystemInfoEvtIoStop;

NTSTATUS
SystemInfoDeviceAdd(
	IN WDFDRIVER Driver,
	IN PWDFDEVICE_INIT DeviceInit
);

EXTERN_C_END
