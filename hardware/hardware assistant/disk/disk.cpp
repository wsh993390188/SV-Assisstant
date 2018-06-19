#include "stdafx.h"
#include "disk.h"
#include <direct.h>  
#include <io.h> 
#include <stdlib.h>  
/************************************************************************/
/*  From CrystalDiskInfo												*/
/************************************************************************/
ATA_SMART_INFO::ATA_SMART_INFO()
{
	::ZeroMemory(&this->Identify, sizeof(Identify));
	this->deviceid = _T("");
	this->diskSize = _T("");
	this->firmware = _T("");
	this->interfaceTypeWmi = _T("");
	this->mediaType = _T("");
	this->model = _T("");
	this->MajorVersion = _T("");
	this->MinorVersion = _T("");
	this->CurrentTransferMode = _T("");
	this->MaxTransferMode = _T("");
	this->Interface = _T("");
	this->SmartKeyName = _T("");
	this->SsdVendorString = _T("");
	this->mediaType = _T("");
	this->model = _T("");
	this->pnpDeviceId = _T("");
	AttributeCount = -1;
	DiskVendorId = -1;
	HostWrites = -1;
	Major = -1;
	TransferModeType = -1;
	HostReads = -1;
	GBytesErased = -1;
	NandWrites = -1;
	WearLevelingCount = -1;
	Life = -1;
	HostReadsWritesUnit = HOST_READS_WRITES_UNKNOWN;
	PowerOnRawValue = -1;
	PowerOnCount = -1;
	Temperature = -1;
	TemperatureMultiplier = 1.00;
	DetectedPowerOnHours = -1;
	MeasuredPowerOnHours = -1;
	DetectedTimeUnitType = -1;
	MeasuredTimeUnitType = -1;
	IsSsd = FALSE;
	IsRawValues8 = FALSE;
	IsRawValues7 = FALSE;
	PowerOnStartRawValue = -1;
	this->physicalDriveId = -1;
	this->scsiBus = -1;
	this->scsiPort = -1;
	this->scsiTargetId = -1;
	for (BYTE i : this->SmartReadData)
	{
		SmartReadData[i] = 0;
	}
	for (BYTE i : this->SmartReadThreshold)
	{
		SmartReadThreshold[i] = 0;
	}
	::ZeroMemory(Attribute, sizeof(SMART_ATTRIBUTE) * MAX_ATTRIBUTE);
	::ZeroMemory(Threshold, sizeof(SMART_THRESHOLD) * MAX_ATTRIBUTE);
}

HANDLE Disk::OpenDevice(LPCTSTR filename)
{
	HANDLE hDevice;//打开设备
	hDevice = CreateFile(filename, //文件名
		GENERIC_READ | GENERIC_WRITE, // 读写方式
		FILE_SHARE_READ | FILE_SHARE_WRITE, // 共享方式
		NULL, // 默认的安全描述符
		OPEN_EXISTING,// 创建方式 
		0, // 不需设置文件属性
		NULL); // 不需参照模板文件
	return hDevice;
}

BOOL Disk::IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo)
{
	PSENDCMDINPARAMS pSCIP; // 输入数据结构指针
	PSENDCMDOUTPARAMS pSCOP; // 输出数据结构指针
	DWORD dwOutBytes; // IOCTL输出数据长度
	BOOL bResult;// IOCTL返回值
				   // 申请输入/输 出数据结构空间
	pSCIP = (PSENDCMDINPARAMS)::GlobalAlloc(LMEM_ZEROINIT,
		sizeof(SENDCMDINPARAMS) - 1);
	pSCOP = (PSENDCMDOUTPARAMS)::GlobalAlloc(LMEM_ZEROINIT,
		sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);
	// 指定ATA/ATAPI命令的寄存器值
	// pSCIP->irDriveRegs.bFeaturesReg = 0;
	// pSCIP- >irDriveRegs.bSectorCountReg = 0;
	// pSCIP->irDriveRegs.bSectorNumberReg = 0;
	// pSCIP->irDriveRegs.bCylLowReg = 0;
	// pSCIP->irDriveRegs.bCylHighReg = 0;
	// pSCIP->irDriveRegs.bDriveHeadReg = 0;
	pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
	// 指定输入/输出数据缓冲区大小
	pSCIP->cBufferSize = 0;
	pSCOP->cBufferSize = sizeof(IDINFO);
	// IDENTIFY DEVICE
	bResult = ::DeviceIoControl(hDevice, // 设备句柄
		DFP_RECEIVE_DRIVE_DATA, // 指定IOCTL
		pSCIP, sizeof(SENDCMDINPARAMS) - 1, // 输入数据缓冲区
		pSCOP, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // 输出数据缓冲区
		&dwOutBytes,// 输出数据长度
		(LPOVERLAPPED)NULL); // 用同步I/O
							  // 复制设备参数结构
	memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO)); // 释放输入/输出数据空间
	GlobalFree(pSCOP);
	GlobalFree(pSCIP);
	return bResult;
}

BOOL Disk::IdentifyDeviceAsScsi(HANDLE hDevice, int nDrive, PIDINFO pIdInfo)
{
	PSENDCMDINPARAMS pSCIP; //输入数据结构指针
	PSENDCMDOUTPARAMS pSCOP; //输出数据结构指针
	PSRB_IO_CONTROL pSRBIO; //SCSI输入输出数据结构指针
	DWORD dwOutBytes; // IOCTL输出数据长度
	BOOL bResult;// IOCTL返回值
				   //申请输入/输出数据结构空间
	pSRBIO = (PSRB_IO_CONTROL)::GlobalAlloc(LMEM_ZEROINIT,
		sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);
	pSCIP = (PSENDCMDINPARAMS)((char *)pSRBIO + sizeof(SRB_IO_CONTROL));
	pSCOP = (PSENDCMDOUTPARAMS)((char *)pSRBIO + sizeof(SRB_IO_CONTROL));
	//填充输入/输出数据
	pSRBIO->HeaderLength = sizeof(SRB_IO_CONTROL);
	pSRBIO->Timeout = 10000;
	pSRBIO->Length = sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1;
	pSRBIO->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
	strncpy_s((char *)pSRBIO->Signature,9 ,"SCSIDISK", 8);
	// 指定ATA/ATAPI命令的寄存器值
	// pSCIP- >irDriveRegs.bFeaturesReg = 0;
	// pSCIP->irDriveRegs.bSectorCountReg = 0;
	// pSCIP->irDriveRegs.bSectorNumberReg = 0;
	// pSCIP->irDriveRegs.bCylLowReg = 0;
	// pSCIP->irDriveRegs.bCylHighReg = 0;
	// pSCIP- >irDriveRegs.bDriveHeadReg = 0;
	pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
	pSCIP->bDriveNumber = nDrive;
	// IDENTIFY DEVICE
	bResult = DeviceIoControl(
		hDevice, //设备句柄
		IOCTL_SCSI_MINIPORT, //指定IOCTL
		pSRBIO, 
		sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1, // 输入数据缓冲区 
		pSRBIO, 
		sizeof(SRB_IO_CONTROL) +sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // 输出 数据缓冲区
		&dwOutBytes, //输出数据长度
		(LPOVERLAPPED)NULL); //用同步I/O
	memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));//复制设备参数结构
	GlobalFree(pSRBIO);//释放输入/输出数据空间
	GlobalFree(pSCOP);//释放输入/输出数据空间
	GlobalFree(pSCIP);//释放输入/输出数据空间
	return bResult;
}

void Disk::AdjustString(char* str, int len)
{
	char ch;
	int i;
	//两两颠倒
	for (i = 0; i < len; i += 2)
	{
		ch = str[i];
		str[i] = str[i + 1];
		str[i + 1] = ch;
	}
	//若是右对齐的,调整为左对齐(去掉左边的空格)
	i = 0;
	while (i < len && str[i] == ' ') i++;
	memmove(str, &str[i], len - i);
	//去掉右边的空格
	i = len - 1;
	while (i >= 0 && str[i] == ' ')
	{
		str[i] = '\0';
		i--;
	}
}

BOOL Disk::GetPhysicalDriveInfoInNT(int nDrive, PIDINFO pIdInfo)
{
	HANDLE hDevice; // 设备句柄
	BOOL bResult; // 返回结果
	TCHAR szFileName[20]; // 文件名
	memset(szFileName, '0', 20);
	wsprintf(szFileName, _T("\\\\.\\PhysicalDrive%d"), nDrive);
	//_ASSERTE(_CrtCheckMemory());
	hDevice = OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// IDENTIFY DEVICE
	bResult = IdentifyDevice(hDevice, pIdInfo);
	if (bResult)
	{
		// 调整字符串
		AdjustString(pIdInfo->sSerialNumber, 20);
		AdjustString(pIdInfo->sModelNumber, 40);
		AdjustString(pIdInfo->sFirmwareRev, 8);
	}
	CloseHandle(hDevice);
	return bResult;
}

BOOL Disk::GetIdeDriveAsScsiInfoInNT(int nDrive, PIDINFO pIdInfo)
{
	HANDLE hDevice; // 设备句柄
	BOOL bResult; // 返回结果
	TCHAR szFileName[20]; // 文件名 
	memset(szFileName, '0', 20);
	wsprintf(szFileName, _T("\\\\.\\Scsi%d"), nDrive / 2);
	hDevice = OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// IDENTIFY DEVICE
	bResult = IdentifyDeviceAsScsi(hDevice, nDrive % 2, pIdInfo);
	// 检查是不是空串
	if (pIdInfo->sModelNumber[0] == '\\0')
	{
		bResult = FALSE;
	}
	if (bResult)
	{
		// 调整字符串
		AdjustString(pIdInfo->sSerialNumber, 20);
		AdjustString(pIdInfo->sModelNumber, 40);
		AdjustString(pIdInfo->sFirmwareRev, 8);
	}
	CloseHandle(hDevice);
	return bResult;
}

