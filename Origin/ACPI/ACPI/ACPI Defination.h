#pragma once

#pragma pack(push, 1)
#pragma warning( disable : 4200 )
/************************************************************************/
/*                       RSDP RSDT XSDT Table                           */
/************************************************************************/
typedef struct _ACPI_RSDP_STRUCTURE
{
	BYTE								Signature[8];	// must be 'RSD PTR '
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								Revision;
	DWORD								RsdtAddress;
	DWORD								Length;
	ULONGLONG							XsdtAddress;
	BYTE								Extended_Checksum;
	BYTE								Reserve[3];
}ACPI_RSDP_STRUCTURE;

typedef struct _ACPI_RSDT_STRUCTURE
{
	BYTE								Signature[4]; // must be 'RSDT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								Entry[];
}ACPI_RSDT_STRUCTURE;

typedef struct _ACPI_XSDT_STRUCTURE
{
	BYTE								Signature[4];	// must be 'XSDT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	ULONGLONG							Entry[];
}ACPI_XSDT_STRUCTURE;

/************************************************************************/
/*                             FADT Table                               */
/************************************************************************/

typedef struct _ACPI_FADT_STRUCTURE
{
	BYTE								Signature[4];	// must be 'FACP' 
	DWORD								Length;
	BYTE								FADTMajorVersion;
	BYTE								Checksum;
	BYTE								OEMID[6];
	ULONGLONG							OEMTableID;
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								FIRMWARE_CTRL;
	DWORD								DSDT;
	BYTE								Reserve;
	BYTE								Preferred_PM_Profile;
	USHORT								SCI_INT;
	DWORD								SMI_CMD;
	BYTE								ACPI_ENABLE;
	BYTE								ACPI_DISABLE;
	BYTE								S4BIOS_REQ;
	BYTE								PSTATE_CNT;
	DWORD								PM1a_EVT_BLK;
	DWORD								PM1b_EVT_BLK;
	DWORD								PM1a_CNT_BLK;
	DWORD								PM1b_CNT_BLK;
	DWORD								PM2_CNT_BLK;
	DWORD								PM_TMR_BLK;
	DWORD								GPE0_BLK;
	DWORD								GPE1_BLK;
	BYTE								PM1_EVT_LEN;
	BYTE								PM1_CNT_LEN;
	BYTE								PM2_CNT_LEN;
	BYTE								PM_TMR_LEN;
	BYTE								GPE0_BLK_LEN;
	BYTE								GPE1_BLK_LEN;
	BYTE								GPE1_BASE;
	BYTE								CST_CNT;
	USHORT								P_LVL2_LAT;
	USHORT								P_LVL3_LAT;
	USHORT								FLUSH_SIZE;
	USHORT								FLUSH_STRIDE;
	BYTE								DUTY_OFFSET;
	BYTE								DUTY_WIDTH;
	BYTE								DAY_ALRM;
	BYTE								MON_ALRM;
	BYTE								CENTURY;
	union
	{
		struct
		{
			USHORT						LEGACY_DEVICES : 1;
			USHORT						_8042 : 1;
			USHORT						VGANotPresent : 1;
			USHORT						MSINotSupported : 1;
			USHORT						PCIeASPMControls : 1;
			USHORT						CMOSRTCNotPresent : 1;
			USHORT						Reserved : 10;
		}bits;
		USHORT							IAPC_BOOT_ARCH;
	}						IAPC_BOOT_ARCH;
	BYTE								Reserved;
	union
	{
		struct
		{
			DWORD						WBINVD : 1;
			DWORD						WBINVD_FLUSH : 1;
			DWORD						PROC_C1 : 1;
			DWORD						P_LVL2_UP : 1;
			DWORD						PWR_BUTTON : 1;
			DWORD						SLP_BUTTON : 1;
			DWORD						FIX_RTC : 1;
			DWORD						RTC_S4 : 1;
			DWORD						TMR_VAL_EXT : 1;
			DWORD						DCK_CAP : 1;
			DWORD						RESET_REG_SUP : 1;
			DWORD						SEALED_CASE : 1;
			DWORD						HEADLESS : 1;
			DWORD						CPU_SW_SLP : 1;
			DWORD						PCI_EXP_WAK : 1;
			DWORD						USE_PLATFORM_CLOCK : 1;
			DWORD						S4_RTC_STS_VALID : 1;
			DWORD						REMOTE_POWER_ON_CAPABLE : 1;
			DWORD						FORCE_APIC_CLUSTER_MODEL : 1;
			DWORD						FORCE_APIC_PHYSICAL_DESTINATION_MODE : 1;
			DWORD						HW_REDUCED_ACPI : 1;
			DWORD						LOW_POWER_S0_IDLE_CAPABLE : 1;
			DWORD						Reserved : 10;
		}bits;
		DWORD							Flags;
	}						Flags;
	DWORD								RESET_REG[3];
	BYTE								RESET_VALUE;
	union
	{
		struct
		{
			USHORT						PSCI_COMPLIANT : 1;
			USHORT						PSCI_USE_HVC : 1;
			USHORT						Reserve : 14;
		}bits;
		USHORT							ARM_BOOT_ARCH;
	}						ARM_BOOT_ARCH;
	BYTE								FADTMinorVersion;
	ULONGLONG							X_FIRMWARE_CTRL;
	ULONGLONG							X_DSDT;
	DWORD								X_PM1a_EVT_BLK[3];
	DWORD								X_PM1b_EVT_BLK[3];
	DWORD								X_PM1a_CNT_BLK[3];
	DWORD								X_PM1b_CNT_BLK[3];
	DWORD								X_PM2_CNT_BLK[3];
	DWORD								X_PM_TMR_BLK[3];
	DWORD								X_GPE0_BLK[3];
	DWORD								X_GPE1_BLK[3];
	DWORD								SLEEP_CONTROL_REG[3];
	DWORD								SLEEP_STATUS_REG[3];
	ULONGLONG							HypervisorVendorIdentity;
}ACPI_FADT_STRUCTURE;

