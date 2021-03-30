/*++

Module Name:

ioctl.h

Abstract:

This module defines some ioctl for applications.

Environment:

kernel

--*/

#pragma once

#ifndef IOCTL_H
#define IOCTL_H

#ifndef CTL_CODE
#pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

#define IOCTL_RING0_OPERATOR \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif