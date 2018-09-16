/*---------------------------------------------------------------------------*/
//       Author : Minkyu Kim
//          Web : http://naraeon.net/
//                https://github.com/ebangin127/cdi-naraeon
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include "diskdefination.h"

static const int MAX_ATTRIBUTE = 30;
static const int NVME_ATTRIBUTE = 15;

typedef SMART_ATTRIBUTE SMART_ATTRIBUTE_LIST[MAX_ATTRIBUTE];

void NVMeSmartToATASmart(UCHAR* NVMeSmartBuf, void* ATASmartBufUncasted);