BOOL Disk::GetPhysicalSMARTInfo(INT nDrive,BYTE target, ATA_SMART_INFO& SMART_info)
{
	HANDLE hDevice; // 设备句柄
	BOOL bResult = FALSE; // 返回结果
	TCHAR szFileName[20]; // 文件名 
	DWORD dwOutBytes; // IOCTL输出数据长度
	memset(szFileName, '0', 20);
	wsprintf(szFileName, _T("\\\\.\\PhysicalDrive%d"), nDrive); 
	hDevice = OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	SMART_READ_DATA_OUTDATA SendCmdOutParam;
	SENDCMDINPARAMS sendCmd;

	::ZeroMemory(&SendCmdOutParam, sizeof(SMART_READ_DATA_OUTDATA));
	::ZeroMemory(&sendCmd, sizeof(SENDCMDINPARAMS));

	sendCmd.irDriveRegs.bFeaturesReg = READ_ATTRIBUTES;
	sendCmd.irDriveRegs.bSectorCountReg = 1;
	sendCmd.irDriveRegs.bSectorNumberReg = 1;
	sendCmd.irDriveRegs.bCylLowReg = SMART_CYL_LOW;
	sendCmd.irDriveRegs.bCylHighReg = SMART_CYL_HI;
	sendCmd.irDriveRegs.bDriveHeadReg = target;
	sendCmd.irDriveRegs.bCommandReg = SMART_CMD;
	sendCmd.cBufferSize = READ_ATTRIBUTE_BUFFER_SIZE;

	bResult = ::DeviceIoControl(
		hDevice,
		DFP_RECEIVE_DRIVE_DATA,
		&sendCmd, sizeof(SENDCMDINPARAMS),
		&SendCmdOutParam, sizeof(SMART_READ_DATA_OUTDATA),
		&dwOutBytes, NULL);
	//printf( "%d",GetLastError());	
	CloseHandle(hDevice);
	
	if (bResult == FALSE || dwOutBytes != sizeof(SMART_READ_DATA_OUTDATA))
	{
		return FALSE;
	}

	::CopyMemory(&(SMART_info.SmartReadData),&(SendCmdOutParam.SendCmdOutParam.bBuffer), 512);
	return TRUE;
}

BOOL Disk::GetPhysicalThresholdInfo(INT nDrive, BYTE target, ATA_SMART_INFO & SMART_info)
{
	HANDLE hDevice; // 设备句柄
	BOOL bResult = FALSE; // 返回结果
	TCHAR szFileName[20]; // 文件名 
	DWORD dwOutBytes; // IOCTL输出数据长度
	memset(szFileName, '0', 20);
	wsprintf(szFileName, _T("\\\\.\\PhysicalDrive%d"), nDrive);
	hDevice = OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	SMART_READ_DATA_OUTDATA SendCmdOutParam;
	SENDCMDINPARAMS sendCmd;

	::ZeroMemory(&SendCmdOutParam, sizeof(SMART_READ_DATA_OUTDATA));
	::ZeroMemory(&sendCmd, sizeof(SENDCMDINPARAMS));

	sendCmd.irDriveRegs.bFeaturesReg = READ_THRESHOLDS;
	sendCmd.irDriveRegs.bSectorCountReg = 1;
	sendCmd.irDriveRegs.bSectorNumberReg = 1;
	sendCmd.irDriveRegs.bCylLowReg = SMART_CYL_LOW;
	sendCmd.irDriveRegs.bCylHighReg = SMART_CYL_HI;
	sendCmd.irDriveRegs.bDriveHeadReg = target;
	sendCmd.irDriveRegs.bCommandReg = SMART_CMD;
	sendCmd.cBufferSize = READ_THRESHOLD_BUFFER_SIZE;

	bResult = ::DeviceIoControl(
		hDevice,
		DFP_RECEIVE_DRIVE_DATA,
		&sendCmd, sizeof(SENDCMDINPARAMS),
		&SendCmdOutParam, sizeof(SMART_READ_DATA_OUTDATA),
		&dwOutBytes, NULL);
//	printf("%d", GetLastError());
	CloseHandle(hDevice);

	if (bResult == FALSE || dwOutBytes != sizeof(SMART_READ_DATA_OUTDATA))
	{
		return FALSE;
	}

	::CopyMemory(&(SMART_info.SmartReadThreshold), &(SendCmdOutParam.SendCmdOutParam.bBuffer), 512);
	return TRUE;
}

