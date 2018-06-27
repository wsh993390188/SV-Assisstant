/***************************************************************************************
* AUTHOR : futosky
* DATE   : 2012-03
* MODULE : GetNotify.c
*
****************************************************************************************
* Copyright (C) 2012 futosky.
****************************************************************************************/
/*
IoRegisterFsRegistrationChangeString         文件系统回调

IoRegisterLastChanceShutdownNotification     IopNotifyLastChanceShutdownQueueHead
            
IoRegisterShutdownNotification               IopNotifyShutdownQueueHead

IoRegisterPlugPlayNotification               即插即用

CmRegisterCallback                           注册表

KeRegisterBugCheckCallback                   错误检测
KeRegisterBugCheckReasonCallback 

PsSetCreateProcessNotifyRoutine              PspCreateProcessNotifyRoutine 
PsSetCreateThreadNotifyRoutine               PspCreateThreadNotifyRoutine
PsSetLoadImageNotifyRoutine                  PspLoadImageNotifyRoutine

SeRegisterLogonSessionTerminatedRoutine      注销用户  回调
*/
#include "GetNotify.h"

NTSTATUS GetCreateProcessNotify()
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)PsSetCreateProcessNotifyRoutine;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"PsSetCreateProcessNotifyRoutine");
	int i=0;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		/*
		805d0c27 56              push    esi
        805d0c28 57              push    edi
        805d0c29 7464            je      nt!PsSetCreateProcessNotifyRoutine+0x73 (805d0c8f)
        805d0c2b bf404a5680      mov     edi,offset nt!PspCreateProcessNotifyRoutine (80564a40)         指令bf 40 4a 56 80即包含地址80564a40
        805d0c30 57              push    edi
        */
		if((*(UCHAR*)Base==0x64)&&(*(UCHAR*)(Base+1)==0xbf))
		{
			Address=*(ULONG*)(Base+2);
			//DbgPrint("进程创建回调函数数组地址:%d\n",Address); //数组是EX_FAST_REF类型
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
        //KdPrint(("没有找到进程创建回调函数数组\n"));
        return STATUS_UNSUCCESSFUL;
     }
            
	Ref=(PEX_FAST_REF)(Address);
            
	for(i=0;i<8;i++)
	{
		PEX_CALLBACK_ROUTINE_BLOCK Point;

		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);//得到存有函数地址的结构
		
		if(MmIsAddressValid((PVOID)Point))
		{
			DbgPrint("回调入口:%X  类型:CreateProcess\n",(ULONG)(Point->Function));
			CreateProcessNum++;
		}
		Ref++;
	}
	return STATUS_SUCCESS;
}

NTSTATUS GetCreateThreadNotify()
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)PsSetCreateThreadNotifyRoutine;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"PsSetCreateThreadNotifyRoutine");
	int i=0;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0x56)&&(*(UCHAR*)(Base+1)==0xbe))
		{
			Address=*(ULONG*)(Base+2);
			//DbgPrint("线程创建回调函数数组地址:%d\n",Address); //数组是EX_FAST_REF类型
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
        return STATUS_UNSUCCESSFUL;
     }
            
	Ref=(PEX_FAST_REF)(Address);
            
	for(i=0;i<8;i++)
	{
		PEX_CALLBACK_ROUTINE_BLOCK Point;

		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);//得到存有函数地址的结构
		
		if(MmIsAddressValid((PVOID)Point))
		{
			CreateThreadNum++;
			DbgPrint("回调入口:%X  类型:CreateThread\n",(ULONG)(Point->Function));
		}
		Ref++;
	}
	return STATUS_SUCCESS;
}

NTSTATUS GetLoadImageNotify()
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)PsSetLoadImageNotifyRoutine;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"PsSetLoadImageNotifyRoutine");
	int i=0;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0x56)&&(*(UCHAR*)(Base+1)==0xbe))
		{
			Address=*(ULONG*)(Base+2);
			//DbgPrint("加载模块回调函数数组地址:%d\n",Address); 
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
        return STATUS_UNSUCCESSFUL;
     }
            
	Ref=(PEX_FAST_REF)(Address);
            
	for(i=0;i<8;i++)
	{
		PEX_CALLBACK_ROUTINE_BLOCK Point;

		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);
		
		if(MmIsAddressValid((PVOID)Point))
		{
			LoadImageNum++;
			DbgPrint("回调入口:%X  类型:LoadImage\n",(ULONG)(Point->Function));
		}
		Ref++;
	}
	return STATUS_SUCCESS;
}

