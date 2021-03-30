/*++

Module Name:

device.c - Device handling events for example driver.

Abstract:

This file contains the device entry points and callbacks.

Environment:

Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "device.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, LenovoRing0DeviceAdd)
#pragma alloc_text (PAGE, LenovoRing0EvtIoDeviceControl)
#pragma alloc_text (PAGE, LenovoRing0EvtIoStop)
#pragma alloc_text (PAGE, LenovoRing0EvtDeviceFileCreate)
#pragma alloc_text (PAGE, LenovoRing0EvtDeviceFileClose)
#endif

NTSTATUS LenovoRing0DeviceAdd(
	IN WDFDRIVER Driver,
	IN PWDFDEVICE_INIT DeviceInit
)
/*++

Routine Description:

Called by the DriverEntry to create a control-device. This call is
responsible for freeing the memory for DeviceInit.

Arguments:

DriverObject - a pointer to the object that represents this device
driver.

DeviceInit - Pointer to a driver-allocated WDFDEVICE_INIT structure.

Return Value:

STATUS_SUCCESS if initialized; an error otherwise.

--*/
{
	NTSTATUS						status;
	WDF_OBJECT_ATTRIBUTES			deviceAttributes;
	WDF_FILEOBJECT_CONFIG           fileConfig;
	WDF_IO_QUEUE_CONFIG				ioQueueConfig;
	WDFQUEUE                        queue;
	WDFDEVICE						device;
	DECLARE_CONST_UNICODE_STRING(ntDeviceName, DEVICE_NAME_STRING);
	DECLARE_CONST_UNICODE_STRING(symbolicLinkName, SYMBOLIC_NAME_STRING);

	UNREFERENCED_PARAMETER(Driver);

	PAGED_CODE();

	//
	// Set exclusive to TRUE so that no more than one app can talk to the
	// control device at any time.
	//
	WdfDeviceInitSetExclusive(DeviceInit, TRUE);

	WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoBuffered);

	status = WdfDeviceInitAssignName(DeviceInit, &ntDeviceName);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, DBG_INIT, "WdfDeviceInitAssignName failed %!STATUS!", status);
		goto End;
	}

	// 	//
	// 	// In order to support METHOD_NEITHER Device controls, or
	// 	// NEITHER device I/O type, we need to register for the
	// 	// EvtDeviceIoInProcessContext callback so that we can handle the request
	// 	// in the calling threads context.
	// 	//
	// 	WdfDeviceInitSetIoInCallerContextCallback(DeviceInit,
	// 					LenovoRing0EvtDeviceIoInCallerContext);

	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

	WDF_FILEOBJECT_CONFIG_INIT(
		&fileConfig,
		LenovoRing0EvtDeviceFileCreate,
		LenovoRing0EvtDeviceFileClose,
		WDF_NO_EVENT_CALLBACK // not interested in Cleanup
	);

	WdfDeviceInitSetFileObjectConfig(DeviceInit,
		&fileConfig,
		WDF_NO_OBJECT_ATTRIBUTES);

	status = WdfDeviceCreate(&DeviceInit,
		&deviceAttributes,
		&device);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, DBG_INIT, "WdfDeviceCreate failed %!STATUS!", status);
		goto End;
	}

	//
	// Create a symbolic link for the control object so that usermode can open
	// the device.
	//

	status = WdfDeviceCreateSymbolicLink(device,
		&symbolicLinkName);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, DBG_INIT, "WdfDeviceCreateSymbolicLink failed %!STATUS!", status);
		goto End;
	}

	//
	// Configure a default queue so that requests that are not
	// configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
	// other queues get dispatched here.
	//
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig,
		WdfIoQueueDispatchSequential);

	ioQueueConfig.EvtIoDefault = LenovoRing0EvtDeviceDefault;
	ioQueueConfig.EvtIoDeviceControl = LenovoRing0EvtIoDeviceControl;
	ioQueueConfig.EvtIoStop = LenovoRing0EvtIoStop;

	WDF_OBJECT_ATTRIBUTES_INIT(&deviceAttributes);

	status = WdfIoQueueCreate(device,
		&ioQueueConfig,
		&deviceAttributes,
		&queue // pointer to default queue
	);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, DBG_INIT, "WdfIoQueueCreate failed %!STATUS!", status);
		goto End;
	}

	//
	// Control devices must notify WDF when they are done initializing.   I/O is
	// rejected until this call is made.
	//
	WdfControlFinishInitializing(device);

End:
	//
	// If the device is created successfully, framework would clear the
	// DeviceInit value. Otherwise device create must have failed so we
	// should free the memory ourself.
	//
	if (DeviceInit != NULL) {
		WdfDeviceInitFree(DeviceInit);
	}

	return status;
}

VOID
LenovoRing0EvtIoDeviceControl(
	_In_ WDFQUEUE Queue,
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength,
	_In_ ULONG IoControlCode
)
/*++

Routine Description:

This event is invoked when the framework receives IRP_MJ_DEVICE_CONTROL request.

Arguments:

Queue -  Handle to the framework queue object that is associated with the
I/O request.

Request - Handle to a framework request object.

OutputBufferLength - Size of the output buffer in bytes

InputBufferLength - Size of the input buffer in bytes

IoControlCode - I/O control code.

Return Value:

VOID

--*/
{
	UNREFERENCED_PARAMETER(Queue);
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION,
		DBG_OPERATOR,
		"%!FUNC! Queue 0x%p, Request 0x%p IoControlCode %d",
		Queue, Request, IoControlCode);
	NTSTATUS status;
	switch (IoControlCode)
	{
	case IOCTL_RING0_OPERATOR:
		status = ExcuteOperator(Request, InputBufferLength, OutputBufferLength);
		break;
	default:
		status = STATUS_INVALID_INFO_CLASS;
		TraceEvents(TRACE_LEVEL_WARNING, DBG_OPERATOR, "%!FUNC! Invalid Ioctl %d", IoControlCode);
		WdfRequestComplete(Request, status);
		break;
	}
	return;
}