/************************************************************************/
/*                             FACS Table                               */
/************************************************************************/

typedef struct _ACPI_FACS_STRUCTURE
{
	BYTE								Signature[4];	// must be 'FACS' 
	DWORD								Length;
	DWORD								HardwareSignature;
	DWORD								FirmwareWakingVector;
	DWORD								GlobalLock;
	union
	{
		struct
		{
			DWORD						S4BIOS_F : 1;
			DWORD						_64BIT_WAKE_SUPPORTED_F : 1;
			DWORD						Reserve : 30;
		}bits;
		DWORD							Flags;
	}						Flags;
	ULONGLONG							XFirmwareWakingVector;
	BYTE								Version;
	BYTE								Reserve[3];
	union
	{
		struct
		{
			DWORD						_64BIT_WAKE_F : 1;
			DWORD						Reserved : 31;
		}bits;
		DWORD							OSPMFlags;
	}						OSPMFlags;
	BYTE								Reserved[24];
}ACPI_FACS_STRUCTURE;

/************************************************************************/
/*                             APIC Table                               */
/************************************************************************/

typedef struct _APIC_Structure
{
	BYTE								Type;		// must be 0
	BYTE								Length;		//must be 8
	BYTE								ACPI_Processor_UUID;
	BYTE								APIC_ID;
	union
	{
		struct
		{
			DWORD						Enables : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Flags;
	}									Flags;
}APIC_Structure;

typedef struct _IO_APIC_Structure
{
	BYTE								Type;		// must be 1
	BYTE								Length;		// must be 12
	BYTE								IO_APIC_ID;
	BYTE								Reserved;
	DWORD								IO_APIC_Address;
	DWORD								Global_System_Interrupt_Base;
}IO_APIC_Structure;

typedef struct _Interrupt_Source_Override_Structure
{
	BYTE								Type;		// must be 2
	BYTE								Length;		// must be 10
	BYTE								Bus;
	BYTE								Source;
	DWORD								Global_System_Interrupt;
	union
	{
		struct
		{
			USHORT						Polarity : 2;
			USHORT						Trigger_Mode : 2;
			USHORT						Reserve : 12;
		}bits;
		USHORT							Flags;
	}									Flags;
}Interrupt_Source_Override_Structure;

typedef struct _NMI_Source_Structure
{
	BYTE								Type;		// must be 3
	BYTE								Length;		// must be 8
	union
	{
		struct
		{
			USHORT						Polarity : 2;
			USHORT						Trigger_Mode : 2;
			USHORT						Reserve : 12;
		}bits;
		USHORT							Flags;
	}									Flags;
	DWORD								Global_System_Interrupt;
}NMI_Source_Structure;

typedef struct _Local_APIC_NMI_Structure
{
	BYTE								Type;		// must be 4
	BYTE								Length;		// must be 6
	BYTE								ACPI_Processor_UID;
	union
	{
		struct
		{
			USHORT						Polarity : 2;
			USHORT						Trigger_Mode : 2;
			USHORT						Reserve : 12;
		}bits;
		USHORT							Flags;
	}									Flags;
	BYTE								Local_APIC_LINT;
}Local_APIC_NMI_Structure;


typedef struct _Local_APIC_Address_Override_Structure
{
	BYTE								Type;		// must be 5
	BYTE								Length;		// must be 12
	USHORT								Reserved;
	ULONGLONG							Local_APIC_Address;
}Local_APIC_Address_Override_Structure;

typedef struct _IO_SAPIC_Structure
{
	BYTE								Type;		// must be 6
	BYTE								Length;		// must be 16
	BYTE								IO_APIC_ID;
	BYTE								Reserved;
	DWORD								Global_System_Interrupt_Base;
	ULONGLONG							IO_SAPIC_Address;
}IO_SAPIC_Structure;

typedef struct _Local_SAPIC_Structure
{
	BYTE								Type;		// must be 7
	BYTE								Length;		// Length of the Local SAPIC Structure in bytes.
	BYTE								ACPI_Processor_ID;
	BYTE								Local_SAPIC_ID;
	BYTE								Local_SAPIC_EID;
	BYTE								Reserve[3];
	union
	{
		struct
		{
			DWORD						Enabled : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Flags;
	}									Flags;
	DWORD								ACPI_Processor_UID_Value;
	UCHAR								ACPI_Processor_UID_String[1];
}_Local_SAPIC_Structure;

typedef struct _Platform_Interrupt_Source_Structure
{
	BYTE								Type;		// must be 8
	BYTE								Length;		// must be 16
	union
	{
		struct
		{
			USHORT						Polarity : 2;
			USHORT						Trigger_Mode : 2;
			USHORT						Reserve : 12;
		}bits;
		USHORT							Flags;
	}									Flags;
	BYTE								Interrupt_Type;
	BYTE								ProcessorID;
	BYTE								ProcessorEID;
	BYTE								IO_SAPIC_Vector;
	DWORD								Global_System_Interrupt;
	union
	{
		struct
		{
			DWORD						CPEI_Processor_Override : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Platform_Interrupt_Source_Flags;
	}Platform_Interrupt_Source_Flags;

}Platform_Interrupt_Source_Structure;

typedef struct _Processor_Local_X2APIC_Structure
{
	BYTE								Type;		// must be 9
	BYTE								Length;		// must be 16
	USHORT								Reserved;
	DWORD								X2APIC_ID;
	DWORD								Flags;
	DWORD								ACPI_Processor_UID;
}_Processor_Local_X2APIC_Structure;

typedef struct _Local_X2APIC_NMI_Structure
{
	BYTE								Type;		// must be 10
	BYTE								Length;		// must be 12
	union
	{
		struct
		{
			USHORT						Polarity : 2;
			USHORT						Trigger_Mode : 2;
			USHORT						Reserve : 12;
		}bits;
		USHORT							Flags;
	}						Flags;
	DWORD								ACPI_Processor_UID;
	BYTE								Local_x2APIC_LINT;
	BYTE								Reserve[3];
}Local_X2APIC_NMI_Structure;

typedef struct _GIC_CPU_Interface_Structure
{
	BYTE								Type;		// must be 11
	BYTE								Length;		// must be 80
	USHORT								Reserves;
	DWORD								CPU_Interface_Number;
	DWORD								ACPI_Processor_UID;
	union
	{
		struct
		{
			DWORD						Enables : 1;
			DWORD						Performance_Interrupt_Mode : 1;
			DWORD						VGIC_Maintenance_interrupt_Mode : 1;
			DWORD						Reserved : 29;
		}bits;
		DWORD							Flags;
	}									Flags;
	DWORD								Parking_Protocol_Version;
	DWORD								Performance_Interrupt_GSIV;
	ULONGLONG							Parked_Address;
	ULONGLONG							Physical_Base_Address;
	ULONGLONG							GICV;
	ULONGLONG							GICH;
	DWORD								VGIC_Maintenance_interrupt;
	ULONGLONG							GICR_Base_Address;
	ULONGLONG							MPIDR;
	BYTE								Processor_Power_Efficiency_Class;
	BYTE								Reserve[3];
}GIC_CPU_Interface_Structure;

typedef struct _GIC_Distributor_Structure
{
	BYTE								Type;		// must be 12
	BYTE								Length;		// must be 24
	USHORT								Reserves;
	DWORD								GICID;
	ULONGLONG							Physical_Base_Address;
	DWORD								System_Vector_Base;
	BYTE								GIC_Version;
	BYTE								Reserve[3];
}GIC_Distributor_Structure;

typedef struct _GIC_MSI_Frame_Structure
{
	BYTE								Type;		// must be 13
	BYTE								Length;		// must be 24
	USHORT								Reserve;
	DWORD								GIC_MSI_Frame_ID;
	ULONGLONG							Physical_Base_Address;
	union
	{
		struct
		{
			DWORD						SPI_Count_Or_Base_Select : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Flags;
	}									Flags;
	USHORT								SPI_Count;
	USHORT								SPI_Base;
}GIC_MSI_Frame_Structure;

typedef struct _GIC_Interrupt_Translation_Service_Structure
{
	BYTE								Type;		// must be 14
	BYTE								Length;		// must be 16
	USHORT								Reserve;
	ULONGLONG							Discovery_Range_Base_Address;
	DWORD								Discovery_Range_Length;
}GIC_Interrupt_Translation_Service_Structure;

typedef struct _ACPI_MADT_STRUCTURE
{
	BYTE								Signature[4];	// must be 'APIC' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								LocalInterruptControllerAddress;
	union
	{
		struct
		{
			DWORD						PCAT_COMPAT : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Flags;
	}									Flags;
	DWORD								InterruptControllerStructure[];
}ACPI_MADT_STRUCTURE, *PACPI_MADT_STRUCTURE;

/************************************************************************/
/*                         SBST Table                                   */
/************************************************************************/

typedef struct _ACPI_SBST_STRUCTURE			//Smart Battery Description Table
{
	BYTE								Signature[4];	// must be 'SBST' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								Warning_Energy_Level;
	DWORD								Low_Energy_Level;
	DWORD								Critical_Energy_Level;
}ACPI_SBST_STRUCTURE;

/************************************************************************/
/*                          ECDT Table                                  */
/************************************************************************/

typedef struct _ACPI_ECDT_STRUCTURE			//Embedded Controller Boot Resources Table
{
	BYTE								Signature[4];	// must be 'ECDT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	BYTE								EC_CONTROL[12];
	BYTE								ECDT_DATA[12];
	DWORD								UID;
	BYTE								GPE_BIT;
	DWORD								EC_ID[];	//Value Variable
}ACPI_ECDT_STRUCTURE;

/************************************************************************/
/*                          SRAT Table									*/
/************************************************************************/

typedef struct _Processor_Local_APIC_SAPIC_Affinity_Structure
{
	BYTE								Type;		//must be 0
	BYTE								Length;		//must be 16
	BYTE								Proximity_Domain7_0;
	BYTE								APIC_ID;
	union
	{
		struct
		{
			DWORD						Enabled : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Flags;
	}						Flags;
	BYTE								Local_SAPIC_EID;
	BYTE								Proximity_Domain31_8[3];
	DWORD								Clock_Domain;
}Processor_Local_APIC_SAPIC_Affinity_Structure;

typedef struct _Memory_Affinity_Structure
{
	BYTE								Type;		//must be 1
	BYTE								Length;		//must be 40
	DWORD								Proximity_Domain;
	USHORT								Reserved;
	DWORD								Base_Address_Low;
	DWORD								Base_Address_High;
	DWORD								Length_Low;
	DWORD								Length_High;
	DWORD								Reserves;
	union
	{
		struct
		{
			DWORD						Enabled : 1;
			DWORD						Hot_Pluggable : 1;
			DWORD						NonVolatile : 1;
			DWORD						Reserve : 29;
		}bits;
		DWORD							Flags;
	}						Flags;
	ULONGLONG							Reserve;
}Memory_Affinity_Structure;

typedef struct _Processor_Local_X2APIC_Affinity_Structure
{
	BYTE								Type;		//must be 2
	BYTE								Length;		//must be 24
	USHORT								Reserved;
	DWORD								Proximity_Domain;
	DWORD								X2APIC_ID;
	union
	{
		struct
		{
			DWORD						Enabled : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Flags;
	}						Flags;
	DWORD								Clock_Domain;
	DWORD								Reserve;
}Processor_Local_X2APIC_Affinity_Structure;

typedef struct _GICC_Affinity_Structure
{
	BYTE								Type;		//must be 3
	BYTE								Length;		//must be 18
	DWORD								Proximity_Domain;
	DWORD								ACPI_Processor_ID;
	union
	{
		struct
		{
			DWORD						Enabled : 1;
			DWORD						Reserve : 31;
		}bits;
		DWORD							Flags;
	}						Flags;
	DWORD								Clock_Domain;
}GICC_Affinity_Structure;

typedef struct _ACPI_SRAT_STRUCTURE			//System Resource Affinity Table
{
	BYTE								Signature[4];	// must be 'SRAT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								Reserved[3];
	DWORD								Static_Resource_Allocation_Structure[];	// A list of static resource allocation structures for the platform.
}ACPI_SRAT_STRUCTURE;

/************************************************************************/
/*                         SLIT Table                                   */
/************************************************************************/

typedef struct _ACPI_SLIT_STRUCTURE			//System Locality Distance Information Table
{
	BYTE								Signature[4];	// must be 'SLIT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	ULONGLONG							Number_of_System_Localities;
	DWORD								Entry[];	// [0][0]……[Number of System Localities - 1][Number of System Localities - 1]
}ACPI_SLIT_STRUCTURE;

/************************************************************************/
/*                            CPEP Table                                */
/************************************************************************/

typedef struct _Corrected_Platform_Error_Polling_Processor_Structure
{
	BYTE								Type;		// must be 0
	BYTE								Length;		// must be 8
	BYTE								ProcessorID;
	BYTE								ProcessorEID;
	DWORD								Polling_Interval;
}Corrected_Platform_Error_Polling_Processor_Structure;

typedef struct _ACPI_CPEP_STRUCTURE			//Corrected Platform Error Polling Table
{
	BYTE								Signature[4];	// must be 'SLIT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	ULONGLONG							Reserved;
	DWORD								CPEP_Processor_Structure[];	// _Corrected_Platform_Error_Polling_Processor_Structure
}ACPI_CPEP_STRUCTURE;

/************************************************************************/
/*                           MSCT Table                                 */
/************************************************************************/

typedef struct _Maximum_Proximity_Domain_Information
{
	BYTE								Revision;	// must be 1
	BYTE								Length;		// must be 22
	DWORD								Proximity_Domain_Range_Low;
	DWORD								Proximity_Domain_Range_High;
	DWORD								Maximum_Processor_Capacity;
	ULONGLONG							Maximum_Memory_Capacity;
}Maximum_Proximity_Domain_Information;

typedef struct _ACPI_MSCT_STRUCTURE			//Maximum System Characteristics Table
{
	BYTE								Signature[4];	// must be 'MSCT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								OffsetProxDomInfo;
	DWORD								Maximum_Number_of_Proximity_Domains;
	DWORD								Maximum_Number_of_Clock_Domains;
	ULONGLONG							Maximum_Physical_Address;
	//Proximity Domain Information Structure offset [OffsetProxDomInfo]
}ACPI_MSCT_STRUCTURE;

/************************************************************************/
/*								RASF Table                              */
/************************************************************************/

typedef struct _RASF_Communication_Channel
{
	BYTE								Signature[4]; // must be 0x52415346
	USHORT								Command;
	USHORT								Status;
	USHORT								Version;
	ULONGLONG							RAS_Capabilities[2];
	ULONGLONG							Set_RAS_Capabilities[2];
	USHORT								Number_of_RASF_Parameter_blocks;
	DWORD								Set_RAS_Capabilities_Status;
	DWORD								Parameter_Blocks[];
}RASF_Communication_Channel;

typedef struct _Parameter_Block
{
	USHORT								Type; // must be 0x52415346
	USHORT								Version;
	USHORT								Length;
	USHORT								Patrol_Scrub_Command;	//INPUT
	ULONGLONG							Requested_Address_Range[2];	//INPUT
	ULONGLONG							Actual_Address_Range[2];	//OUTPUT
	USHORT								Flags;	// OUTPUT
	BYTE								Requested_Speed;	//INPUT
}Parameter_Block;


typedef struct _ACPI_RASF_STRUCTURE			//Maximum System Characteristics Table
{
	BYTE								Signature[4];	// must be 'RASF' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								OffsetProxDomInfo;
	DWORD								PCC_Sub_channel_structure[3]; // OSPM should use this value to identify the PCC Sub channel structure in the RASF table
}ACPI_RASF_STRUCTURE;

/************************************************************************/
/*                             MPST Table                               */
/************************************************************************/

//这咋写结构 ACPI6.1
// typedef struct _ACPI_MPST_STRUCTURE			//Maximum System Characteristics Table
// {
// 	BYTE								Signature[4];	// must be 'MPST' 
// 	DWORD								Length;
// 	BYTE								Revision;
// 	BYTE								Checksum;
// 	BYTE								OEMID[6];
// 	BYTE								OEMTableID[8];
// 	DWORD								OEMRevision;
// 	BYTE								CreatorID[4];
// 	DWORD								CreatorRevision;
// 	BYTE								MPST_Platform_Communication_Channel_Identifier;
// 	BYTE								Reserve[3];
// 	USHORT								Memory_Power_Node_Count;
// 	USHORT								Reserves;
// 
// }ACPI_MPST_STRUCTURE;

/************************************************************************/
/*                             PMTT Table                               */
/************************************************************************/

typedef struct _Common_Memory_Aggregator_Device_Structure
{
	BYTE								Type;
	BYTE								Reserve;
	USHORT								Length;
	USHORT								Flags;
	USHORT								Reserves;
	DWORD								Type_Specific_Data[]; // Address for Socket_Structure
}Common_Memory_Aggregator_Device_Structure;

typedef struct _Socket_Structure
{
	BYTE								Type;
	BYTE								Reserve;
	USHORT								Length;
	USHORT								Flags;
	USHORT								Reserves;
	USHORT								Socket_Identifier;
	DWORD								Memory_Controller_Structure[]; // Address for Memory Controller Structure
}Socket_Structure;

typedef struct _Memory_Controller_Structure
{
	BYTE								Type;
	BYTE								Reserve;
	USHORT								Length;
	USHORT								Flags;
	USHORT								Reserves;
	DWORD								Read_Latency;
	DWORD								Write_Latency;
	DWORD								Read_Bandwidth;
	DWORD								Write_Bandwidth;
	USHORT								Optimal_access_unit;
	USHORT								Optimal_access_alignment;
	USHORT								Reserved;
	USHORT								Number_of_Proximity_Domains; // m
	DWORD								Proximity_Domain; // Length of 4 * m
														  //Physical Component Identifier Structure
}Memory_Controller_Structure;

typedef	struct _Physical_Components_Identifier_Structure
{
	BYTE								Type;
	BYTE								Reserve;
	USHORT								Length;
	USHORT								Flags;
	USHORT								Reserves;
	USHORT								Physical_Component_Identifier;
	USHORT								Reserved;
	DWORD								Size_Of_DIMM;
	DWORD								SMIBIOS_Handle;
}Physical_Components_Identifier_Structure;

typedef struct _ACPI_PMTT_STRUCTURE			//Maximum System Characteristics Table
{
	BYTE								Signature[4];	// must be 'PMTT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								Reserve;
	UCHAR								Memory_Aggregator_Device_Structure[]; // Address for _Common_Memory_Aggregator_Device_Structure
}ACPI_PMTT_STRUCTURE;

/************************************************************************/
/*                         BGRT Table									*/
/************************************************************************/

typedef struct _ACPI_BGRT_STRUCTURE			//Maximum System Characteristics Table
{
	BYTE								Signature[4];	// must be 'BGRT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	USHORT								Version;
	BYTE								Status;
	BYTE								Image_Type;
	ULONGLONG							Image_Address;
	DWORD								Image_Offset_X;
	DWORD								Image_Offset_Y;
}ACPI_BGRT_STRUCTURE;

/************************************************************************/
/*                            FPDT Table                                */
/************************************************************************/

//暂时不会搞
typedef	struct _Performance_Record_Structure
{
	USHORT								Performance_Record_Type;
	BYTE								Record_Length;
	BYTE								Revision;
	DWORD								Reserved;
	BYTE								Data[8];
}Performance_Record_Structure;

typedef struct _ACPI_FPDT_STRUCTURE			//Maximum System Characteristics Table
{
	BYTE								Signature[4];	// must be 'FPDT' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	Performance_Record_Structure		Performance_Records[];
}ACPI_FPDT_STRUCTURE, *PACPI_FPDT_STRUCTURE;


/************************************************************************/
/*                             MCFG Table                               */
/************************************************************************/

typedef struct _Configuration_space_base_address_structure
{
	ULONGLONG							Base_Address;
	USHORT								PCI_Segment_Group_Number;
	BYTE								Start_Bus_Number;
	BYTE								End_Bus_Number;
	DWORD								Reserved;
}Configuration_space_base_address_structure;

typedef struct _ACPI_MCFG_STRUCTURE			//Memory mapped configuration space base address Description Table
{
	BYTE								Signature[4];	// must be 'MCFG' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	ULONGLONG							Reserved;
	Configuration_space_base_address_structure	Configuration_space_base_address;
}ACPI_MCFG_STRUCTURE;


/************************************************************************/
/*                               HPET Table                             */
/************************************************************************/


typedef struct _ACPI_HPET_STRUCTURE			//IA-PC High Precision Event Timer
{
	BYTE								Signature[4];	// must be 'HPET' 
	DWORD								Length;
	BYTE								Revision;
	BYTE								Checksum;
	BYTE								OEMID[6];
	BYTE								OEMTableID[8];
	DWORD								OEMRevision;
	BYTE								CreatorID[4];
	DWORD								CreatorRevision;
	DWORD								Event_Timer_Block_ID;
	DWORD								BASE_ADDRESS_Lower_32Bit[3];
	BYTE								HPET_Number;
	DWORD								Minimum_Clock_Tick;
	BYTE								Page_Protection_and_OEM_Attribute;
}ACPI_HPET_STRUCTURE;

#pragma warning( default : 4200 )
#pragma  pack(pop)