BOOL Disk::GetScsiSMARTInfo(INT scsiPort, INT scsiTargetId, ATA_SMART_INFO & SMART_info)
{
	HANDLE hDevice; // 设备句柄
	BOOL bResult = FALSE; // 返回结果
	TCHAR szFileName[20]; // 文件名 
	memset(szFileName, '0', 20);
	wsprintf(szFileName, _T("\\\\.\\Scsi%d"), scsiPort);
	hDevice = OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	BYTE buffer[sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS) + READ_THRESHOLD_BUFFER_SIZE];
	SRB_IO_CONTROL *p = (SRB_IO_CONTROL *)buffer;
	SENDCMDINPARAMS *pin = (SENDCMDINPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
	DWORD dummy;
	memset(buffer, 0, sizeof(buffer));
	p->HeaderLength = sizeof(SRB_IO_CONTROL);
	p->Timeout = 2;
	p->Length = sizeof(SENDCMDOUTPARAMS) + READ_THRESHOLD_BUFFER_SIZE;
	p->ControlCode = IOCTL_SCSI_MINIPORT_READ_SMART_THRESHOLDS;
	memcpy((char *)p->Signature, "SCSIDISK", 8);
	pin->irDriveRegs.bFeaturesReg = READ_ATTRIBUTES;
	pin->irDriveRegs.bSectorCountReg = 1;
	pin->irDriveRegs.bSectorNumberReg = 1;
	pin->irDriveRegs.bCylLowReg = SMART_CYL_LOW;
	pin->irDriveRegs.bCylHighReg = SMART_CYL_HI;
	pin->irDriveRegs.bCommandReg = SMART_CMD;
	pin->cBufferSize = READ_ATTRIBUTE_BUFFER_SIZE;
	pin->bDriveNumber = scsiTargetId;

	if (DeviceIoControl(hDevice, IOCTL_SCSI_MINIPORT,
		buffer, sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1,
		buffer, sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS) + READ_THRESHOLD_BUFFER_SIZE,
		&dummy, NULL))
	{
		SENDCMDOUTPARAMS *pOut = (SENDCMDOUTPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
		if (*(pOut->bBuffer) > 0)
		{

			memcpy_s(&(SMART_info.SmartReadData), 512, &(pOut->bBuffer), 512);
			CloseHandle(hDevice);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Disk::GetScsiThresholdInfo(INT scsiPort, INT scsiTargetId, ATA_SMART_INFO & SMART_info)
{
	HANDLE hDevice; // 设备句柄
	BOOL bResult = FALSE; // 返回结果
	TCHAR szFileName[20]; // 文件名 
	memset(szFileName, '0', 20);
	wsprintf(szFileName, _T("\\\\.\\Scsi%d"), scsiPort);
	hDevice = OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	BYTE buffer[sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS) + READ_THRESHOLD_BUFFER_SIZE];
	SRB_IO_CONTROL *p = (SRB_IO_CONTROL *)buffer;
	SENDCMDINPARAMS *pin = (SENDCMDINPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
	DWORD dummy;
	memset(buffer, 0, sizeof(buffer));
	p->HeaderLength = sizeof(SRB_IO_CONTROL);
	p->Timeout = 2;
	p->Length = sizeof(SENDCMDOUTPARAMS) + READ_THRESHOLD_BUFFER_SIZE;
	p->ControlCode = IOCTL_SCSI_MINIPORT_READ_SMART_THRESHOLDS;
	memcpy((char *)p->Signature, "SCSIDISK", 8);
	pin->irDriveRegs.bFeaturesReg = READ_THRESHOLDS;
	pin->irDriveRegs.bSectorCountReg = 1;
	pin->irDriveRegs.bSectorNumberReg = 1;
	pin->irDriveRegs.bCylLowReg = SMART_CYL_LOW;
	pin->irDriveRegs.bCylHighReg = SMART_CYL_HI;
	pin->irDriveRegs.bCommandReg = SMART_CMD;
	pin->cBufferSize = READ_THRESHOLD_BUFFER_SIZE;
	pin->bDriveNumber = scsiTargetId;

	if (DeviceIoControl(hDevice, IOCTL_SCSI_MINIPORT,
		buffer, sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1,
		buffer, sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS) + READ_THRESHOLD_BUFFER_SIZE,
		&dummy, NULL))
	{
		SENDCMDOUTPARAMS *pOut = (SENDCMDOUTPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
		if (*(pOut->bBuffer) > 0)
		{

			memcpy_s(&(SMART_info.SmartReadThreshold), 512, &(pOut->bBuffer), 512);
			CloseHandle(hDevice);
			return TRUE;
		}
	}
	return FALSE;
}

VOID Disk::UpdateDate()
{
	SMARTinfo.clear();
	PATA_SMART_INFO Infotmp = NULL;
	CDiskQuery diskQuery(L"root\\CIMV2", L"SELECT * FROM Win32_SCSIController");
	diskQuery.ExcuteFun();
	Infotmp = new ATA_SMART_INFO[diskQuery.wmi_info.size()];
	for (USHORT i = 0; i < diskQuery.wmi_info.size(); ++i)
	{
		Infotmp[i].model = (CStringW)diskQuery.wmi_info[i].model;
		Infotmp[i].deviceid = (CStringW)diskQuery.wmi_info[i].deviceid;
		Infotmp[i].diskSize = (CStringW)diskQuery.wmi_info[i].diskSize;
		Infotmp[i].mediaType = (CStringW)diskQuery.wmi_info[i].mediaType;
		Infotmp[i].interfaceTypeWmi = (CStringW)diskQuery.wmi_info[i].interfaceTypeWmi;
		Infotmp[i].pnpDeviceId = (CStringW)diskQuery.wmi_info[i].pnpDeviceId;
		Infotmp[i].firmware = (CStringW)diskQuery.wmi_info[i].firmware;
		Infotmp[i].physicalDriveId = diskQuery.wmi_info[i].physicalDriveId;
		Infotmp[i].scsiBus = diskQuery.wmi_info[i].scsiBus;
		Infotmp[i].scsiPort = diskQuery.wmi_info[i].scsiPort;
		Infotmp[i].scsiTargetId = diskQuery.wmi_info[i].scsiTargetId;
		SMARTinfo.emplace_back(Infotmp[i]);
		GetPhysicalDriveInfoInNT(SMARTinfo[i].physicalDriveId, &(SMARTinfo[i].Identify));


		SMARTinfo[i].Major = GetAtaMajorVersion(SMARTinfo[i].Identify.wMajorVersion.MajorVersion, SMARTinfo[i].MajorVersion);
		GetAtaMinorVersion(SMARTinfo[i].Identify.wMinorVersion, SMARTinfo[i].MinorVersion);
		SMARTinfo[i].TransferModeType = GetTransferMode(
			SMARTinfo[i].Identify.wMultiWordDMA.MultiWordDMA,
			SMARTinfo[i].Identify.wReserved69[7],
			SMARTinfo[i].Identify.wReserved69[8],
			SMARTinfo[i].Identify.wUltraDMA.UltraDMA,
			SMARTinfo[i].CurrentTransferMode,
			SMARTinfo[i].MaxTransferMode,
			SMARTinfo[i].Interface);

		SMARTinfo[i].DetectedTimeUnitType = GetTimeUnitType(SMARTinfo[i].model, SMARTinfo[i].firmware, SMARTinfo[i].Major, SMARTinfo[i].TransferModeType);

		if (SMARTinfo[i].DetectedTimeUnitType == POWER_ON_MILLI_SECONDS)
		{
			SMARTinfo[i].MeasuredTimeUnitType = POWER_ON_MILLI_SECONDS;
		}
		else if (SMARTinfo[i].DetectedTimeUnitType == POWER_ON_10_MINUTES)
		{
			SMARTinfo[i].MeasuredTimeUnitType = POWER_ON_10_MINUTES;
		}

		CString firmwareRevInt(SMARTinfo[i].firmware.c_str());
		firmwareRevInt.Replace(_T("."), _T(""));
		if (SMARTinfo[i].model.find(_T("ADATA SSD")) == 0 && _ttoi(firmwareRevInt) == 346)
		{
			SMARTinfo[i].TemperatureMultiplier = 0.5;
		}

		GetPhysicalSMARTInfo(SMARTinfo[i].physicalDriveId, 0xA0, SMARTinfo[i]);
		GetPhysicalThresholdInfo(SMARTinfo[i].physicalDriveId, 0xA0, SMARTinfo[i]);
		FillSmartData(&SMARTinfo[i]);
		CheckSsdSupport(SMARTinfo[i]);
		GetSMARTIDINFO(&SMARTinfo[i]);
		FillSmartThreshold(&SMARTinfo[i]);
	}
	delete[] Infotmp;
	Infotmp = NULL;
}

/************************************************************************/
/*                          Excute	                                    */
/************************************************************************/

BOOL Disk::FillSmartData(ATA_SMART_INFO* asi)
{
	asi->AttributeCount = 0;
	int j = 0;
	for (int i = 0; i < MAX_ATTRIBUTE; i++)
	{
		DWORD rawValue = 0;
		memcpy(&(asi->Attribute[j]),
			&(asi->SmartReadData[i * sizeof(SMART_ATTRIBUTE) + 2]), sizeof(SMART_ATTRIBUTE));
		if (asi->Attribute[j].Id != 0)
			++j;
	}
	asi->AttributeCount = j;

	if (asi->AttributeCount > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID Disk::GetSMARTIDINFO(ATA_SMART_INFO* asi)
{
	int j = 0;
	for (UINT i = 0; i <= asi->AttributeCount; i++)
	{
		DWORD rawValue = 0;
// 		memcpy(&(asi->Attribute[j]),
// 			&(asi->SmartReadData[i * sizeof(SMART_ATTRIBUTE) + 2]), sizeof(SMART_ATTRIBUTE));
		if (asi->Attribute[j].Id != 0)
		{
			switch (asi->Attribute[j].Id)
			{
			case 0x09: // Power on Hours
				rawValue = MAKELONG(
					MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
					MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
				);
				if (asi->DiskVendorId == SSD_VENDOR_INDILINX)
				{
					rawValue = asi->Attribute[j].WorstValue * 256 + asi->Attribute[j].CurrentValue;
				}
				// Intel SSD 520 Series and etc...
				else if (
					(asi->DetectedTimeUnitType == POWER_ON_MILLI_SECONDS)
					|| (asi->DetectedTimeUnitType == POWER_ON_HOURS && rawValue >= 0x0DA000)
					|| (asi->model.find(_T("Intel")) == 0 && rawValue >= 0x0DA000)
					)
				{
					asi->MeasuredTimeUnitType = POWER_ON_MILLI_SECONDS;
					int value = 0;
					rawValue = value = asi->Attribute[j].RawValue[2] * 256 * 256
						+ asi->Attribute[j].RawValue[1] * 256
						+ asi->Attribute[j].RawValue[0] - 0x0DA753; // https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=560;id=diskinfo#560
					if (value < 0)
					{
						rawValue = 0;
					}
				}

				asi->PowerOnRawValue = rawValue;
				asi->DetectedPowerOnHours = GetPowerOnHours(rawValue, asi->DetectedTimeUnitType);
				asi->MeasuredPowerOnHours = GetPowerOnHours(rawValue, asi->MeasuredTimeUnitType);
				break;
			case 0x0C: // Power On Count
				rawValue = MAKELONG(
					MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
					MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
				);
				if (asi->DiskVendorId == SSD_VENDOR_INDILINX)
				{
					rawValue = asi->Attribute[j].WorstValue * 256 + asi->Attribute[j].CurrentValue;
				}
				asi->PowerOnCount = rawValue;
				break;
			case 0xBE:
				if (asi->Attribute[j].RawValue[0] > 0 && asi->Attribute[j].RawValue[0] < 100)
				{
					asi->Temperature = asi->Attribute[j].RawValue[0];
				}
				break;
			case 0xC2: // Temperature
				if (asi->model.find(_T("SAMSUNG SV")) == 0 && (asi->Attribute[j].RawValue[1] != 0 || asi->Attribute[j].RawValue[0] > 70))
				{
					asi->Temperature = MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]) / 10;
				}
				else if (asi->Attribute[j].RawValue[0] > 0 && asi->TemperatureMultiplier < 1.0)//(asi->DiskVendorId == SSD_VENDOR_SANDFORCE)
				{
					asi->Temperature = (DWORD)(asi->Attribute[j].RawValue[0] * asi->TemperatureMultiplier);
				}
				else if (asi->Attribute[j].RawValue[0] > 0)
				{
					asi->Temperature = asi->Attribute[j].RawValue[0];
				}

				if (asi->Temperature >= 100)
				{
					asi->Temperature = -1000;
				}
				break;
			case 0xBB:
				if (asi->DiskVendorId == SSD_VENDOR_MTRON)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xCA:
				if (asi->DiskVendorId == SSD_VENDOR_MICRON)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xD1:
				if (asi->DiskVendorId == SSD_VENDOR_INDILINX)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xF7:
				if (asi->DiskVendorId == SSD_VENDOR_KINGSTON)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xE8:
				if (asi->DiskVendorId == SSD_VENDOR_INTEL || asi->DiskVendorId == SSD_VENDOR_PLEXTOR || asi->DiskVendorId == SSD_VENDOR_SANDISK)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				else if (asi->DiskVendorId == SSD_VENDOR_OCZ)
				{
					asi->HostWrites = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				break;
			case 0xE9:
				if (asi->DiskVendorId == SSD_VENDOR_OCZ || asi->DiskVendorId == SSD_VENDOR_OCZ_VECTOR)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SANDISK && asi->HostReadsWritesUnit == HOST_READS_WRITES_GB)
				{
					asi->NandWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_PLEXTOR || asi->DiskVendorId == SSD_VENDOR_KINGSTON)
				{
					asi->NandWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				break;
			case 0xE1:
				if (asi->DiskVendorId == SSD_VENDOR_INTEL)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					) * 0.03125); //  0.03125 = 65536 * 512 / 1024 / 1024 / 1024;
				}
				break;
			case 0xF1:
				if (asi->DiskVendorId == HDD_SSD_VENDOR_TOSHIBA && asi->HostReadsWritesUnit == HOST_READS_WRITES_GB)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_INTEL || asi->DiskVendorId == HDD_SSD_VENDOR_TOSHIBA)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					) * 0.03125); //  0.03125 = 65536 * 512 / 1024 / 1024 / 1024;
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SANDFORCE || asi->DiskVendorId == SSD_VENDOR_OCZ_VECTOR || asi->DiskVendorId == SSD_VENDOR_CORSAIR || asi->DiskVendorId == SSD_VENDOR_KINGSTON)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SAMSUNG && asi->HostReadsWritesUnit == HOST_READS_WRITES_GB)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SAMSUNG)
				{
					asi->HostWrites = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				else if (asi->DiskVendorId == HDD_VENDOR_WESTERN_DIGITAL)
				{
					asi->HostWrites = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				else if (asi->DiskVendorId == SSD_VENDOR_PLEXTOR || asi->DiskVendorId == SSD_VENDOR_MICRON_MU02)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					)) * 32 / 1024;
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SANDISK && asi->HostReadsWritesUnit == HOST_READS_WRITES_GB)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SANDISK)
				{
					asi->HostWrites = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				/*
				else if(asi->DiskVendorId == HDD_SSD_VENDOR_SEAGATE)
				{
				asi->HostWrites  = (INT)(
				(UINT64)
				( (UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[1] * 256
				+ (UINT64)asi->Attribute[j].RawValue[0])
				* 512 / 1024 / 1024 / 1024);
				}
				*/
				break;
			case 0xF2:
				if (asi->DiskVendorId == HDD_SSD_VENDOR_TOSHIBA && asi->HostReadsWritesUnit == HOST_READS_WRITES_GB)
				{
					asi->HostReads = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_INTEL || asi->DiskVendorId == HDD_SSD_VENDOR_TOSHIBA)
				{
					asi->HostReads = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					) * 0.03125); //  0.03125 = 65536 * 512 / 1024 / 1024 / 1024;
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SANDFORCE || asi->DiskVendorId == SSD_VENDOR_OCZ_VECTOR || asi->DiskVendorId == SSD_VENDOR_CORSAIR || asi->DiskVendorId == SSD_VENDOR_KINGSTON)
				{
					asi->HostReads = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SAMSUNG && asi->HostReadsWritesUnit == HOST_READS_WRITES_GB)
				{
					asi->HostReads = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SAMSUNG)
				{
					asi->HostReads = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				else if (asi->DiskVendorId == HDD_VENDOR_WESTERN_DIGITAL)
				{
					asi->HostReads = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				else if (asi->DiskVendorId == SSD_VENDOR_PLEXTOR || asi->DiskVendorId == SSD_VENDOR_MICRON_MU02)
				{
					asi->HostReads = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					)) * 32 / 1024;
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SANDISK && asi->HostReadsWritesUnit == HOST_READS_WRITES_GB)
				{
					asi->HostReads = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_SANDISK)
				{
					asi->HostReads = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				else if (asi->DiskVendorId == SSD_VENDOR_MICRON)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				/*
				else if(asi->DiskVendorId == HDD_SSD_VENDOR_SEAGATE)
				{
				asi->HostReads  = (INT)(
				(UINT64)
				( (UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
				+ (UINT64)asi->Attribute[j].RawValue[1] * 256
				+ (UINT64)asi->Attribute[j].RawValue[0])
				* 512 / 1024 / 1024 / 1024);
				}
				*/
				break;
			case 0xF9:
				if (asi->DiskVendorId == SSD_VENDOR_INTEL)
				{
					asi->NandWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				else if (asi->DiskVendorId == SSD_VENDOR_OCZ_VECTOR)
				{
					asi->NandWrites = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 16 / 1024 / 1024);
				}
				break;
			case 0x64:
				if (asi->DiskVendorId == SSD_VENDOR_SANDFORCE)
				{
					asi->GBytesErased = MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					);
				}
				break;
			case 0xAD:
				if (asi->DiskVendorId == SSD_VENDOR_MICRON)
				{
					asi->WearLevelingCount = (INT)MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					);
				}
				break;
			case 0xB1:
				if (asi->DiskVendorId == SSD_VENDOR_SAMSUNG)
				{
					asi->WearLevelingCount = (INT)MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					);
				}
				/*
				else if(asi->DiskVendorId == SSD_VENDOR_PLEXTOR)
				{
				asi->NandWrites  = (INT)(MAKELONG(
				MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
				MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
				)) * asi->PlextorNandWritesUnit / 1024;
				}
				*/
				break;
			case 0xB3:
			case 0xB4:
				if (asi->DiskVendorId == SSD_VENDOR_SAMSUNG)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xE7:
				if (asi->DiskVendorId == SSD_VENDOR_SANDFORCE || asi->DiskVendorId == SSD_VENDOR_CORSAIR || asi->DiskVendorId == SSD_VENDOR_KINGSTON)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xAA:
				if (asi->DiskVendorId == SSD_VENDOR_JMICRON)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xA9:
				if (asi->DiskVendorId == SSD_VENDOR_MICRON_MU02)
				{
					if (asi->Attribute[j].CurrentValue <= 100)
					{
						asi->Life = asi->Attribute[j].CurrentValue;
					}
				}
				break;
			case 0xC6:
				if (asi->DiskVendorId == SSD_VENDOR_OCZ_VECTOR)
				{
					asi->HostReads = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				break;
			case 0xC7:
				if (asi->DiskVendorId == SSD_VENDOR_OCZ_VECTOR)
				{
					asi->HostWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					));
				}
				break;
			case 0xF5:
				// NAND Page Size = 8KBytes
				// http://www.overclock.net/t/1145150/official-crucial-ssd-owners-club/1290
				if (asi->DiskVendorId == SSD_VENDOR_MICRON)
				{
					asi->NandWrites = (INT)((UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 8 / 1024 / 1024);
				}
				else if (asi->DiskVendorId == SSD_VENDOR_MICRON_MU02)
				{
					asi->NandWrites = (INT)(MAKELONG(
						MAKEWORD(asi->Attribute[j].RawValue[0], asi->Attribute[j].RawValue[1]),
						MAKEWORD(asi->Attribute[j].RawValue[2], asi->Attribute[j].RawValue[3])
					)) * 32 / 1024;
				}
				break;
			case 0xF6:
				if (asi->DiskVendorId == SSD_VENDOR_MICRON)
				{
					asi->HostWrites = (INT)(
						(UINT64)
						((UINT64)asi->Attribute[j].RawValue[5] * 256 * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[4] * 256 * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[3] * 256 * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[2] * 256 * 256
							+ (UINT64)asi->Attribute[j].RawValue[1] * 256
							+ (UINT64)asi->Attribute[j].RawValue[0])
						* 512 / 1024 / 1024 / 1024);
				}
				break;
			default:
				break;
			}
			++j;
		}
	}
}


