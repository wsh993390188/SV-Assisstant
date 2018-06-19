#include "stdafx.h"
#include "Process.h"
#include "CConnect.h"
#pragma comment(lib,"ws2_32.lib")
#pragma  comment(lib,"iphlpapi.lib")

CConnect::CConnect()
{
	hMod = LoadLibrary(L"Iphlpapi.dll");
	if (!hMod)
	{
		OutputDebugString(L"加载Iphlpapi.dll出错");
		return;
	}
	m_pfnGetTcpTable = (PFNGetExtendedTcpTable)::GetProcAddress(hMod, "GetExtendedTcpTable");
	m_pfnGetUdpTable = (PFNGetExtendedUdpTable)::GetProcAddress(hMod, "GetExtendedUdpTable");
}

int CConnect::GetTcpConnect(std::vector<ConnInfo>& connInfo)
{
	ConnInfo temp = {};
	temp.uPro = TCP;
	PMIB_TCPTABLE_OWNER_PID pTcpTable = new MIB_TCPTABLE_OWNER_PID;
	DWORD dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
	if (m_pfnGetTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER)
	{
		delete pTcpTable;
		pTcpTable = (MIB_TCPTABLE_OWNER_PID *)new char[dwSize];
	}
	if (m_pfnGetTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) != NO_ERROR)
	{
		OutputDebugString(L"获取TCP连接出错");
		delete pTcpTable;
		return 0;
	}
	int nNum = (int)pTcpTable->dwNumEntries;

	for (int i = 0; i < nNum; i++)
	{
		if (MIB_TCP_STATE_TIME_WAIT == pTcpTable->table[i].dwState) continue;
		temp.dwLocalIp = inet_ntoa(*(in_addr*)&pTcpTable->table[i].dwLocalAddr);
		temp.dwRemoteIp = inet_ntoa(*(in_addr*)&pTcpTable->table[i].dwRemoteAddr);
		temp.nLocalPort = htons(pTcpTable->table[i].dwLocalPort);
		temp.nRemotePort = htons(pTcpTable->table[i].dwRemotePort);
		temp.uState = GetState(pTcpTable->table[i].dwState);
		temp.nPId = pTcpTable->table[i].dwOwningPid;
		temp.strPath = CProcess::GetProcessNameById(pTcpTable->table[i].dwOwningPid);
		connInfo.emplace_back(temp);
	}

	delete pTcpTable;
	return 1;
}

std::wstring CConnect::GetState(int nState)
{
	std::wstring strStatus{};
	switch (nState)
	{
	case MIB_TCP_STATE_CLOSED:
		strStatus = L"CLOSED";
		break;
	case MIB_TCP_STATE_LISTEN:
		strStatus = L"LISTEN";
		break;
	case MIB_TCP_STATE_SYN_SENT:
		strStatus = L"SYN_SENT";
		break;
	case MIB_TCP_STATE_SYN_RCVD:
		strStatus = L"SYN_RCVD";
		break;
	case MIB_TCP_STATE_ESTAB:
		strStatus = L"ESTAB";
		break;
	case MIB_TCP_STATE_FIN_WAIT1:
		strStatus = L"FIN_WAIT1";
		break;
	case MIB_TCP_STATE_FIN_WAIT2:
		strStatus = L"FIN_WAIT2";
		break;
	case MIB_TCP_STATE_CLOSE_WAIT:
		strStatus = L"CLOSE WAIT";
		break;
	case MIB_TCP_STATE_CLOSING:
		strStatus = L"CLOSING";
		break;
	case MIB_TCP_STATE_LAST_ACK:
		strStatus = L"LAST ACK";
		break;
	case MIB_TCP_STATE_TIME_WAIT:
		strStatus = L"TIME WAIT";
		break;
	case MIB_TCP_STATE_DELETE_TCB:
		strStatus = L"DELETE TCB";
		break;
	default:
		strStatus = L"<ERR>";
	}
	return strStatus;
}

int CConnect::GetUdpConnect(std::vector<ConnInfo> & connInfo)
{
	ConnInfo temp = {};
	temp.uPro = UDP;
	PMIB_UDPTABLE_OWNER_PID pUdpTable = new MIB_UDPTABLE_OWNER_PID;
	DWORD dwSize = sizeof(MIB_UDPTABLE_OWNER_PID);
	if (m_pfnGetUdpTable(pUdpTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0) == ERROR_INSUFFICIENT_BUFFER)
	{
		delete pUdpTable;
		pUdpTable = (MIB_UDPTABLE_OWNER_PID *)new char[dwSize];
	}
	if (m_pfnGetUdpTable(pUdpTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR)
	{
		OutputDebugString(_T("获取UDP连接出错"));
		delete pUdpTable;
		return 0;
	}
	int nNum = (int)pUdpTable->dwNumEntries;
	for (int i = 0; i < nNum; i++)
	{
		temp.dwLocalIp = inet_ntoa(*(in_addr*)&pUdpTable->table[i].dwLocalAddr);
		temp.nLocalPort = htons(pUdpTable->table[i].dwLocalPort);
		temp.nPId = pUdpTable->table[i].dwOwningPid;
		temp.strPath = CProcess::GetProcessNameById(pUdpTable->table[i].dwOwningPid);
		connInfo.emplace_back(temp);
	}
	delete pUdpTable;
	return 1;
}


CConnect::~CConnect()
{
	FreeLibrary(hMod);
}