NTSTATUS GetRegisterNotify()
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)CmRegisterCallback;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"CmRegisterCallback");
	int i=0;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0xc7)&&(*(UCHAR*)(Base+1)==0x45)&&(*(UCHAR*)(Base+2)==0x08))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
		DbgPrint("获取注册表回调数组失败\n");
        return STATUS_UNSUCCESSFUL;
     }
            
	Ref=(PEX_FAST_REF)(Address);
            
	for(i=0;i<8;i++)
	{
		PEX_CALLBACK_ROUTINE_BLOCK Point;

		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);
		
		if(MmIsAddressValid((PVOID)Point))
		{
			RegNum++;
			DbgPrint("回调入口:%X  类型:CmpCallBack\n",(ULONG)(Point->Function));
		}
		Ref++;
	}
	return STATUS_SUCCESS;
}
//804fa138 c7400458d75580  mov     dword ptr [eax+4],offset nt!KeBugCheckCallbackListHead (8055d758)
/*
NTKERNELAPI
BOOLEAN
KeRegisterBugCheckCallback (
    __out PKBUGCHECK_CALLBACK_RECORD CallbackRecord,
    __in PKBUGCHECK_CALLBACK_ROUTINE CallbackRoutine,
    __in PVOID Buffer,
    __in ULONG Length,
    __in PUCHAR Component
    )

	typedef struct _KBUGCHECK_CALLBACK_RECORD {
    LIST_ENTRY Entry;
    PKBUGCHECK_CALLBACK_ROUTINE CallbackRoutine;
    PVOID Buffer;
    ULONG Length;
    PUCHAR Component;
    ULONG_PTR Checksum;
    UCHAR State;
} KBUGCHECK_CALLBACK_RECORD, *PKBUGCHECK_CALLBACK_RECORD;

LIST_ENTRY KeBugCheckCallbackListHead;
LIST_ENTRY KeBugCheckReasonCallbackListHead;

*/
NTSTATUS GetBugCheckCallBack()
{
	//UCHAR* Base=(UCHAR*)KeRegisterBugCheckCallback;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"KeRegisterBugCheckCallback");
	int i=0;
	PKBUGCHECK_CALLBACK_RECORD pBugCheckRecordPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0xc7)&&(*(UCHAR*)(Base+1)==0x40)&&(*(UCHAR*)(Base+2)==0x04))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
		DbgPrint("获取BugCheck回调数组失败\n");
        return STATUS_UNSUCCESSFUL;
     }
	 pBugCheckRecordPack=Address;
	 pListEntry=&(pBugCheckRecordPack->Entry);
	 pListNext=pListEntry->Flink;
	 {
		pBugCheckRecordPack=pListNext;
		BugCheckNum++;
		DbgPrint("回调入口:%X  类型:BugCheckCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);
	 }
	 while(pListNext->Flink!=pListEntry)
	 {
		pListNext=pListNext->Flink;
	    pBugCheckRecordPack=pListNext;
		BugCheckNum++;
		DbgPrint("回调入口:%X  类型:BugCheckCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);
	 }
}

NTSTATUS GetBugCheckReasonCallBack()
{
	//UCHAR* Base=(UCHAR*)KeRegisterBugCheckReasonCallback;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"KeRegisterBugCheckReasonCallback");
	int i=0;
	PKBUGCHECK_CALLBACK_RECORD pBugCheckRecordPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0xc7)&&(*(UCHAR*)(Base+1)==0x40)&&(*(UCHAR*)(Base+2)==0x04))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
		DbgPrint("获取BugCheckReason回调数组失败\n");
        return STATUS_UNSUCCESSFUL;
     }
	 pBugCheckRecordPack=Address;
	 pListEntry=&(pBugCheckRecordPack->Entry);
	 pListNext=pListEntry->Flink;
	 {
		pBugCheckRecordPack=pListNext;
		BugCheckNum++;
		DbgPrint("回调入口:%X  类型:BugCheckReasonCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);
	 }
	 while(pListNext->Flink!=pListEntry)
	 {
		pListNext=pListNext->Flink;
	    pBugCheckRecordPack=pListNext;
		BugCheckNum++;
		DbgPrint("回调入口:%X  类型:BugCheckReasonCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);
	 }
}
NTSTATUS GetFileSystemNotifyCallBack()
{
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	PNOTIFICATION_PACKET pNotificationPacket;

	PDRIVER_OBJECT Driver_Object_Temp;//用于遍历文件系统回调时保存遍历到的驱动对象以便比较
	
	GetListHeadAddr();
	pNotificationPacket=IopFsNotifyChangeQueueHead;
	pListEntry=&(pNotificationPacket->ListEntry);
	pListEntry=pListEntry->Flink;
	pListNext=pListEntry;
	Driver_Object_Temp=*(ULONG*)((ULONG)(pListNext)+8);
	{
		FileSystemRoutine=*(ULONG*)((ULONG)(pListNext)+12);
		FsNotifyNum++;
		DbgPrint("回调入口:%X  类型:FsNotifyChange  驱动对象:%X  驱动名称:%wZ  包地址:%X\n",FileSystemRoutine,(ULONG)Driver_Object_Temp,&Driver_Object_Temp->DriverName,(ULONG)pListNext);
	}
	while(pListNext->Flink->Flink!=pListEntry)
	{
		pListNext=pListNext->Flink;
	    Driver_Object_Temp=*(ULONG*)((ULONG)(pListNext)+8);
		{
			FileSystemRoutine=*(ULONG*)((ULONG)(pListNext)+12);
			FsNotifyNum++;
		    DbgPrint("回调入口:%X  类型:FsNotifyChange  驱动对象:%X  驱动名称:%wZ  包地址:%X\n",FileSystemRoutine,(ULONG)Driver_Object_Temp,&Driver_Object_Temp->DriverName,(ULONG)pListNext);
	    }
	}
}
/*
LIST_ENTRY IopNotifyShutdownQueueHead;
LIST_ENTRY IopNotifyLastChanceShutdownQueueHead;

typedef struct _SHUTDOWN_PACKET {
    LIST_ENTRY ListEntry;
    PDEVICE_OBJECT DeviceObject;
} SHUTDOWN_PACKET, *PSHUTDOWN_PACKET;
*/
//#define IRP_MJ_SHUTDOWN                 0x10
//根据设备对象得到ShutDwon例程地址
ULONG GetNotifyAddr(PDEVICE_OBJECT Device_Object)
{
	ULONG Addr;
	PDRIVER_OBJECT Driver_Object;
	Driver_Object=Device_Object->DriverObject;
	Addr=*(ULONG*)((ULONG)Driver_Object+0x38+0x40);
	return Addr;
}
NTSTATUS GetShutDownCallBack()
{
	//UCHAR* Base=(UCHAR*)IoRegisterShutdownNotification;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"IoRegisterShutdownNotification");
	int i=0;
	ULONG NotifyAddr;//回调函数地址
	PSHUTDOWN_PACKET pShutDownPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0x8b)&&(*(UCHAR*)(Base+1)==0xd7)&&(*(UCHAR*)(Base+2)==0xb9))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
		DbgPrint("获取ShutDown回调数组失败\n");
        return STATUS_UNSUCCESSFUL;
     }
	 pShutDownPack=Address;
	 pListEntry=&(pShutDownPack->ListEntry);
	 pListNext=pListEntry->Flink;
	 {
		pShutDownPack=pListNext;
		NotifyAddr=GetNotifyAddr(pShutDownPack->DeviceObject);
		ShutDownNum++;
		DbgPrint("回调入口:%X  类型:ShutDown  设备对象:%X\n",NotifyAddr,pShutDownPack->DeviceObject);
	 }
	 while(pListNext->Flink!=pListEntry)
	 {
		pListNext=pListNext->Flink;
	    pShutDownPack=pListNext;
		NotifyAddr=GetNotifyAddr(pShutDownPack->DeviceObject);
		ShutDownNum++;
		DbgPrint("回调入口:%X  类型:ShutDown  设备对象:%X\n",NotifyAddr,pShutDownPack->DeviceObject);
	 }
}