BOOL Disk::FillSmartThreshold(ATA_SMART_INFO* asi)
{
	// 2016/04/18
	// https://github.com/hiyohiyo/CrystalDiskInfo/issues/1
	int count = 0;
	for (int i = 0; i < MAX_ATTRIBUTE; i++)
	{
		SMART_THRESHOLD* pst = (SMART_THRESHOLD*)&(asi->SmartReadThreshold[i * sizeof(SMART_THRESHOLD) + 2]);
		if (pst->Id != 0)
		{
			for (DWORD j = 0; j < asi->AttributeCount; j++)
			{
				if (pst->Id == asi->Attribute[j].Id)
				{
					memcpy(&(asi->Threshold[j]), pst, sizeof(SMART_THRESHOLD));
					count++;
				}
			}
		}
	}
	if (count > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD Disk::GetAtaMajorVersion(WORD w80, wstring &majorVersion)
{
	DWORD major = 0;

	if (w80 == 0x0000 || w80 == 0xFFFF)
	{
		return 0;
	}

	for (int i = 14; i > 0; i--)
	{
		if ((w80 >> i) & 0x1)
		{
			major = i;
			break;
		}
	}

	if (major > 10)
	{
		majorVersion = _T("");
	}
	else if (major == 11)
	{
		majorVersion = _T("ACS-4");
	}
	else if (major == 10)
	{
		majorVersion = _T("ACS-3");
	}
	else if (major == 9)
	{
		majorVersion = _T("ACS-2");
	}
	else if (major == 8)
	{
		majorVersion = _T("ATA8-ACS");
	}
	else if (major >= 4)
	{
		TCHAR* tmp = new TCHAR[1024];
		wsprintf(tmp, _T("ATA/ATAPI-%d"), w80);
		majorVersion = tmp;
		if (tmp)
		{
			delete[] tmp;
			tmp = NULL;
		}
	}
	else if (major == 0)
	{
		majorVersion = _T("----");
	}
	else
	{
		TCHAR* tmp = new TCHAR[1024];
		wsprintf(tmp, _T("ATA-%d"), w80);
		majorVersion = tmp;
		if (tmp)
		{
			delete[] tmp;
			tmp = NULL;
		}
	}

	return major;
}

// Last Update : 2011/03/21
// Reference : http://www.t13.org/Documents/MinutesDefault.aspx?DocumentType=4&DocumentStage=2
//           - d2161r0-ATAATAPI_Command_Set_-_3.pdf
//           - d1153r18-ATA-ATAPI-4.pdf 
VOID Disk::GetAtaMinorVersion(WORD w81, wstring &minor)
{
	switch (w81)
	{
	case 0x0000:
	case 0xFFFF:
		minor = _T("Not Reported");									break;
		//minor = _T("----");											break;
	case 0x0001:	minor = _T("ATA (ATA-1) X3T9.2 781D prior to revision 4");	break;
	case 0x0002:	minor = _T("ATA-1 published, ANSI X3.221-1994");			break;
	case 0x0003:	minor = _T("ATA (ATA-1) X3T10 781D revision 4");			break;
	case 0x0004:	minor = _T("ATA-2 published, ANSI X3.279-1996");			break;
	case 0x0005:	minor = _T("ATA-2 X3T10 948D prior to revision 2k");		break;
	case 0x0006:	minor = _T("ATA-3 X3T10 2008D revision 1");					break;
	case 0x0007:	minor = _T("ATA-2 X3T10 948D revision 2k");					break;
	case 0x0008:	minor = _T("ATA-3 X3T10 2008D revision 0");					break;
	case 0x0009:	minor = _T("ATA-2 X3T10 948D revision 3");					break;
	case 0x000A:	minor = _T("ATA-3 published, ANSI X3.298-199x");			break;
	case 0x000B:	minor = _T("ATA-3 X3T10 2008D revision 6");					break;
	case 0x000C:	minor = _T("ATA-3 X3T13 2008D revision 7 and 7a");			break;
	case 0x000D:	minor = _T("ATA/ATAPI-4 X3T13 1153D version 6");			break;
	case 0x000E:	minor = _T("ATA/ATAPI-4 T13 1153D version 13");				break;
	case 0x000F:	minor = _T("ATA/ATAPI-4 X3T13 1153D version 7");			break;
	case 0x0010:	minor = _T("ATA/ATAPI-4 T13 1153D version 18");				break;
	case 0x0011:	minor = _T("ATA/ATAPI-4 T13 1153D version 15");				break;
	case 0x0012:	minor = _T("ATA/ATAPI-4 published, ANSI INCITS 317-1998");	break;
	case 0x0013:	minor = _T("ATA/ATAPI-5 T13 1321D version 3");				break;
	case 0x0014:	minor = _T("ATA/ATAPI-4 T13 1153D version 14");				break;
	case 0x0015:	minor = _T("ATA/ATAPI-5 T13 1321D version 1");				break;
	case 0x0016:	minor = _T("ATA/ATAPI-5 published, ANSI INCITS 340-2000");	break;
	case 0x0017:	minor = _T("ATA/ATAPI-4 T13 1153D version 17");				break;
	case 0x0018:	minor = _T("ATA/ATAPI-6 T13 1410D version 0");				break;
	case 0x0019:	minor = _T("ATA/ATAPI-6 T13 1410D version 3a");				break;
	case 0x001A:	minor = _T("ATA/ATAPI-7 T13 1532D version 1");				break;
	case 0x001B:	minor = _T("ATA/ATAPI-6 T13 1410D version 2");				break;
	case 0x001C:	minor = _T("ATA/ATAPI-6 T13 1410D version 1");				break;
	case 0x001D:	minor = _T("ATA/ATAPI-7 published ANSI INCITS 397-2005.");	break;
	case 0x001E:	minor = _T("ATA/ATAPI-7 T13 1532D version 0");				break;
	case 0x001F:	minor = _T("ACS-3 Revision 3b");							break;
	case 0x0021:	minor = _T("ATA/ATAPI-7 T13 1532D version 4a");				break;
	case 0x0022:	minor = _T("ATA/ATAPI-6 published, ANSI INCITS 361-2002");	break;
	case 0x0027:	minor = _T("ATA8-ACS version 3c");							break;
	case 0x0028:	minor = _T("ATA8-ACS version 6");							break;
	case 0x0029:	minor = _T("ATA8-ACS version 4");							break;
	case 0x0031:	minor = _T("ACS-2 Revision 2");								break;
	case 0x0033:	minor = _T("ATA8-ACS version 3e");							break;
	case 0x0039:	minor = _T("ATA8-ACS version 4c");							break;
	case 0x0042:	minor = _T("ATA8-ACS version 3f");							break;
	case 0x0052:	minor = _T("ATA8-ACS version 3b");							break;
	case 0x005E:	minor = _T("ACS-4 Revision 5");								break;
	case 0x006D:	minor = _T("ACS-3 Revision 4");								break;
	case 0x0082:	minor = _T("ACS-2 published, ANSI INCITS 482-2012");		break;
	case 0x0107:	minor = _T("ATA8-ACS version 2d");							break;
	case 0x010A:	minor = _T("ACS-3 published, ANSI INCITS 522-2014");		break;
	case 0x0110:	minor = _T("ACS-2 Revision 3");								break;
	case 0x011B:	minor = _T("ACS-3 Revision 4");								break;
	default:	//	minor.Format(_T("Reserved [%04Xh]"), w81);					break;
		TCHAR* tmp = new TCHAR[1024];
		wsprintf(tmp, _T("ATA-%d"), w81);
		minor = tmp;
		if (tmp)
		{
			delete[] tmp;
			tmp = NULL;
		}
		break;
	}
}

DWORD Disk::GetPowerOnHours(DWORD rawValue, DWORD timeUnitType)
{
	switch (timeUnitType)
	{
	case POWER_ON_UNKNOWN:
		return 0;
		break;
	case POWER_ON_HOURS:
		return rawValue;
		break;
	case POWER_ON_MINUTES:
		return rawValue / 60;
		break;
	case POWER_ON_HALF_MINUTES:
		return rawValue / 120;
		break;
	case POWER_ON_SECONDS:
		return rawValue / 60 / 60;
		break;
	case POWER_ON_10_MINUTES:
		return rawValue / 6;
		break;
	case POWER_ON_MILLI_SECONDS:
		return rawValue;
		break;
	default:
		return rawValue;
		break;
	}
}

DWORD Disk::GetTransferMode(WORD w63, WORD w76, WORD w77, WORD w88, wstring &current, wstring &max, wstring &type)
{
	DWORD tm = TRANSFER_MODE_PIO;
	current = max = _T("");
	type = _T("Parallel ATA");

	// Multiword DMA or PIO
	if (w63 & 0x0700)
	{
		tm = TRANSFER_MODE_PIO_DMA;
		current = max = _T("PIO/DMA");
	}

	// Ultara DMA Max Transfer Mode
	if (w88 & 0x0040) { tm = TRANSFER_MODE_ULTRA_DMA_133; max = _T("UDMA/133"); }
	else if (w88 & 0x0020) { tm = TRANSFER_MODE_ULTRA_DMA_100; max = _T("UDMA/100"); }
	else if (w88 & 0x0010) { tm = TRANSFER_MODE_ULTRA_DMA_66;  max = _T("UDMA/66"); }
	else if (w88 & 0x0008) { tm = TRANSFER_MODE_ULTRA_DMA_44;  max = _T("UDMA/44"); }
	else if (w88 & 0x0004) { tm = TRANSFER_MODE_ULTRA_DMA_33;  max = _T("UDMA/33"); }
	else if (w88 & 0x0002) { tm = TRANSFER_MODE_ULTRA_DMA_25;  max = _T("UDMA/25"); }
	else if (w88 & 0x0001) { tm = TRANSFER_MODE_ULTRA_DMA_16;  max = _T("UDMA/16"); }

	// Ultara DMA Current Transfer Mode
	if (w88 & 0x4000) { current = _T("UDMA/133"); }
	else if (w88 & 0x2000) { current = _T("UDMA/100"); }
	else if (w88 & 0x1000) { current = _T("UDMA/66"); }
	else if (w88 & 0x0800) { current = _T("UDMA/44"); }
	else if (w88 & 0x0400) { current = _T("UDMA/33"); }
	else if (w88 & 0x0200) { current = _T("UDMA/25"); }
	else if (w88 & 0x0100) { current = _T("UDMA/16"); }

	// Serial ATA
	if (w76 != 0x0000 && w76 != 0xFFFF)
	{
		current = max = _T("SATA/150");
		type = _T("Serial ATA");
	}

	if (w76 & 0x0010) { tm = TRANSFER_MODE_UNKNOWN; current = max = _T("----"); }
	else if (w76 & 0x0008) { tm = TRANSFER_MODE_SATA_600; current = _T("----"); max = _T("SATA/600"); }
	else if (w76 & 0x0004) { tm = TRANSFER_MODE_SATA_300; current = _T("----"); max = _T("SATA/300"); }
	else if (w76 & 0x0002) { tm = TRANSFER_MODE_SATA_150; current = _T("----"); max = _T("SATA/150"); }

	// 2013/5/1 ACS-3
	if (((w77 & 0x000E) >> 1) == 3) { current = _T("SATA/600"); }
	else if (((w77 & 0x000E) >> 1) == 2) { current = _T("SATA/300"); }
	else if (((w77 & 0x000E) >> 1) == 1) { current = _T("SATA/150"); }

	return tm;
}

VOID Disk::CheckSsdSupport(ATA_SMART_INFO &asi)
{
	// Old SSD Detection
	if (IsSsdOld(asi))
	{
		asi.IsSsd = TRUE;
	}

	if (asi.model.find(_T("STT")) != 0 && asi.model.find(_T("ST")) == 0)
	{
		if (asi.IsSsd)
		{
			asi.SmartKeyName = _T("SmartSsd");
			asi.DiskVendorId = HDD_SSD_VENDOR_SEAGATE;
		}
		else
		{
			asi.SmartKeyName = _T("Smart");
			asi.DiskVendorId = HDD_SSD_VENDOR_SEAGATE;
		}
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
	}
	else if (asi.model.find(_T("WDC ")) == 0 && !asi.IsSsd)
	{
		asi.SmartKeyName = _T("Smart");
		asi.DiskVendorId = HDD_VENDOR_WESTERN_DIGITAL;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
	}
	else if (IsSsdMtron(asi))
	{
		asi.SmartKeyName = _T("SmartMtron");
		asi.DiskVendorId = SSD_VENDOR_MTRON;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdJMicron60x(asi))
	{
		asi.SmartKeyName = _T("SmartJMicron60x");
		asi.DiskVendorId = SSD_VENDOR_JMICRON;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
		asi.IsRawValues8 = TRUE;
	}
	else if (IsSsdJMicron61x(asi))
	{
		asi.SmartKeyName = _T("SmartJMicron61x");
		asi.DiskVendorId = SSD_VENDOR_JMICRON;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdIndlinx(asi))
	{
		asi.SmartKeyName = _T("SmartIndlinx");
		asi.DiskVendorId = SSD_VENDOR_INDILINX;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
		asi.IsRawValues8 = TRUE;
	}
	else if (IsSsdIntel(asi))
	{
		asi.SmartKeyName = _T("SmartIntel");
		asi.DiskVendorId = SSD_VENDOR_INTEL;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdSamsung(asi))
	{
		asi.SmartKeyName = _T("SmartSamsung");
		asi.DiskVendorId = SSD_VENDOR_SAMSUNG;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdMicronMU02(asi))
	{
		asi.SmartKeyName = _T("SmartMicronMU02");
		asi.DiskVendorId = SSD_VENDOR_MICRON_MU02;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdMicron(asi))
	{
		asi.SmartKeyName = _T("SmartMicron");
		asi.DiskVendorId = SSD_VENDOR_MICRON;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdSandForce(asi))
	{
		asi.SmartKeyName = _T("SmartSandForce");
		asi.DiskVendorId = SSD_VENDOR_SANDFORCE;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
		asi.IsRawValues7 = TRUE;
	}
	else if (IsSsdOcz(asi))
	{
		asi.SmartKeyName = _T("SmartOcz");
		asi.DiskVendorId = SSD_VENDOR_OCZ;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdOczVector(asi))
	{
		asi.SmartKeyName = _T("SmartOczVector");
		asi.DiskVendorId = SSD_VENDOR_OCZ_VECTOR;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}

	else if (IsSsdPlextor(asi))
	{
		asi.SmartKeyName = _T("SmartPlextor");
		asi.DiskVendorId = SSD_VENDOR_PLEXTOR;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;

		//	asi.PlextorNandWritesUnit = CheckPlextorNandWritesUnit(asi);
	}
	else if (IsSsdSanDisk(asi))
	{
		asi.DiskVendorId = SSD_VENDOR_SANDISK;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
		asi.IsSsd = TRUE;
	}
	else if (IsSsdKingston(asi))
	{
		asi.SmartKeyName = _T("SmartKingston");
		asi.DiskVendorId = SSD_VENDOR_KINGSTON;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
	}
	else if (asi.model.find(_T("TOSHIBA")) == 0 && asi.IsSsd)
	{
		asi.SmartKeyName = _T("SmartToshiba");
		asi.DiskVendorId = HDD_SSD_VENDOR_TOSHIBA;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
	}
	else if (asi.model.find(_T("Corsair")) == 0)
	{
		asi.SmartKeyName = _T("SmartCorsair");
		asi.DiskVendorId = SSD_VENDOR_CORSAIR;
		asi.SsdVendorString = ssdVendorString[asi.DiskVendorId];
	}
}

BOOL Disk::IsSsdOld(ATA_SMART_INFO &asi)
{
	return asi.model.find(_T("OCZ")) == 0
		|| asi.model.find(_T("SPCC")) == 0
		|| asi.model.find(_T("PATRIOT")) == 0
		|| asi.model.find(_T("Solid")) != -1
		|| asi.model.find(_T("SSD")) != -1
		|| asi.model.find(_T("SiliconHardDisk")) != -1
		|| asi.model.find(_T("PHOTOFAST")) == 0
		|| asi.model.find(_T("STT_FTM")) == 0
		|| asi.model.find(_T("Super Talent")) == 0
		;
}

BOOL Disk::IsSsdMtron(ATA_SMART_INFO &asi)
{
	return ((asi.Attribute[0].Id == 0xBB && asi.AttributeCount == 1) || asi.model.find(_T("MTRON")) == 0);
}

BOOL Disk::IsSsdJMicron60x(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	if (asi.Attribute[0].Id == 0x0C
		&& asi.Attribute[1].Id == 0x09
		&& asi.Attribute[2].Id == 0xC2
		&& asi.Attribute[3].Id == 0xE5
		&& asi.Attribute[4].Id == 0xE8
		&& asi.Attribute[5].Id == 0xE9
		//	&& asi.Attribute[ 6].Id == 0xEA
		//	&& asi.Attribute[ 7].Id == 0xEB
		)
	{
		flagSmartType = TRUE;
	}

	return flagSmartType;
}

BOOL Disk::IsSsdJMicron61x(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x02
		&& asi.Attribute[2].Id == 0x03
		&& asi.Attribute[3].Id == 0x05
		&& asi.Attribute[4].Id == 0x07
		&& asi.Attribute[5].Id == 0x08
		&& asi.Attribute[6].Id == 0x09
		&& asi.Attribute[7].Id == 0x0A
		&& asi.Attribute[8].Id == 0x0C
		&& asi.Attribute[9].Id == 0xA8
		&& asi.Attribute[10].Id == 0xAF
		&& asi.Attribute[11].Id == 0xC0
		&& asi.Attribute[12].Id == 0xC2
		//	&& asi.Attribute[13].Id == 0xF0
		//	&& asi.Attribute[14].Id == 0xAA
		//	&& asi.Attribute[15].Id == 0xAD
		)
	{
		flagSmartType = TRUE;
	}

	return flagSmartType;
}

BOOL Disk::IsSsdIndlinx(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x09
		&& asi.Attribute[2].Id == 0x0C
		&& asi.Attribute[3].Id == 0xB8
		&& asi.Attribute[4].Id == 0xC3
		&& asi.Attribute[5].Id == 0xC4
		//	&& asi.Attribute[ 6].Id == 0xC5
		//	&& asi.Attribute[ 7].Id == 0xC6
		//	&& asi.Attribute[ 8].Id == 0xC7
		//	&& asi.Attribute[ 9].Id == 0xC8
		//	&& asi.Attribute[10].Id == 0xC9
		//	&& asi.Attribute[11].Id == 0xCA
		//	&& asi.Attribute[12].Id == 0xCB
		//	&& asi.Attribute[13].Id == 0xCC
		//	&& asi.Attribute[14].Id == 0xCD
		//	&& asi.Attribute[15].Id == 0xCE
		//	&& asi.Attribute[16].Id == 0xCF
		//	&& asi.Attribute[17].Id == 0xD0
		//	&& asi.Attribute[18].Id == 0xD1
		//	&& asi.Attribute[19].Id == 0xD2
		//	&& asi.Attribute[20].Id == 0xD3
		)
	{
		flagSmartType = TRUE;
	}

	/*
	asi.model.find(_T("OCZ-VERTEX")) == 0
	|| asi.model.find(_T("G-Monster-V3")) == 0
	|| asi.model.find(_T("G-Monster-V5")) == 0
	|| (asi.model.find(_T("STT_FTM")) == 0 && asi.model.find(_T("GX")) > 0)
	|| asi.model.find(_T("Solidata")) == 0
	*/

	return flagSmartType;

}

