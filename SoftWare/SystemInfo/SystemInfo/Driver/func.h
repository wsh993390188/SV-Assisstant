#pragma once

#ifdef _WIN64
#define PSP_MAX_CREATE_PROCESS_NOTIFY 64 
#define PNP_MAX_DEVICECLASS_NOTIFY 13
#else  
#define PSP_MAX_CREATE_PROCESS_NOTIFY 8  
#define PNP_MAX_DEVICECLASS_NOTIFY 1
#endif  

typedef ULONGLONG DWORD64;
typedef ULONG   DWORD;
typedef UCHAR   BYTE;
typedef USHORT	WORD;

typedef enum _EPROCESS_OFFSET
{
	EPROCESS_SIZE,
	PEB_OFFSET,
	FILE_NAME_OFFSET,
	PROCESS_LINK_OFFSET,
	PROCESS_ID_OFFSET,
	EXIT_TIME_OFFSET,
	FULL_FILE_NAME_OFFSET,
	InheritedFromUniqueProcessId_OFFSET
}EPROCESS_OFFSET;

typedef enum _FileSystemQueueHead_OFFSET
{
	NETWORKFILESYSTEM = 0,
	CDROMFILESYSTEM = 1,
	DISKFILESYSTEM = 2,
	TAPEFILESYSTEM = 3
}EPROCESS_OFFSET;

typedef struct _SE_AUDIT_PROCESS_CREATION_INFO
{
	POBJECT_NAME_INFORMATION ImageFileName;
} SE_AUDIT_PROCESS_CREATION_INFO, *PSE_AUDIT_PROCESS_CREATION_INFO;

#pragma pack(push)
#pragma pack(1)           
typedef struct _EX_FAST_REF
{
	union
	{
		PVOID Object;
		ULONG_PTR RefCnt : 3;
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

//
// Shutdown entry for registed devices
//
typedef struct _SHUTDOWN_ENTRY
{
	LIST_ENTRY ShutdownList;
	PDEVICE_OBJECT DeviceObject;
} SHUTDOWN_ENTRY, *PSHUTDOWN_ENTRY;

//
// F/S Notification entry for registered File Systems
//
typedef struct _FS_CHANGE_NOTIFY_ENTRY
{
	LIST_ENTRY FsChangeNotifyList;
	PDRIVER_OBJECT DriverObject;
	PVOID64 FSDNotificationProc;
} FS_CHANGE_NOTIFY_ENTRY, *PFS_CHANGE_NOTIFY_ENTRY;

typedef struct _PNP_NOTIFY_ENTRY
{
	LIST_ENTRY PnpNotifyList;
	IO_NOTIFICATION_EVENT_CATEGORY EventCategory;
	PVOID Context;
	UNICODE_STRING Guid;
	PFILE_OBJECT FileObject;
	PDRIVER_OBJECT DriverObject;
	PDRIVER_NOTIFICATION_CALLBACK_ROUTINE PnpNotificationProc;
} PNP_NOTIFY_ENTRY, *PPNP_NOTIFY_ENTRY;

typedef struct _POWER_NOTIFY_ENTRY
{
	LIST_ENTRY PowerNotifyList;
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING Guid;
	PPOWER_SETTING_CALLBACK PowerSettingCallbackNotify;
	PVOID Context;
} POWER_NOTIFY_ENTRY, *PPOWER_NOTIFY_ENTRY;

typedef struct _SETUP_NOTIFY_DATA
{
	LIST_ENTRY ListEntry;
	IO_NOTIFICATION_EVENT_CATEGORY EventCategory;
	PVOID64	Unknown1;
	PDRIVER_NOTIFICATION_CALLBACK_ROUTINE Callback;
	PVOID Context;
	PDRIVER_OBJECT DriverObject;
	USHORT RefCount;
	BOOLEAN Unregistered;
	PFAST_MUTEX Lock;
} SETUP_NOTIFY_DATA, *PSETUP_NOTIFY_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY {
	// Start from Windows XP
	LIST_ENTRY InLoadOrderLinks;//�����Ա��ϵͳ���м���(������ֹͣû��ж��)�Ѿ���ȡ���ڴ��� ���ǹ�ϵ��һ��  ����Ҫ�������� ˫���� �����м��ĸ��ڵ㶼���Ա����������� �������������������һ���ڵ�
	LIST_ENTRY InMemoryOrderLinks;//ϵͳ�Ѿ��� û�б���ʼ�� û�е���DriverEntry������̵�ʱ�� ͨ�����������̴�������  
	LIST_ENTRY InInitializationOrderLinks;//�Ѿ�����DriverEntry���������������������
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union {
		ULONG TimeDateStamp;
		PVOID LoadedImports;
	};
	PVOID EntryPointActivationContext;        //_ACTIVATION_CONTEXT *
	PVOID PatchInformation;

	// Start from Windows Vista
	LIST_ENTRY ForwarderLinks;
	LIST_ENTRY ServiceTagLinks;
	LIST_ENTRY StaticLinks;
	PVOID ContextInformation;
	PVOID OriginalBase;
	LARGE_INTEGER LoadTime;
	
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _DIRECTORY_BASIC_INFORMATION {
	UNICODE_STRING ObjectName;
	UNICODE_STRING ObjectTypeName;
} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

typedef enum _OSVERSION
{
	WIN7,
	WIN8,
	WIN81,
	WIN10
}OSVERSION;

EXTERN_C_START

DWORD64 GetPlantformDependentInfo(DWORD64 dwFlag);


DWORD64 GetFunctionAddr(IN PCWSTR FunctionName);

/************************************************************************/
/*                               NT δ��������	                        */
/************************************************************************/
NTKERNELAPI
NTSTATUS
ObReferenceObjectByName(
	IN PUNICODE_STRING ObjectName,
	IN ULONG Attributes,
	IN PACCESS_STATE PassedAccessState OPTIONAL,
	IN ACCESS_MASK DesiredAccess OPTIONAL,
	IN POBJECT_TYPE ObjectType,
	IN KPROCESSOR_MODE AccessMode,
	IN OUT PVOID ParseContext OPTIONAL,
	OUT PVOID *Object
);

NTKERNELAPI
NTSTATUS ZwQueryDirectoryObject(
	_In_      HANDLE  DirectoryHandle,
	_Out_opt_ PVOID   Buffer,
	_In_      ULONG   Length,
	_In_      BOOLEAN ReturnSingleEntry,
	_In_      BOOLEAN RestartScan,
	_Inout_   PULONG  Context,
	_Out_opt_ PULONG  ReturnLength
);

#if (NTDDI_VERSION >= NTDDI_WIN2K)
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS
NTAPI
ZwOpenDirectoryObject(
	_Out_ PHANDLE DirectoryHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes
);
#endif

extern POBJECT_TYPE *IoDriverObjectType;

EXTERN_C_END

