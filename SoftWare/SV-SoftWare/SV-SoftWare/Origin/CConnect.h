#pragma once

typedef DWORD(WINAPI *PFNGetExtendedUdpTable)(
	__out         PVOID pUdpTable,//返回查询结构体指针
	__inout       PDWORD pdwSize,//估计结构体大小
	__in          BOOL bOrder,//是否排序
	__in          ULONG ulAf,//是AF_INET还是AF_INET6
	__in          UDP_TABLE_CLASS TableClass,//返回结构体的种类
	__in          ULONG Reserved//保留值
	);

typedef DWORD(WINAPI *PFNGetExtendedTcpTable)(
	__out         PVOID pTcpTable,
	__inout		  PDWORD pdwSize,//大小
	__in          BOOL bOrder,
	__in          ULONG ulAf,
	__in          TCP_TABLE_CLASS TableClass,
	__in          ULONG Reserved
	);

typedef DWORD(WINAPI *PFNSetTcpEntry)(
	IN PMIB_TCPROW pTcpRow
	);

class CConnect
{
public:
	CConnect();
	int GetTcpConnect(std::vector<ConnInfo>& connInfo);
	static std::wstring GetState(int nState);
	int GetUdpConnect(std::vector<ConnInfo>& connInfo);
	~CConnect();
private:
	HMODULE hMod;
	PFNGetExtendedTcpTable m_pfnGetTcpTable;
	PFNGetExtendedUdpTable m_pfnGetUdpTable;
};

