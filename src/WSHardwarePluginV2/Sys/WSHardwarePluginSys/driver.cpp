/*++

Module Name:

driver.c

Abstract:

This file contains the driver entry points and callbacks.

Environment:

Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "driver.tmh"

#ifdef ALLOC_PRAGMA

#pragma alloc_text( INIT, DriverEntry )
#pragma alloc_text (PAGE, LenovoRing0EvtDriverContextCleanup)
#pragma alloc_text (PAGE, LenovoRing0EvtDriverUnload)

#endif

NTSTATUS
DriverEntry(
	IN OUT PDRIVER_OBJECT   DriverObject,
	IN PUNICODE_STRING      RegistryPath
)
/*++

Routine Description:
DriverEntry initializes the driver and is the first routine called by the
system after the driver is loaded. DriverEntry specifies the other entry
points in the function driver, such as EvtDevice and DriverUnload.

Parameters Description:

DriverObject - represents the instance of the function driver that is loaded
into memory. DriverEntry must initialize members of DriverObject before it
returns to the caller. DriverObject is allocated by the system before the
driver is loaded, and it is released by the system after the system unloads
the function driver from memory.

RegistryPath - represents the driver specific path in the Registry.
The function driver can use the path to store driver related data between
reboots. The path does not store hardware instance specific data.

Return Value:

STATUS_SUCCESS if successful,
STATUS_UNSUCCESSFUL otherwise.

--*/
{
	WDF_DRIVER_CONFIG config;
	NTSTATUS status;
	WDF_OBJECT_ATTRIBUTES attributes;
	WDFDRIVER                      hDriver;
	PWDFDEVICE_INIT                pInit = NULL;

	//
	// Initialize WPP Tracing
	//
	WPP_INIT_TRACING(DriverObject, RegistryPath);

	TraceEvents(TRACE_LEVEL_INFORMATION, DBG_INIT, "%!FUNC! Entry");
	TraceEvents(TRACE_LEVEL_INFORMATION, DBG_INIT, "Built %s %s", __DATE__, __TIME__);

	// Register a cleanup callback so that we can call WPP_CLEANUP when
	// the framework driver object is deleted during driver unload.
	//
	WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
	attributes.EvtCleanupCallback = LenovoRing0EvtDriverContextCleanup;

	WDF_DRIVER_CONFIG_INIT(
		&config,
		WDF_NO_EVENT_CALLBACK // This is a non-pnp driver.
	);

	//
	// Tell the framework that this is non-pnp driver so that it doesn't
	// set the default AddDevice routine.
	//
	config.DriverInitFlags |= WdfDriverInitNonPnpDriver;

	//
	// NonPnp driver must explicitly register an unload routine for
	// the driver to be unloaded.
	//
	config.EvtDriverUnload = LenovoRing0EvtDriverUnload;

	status = WdfDriverCreate(DriverObject,
		RegistryPath,
		&attributes,
		&config,
		&hDriver
	);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, DBG_INIT, "WdfDriverCreate failed %!STATUS!", status);
		WPP_CLEANUP(DriverObject);
		return status;
	}

	//
	//
	// In order to create a control device, we first need to allocate a
	// WDFDEVICE_INIT structure and set all properties.
	//
	pInit = WdfControlDeviceInitAllocate(
		hDriver,
		&SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RW_RES_R
	);

	if (pInit == NULL) {
		status = STATUS_INSUFFICIENT_RESOURCES;
		TraceEvents(TRACE_LEVEL_ERROR, DBG_INIT, "WdfControlDeviceInitAllocate failed %!STATUS!", status);
		WPP_CLEANUP(DriverObject);
		return status;
	}

	//
	// Call NonPnpDeviceAdd to create a deviceobject to represent our
	// software device.
	//
	status = LenovoRing0DeviceAdd(hDriver, pInit);

	TraceEvents(TRACE_LEVEL_INFORMATION, DBG_INIT, "%!FUNC! Exit");

	return status;
}

VOID
LenovoRing0EvtDriverContextCleanup(
	_In_ WDFOBJECT DriverObject
)
/*++
Routine Description:

Free all the resources allocated in DriverEntry.

Arguments:

DriverObject - handle to a WDF Driver object.

Return Value:

VOID.

--*/
{
	PAGED_CODE();

	//
	// Stop WPP Tracing
	//
	WPP_CLEANUP(WdfDriverWdmGetDriverObject((WDFDRIVER)DriverObject));
}

VOID
LenovoRing0EvtDriverUnload(
	IN WDFDRIVER Driver
)
{
	/*++
	Routine Description:

	Called by the I/O subsystem just before unloading the driver.
	You can free the resources created in the DriverEntry either
	in this routine or in the EvtDriverContextCleanup callback.

	Arguments:

	Driver - Handle to a framework driver object created in DriverEntry

	Return Value:

	VOID

	--*/

	UNREFERENCED_PARAMETER(Driver);
	PAGED_CODE();
	return;
}