BOOL Disk::IsSsdIntel(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	if (asi.Attribute[0].Id == 0x03
		&& asi.Attribute[1].Id == 0x04
		&& asi.Attribute[2].Id == 0x05
		&& asi.Attribute[3].Id == 0x09
		&& asi.Attribute[4].Id == 0x0C
		)
	{
		if (asi.Attribute[5].Id == 0xC0 && asi.Attribute[6].Id == 0xE8 && asi.Attribute[7].Id == 0xE9)
		{
			flagSmartType = TRUE;
		}
		else if (asi.Attribute[5].Id == 0xC0 && asi.Attribute[6].Id == 0xE1)
		{
			flagSmartType = TRUE;
		}
		else if (asi.Attribute[5].Id == 0xAA && asi.Attribute[6].Id == 0xAB && asi.Attribute[7].Id == 0xAC)
		{
			flagSmartType = TRUE;
		}
	}

	return (asi.model.find(_T("INTEL")) != -1 || flagSmartType == TRUE);
}


// http://www.samsung.com/us/business/oem-solutions/pdfs/General_NSSD_25_SATA_III_Spec_0.2.pdf
BOOL Disk::IsSsdSamsung(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	// SM951
	if (asi.Attribute[0].Id == 0x05
		&& asi.Attribute[1].Id == 0x09
		&& asi.Attribute[2].Id == 0x0C
		&& asi.Attribute[3].Id == 0xAA
		&& asi.Attribute[4].Id == 0xAB
		&& asi.Attribute[5].Id == 0xAC
		&& asi.Attribute[6].Id == 0xAD
		&& asi.Attribute[7].Id == 0xAE
		&& asi.Attribute[8].Id == 0xB2
		&& asi.Attribute[9].Id == 0xB4
		)
	{
		flagSmartType = TRUE;
		asi.HostReadsWritesUnit = HOST_READS_WRITES_GB;
	}
	else if (asi.Attribute[0].Id == 0x09
		&& asi.Attribute[1].Id == 0x0C
		&& asi.Attribute[2].Id == 0xB2
		&& asi.Attribute[3].Id == 0xB3
		&& asi.Attribute[4].Id == 0xB4
		)
	{
		flagSmartType = TRUE;
	}
	else
		if (asi.Attribute[0].Id == 0x09
			&& asi.Attribute[1].Id == 0x0C
			&& asi.Attribute[2].Id == 0xB1
			&& asi.Attribute[3].Id == 0xB2
			&& asi.Attribute[4].Id == 0xB3
			&& asi.Attribute[5].Id == 0xB4
			&& asi.Attribute[6].Id == 0xB7
			)
		{
			flagSmartType = TRUE;
		}
		else
			if (asi.Attribute[0].Id == 0x09
				&& asi.Attribute[1].Id == 0x0C
				&& asi.Attribute[2].Id == 0xAF
				&& asi.Attribute[3].Id == 0xB0
				&& asi.Attribute[4].Id == 0xB1
				&& asi.Attribute[5].Id == 0xB2
				&& asi.Attribute[6].Id == 0xB3
				&& asi.Attribute[7].Id == 0xB4
				)
			{
				flagSmartType = TRUE;
			}
			else
				if (asi.Attribute[0].Id == 0x05
					&& asi.Attribute[1].Id == 0x09
					&& asi.Attribute[2].Id == 0x0C
					&& asi.Attribute[3].Id == 0xB1
					&& asi.Attribute[4].Id == 0xB3
					&& asi.Attribute[5].Id == 0xB5
					&& asi.Attribute[6].Id == 0xB6
					)
				{
					flagSmartType = TRUE;
				}

	return ((asi.model.find(_T("SAMSUNG")) != -1 && asi.IsSsd) || (asi.model.find(_T("MZ-")) != -1 && asi.IsSsd) || flagSmartType == TRUE);
}