NTSTATUS GetLastChanceShutDownCallBack()
{
	//UCHAR* Base=(UCHAR*)IoRegisterLastChanceShutdownNotification;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"IoRegisterLastChanceShutdownNotification");
	int i=0;
	ULONG NotifyAddr;//回调函数地址
	PSHUTDOWN_PACKET pShutDownPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0x8b)&&(*(UCHAR*)(Base+1)==0xd7)&&(*(UCHAR*)(Base+2)==0xb9))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
		DbgPrint("获取LastChanceShutDown回调数组失败\n");
        return STATUS_UNSUCCESSFUL;
     }
	 pShutDownPack=Address;
	 pListEntry=&(pShutDownPack->ListEntry);
	 pListEntry=pListEntry->Flink;
	 pListNext=pListEntry;
	 if(pListNext!=NULL)
	 {
		 {
			pShutDownPack=pListNext;
			NotifyAddr=GetNotifyAddr(pShutDownPack->DeviceObject);
			DbgPrint("回调入口:%X  类型:LastChanceShutDown  设备对象:%X\n",NotifyAddr,pShutDownPack->DeviceObject);
		 }
		 while(pListNext->Flink!=pListEntry)
		 {
			pListNext=pListNext->Flink;
			NotifyAddr=GetNotifyAddr(pShutDownPack->DeviceObject);
			DbgPrint("回调入口:%X  类型:LastChanceShutDown  设备对象:%X\n",NotifyAddr,pShutDownPack->DeviceObject);
		 }
	 }
}

