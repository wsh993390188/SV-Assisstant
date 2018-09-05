/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/
#pragma once
//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_SystemTest,
    0x30809efd,0x6125,0x42dc,0x85,0xbc,0x63,0x1e,0x61,0x4a,0x60,0x0d);
// {30809efd-6125-42dc-85bc-631e614a600d}
