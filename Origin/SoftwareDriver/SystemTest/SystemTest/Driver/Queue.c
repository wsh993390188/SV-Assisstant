/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "Driver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, SystemTestQueueInitialize)
#endif

NTSTATUS
SystemTestQueueInitialize(
    _In_ WDFDEVICE Device
    )
/*++

Routine Description:

     The I/O dispatch callbacks for the frameworks device object
     are configured in this function.

     A single default I/O Queue is configured for parallel request
     processing, and a driver context memory allocation is created
     to hold our structure QUEUE_CONTEXT.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    VOID

--*/
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;

    PAGED_CODE();

    //
    // Configure a default queue so that requests that are not
    // configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
    // other queues get dispatched here.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchParallel
        );

    queueConfig.EvtIoDeviceControl = SystemTestEvtIoDeviceControl;
    queueConfig.EvtIoStop = SystemTestEvtIoStop;

    status = WdfIoQueueCreate(
                 Device,
                 &queueConfig,
                 WDF_NO_OBJECT_ATTRIBUTES,
                 &queue
                 );

    if(!NT_SUCCESS(status)) {
        return status;
    }

    return status;
}

VOID
SystemTestEvtIoDeviceControl(
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
	NTSTATUS status = STATUS_SUCCESS;
	size_t Resquestlength = 0;
	switch (IoControlCode)
	{
	case GetProcessNum:
		if (!OutputBufferLength)
			status = STATUS_BUFFER_TOO_SMALL;
		else
		{
			DWORD *buffer = NULL;
			size_t bufferlength = 0;
			WdfRequestRetrieveOutputBuffer(Request, sizeof(short), &buffer, &bufferlength);
			DWORD dwcount = GetAllProcessorNum();
			*buffer = dwcount;
		}
		WdfRequestCompleteWithInformation(Request, status, sizeof(DWORD));
		break;
	case EnumProc:
		status = EnumProcessList(Request, InputBufferLength, OutputBufferLength);
		WdfRequestCompleteWithInformation(Request, status, OutputBufferLength);
		break;
	case GetModuleNum:
		if (!OutputBufferLength)
			status = STATUS_BUFFER_TOO_SMALL;
		else
		{
			DWORD *buffer = NULL;
			size_t bufferlength = 0;
			DWORD dwcount = 0;
			WdfRequestRetrieveOutputBuffer(Request, sizeof(short), &buffer, &bufferlength);
			status = GetAllModuleNum(&dwcount);
			*buffer = dwcount;
		}
		WdfRequestCompleteWithInformation(Request, status, sizeof(DWORD));
		break;
	case EnumModule:
		status = GetModuleBase(Request, InputBufferLength, OutputBufferLength);
		WdfRequestCompleteWithInformation(Request, status, OutputBufferLength);
		break;
	case ProcessorCallBack:
		status = GetProcessorCallbackAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case ThreadCallback:
		status = GetThreadCallbackAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case LoadImageCallback:
		status = GetLoadImageCallbackAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case CmpCallback:
		status = GetCmpRegisterCallbackAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case BugCallback:
		status = GetKeBugCheckCallbackAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case BugReasonCallback:
		status = GetKeRegisterBugCheckReasonCallbackAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case ShutdownCallback:
		status = GetIoRegisterShutdownNotificationAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case LastShutdownCallback:
		status = GetIoRegisterLastChanceShutdownNotificationAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case PlugPlayCallback:
		status = GetIoRegisterPlugPlayNotificationAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case PowerSettingCallback:
		status = GetPoRegisterPowerSettingCallbackAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case FsNotifyChangeCallback:
		status = GetIoRegisterFsRegistrationChangeAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case EnumFilterDriver:
		status = EnumFilterDriverFunc(Request, InputBufferLength, OutputBufferLength);
		WdfRequestCompleteWithInformation(Request, status, OutputBufferLength);
		break;
	case EnumDPCTimer:
		status = EnumDPCTimerInfo(Request, InputBufferLength, OutputBufferLength);
		WdfRequestCompleteWithInformation(Request, status, OutputBufferLength);
		break;
	case GETGPTABLE:
		status = EnumGdtTable(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETSSDTABLE:
		status = EnumSSDTable(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETSSDTSHADOWTABLE:
		status = EnumShadowSSDTable(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETFSDHOOKLIST:
		status = EnumFSDAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETKBDHOOK:
		status = EnumKdbAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETMOUSEHOOK:
		status = EnumMouseAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETI8042PRTHOOK:
		status = EnumI8042PrtAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETPARTMGRHOOK:
		status = EnumPartmgrAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETDISKHOOK:
		status = EnumDiskAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETATAPIHOOK:
		status = EnumAtapiAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETACPIHOOK:
		status = EnumACPIAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETSCSIHOOK:
		status = EnumScsiAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETIDTABLE:
		status = EnumIDTableAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case MSGHOOK_GETADDR:
		status = MsgHook_GetAddress(Request, InputBufferLength, OutputBufferLength);
		WdfRequestCompleteWithInformation(Request, status, OutputBufferLength);
		break;
	case GETTCPIPHOOK:
		status = EnumTcpipAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETNSIPROXYHOOK:
		status = EnumNsiproxyAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETTDXHOOK:
		status = EnumTdxAddress(Request, InputBufferLength, OutputBufferLength, &Resquestlength);
		WdfRequestCompleteWithInformation(Request, status, Resquestlength);
		break;
	case GETALLACPITABLE:
		status = EnumAllACPITable(Request, InputBufferLength, OutputBufferLength);
		WdfRequestCompleteWithInformation(Request, status, OutputBufferLength);
		break;
	default:
		WdfRequestComplete(Request, STATUS_SUCCESS);
		break;
	}

    return;
}

VOID
SystemTestEvtIoStop(
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

    return;
}
