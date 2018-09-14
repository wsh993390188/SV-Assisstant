// stdafx.cpp : 只包括标准包含文件的源文件
// sv-assistant.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用
#ifdef _DEBUG
void OutputDebugPrintf(const TCHAR* strOutputString, ...)
{
	TCHAR strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
#ifdef UNICODE
	_vsnwprintf_s(strBuffer, 4095, strOutputString, vlArgs);
#else
	_vsnprintf_s(strBuffer, 4095, strOutputString, vlArgs);
#endif
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
#ifdef ZX_OutputLog
	Logger::Instance()->OutputLogInfo(el::Level::Debug, strBuffer);
#endif
#ifdef UNICODE
	wcscat_s(strBuffer, 4095,L"\n");
#else
	strcat_s(strBuffer, 4095, "\n");
#endif
	OutputDebugString(strBuffer);
}
#else
void OutputDebugPrintf(const TCHAR* strOutputString, ...) 
{
#ifdef ZX_OutputLog
	TCHAR strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
#ifdef UNICODE
	_vsnwprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
#else
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
#endif
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, strBuffer);
#endif
}
#endif

