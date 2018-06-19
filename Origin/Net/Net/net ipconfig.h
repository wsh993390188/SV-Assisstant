#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#pragma comment(lib, "IPHLPAPI.lib")
using namespace std;

typedef map<string, string> MAPSTRING;

BOOL GetAdaptersInfo_WinApi(vector<MAPSTRING >& ipconfig);