//回调函数
NTSTATUS DriverDevInterxNotifyCallBack()
{
	return STATUS_SUCCESS;
}

NTSTATUS GetPlugPlayCallBack(PDRIVER_OBJECT pDriverObject)//自己注册一个即插即用回调
{
	NTSTATUS  result;
	PVOID DeviceData=(PVOID)ExAllocatePool(NonPagedPool,256);

	result=IoRegisterPlugPlayNotification(EventCategoryDeviceInterfaceChange,
                                   PNPNOTIFY_DEVICE_INTERFACE_INCLUDE_EXISTING_INTERFACES,
                                   (PVOID)(&GUID_CLASS_USBKEY),
                                   pDriverObject,
                                   (PDRIVER_NOTIFICATION_CALLBACK_ROUTINE)DriverDevInterxNotifyCallBack, 
                                   NULL,
                                   &NotificationEntry);

	if(result==STATUS_SUCCESS)
	{
		if(NotificationEntry!=NULL)
		{
	        DbgPrint("NotificationEntry:%x\n",NotificationEntry);
		}
		else
		{
			 DbgPrint("NULL\n");
		}
		return result;
	}
	IoUnregisterPlugPlayNotification(NotificationEntry);
	return result;
}
/*NTSTATUS GetPlugPlayCallBack()
{
	//UCHAR* Base=(UCHAR*)IoRegisterPlugPlayNotification;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"IoRegisterPlugPlayNotification");
	int i=0;
	ULONG NotifyAddr;//回调函数地址
	PPNP_NOTIFY_ENTRY pPnpNotifyPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0x80)&&(*(UCHAR*)(Base+1)==0xc7)&&(*(UCHAR*)(Base+2)==0x03))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	DbgPrint("%x\n",Base);
	 if(Address==0)
     {
		DbgPrint("获取PlugPlay回调数组失败\n");
        return STATUS_UNSUCCESSFUL;
     }
	 pPnpNotifyPack=Address;
	 pListEntry=&(pPnpNotifyPack->PnpNotifyList);
	 pListNext=pListEntry->Flink;
	 {
		pPnpNotifyPack=pListNext;
		PlugPlayNum++;
		DbgPrint("回调入口:%X  类型:PlugPlay  包地址:%X\n",pPnpNotifyPack->PnpNotificationProc,pPnpNotifyPack);
	 }
	 while(pListNext->Flink!=pListEntry)
	 {
		pListNext=pListNext->Flink;
	    pPnpNotifyPack=pListNext;
		PlugPlayNum++;
		DbgPrint("回调入口:%X  类型:PlugPlay  包地址:%X\n",pPnpNotifyPack->PnpNotificationProc,pPnpNotifyPack);
	 }
}*/

//Lego回调
/*
nt!PsSetLegoNotifyRoutine:
805d299a 8bff            mov     edi,edi
805d299c 55              push    ebp
805d299d 8bec            mov     ebp,esp
805d299f 8b4508          mov     eax,dword ptr [ebp+8]
805d29a2 a3c0d26780      mov     dword ptr [nt!PspLegoNotifyRoutine (8067d2c0)],eax        只允许设置一个，未导出变量保存
805d29a7 b8d0000000      mov     eax,0D0h
805d29ac 5d              pop     ebp
805d29ad c20400          ret     4
*/


