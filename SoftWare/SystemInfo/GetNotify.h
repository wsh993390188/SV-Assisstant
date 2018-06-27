/***************************************************************************************
* AUTHOR : futosky
* DATE   : 2012-03
* MODULE : GetNotify.h
*
****************************************************************************************
* Copyright (C) 2012 futosky.
****************************************************************************************/

#include <ntddk.h>


typedef BOOLEAN BOOL;
typedef unsigned long DWORD;
typedef DWORD * PDWORD;
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#include   <initguid.h> 
//------------------------------------------------------------------------------------------//文件系统回调
ULONG FileSystemRoutine;//回调函数地址
//文件系统回调链表头
//__declspec(dllimport) LIST_ENTRY IopFsNotifyChangeQueueHead;     //全局变量
ULONG IopFsNotifyChangeQueueHead;
//链表结构
typedef struct _NOTIFICATION_PACKET {
    LIST_ENTRY ListEntry;
    PDRIVER_OBJECT DriverObject;
    ULONG NotificationRoutine;
} NOTIFICATION_PACKET, *PNOTIFICATION_PACKET;
NTSTATUS
  IoRegisterFsRegistrationChange(
    IN PDRIVER_OBJECT  DriverObject,
    IN ULONG DriverNotificationRoutine
    );
//------------------------------------------------------------------------------------------//
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString);

VOID DriverUnload(IN PDRIVER_OBJECT pDriverObj);
ULONG PsSetLegoNotifyRoutine(PVOID notifyroutine);
VOID GetListHeadAddr();

ULONG FsNotifyNum=0;//文件系统回调个数
ULONG ShutDownNum=0;//关机回调个数
ULONG CreateThreadNum=0;//创建线程回调个数
ULONG CreateProcessNum=0;//创建进程回调个数
ULONG LoadImageNum=0;//加载模块回调个数
ULONG LegoNum=0;//Lego回调个数
ULONG LeaveSessionNum=0;//注销回调个数
ULONG BugCheckNum=0;//BugCheck回调个数
ULONG RegNum=0;//注册表回调个数
ULONG PlugPlayNum=0;//PlugPlay回调个数


#pragma pack(push)
#pragma pack(1)           
typedef struct _EX_FAST_REF
{
    union
    {
        PVOID Object;
        ULONG_PTR RefCnt:3;
        ULONG_PTR Value;
    };
} EX_FAST_REF, *PEX_FAST_REF;

typedef struct _EX_CALLBACK_ROUTINE_BLOCK
{
    EX_RUNDOWN_REF RundownProtect;
    PEX_CALLBACK_FUNCTION Function;
    PVOID Context;
} EX_CALLBACK_ROUTINE_BLOCK, *PEX_CALLBACK_ROUTINE_BLOCK;
#pragma pack(pop)

typedef struct _SHUTDOWN_PACKET {
    LIST_ENTRY ListEntry;
    PDEVICE_OBJECT DeviceObject;
} SHUTDOWN_PACKET, *PSHUTDOWN_PACKET;

typedef struct _PNP_NOTIFY_ENTRY
{
    LIST_ENTRY PnpNotifyList;
    IO_NOTIFICATION_EVENT_CATEGORY EventCategory;
    PVOID Context;
    UNICODE_STRING Guid;
    PFILE_OBJECT FileObject;
    PDRIVER_NOTIFICATION_CALLBACK_ROUTINE PnpNotificationProc;
} PNP_NOTIFY_ENTRY, *PPNP_NOTIFY_ENTRY;


typedef NTSTATUS (*PSE_LOGON_SESSION_TERMINATED_ROUTINE) (
  IN PLUID LogonId
  );


typedef struct _SEP_LOGON_SESSION_TERMINATED_NOTIFICATION {
    struct _SEP_LOGON_SESSION_TERMINATED_NOTIFICATION *Next;
    PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine;
} SEP_LOGON_SESSION_TERMINATED_NOTIFICATION, *PSEP_LOGON_SESSION_TERMINATED_NOTIFICATION;

NTSTATUS
SeRegisterLogonSessionTerminatedRoutine(
    IN PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine
    );

//得到函数地址
ULONG GetFunctionAddr( IN PCWSTR FunctionName)
{
    UNICODE_STRING UniCodeFunctionName;
    RtlInitUnicodeString( &UniCodeFunctionName, FunctionName );
    return (ULONG)MmGetSystemRoutineAddress( &UniCodeFunctionName );   
}  

typedef NTSTATUS (*PDRIVER_NOTIFICATION_CALLBACK_ROUTINE) (
    IN PVOID NotificationStructure,
    IN PVOID Context
    );

DEFINE_GUID(GUID_CLASS_USBKEY,0x36FC9E60, 0xC465, 0x11CF, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x000, 0x00);
PVOID  NotificationEntry = NULL;