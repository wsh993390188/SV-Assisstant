#pragma once

typedef struct _HANDLEENTRY
{
	UINT64  phead;
	UINT64  pOwner;
	UCHAR   bType;
	UCHAR   bFlags;
	USHORT  wUniq;
}HANDLEENTRY, *PHANDLEENTRY;

typedef struct _HOOK_INFO
{
	HANDLE       hHandle;                    //钩子的句柄   句柄是全局的 可以UnhookWindowsHookEx  把钩子卸掉  
	int          Unknown1;
	PVOID        Win32Thread;                 //一个指向 win32k!_W32THREAD 结构体的指针  
	PVOID        Unknown2;
	PVOID        SelfHook;                   //指向结构体的首地址  
	PVOID        NextHook;                   //指向下一个钩子结构体  
	int          iHookType;                 //钩子的类型， winuser.h 中有定义  
	PVOID        OffPfn;                    //钩子函数的地址偏移，相对于所在模块的偏移  
	int          iHookFlags;
	int          iMod;                      //钩子函数做在模块的索引号码，通过查询 WowProcess 结构可以得到模块的基地址。  
	PVOID        Win32ThreadHooked;         // ？？？被钩的线程的结构指针，不知道  
											//下面还有，省略。。。  
} HOOK_INFO, *PHOOK_INFO;

NTSTATUS MsgHook_GetAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength);

NTSTATUS VxkCopyMemory(PVOID pDestination, PVOID pSourceAddress, SIZE_T SizeOfCopy);