BOOL Disk::IsSsdSandForce(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x05
		&& asi.Attribute[2].Id == 0x09
		&& asi.Attribute[3].Id == 0x0C
		&& asi.Attribute[4].Id == 0x0D
		&& asi.Attribute[5].Id == 0x64
		&& asi.Attribute[6].Id == 0xAA
		)
	{
		flagSmartType = TRUE;
	}

	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x05
		&& asi.Attribute[2].Id == 0x09
		&& asi.Attribute[3].Id == 0x0C
		&& asi.Attribute[4].Id == 0xAB
		&& asi.Attribute[5].Id == 0xAC
		)
	{
		flagSmartType = TRUE;
	}

	// TOSHIBA + SandForce
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=1116;id=diskinfo#1116
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=1136;id=diskinfo#1136
	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x02
		&& asi.Attribute[2].Id == 0x03
		&& asi.Attribute[3].Id == 0x05
		&& asi.Attribute[4].Id == 0x07
		&& asi.Attribute[5].Id == 0x08
		&& asi.Attribute[6].Id == 0x09
		&& asi.Attribute[7].Id == 0x0A
		&& asi.Attribute[8].Id == 0x0C
		&& asi.Attribute[9].Id == 0xA7
		&& asi.Attribute[10].Id == 0xA8
		&& asi.Attribute[11].Id == 0xA9
		&& asi.Attribute[12].Id == 0xAA
		&& asi.Attribute[13].Id == 0xAD
		&& asi.Attribute[14].Id == 0xAF
		&& asi.Attribute[15].Id == 0xB1
		)
	{
		flagSmartType = TRUE;
	}

	return (asi.model.find(_T("SandForce")) != -1 || flagSmartType);
}

