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
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfDeviceInitAssignName failed %!STATUS!", status);
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

	status = WdfDeviceCreate(&DeviceInit,
		&deviceAttributes,
		&device);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfDeviceCreate failed %!STATUS!", status);
		goto End;
	}

	//
	// Create a symbolic link for the control object so that usermode can open
	// the device.
	//

	status = WdfDeviceCreateSymbolicLink(device,
		&symbolicLinkName);

	//
	// Configure a default queue so that requests that are not
	// configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
	// other queues get dispatched here.
	//
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig,
		WdfIoQueueDispatchSequential);

	ioQueueConfig.EvtIoDeviceControl = LenovoRing0EvtIoDeviceControl;
	ioQueueConfig.EvtIoStop = LenovoRing0EvtIoStop;

	WDF_OBJECT_ATTRIBUTES_INIT(&deviceAttributes);

	status = WdfIoQueueCreate(device,
		&ioQueueConfig,
		&deviceAttributes,
		&queue // pointer to default queue
	);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfIoQueueCreate failed %!STATUS!", status);
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
		TRACE_DEVICE,
		"%!FUNC! Queue 0x%p, Request 0x%p IoControlCode %d",
		Queue, Request, IoControlCode);
	NTSTATUS status;
	switch (IoControlCode)
	{
	case READ_PORT:
		status = ReadIoPort(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case WRITE_PORT:
		status = WriteIoPort(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case READ_MSR:
		status = ReadMsr(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case WRITE_MSR:
		status = WriteMsr(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case READ_PMC:
		status = ReadPmc(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case READ_MEMORY:
		status = ReadMemory(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case WRITE_MEMORY:
		status = WriteMemory(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case READ_PCI_CONFIG:
		status = ReadPciConfig(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case WRITE_PCI_CONFIG:
		status = WritePciConfig(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case READ_MSR_THREAD:
		status = ReadMsrThread(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case WRITE_MSR_THREAD:
		status = WriteMsrThread(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
	case READ_TSC:
		status = ReadTsc(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	case READ_TSC_THREAD:
		status = ReadTscThread(
			Request,
			OutputBufferLength,
			InputBufferLength
		);
		break;
	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		WdfRequestComplete(Request, STATUS_SUCCESS);
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
	TraceEvents(TRACE_LEVEL_INFORMATION,
		TRACE_DEVICE,
		"%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d",
		Queue, Request, ActionFlags);

	return;
}

// VOID
// LenovoRing0EvtDeviceIoInCallerContext(
// 	IN WDFDEVICE  Device,
// 	IN WDFREQUEST Request
// )
// /*++
// Routine Description:
//
// This I/O in-process callback is called in the calling threads context/address
// space before the request is subjected to any framework locking or queueing
// scheme based on the device pnp/power or locking attributes set by the
// driver. The process context of the calling app is guaranteed as long as
// this driver is a top-level driver and no other filter driver is attached
// to it.
//
// This callback is only required if you are handling method-neither IOCTLs,
// or want to process requests in the context of the calling process.
//
// Driver developers should avoid defining neither IOCTLs and access user
// buffers, and use much safer I/O tranfer methods such as buffered I/O
// or direct I/O.
//
// Arguments:
//
// Device - Handle to a framework device object.
//
// Request - Handle to a framework request object. Framework calls
// PreProcess callback only for Read/Write/ioctls and internal
// ioctl requests.
//
// Return Value:
//
// VOID
//
// --*/
// {
// 	NTSTATUS                   status = STATUS_SUCCESS;
// 	PREQUEST_CONTEXT            reqContext = NULL;
// 	WDF_OBJECT_ATTRIBUTES           attributes;
// 	WDF_REQUEST_PARAMETERS  params;
// 	size_t              inBufLen, outBufLen;
// 	PVOID              inBuf, outBuf;
//
// 	PAGED_CODE();
//
// 	WDF_REQUEST_PARAMETERS_INIT(&params);
//
// 	WdfRequestGetParameters(Request, &params);
//
// 	TraceEvents(TRACE_LEVEL_VERBOSE, DBG_IOCTL, "Entered NonPnpEvtDeviceIoInCallerContext %p \n",
// 		Request);
//
// 	//
// 	// Check to see whether we have recevied a METHOD_NEITHER IOCTL. if not
// 	// just send the request back to framework because we aren't doing
// 	// any pre-processing in the context of the calling thread process.
// 	//
// 	if (!(params.Type == WdfRequestTypeDeviceControl &&
// 		params.Parameters.DeviceIoControl.IoControlCode ==
// 		IOCTL_NONPNP_METHOD_NEITHER)) {
// 		//
// 		// Forward it for processing by the I/O package
// 		//
// 		status = WdfDeviceEnqueueRequest(Device, Request);
// 		if (!NT_SUCCESS(status)) {
// 			TraceEvents(TRACE_LEVEL_ERROR, DBG_IOCTL,
// 				"Error forwarding Request 0x%x", status);
// 			goto End;
// 		}
//
// 		return;
// 	}
//
// 	TraceEvents(TRACE_LEVEL_VERBOSE, DBG_IOCTL, "EvtIoPreProcess: received METHOD_NEITHER ioctl \n");
//
// 	//
// 	// In this type of transfer, the I/O manager assigns the user input
// 	// to Type3InputBuffer and the output buffer to UserBuffer of the Irp.
// 	// The I/O manager doesn't copy or map the buffers to the kernel
// 	// buffers.
// 	//
// 	status = WdfRequestRetrieveUnsafeUserInputBuffer(Request, 0, &inBuf, &inBufLen);
// 	if (!NT_SUCCESS(status)) {
// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_IOCTL,
// 			"Error WdfRequestRetrieveUnsafeUserInputBuffer failed 0x%x", status);
// 		goto End;
// 	}
//
// 	status = WdfRequestRetrieveUnsafeUserOutputBuffer(Request, 0, &outBuf, &outBufLen);
// 	if (!NT_SUCCESS(status)) {
// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_IOCTL,
// 			"Error WdfRequestRetrieveUnsafeUserOutputBuffer failed 0x%x", status);
// 		goto End;
// 	}
//
// 	//
// 	// Allocate a context for this request so that we can store the memory
// 	// objects created for input and output buffer.
// 	//
// 	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, REQUEST_CONTEXT);
//
// 	status = WdfObjectAllocateContext(Request, &attributes, &reqContext);
// 	if (!NT_SUCCESS(status)) {
// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_IOCTL,
// 			"Error WdfObjectAllocateContext failed 0x%x", status);
// 		goto End;
// 	}
//
// 	//
// 	// WdfRequestProbleAndLockForRead/Write function checks to see
// 	// whether the caller in the right thread context, creates an MDL,
// 	// probe and locks the pages, and map the MDL to system address
// 	// space and finally creates a WDFMEMORY object representing this
// 	// system buffer address. This memory object is associated with the
// 	// request. So it will be freed when the request is completed. If we
// 	// are accessing this memory buffer else where, we should store these
// 	// pointers in the request context.
// 	//
//
// #pragma prefast(suppress:6387, "If inBuf==NULL at this point, then inBufLen==0")
// 	status = WdfRequestProbeAndLockUserBufferForRead(Request,
// 		inBuf,
// 		inBufLen,
// 		&reqContext->InputMemoryBuffer);
//
// 	if (!NT_SUCCESS(status)) {
// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_IOCTL,
// 			"Error WdfRequestProbeAndLockUserBufferForRead failed 0x%x", status);
// 		goto End;
// 	}
//
// #pragma prefast(suppress:6387, "If outBuf==NULL at this point, then outBufLen==0")
// 	status = WdfRequestProbeAndLockUserBufferForWrite(Request,
// 		outBuf,
// 		outBufLen,
// 		&reqContext->OutputMemoryBuffer);
// 	if (!NT_SUCCESS(status)) {
// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_IOCTL,
// 			"Error WdfRequestProbeAndLockUserBufferForWrite failed 0x%x", status);
// 		goto End;
// 	}
//
// 	//
// 	// Finally forward it for processing by the I/O package
// 	//
// 	status = WdfDeviceEnqueueRequest(Device, Request);
// 	if (!NT_SUCCESS(status)) {
// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_IOCTL,
// 			"Error WdfDeviceEnqueueRequest failed 0x%x", status);
// 		goto End;
// 	}
//
// 	return;
//
// End:
//
// 	TraceEvents(TRACE_LEVEL_VERBOSE, DBG_IOCTL, "EvtIoPreProcess failed %x \n", status);
// 	WdfRequestComplete(Request, status);
// 	return;
// }