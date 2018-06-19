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
#include <initguid.h>

DEFINE_GUID (GUID_DEVINTERFACE_ZhaoxinRing0,
    0x5fd82ae1,0xef4d,0x4975,0xa6,0x53,0xb2,0xf9,0x8d,0x9c,0x2d,0x9c);
// {5fd82ae1-ef4d-4975-a653-b2f98d9c2d9c}