ULONG GetPsSetLegoNotifyRoutineAddr()
{
	ULONG Addr;
	Addr=(char *) GetFunctionAddr(L"PsSetLegoNotifyRoutine");
	return Addr;
}
void GetLegoCallBack()
{
	ULONG Addr;
	UCHAR* Base;
	int i=0;
	ULONG Address;
	Addr=GetPsSetLegoNotifyRoutineAddr();
	Base=(UCHAR*)Addr;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0x45)&&(*(UCHAR*)(Base+1)==0x08)&&(*(UCHAR*)(Base+2)==0xa3))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
		DbgPrint("获取Lego回调失败\n");
        return STATUS_UNSUCCESSFUL;
     }
	 if(*(ULONG*)Address==0)
	 {
		 ;
	 }
	 else
	 {
	    LegoNum=1;
		DbgPrint("回调入口:%X  类型:Lego\n",*(ULONG*)Address);
	 }
}

//注销  回调
void GetLeaveSessionCallback()
{
	UCHAR* Base;
	int i=0;
	ULONG Address;
	
	PSEP_LOGON_SESSION_TERMINATED_NOTIFICATION CallbackList=NULL;
	PSEP_LOGON_SESSION_TERMINATED_NOTIFICATION TmpCallbackList=NULL;
	Address=0;
	Base=(UCHAR*)GetFunctionAddr(L"SeRegisterLogonSessionTerminatedRoutine");
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0xf4)&&(*(UCHAR*)(Base+1)==0xff)&&(*(UCHAR*)(Base+2)==0xa1))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	 if(Address==0)
     {
		DbgPrint("获取注销回调失败\n");
        return STATUS_UNSUCCESSFUL;
     }
	 
	 CallbackList=Address;
	 if(CallbackList->Next==0)
	 {
		 ;
	 }
	 else
	 {
		 TmpCallbackList=CallbackList->Next;
		 while(1)
		 {
			 LeaveSessionNum++;
			 DbgPrint("回调入口:%X  类型:SeFileSystem\n",(ULONG)TmpCallbackList->CallbackRoutine);
			 if(TmpCallbackList->Next==0)
			 {
				 break;
			 }
			 TmpCallbackList=TmpCallbackList->Next;
		 }
	 }
}
//入口函数
DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;

	pDriverObj->DriverUnload = DriverUnload;

	DbgPrint("驱动加载成功\n");
    //-----------------------------------------------------------------------//文件系统回调
	GetFileSystemNotifyCallBack();
	//-----------------------------------------------------------------------//进程相关三组回调
	GetCreateProcessNotify();
	GetCreateThreadNotify();
	GetLoadImageNotify();
	//-----------------------------------------------------------------------//注册表
	GetRegisterNotify();
	//-----------------------------------------------------------------------//BugCheck
	GetBugCheckCallBack();
	GetBugCheckReasonCallBack();
	//-----------------------------------------------------------------------//关机回调
	GetShutDownCallBack();
	//GetLastChanceShutDownCallBack();//貌似有问题？？
	//-----------------------------------------------------------------------//PlugPlay回调
	GetPlugPlayCallBack(pDriverObj);
	//-----------------------------------------------------------------------//Lego回调
	GetLegoCallBack();
	//-----------------------------------------------------------------------//注销回调
	GetLeaveSessionCallback();
	//-----------------------------------------------------------------------//
	DbgPrint("创建进程:%d，创建线程:%d，加载模块:%d，注册表:%d，错误检测:%d，Lego回调:%d，关机:%d，注销:%d，即插即用:%d，文件系统改变:%d\n",
		CreateProcessNum,CreateThreadNum,LoadImageNum,RegNum,BugCheckNum,LegoNum,ShutDownNum,LeaveSessionNum,PlugPlayNum,FsNotifyNum);
	return STATUS_SUCCESS;
}
//得到文件系统回调链表头地址
VOID GetListHeadAddr()
{ 
	ULONG Addr;
	UNICODE_STRING IoRegisterFsRegistrationChangeString={0};
    PVOID EnumParameter;
	RtlInitUnicodeString(&IoRegisterFsRegistrationChangeString, L"IoRegisterFsRegistrationChange");
    Addr=MmGetSystemRoutineAddress(&IoRegisterFsRegistrationChangeString);
	Addr=*(ULONG*)((BYTE*)Addr+25);
	IopFsNotifyChangeQueueHead=Addr;
}
//卸载函数
VOID DriverUnload(IN PDRIVER_OBJECT pDriverObj)
{	
	DbgPrint("Unloading...\r\n");
	DbgPrint("Unloaded Success\r\n");
	return STATUS_SUCCESS;
}