// Micron Crucial
BOOL Disk::IsSsdMicronMU02(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;
	CString modelUpper = asi.model.c_str();
	modelUpper.MakeUpper();


	// Crucial BX100 MU02 2015/11/26
	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x05
		&& asi.Attribute[2].Id == 0x09
		&& asi.Attribute[3].Id == 0x0C
		&& asi.Attribute[4].Id == 0xA0
		&& asi.Attribute[5].Id == 0xA1
		&& asi.Attribute[6].Id == 0xA3
		&& asi.Attribute[7].Id == 0xA4
		&& asi.Attribute[8].Id == 0xA5
		&& asi.Attribute[9].Id == 0xA6
		&& asi.Attribute[10].Id == 0xA7
		)
	{
		flagSmartType = TRUE;
	}

	// Crucial BX200 MU01.4 2015/11/26
	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x05
		&& asi.Attribute[2].Id == 0x09
		&& asi.Attribute[3].Id == 0x0C
		&& asi.Attribute[4].Id == 0xA0
		&& asi.Attribute[5].Id == 0xA1
		&& asi.Attribute[6].Id == 0xA3
		&& asi.Attribute[7].Id == 0x94
		&& asi.Attribute[8].Id == 0x95
		&& asi.Attribute[9].Id == 0x96
		&& asi.Attribute[10].Id == 0x97
		)
	{
		flagSmartType = TRUE;
	}

	return flagSmartType;
}

// Micron RealSSD & Crucial
BOOL Disk::IsSsdMicron(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;
	CString modelUpper = asi.model.c_str();
	modelUpper.MakeUpper();

	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x05
		&& asi.Attribute[2].Id == 0x09
		&& asi.Attribute[3].Id == 0x0C
		&& asi.Attribute[4].Id == 0xAA
		&& asi.Attribute[5].Id == 0xAB
		&& asi.Attribute[6].Id == 0xAC
		&& asi.Attribute[7].Id == 0xAD
		&& asi.Attribute[8].Id == 0xAE
		&& asi.Attribute[9].Id == 0xB5
		&& asi.Attribute[10].Id == 0xB7
		)
	{
		flagSmartType = TRUE;
	}

	return modelUpper.Find(_T("P600")) == 0 || modelUpper.Find(_T("C600")) == 0
		|| modelUpper.Find(_T("M6-")) == 0 || modelUpper.Find(_T("M600")) == 0
		|| modelUpper.Find(_T("P500")) == 0 || modelUpper.Find(_T("C500")) == 0
		|| modelUpper.Find(_T("M5-")) == 0 || modelUpper.Find(_T("M500")) == 0
		|| modelUpper.Find(_T("P400")) == 0 || modelUpper.Find(_T("C400")) == 0
		|| modelUpper.Find(_T("M4-")) == 0 || modelUpper.Find(_T("M400")) == 0
		|| modelUpper.Find(_T("P300")) == 0 || modelUpper.Find(_T("C300")) == 0
		|| modelUpper.Find(_T("M3-")) == 0 || modelUpper.Find(_T("M300")) == 0
		|| modelUpper.Find(_T("CRUCIAL")) == 0 || modelUpper.Find(_T("MICRON")) == 0
		|| flagSmartType;
}

BOOL Disk::IsSsdOcz(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;
	CString modelUpper = asi.model.c_str();
	modelUpper.MakeUpper();

	// OCZ-TRION100 2015/11/25
	if (modelUpper.Find(_T("OCZ-TRION")) == 0)
	{
		flagSmartType = TRUE;
	}
	// 2012/3/11
	// OCZ-PETROL - https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=553;id=diskinfo#553
	// OCZ-OCTANE S2 - https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=577;id=diskinfo#577
	// OCZ-VERTEX 4 - http://imageshack.us/a/img269/7506/ocz2.png
	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x03
		&& asi.Attribute[2].Id == 0x04
		&& asi.Attribute[3].Id == 0x05
		&& asi.Attribute[4].Id == 0x09
		&& asi.Attribute[5].Id == 0x0C
		&& asi.Attribute[6].Id == 0xE8
		&& asi.Attribute[7].Id == 0xE9
		)
	{
		flagSmartType = TRUE;
	}

	return (modelUpper.Find(_T("OCZ")) == 0 && flagSmartType);
}

BOOL Disk::IsSsdOczVector(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;
	CString modelUpper = asi.model.c_str();
	modelUpper.MakeUpper();

	/*
	// 2013/1/19
	// OCZ-VECTOR - https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=1031;id=diskinfo#1031
	if (asi.Attribute[0].Id == 0x05
	&& asi.Attribute[1].Id == 0x09
	&& asi.Attribute[2].Id == 0x0C
	&& asi.Attribute[3].Id == 0xAB
	&& asi.Attribute[4].Id == 0xAE
	&& asi.Attribute[5].Id == 0xBB
	&& asi.Attribute[6].Id == 0xC3
	&& asi.Attribute[7].Id == 0xC4
	)
	{
	flagSmartType = TRUE;
	}
	// 2013/3/24
	// OCZ-VECTOR - FW 2.0
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=1185;id=diskinfo#1185
	if (asi.Attribute[0].Id == 0x05
	&& asi.Attribute[1].Id == 0x09
	&& asi.Attribute[2].Id == 0x0C
	&& asi.Attribute[3].Id == 0xAB
	&& asi.Attribute[4].Id == 0xAE
	&& asi.Attribute[5].Id == 0xC3
	&& asi.Attribute[6].Id == 0xC4
	)
	{
	flagSmartType = TRUE;
	}
	*/

	// 2015/11/25
	// PANASONIC RP-SSB240GAK
	// https://crystalmark.info/board/c-board.cgi?cmd=one;no=500;id=#500
	if (asi.Attribute[0].Id == 0x05
		&& asi.Attribute[1].Id == 0x09
		&& asi.Attribute[2].Id == 0x0C
		&& asi.Attribute[3].Id == 0xAB
		&& asi.Attribute[4].Id == 0xAE
		&& asi.Attribute[5].Id == 0xC3
		&& asi.Attribute[6].Id == 0xC4
		&& asi.Attribute[7].Id == 0xC5
		&& asi.Attribute[8].Id == 0xC6
		)
	{
		flagSmartType = TRUE;
	}
	if (modelUpper.Find(_T("PANASONIC RP-SSB")) == 0)
	{
		flagSmartType = TRUE;
	}

	return (modelUpper.Find(_T("OCZ")) == 0 || flagSmartType);
}

BOOL Disk::IsSsdPlextor(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	// 2012/10/10
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=739;id=diskinfo#739
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=829;id=diskinfo#829
	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x05
		&& asi.Attribute[2].Id == 0x09
		&& asi.Attribute[3].Id == 0x0C
		&& asi.Attribute[4].Id == 0xB1
		&& asi.Attribute[5].Id == 0xB2
		&& asi.Attribute[6].Id == 0xB5
		&& asi.Attribute[7].Id == 0xB6
		)
	{
		flagSmartType = TRUE;
	}

	// Added CFD's SSD
	return 	asi.model.find(_T("PLEXTOR")) == 0 || asi.model.find(_T("CSSD-S6T128NM3PQ")) == 0 || asi.model.find(_T("CSSD-S6T256NM3PQ")) == 0
		|| flagSmartType;
}

