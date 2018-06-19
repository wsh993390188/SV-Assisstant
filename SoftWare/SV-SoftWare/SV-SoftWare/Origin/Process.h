// Process.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__3AE4F7B9_1291_4DA9_9D73_C793B1B1B9D6__INCLUDED_)
#define AFX_PROCESS_H__3AE4F7B9_1291_4DA9_9D73_C793B1B1B9D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProcess  
{
public:
	static std::wstring GetProcessNameById(DWORD nPId,BOOL bPath = FALSE);
	CProcess();
	virtual ~CProcess();

};

#endif // !defined(AFX_PROCESS_H__3AE4F7B9_1291_4DA9_9D73_C793B1B1B9D6__INCLUDED_)
