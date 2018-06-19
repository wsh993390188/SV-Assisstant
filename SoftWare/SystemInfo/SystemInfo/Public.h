/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_SystemInfo,
    0x0f22eb40,0xb4ba,0x492c,0x93,0x48,0xa1,0xd9,0xa3,0x39,0xf7,0x05);
// {0f22eb40-b4ba-492c-9348-a1d9a339f705}