BOOL Disk::IsSsdSanDisk(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	// 2013/10/7
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=1425;id=diskinfo#1425
	if (asi.model.find(_T("SanDisk")) >= 0)
	{
		flagSmartType = TRUE;
		asi.SmartKeyName = _T("SmartSanDisk");

		for (int i = 0; i < 30; i++)
		{
			if (asi.Attribute[i].Id == 0xE9) // Total GB Written To NAND
			{
				asi.HostReadsWritesUnit = HOST_READS_WRITES_GB;
				asi.SmartKeyName = _T("SmartSanDiskGb");
			}
		}
	}

	return flagSmartType;
}

BOOL Disk::IsSsdKingston(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	// 2015/11/29
	if (asi.Attribute[0].Id == 0x01
		&& asi.Attribute[1].Id == 0x02
		&& asi.Attribute[2].Id == 0x03
		&& asi.Attribute[3].Id == 0x05
		&& asi.Attribute[4].Id == 0x07
		&& asi.Attribute[5].Id == 0x08
		&& asi.Attribute[6].Id == 0x09
		&& asi.Attribute[7].Id == 0x0A
		&& asi.Attribute[8].Id == 0x0C
		&& asi.Attribute[9].Id == 0xA8
		)
	{
		flagSmartType = TRUE;
		asi.HostReadsWritesUnit = HOST_READS_WRITES_GB;
	}

	return flagSmartType;
}

BOOL Disk::IsSsdToshiba(ATA_SMART_INFO &asi)
{
	BOOL flagSmartType = FALSE;

	if (asi.model.find(_T("Toshiba")) != -1 && asi.IsSsd)
	{
		flagSmartType = TRUE;
		if (asi.model.find(_T("THNSNC")) != -1)
		{
			// TOSHIBA HG3
			// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=1106;id=diskinfo#1106
		}
		else
		{
			asi.HostReadsWritesUnit = HOST_READS_WRITES_GB;
		}
	}

	return flagSmartType;
}

DWORD Disk::GetTimeUnitType(wstring Model, wstring Firmware, DWORD major, DWORD transferMode)
{
	CString model(Model.c_str());

	CString firmware(Firmware.c_str());

	if (model.Find(_T("FUJITSU")) == 0)
	{
		if (major >= 8)
		{
			return POWER_ON_HOURS;
		}
		else
		{
			return POWER_ON_SECONDS;
		}
	}
	else if (model.Find(_T("HITACHI_DK")) == 0)
	{
		return POWER_ON_MINUTES;
	}
	else if (model.Find(_T("MAXTOR")) == 0)
	{
		if (transferMode >= TRANSFER_MODE_SATA_300
			|| model.Find(_T("MAXTOR 6H")) == 0		// Maxtor DiamondMax 11 family
			|| model.Find(_T("MAXTOR 7H500")) == 0	// Maxtor MaXLine Pro 500 family
			|| model.Find(_T("MAXTOR 6L0")) == 0	// Maxtor DiamondMax Plus D740X family
			|| model.Find(_T("MAXTOR 4K")) == 0		// Maxtor DiamondMax D540X-4K family
			)
		{
			return POWER_ON_HOURS;
		}
		else
		{
			return POWER_ON_MINUTES;
		}
	}
	else if (model.Find(_T("SAMSUNG")) == 0)
	{
		if (transferMode >= TRANSFER_MODE_SATA_300)
		{
			return POWER_ON_HOURS;
		}
		else if (-23 >= _tstoi(firmware.Right(3)) && _tstoi(firmware.Right(3)) >= -39)
		{
			return POWER_ON_HALF_MINUTES;
		}
		else if (model.Find(_T("SAMSUNG SV")) == 0
			|| model.Find(_T("SAMSUNG SP")) == 0
			|| model.Find(_T("SAMSUNG HM")) == 0
			|| model.Find(_T("SAMSUNG MP")) == 0
			)
		{
			return POWER_ON_HALF_MINUTES;
		}
		else
		{
			return POWER_ON_HOURS;
		}
	}
	// 2012/1/15
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=504;id=diskinfo#504
	// http://sourceforge.jp/ticket/browse.php?group_id=4394&tid=27443
	else if (
		((model.Find(_T("CFD_CSSD-S6TM128NMPQ")) == 0 || model.Find(_T("CFD_CSSD-S6TM256NMPQ")) == 0) && (firmware.Find(_T("VM21")) == 0 || firmware.Find(_T("VN21")) == 0))
		|| ((model.Find(_T("PX-128M2P")) != -1 || model.Find(_T("PX-256M2P")) != -1) && _tstof(firmware) < 1.059)
		|| (model.Find(_T("Corsair Performance Pro")) == 0 && _tstof(firmware) < 1.059)
		)
	{
		return POWER_ON_10_MINUTES;
	}
	// https://crystalmark.info/bbs/c-board.cgi?cmd=one;no=1174;id=diskinfo#1174
	else if (
		(model.Find(_T("INTEL SSDSC2CW")) == 0 && model.Find(_T("A3")) > 0) // Intel SSD 520 Series
		|| (model.Find(_T("INTEL SSDSC2BW")) == 0 && model.Find(_T("A3")) > 0) // Intel SSD 520 Series
		|| (model.Find(_T("INTEL SSDSC2CT")) == 0 && model.Find(_T("A3")) > 0) // Intel SSD 330 Series
		)
	{
		return POWER_ON_MILLI_SECONDS;
	}
	else
	{
		return POWER_ON_HOURS;
	}
}



Disk::Disk()
{
	this->UpdateDate();
}

Disk::~Disk()
{
}

Disk Diskinfo::m_disk;

void Diskinfo::updatedisk()
{
	TCHAR   szCurDir[MAX_PATH];
	if (GetModuleFileName(NULL, szCurDir, MAX_PATH) == 0) {

		printf("GetCurrentDirectory failed!  Error = %d \n", GetLastError());
		return;

	}
	(_tcsrchr(szCurDir, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	wstring dir(szCurDir);
	dir.append(L"\\HardWare\\Disk\\");
	if (_waccess(dir.c_str(), 0) == -1)
	{
		OutputDebugString(dir.c_str());
		OutputDebugString(L" is not existing;\n now make it");
		for(int i = 0; i < dir.size(); i++)
		{
			if (dir[i] == '\\')
			{
				dir[i] = '\0';
				if (_waccess(dir.c_str(), 0) != 0)
				{
					int flag = _wmkdir(dir.c_str());
					if (flag == 0)
					{
						OutputDebugString(L"make successfully");
					}
					else {
						OutputDebugString(L"make errorly");
						return;
					}
				}
				dir[i] = '\\';
			}
		}
	}
	m_disk.UpdateDate();
	for (const auto& i : m_disk.SMARTinfo)
	{
		wstring filedir(dir);
		wstring temp;
		ChartoWchar(temp, i.Identify.sModelNumber);
		filedir.append(temp);
		if (_waccess(filedir.c_str(), 0) == -1)
		{
			OutputDebugString(filedir.c_str());
			OutputDebugString(L" is not existing;\n now make it");
			int flag = _wmkdir(filedir.c_str());
			if (flag == 0)
			{
				OutputDebugString(L"make successfully");
			}
			else {
				OutputDebugString(L"make errorly");
				continue;
			}
		}
		wofstream outfile;
		outfile.open(filedir + L"\\DiskName");
		outfile << i.Identify.sModelNumber;
		outfile.close();
		outfile.open(filedir + L"\\DiskSize");
		outfile << i.diskSize << _T("Bytes");
		outfile.close();
		outfile.open(filedir + L"\\SerialNumber");
		outfile << i.Identify.sSerialNumber;
		outfile.close();
		outfile.open(filedir + L"\\Firmware");
		outfile << i.firmware;
		outfile.close();
		outfile.open(filedir + L"\\Interface");
		outfile << i.Interface;
		outfile.close();
		outfile.open(filedir + L"\\Standard");
		outfile << i.MajorVersion << _T(" | ") << i.MinorVersion;
		outfile.close();
		outfile.open(filedir + L"\\TransferMode");
		outfile << i.CurrentTransferMode << _T(" | ") << i.MaxTransferMode;
		outfile.close();
		outfile.open(filedir + L"\\PowerOnCount");
		outfile << i.PowerOnCount;
		outfile.close();
		outfile.open(filedir + L"\\PowerOnHours");
		outfile << ((i.PowerOnStartRawValue == -1) ? i.PowerOnRawValue : i.PowerOnStartRawValue);
		outfile.close();
		outfile.open(filedir + L"\\Temperature");
		outfile << i.Temperature;
		outfile.close();
		for (DWORD j = 0; j < i.AttributeCount; j++)
		{
			WCHAR wID[10];
			swprintf(wID, L"%02X",i.Attribute[j].Id);
			outfile.open(filedir + L'\\' + wID);
			//			outfile << hex << setw(2) << setfill(_T('0')) << i.Attribute[j].Id << _T(" ");
			outfile << dec << i.Attribute[j].CurrentValue << endl\
				<< i.Attribute[j].WorstValue << endl << i.Threshold[j].ThresholdValue << endl;
			for (int k = 5; k >= 0; k--)
			{
				outfile << hex << setw(2) << setfill(_T('0')) << i.Attribute[j].RawValue[k];
			}
			outfile.close();
		}
	}
}

void Diskinfo::ChartoWchar(std::wstring& des, const char* src)
{
	const size_t cSize = strlen(src) + 1;
	wchar_t* wc = new wchar_t[cSize];
	::ZeroMemory(wc, cSize);
	swprintf(wc, cSize, L"%hs", src);
	des = wc;
	delete[] wc;
	wc = nullptr;
}