VOID
LenovoRing0EvtIoStop(
	_In_ WDFQUEUE Queue,
	_In_ WDFREQUEST Request,
	_In_ ULONG ActionFlags
)
/*++

Routine Description:

This event is invoked for a power-managed queue before the device leaves the working state (D0).

Arguments:

Queue -  Handle to the framework queue object that is associated with the
I/O request.

Request - Handle to a framework request object.

ActionFlags - A bitwise OR of one or more WDF_REQUEST_STOP_ACTION_FLAGS-typed flags
that identify the reason that the callback function is being called
and whether the request is cancelable.

Return Value:

VOID

--*/
{
	//
	// In most cases, the EvtIoStop callback function completes, cancels, or postpones
	// further processing of the I/O request.
	//
	// Typically, the driver uses the following rules:
	//
	// - If the driver owns the I/O request, it calls WdfRequestUnmarkCancelable
	//   (if the request is cancelable) and either calls WdfRequestStopAcknowledge
	//   with a Requeue value of TRUE, or it calls WdfRequestComplete with a
	//   completion status value of STATUS_SUCCESS or STATUS_CANCELLED.
	//
	//   Before it can call these methods safely, the driver must make sure that
	//   its implementation of EvtIoStop has exclusive access to the request.
	//
	//   In order to do that, the driver must synchronize access to the request
	//   to prevent other threads from manipulating the request concurrently.
	//   The synchronization method you choose will depend on your driver's design.
	//
	//   For example, if the request is held in a shared context, the EvtIoStop callback
	//   might acquire an internal driver lock, take the request from the shared context,
	//   and then release the lock. At this point, the EvtIoStop callback owns the request
	//   and can safely complete or requeue the request.
	//
	// - If the driver has forwarded the I/O request to an I/O target, it either calls
	//   WdfRequestCancelSentRequest to attempt to cancel the request, or it postpones
	//   further processing of the request and calls WdfRequestStopAcknowledge with
	//   a Requeue value of FALSE.
	//
	// A driver might choose to take no action in EvtIoStop for requests that are
	// guaranteed to complete in a small amount of time.
	//
	// In this case, the framework waits until the specified request is complete
	// before moving the device (or system) to a lower power state or removing the device.
	// Potentially, this inaction can prevent a system from entering its hibernation state
	// or another low system power state. In extreme cases, it can cause the system
	// to crash with bugcheck code 9F.
	//
	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER(Request);
	UNREFERENCED_PARAMETER(ActionFlags);
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION,
		DBG_OPERATOR,
		"%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d",
		Queue, Request, ActionFlags);

	return;
}

VOID
LenovoRing0EvtDeviceDefault(
	_In_
	WDFQUEUE Queue,
	_In_
	WDFREQUEST Request
)
{
	TraceEvents(TRACE_LEVEL_INFORMATION,
		DBG_OPERATOR,
		"%!FUNC! Queue 0x%p, Request 0x%p",
		Queue, Request);

	WdfRequestComplete(Request, STATUS_SUCCESS);

	return;
}

VOID
LenovoRing0EvtDeviceFileCreate(
	IN WDFDEVICE Device,
	IN WDFREQUEST Request,
	IN WDFFILEOBJECT FileObject
)
/*++

Routine Description:

	The framework calls a driver's EvtDeviceFileCreate callback
	when the framework receives an IRP_MJ_CREATE request.
	The system sends this request when a user application opens the
	device to perform an I/O operation, such as reading or writing to a device.
	This callback is called in the context of the thread
	that created the IRP_MJ_CREATE request.

Arguments:

	Device - Handle to a framework device object.
	FileObject - Pointer to fileobject that represents the open handle.
	CreateParams - Parameters for create

Return Value:

	None

--*/
{
	//BOOLEAN LegalSignature = FALSE;
	NTSTATUS Status = STATUS_SUCCESS;
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION,
		DBG_OPERATOR,
		"%!FUNC! Device 0x%p, FileObject 0x%p",
		Device, FileObject);
	// 	LegalSignature = VerifyCurrentProcessSignature();
	// 	if (!LegalSignature)
	// 	{
	// 		Status = STATUS_ACCESS_DENIED;
	// 	}
	WdfRequestComplete(Request, Status);
	return;
}

VOID
LenovoRing0EvtDeviceFileClose(
	IN WDFFILEOBJECT    FileObject
)

/*++

Routine Description:

   EvtFileClose is called when all the handles represented by the FileObject
   is closed and all the references to FileObject is removed. This callback
   may get called in an arbitrary thread context instead of the thread that
   called CloseHandle. If you want to delete any per FileObject context that
   must be done in the context of the user thread that made the Create call,
   you should do that in the EvtDeviceCleanp callback.

Arguments:

	FileObject - Pointer to fileobject that represents the open handle.

Return Value:

	None

--*/
{
	PAGED_CODE();
	TraceEvents(TRACE_LEVEL_INFORMATION,
		DBG_OPERATOR,
		"%!FUNC! FileObject 0x%p",
		FileObject);
	return;
}