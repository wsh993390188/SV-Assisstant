#pragma once
#include <string>
#include <iomanip>

namespace Smbios
{
#pragma pack(push,1)
	struct DMI_Header
	{
		union
		{
			struct
			{
				uint8_t type;
				uint8_t length;
				uint16_t handle;
			};
			void* VirtualBase;
		};
	};
#pragma pack(pop)

	namespace SmbiosHeaderType
	{
		class TypeBase
		{
		public:
			explicit TypeBase(const DMI_Header& Header)
				:Header(Header)
			{
			}
			virtual const std::wstring Descriptor() const = 0;

			void* GetVirtualBaseAddress() const noexcept
			{
				return reinterpret_cast<void*>(const_cast<DMI_Header*>(&Header));
			}

			const DMI_Header GetHeader() const
			{
				return Header;
			}

		private:
			const DMI_Header& Header;
		};

		class TypeFactory final
		{
		public:
			static TypeFactory& Instance();
			std::shared_ptr<TypeBase> CreateFactory(const DMI_Header& Type);
			void SetVendor(const std::string& Vendor);
			const std::string& GetVendor();
		private:
			std::shared_ptr<TypeBase> OEMSmbiosType(const DMI_Header& Type);
			TypeFactory() = default;
		private:
			std::string Vendor;
		};

		struct Type0 {};
		struct Type1 {};
		struct Type2 {};
		struct Type3 {};
		struct Type4 {};
		struct Type5 {};
		struct Type6 {};
		struct Type7 {};
		struct Type8 {};
		struct Type9 {};
		struct Type10 {};
		struct Type11 {};
		struct Type12 {};
		struct Type13 {};
		struct Type14 {};
		struct Type15 {};
		struct Type16 {};
		struct Type17 {};
		struct Type18 {};
		struct Type19 {};
		struct Type20 {};
		struct Type21 {};
		struct Type22 {};
		struct Type23 {};
		struct Type24 {};
		struct Type25 {};
		struct Type26 {};
		struct Type27 {};
		struct Type28 {};
		struct Type29 {};
		struct Type30 {};
		struct Type31 {};
		struct Type32 {};
		struct Type33 {};
		struct Type34 {};
		struct Type35 {};
		struct Type36 {};
		struct Type37 {};
		struct Type38 {};
		struct Type39 {};
		struct Type40 {};
		struct Type41 {};
		struct Type42 {};
		struct Type43 {};
		struct Type44 {};
		struct Type126 {};
		struct Type127 {};
		struct Type131 {};
		struct Type133 {};
		struct Type134 {};
		struct Type140 {};
		struct Type170 {};
		struct Type200 {};
	}
#pragma pack(push, 1)
#pragma region Type1
	enum class WakeUpType : uint8_t
	{
		Reserved = 0,
		Other = 1,
		Unknown = 2,
		APMTimer = 3,
		ModemRing = 4,
		LANRemote = 5,
		PowerSwitch = 6,
		PCIPME = 7,
		ACPowerRestored = 8
	};
#pragma endregion
#pragma region Type3
	enum class Type3StatusType : uint8_t
	{
		Reserved = 0,
		Other = 1,
		Unknown = 2,
		Safe = 3,
		Warning = 4,
		Critical = 5,
		NonRecoverable = 6
	};

	enum class Type3SecurityStatus : uint8_t
	{
		Other = 0x01,
		Unknown = 0x02,
		None = 0x03,
		ExternalInterfaceLockedOut = 0x04,
		ExternalInterfaceEnabled = 0x05
	};
#pragma endregion
#pragma region Type5
#pragma pack(push,1)
	struct ErrorCapability
	{
		union
		{
			struct
			{
				uint8_t Other : 1;
				uint8_t Unknown : 1;
				uint8_t None : 1;
				uint8_t SingleBitErrorCorrecting : 1;
				uint8_t DoubleBitErrorCorrecting : 1;
				uint8_t ErrorScrubbing : 1;
				uint8_t Reversed : 2;
			};

			uint8_t Number;
		};

		bool operator == (const ErrorCapability& rhs) const {
			return this->Number == rhs.Number;
		}

		ErrorCapability() = default;
		explicit ErrorCapability(const uint8_t& Num) : Number(Num) {}
	};

	struct MemorySpeeds
	{
		union
		{
			struct
			{
				uint16_t Other : 1;
				uint16_t Unknown : 1;
				uint16_t _70ns : 1;
				uint16_t _60ns : 1;
				uint16_t _50ns : 1;
				uint16_t Reversed : 11;
			};

			uint16_t Number;
		};
		bool operator == (const MemorySpeeds& rhs) const {
			return this->Number == rhs.Number;
		}
		MemorySpeeds() = default;
		explicit MemorySpeeds(const uint16_t& Num) : Number(Num) {}
	};

	struct MemoryTypes
	{
		union
		{
			struct
			{
				uint16_t Other : 1;
				uint16_t Unknown : 1;
				uint16_t Standard : 1;
				uint16_t FastPageMode : 1;
				uint16_t EDO : 1;
				uint16_t Parity : 1;
				uint16_t ECC : 1;
				uint16_t SIMM : 1;
				uint16_t DIMM : 1;
				uint16_t BurstEDO : 1;
				uint16_t SDRAM : 1;
				uint16_t Reversed : 5;
			};

			uint16_t Number;
		};
		bool operator == (const MemoryTypes& rhs) const {
			return this->Number == rhs.Number;
		}
		MemoryTypes() = default;
		explicit MemoryTypes(const uint16_t& Num) : Number(Num) {}
	};

	struct MemoryVoltages
	{
		union
		{
			struct
			{
				uint8_t _5V : 1;
				uint8_t _3_3V : 1;
				uint8_t _2_9V : 1;
				uint8_t Reversed : 5;
			};

			uint8_t Number;
		};
		bool operator == (const MemoryVoltages& rhs) const {
			return this->Number == rhs.Number;
		}
		MemoryVoltages() = default;
		explicit MemoryVoltages(const uint8_t& Num) : Number(Num) {}
	};

#pragma pack(pop)
#pragma endregion
#pragma region Type7
	struct SRAMType
	{
		union
		{
			struct
			{
				uint16_t Other : 1;
				uint16_t Unknown : 1;
				uint16_t NonBurst : 1;
				uint16_t Burst : 1;
				uint16_t PipelineBurst : 1;
				uint16_t Synchronous : 1;
				uint16_t Asynchronous : 1;
				uint16_t Reserved : 9;
			};
			uint16_t Number;
		};
		bool operator == (const SRAMType& rhs) const {
			return this->Number == rhs.Number;
		}
		SRAMType() = default;
		explicit SRAMType(const uint16_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type8
	struct SlotCharacteristics1Field
	{
		union
		{
			struct
			{
				uint8_t Unknown : 1;
				uint8_t Provides5_0volts : 1;
				uint8_t Provides3_3volts : 1;
				uint8_t SlotOpeningIsSharedWithAnotherSlot : 1;
				uint8_t SupportsPCCard16 : 1;
				uint8_t SupportsCardBus : 1;
				uint8_t SupportsZoomVideo : 1;
				uint8_t SupportsModemRingResume : 1;
			};
			uint8_t Number;
		};
		bool operator == (const SlotCharacteristics1Field& rhs) const {
			return this->Number == rhs.Number;
		}
		SlotCharacteristics1Field() = default;
		explicit SlotCharacteristics1Field(const uint8_t& Num) : Number(Num) {}
	};

	struct SlotCharacteristics2Field
	{
		union
		{
			struct
			{
				uint8_t SupportsPowerManagementEventSignal : 1;
				uint8_t SupportsHotplugDevices : 1;
				uint8_t SupportsSMBusSignal : 1;
				uint8_t Supportsbifurcation : 1;
				uint8_t Reverses : 4;
			};
			uint8_t Number;
		};
		bool operator == (const SlotCharacteristics2Field& rhs) const {
			return this->Number == rhs.Number;
		}
		SlotCharacteristics2Field() = default;
		explicit SlotCharacteristics2Field(const uint8_t& Num) : Number(Num) {}
	};

	struct DeviceFuncField
	{
		union
		{
			struct
			{
				uint8_t DeviceNumber : 5;
				uint8_t FunctionNumber : 3;
			};
			uint8_t Number;
		};
		bool operator == (const DeviceFuncField& rhs) const {
			return this->Number == rhs.Number;
		}
		DeviceFuncField() = default;
		explicit DeviceFuncField(const uint8_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type10

	struct OnBroadDeviceInformation
	{
		enum class DeviceTypeStatus
		{
			Enable = 1,
			Disable = 0
		};
		DeviceTypeStatus Status;
		std::string DeviceType;
		std::string DeviceDesc;

		OnBroadDeviceInformation() : Status(DeviceTypeStatus::Disable), DeviceType{}, DeviceDesc{} {}
		bool operator == (const OnBroadDeviceInformation& rhs) const
		{
			return this->Status == rhs.Status && (DeviceType == rhs.DeviceType) && (DeviceDesc == rhs.DeviceDesc);
		}

		friend std::ostream& operator<<(std::ostream& out, const OnBroadDeviceInformation& _myStruct)
		{
			if (_myStruct.Status == DeviceTypeStatus::Enable)
			{
				out << "Enable";
			}
			else
			{
				out << "Disable";
			}
			out << _myStruct.DeviceType;
			out << _myStruct.DeviceDesc;
			return out;
		}
	};

#pragma endregion

#pragma region Type14

	struct GroupAssociationItems
	{
		uint8_t ItemType;
		uint16_t ItemHandle;

		GroupAssociationItems() = default;
		bool operator == (const GroupAssociationItems& rhs) const
		{
			return (this->ItemType == rhs.ItemType) && (ItemHandle == rhs.ItemHandle);
		}

		friend std::ostream& operator<<(std::ostream& out, const GroupAssociationItems& _myStruct)
		{
			out << " ItemType: " << _myStruct.ItemType + 0;
			out << " ItemHandle: " << _myStruct.ItemHandle + 0;
			return out;
		}

		friend std::wostream& operator<<(std::wostream& out, const GroupAssociationItems& _myStruct)
		{
			out << L" ItemType: " << _myStruct.ItemType + 0;
			out << L" ItemHandle: " << _myStruct.ItemHandle + 0;
			return out;
		}
	};

#pragma endregion

#pragma region Type15

	struct EventLogType
	{
		std::string LogType;
		std::string DataFormatType;

		EventLogType() = default;
		bool operator == (const EventLogType& rhs) const
		{
			return (this->LogType == rhs.LogType) && (DataFormatType == rhs.DataFormatType);
		}

		friend std::ostream& operator<<(std::ostream& out, const EventLogType& _myStruct)
		{
			out << " Log Type: " << _myStruct.LogType;
			out << " Variable Data Format Type: " << _myStruct.DataFormatType;
			return out;
		}

		friend std::wostream& operator<<(std::wostream& out, const EventLogType& _myStruct)
		{
			out << L" Log Type: " << _myStruct.LogType.c_str();
			out << L" Variable Data Format Type: " << _myStruct.DataFormatType.c_str();
			return out;
		}
	};

	struct LogStatusField
	{
		union
		{
			struct
			{
				uint8_t LogAreaValid : 1;
				uint8_t LogAreaFull : 1;
				uint8_t Reversed : 6;
			};
			uint8_t Number;
		};
		bool operator == (const LogStatusField& rhs) const {
			return this->Number == rhs.Number;
		}
		LogStatusField() = default;
		explicit LogStatusField(const uint8_t& Num) : Number(Num) {}
	};

	struct AccessMethodAddressField
	{
		union
		{
			struct
			{
				uint16_t IndexAddr;
				uint16_t DataAddr;
			} IO;
			uint32_t Number;
			uint16_t GPNVHandle;
		};
		bool operator == (const AccessMethodAddressField& rhs) const {
			return this->Number == rhs.Number;
		}
		AccessMethodAddressField() = default;
		explicit AccessMethodAddressField(const uint32_t& Num) : Number(Num) {}
	};

#pragma endregion

#pragma region Type17
	struct MemoryModeCapability
	{
		union
		{
			struct
			{
				uint16_t Reversed1 : 1;
				uint16_t Other : 1;
				uint16_t Unknown : 1;
				uint16_t VolatileMmemory : 1;
				uint16_t ByteAccessiblePersistentMemory : 1;
				uint16_t BlockAccessiblePersistentMemory : 1;
				uint16_t Reserved2 : 10;
			};
			uint16_t Number;
		};
		bool operator == (const MemoryModeCapability& rhs) const {
			return this->Number == rhs.Number;
		}
		MemoryModeCapability() = default;
		explicit MemoryModeCapability(const uint16_t& Num) : Number(Num) {}
	};

	struct MemoryTypeDetailField
	{
		union
		{
			struct
			{
				uint16_t Reserved : 1;
				uint16_t Other : 1;
				uint16_t Unknown : 1;
				uint16_t FastPaged : 1;
				uint16_t StaticColumn : 1;
				uint16_t PseudoStatic : 1;
				uint16_t RAMBUS : 1;
				uint16_t Synchronous : 1;
				uint16_t CMOS : 1;
				uint16_t EDO : 1;
				uint16_t  WindowDRAM : 1;
				uint16_t  CacheDRAM : 1;
				uint16_t  NonVolatile : 1;
				uint16_t  RegisteredBuffered : 1;
				uint16_t  UnbufferedUnregistered : 1;
				uint16_t  LRDIMM : 1;
			};
			uint16_t Number;
		};
		bool operator == (const MemoryTypeDetailField& rhs) const {
			return this->Number == rhs.Number;
		}
		MemoryTypeDetailField() = default;
		explicit MemoryTypeDetailField(const uint16_t& Num) : Number(Num) {}
	};

	struct MemoryAttributeField
	{
		union
		{
			struct
			{
				uint8_t Rank : 4;
				uint8_t Reserved : 4;
			};
			uint8_t Number;
		};
		bool operator == (const MemoryAttributeField& rhs) const {
			return this->Number == rhs.Number;
		}
		MemoryAttributeField() = default;
		explicit MemoryAttributeField(const uint8_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type22

	struct ManufacturerDateField
	{
		union
		{
			struct
			{
				uint16_t Date : 5;
				uint16_t Month : 4;
				uint16_t Year : 7;
			};
			uint16_t Number;
		};
		bool operator == (const ManufacturerDateField& rhs) const {
			return this->Number == rhs.Number;
		}
		ManufacturerDateField() = default;
		explicit ManufacturerDateField(const uint16_t& Num) : Number(Num) {}
	};

#pragma endregion

#pragma region Type23

	struct SystemResetCapabilitiesField
	{
		union
		{
			struct
			{
				uint8_t Status : 1;
				uint8_t BootOption : 2;
				uint8_t BootOptionOnLimit : 2;
				uint8_t SystemContainsWatchdogTimer : 1;
				uint8_t Reserved : 2;
			};
			uint8_t Number;
		};
		bool operator == (const SystemResetCapabilitiesField& rhs) const {
			return this->Number == rhs.Number;
		}
		SystemResetCapabilitiesField() = default;
		explicit SystemResetCapabilitiesField(const uint8_t& Num) : Number(Num) {}
	};

#pragma endregion

#pragma region Type24

	struct HardwareSecurity
	{
		union
		{
			struct
			{
				uint8_t FrontPanelResetStatus : 2;
				uint8_t AdministratorPasswordStatus : 2;
				uint8_t KeyboardPasswordStatus : 2;
				uint8_t PowerOnPasswordStatus : 2;
			};
			uint8_t Number;
		};
		bool operator == (const HardwareSecurity& rhs) const {
			return this->Number == rhs.Number;
		}
		HardwareSecurity() = default;
		explicit HardwareSecurity(const uint8_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type26
	struct VoltageProbeStatusField
	{
		union
		{
			struct
			{
				uint8_t Location : 5;
				uint8_t Status : 3;
			};
			uint8_t Number;
		};
		bool operator == (const VoltageProbeStatusField& rhs) const {
			return this->Number == rhs.Number;
		}
		VoltageProbeStatusField() = default;
		explicit VoltageProbeStatusField(const uint8_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type27
	struct DeviceStatusField
	{
		union
		{
			struct
			{
				uint8_t Location : 5;
				uint8_t DeviceType : 3;
			};
			uint8_t Number;
		};
		bool operator == (const DeviceStatusField& rhs) const {
			return this->Number == rhs.Number;
		}
		DeviceStatusField() = default;
		explicit DeviceStatusField(const uint8_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type30
	struct RemoteAccessConnectField
	{
		union
		{
			struct
			{
				uint8_t InboundConnectionEnabled : 1;
				uint8_t OutboundConnectionEnabled : 1;
				uint8_t Reserved : 6;
			};
			uint8_t Number;
		};
		bool operator == (const RemoteAccessConnectField& rhs) const {
			return this->Number == rhs.Number;
		}
		RemoteAccessConnectField() = default;
		explicit RemoteAccessConnectField(const uint8_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type37
	struct MemoryDeviceHandle
	{
		uint8_t DeviceLoad;
		uint16_t DeviceHandle;

		MemoryDeviceHandle() = default;
		bool operator == (const MemoryDeviceHandle& rhs) const
		{
			return (this->DeviceHandle == rhs.DeviceHandle) && (DeviceLoad == rhs.DeviceLoad);
		}

		friend std::ostream& operator<<(std::ostream& out, const MemoryDeviceHandle& _myStruct)
		{
			out << " Device Load: " << _myStruct.DeviceLoad + 0;
			out << " Device Handle: " << _myStruct.DeviceHandle;
			return out;
		}

		friend std::wostream& operator<<(std::wostream& out, const MemoryDeviceHandle& _myStruct)
		{
			out << L" Device Load: " << _myStruct.DeviceLoad + 0;
			out << L" Device Handle: " << _myStruct.DeviceHandle;
			return out;
		}
	};
#pragma endregion

#pragma region Type38
	struct IPMIRevisionField
	{
		union
		{
			struct
			{
				uint8_t TheMostSignificant : 4;
				uint8_t TheLeastSignificant : 4;
			};
			uint8_t Number;
		};
		bool operator == (const IPMIRevisionField& rhs) const {
			return this->Number == rhs.Number;
		}
		IPMIRevisionField() = default;
		explicit IPMIRevisionField(const uint8_t& Num) : Number(Num) {}
	};

	struct BaseAddrOrInterruptField
	{
		union
		{
			struct
			{
				struct Interrupt
				{
					uint8_t InterruptTriggerMode : 1;
					uint8_t InterruptPolarity : 1;
					uint8_t Reserved : 1;
					uint8_t InterruptInfomation : 1;
				};
				struct BaseAddress
				{
					uint8_t LSBitForAddresses : 1;
					uint8_t Reserved : 1;
					uint8_t RegisterSpacing : 2;
				};
			};

			uint8_t Number;
		};
		bool operator == (const BaseAddrOrInterruptField& rhs) const {
			return this->Number == rhs.Number;
		}
		BaseAddrOrInterruptField() = default;
		explicit BaseAddrOrInterruptField(const uint8_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type39
	struct PowerSupplyCharacteristicsField
	{
		union
		{
			struct
			{
				uint16_t PowerSupplyIsHotreplaceable : 1;
				uint16_t PowerSupplyIsPresent : 1;
				uint16_t PowerSupplyIsUnpluggedFromTheWall : 1;
				uint16_t DMTFInputVoltageRangeSwitching : 4;
				uint16_t Status : 3;
				uint16_t DMTFPowerSupplyType : 4;
				uint16_t Reserved : 2;
			};
			uint16_t Number;
		};
		bool operator == (const PowerSupplyCharacteristicsField& rhs) const {
			return this->Number == rhs.Number;
		}
		PowerSupplyCharacteristicsField() = default;
		explicit PowerSupplyCharacteristicsField(const uint16_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type40
	struct AdditionalInformationEntry
	{
		uint16_t ReferencedHandle;
		uint8_t ReferencedOffset;
		std::string Desc;
		std::vector<uint8_t> Values;

		AdditionalInformationEntry() = default;
		bool operator == (const AdditionalInformationEntry& rhs) const
		{
			return this->ReferencedHandle == rhs.ReferencedHandle &&
				(ReferencedOffset == rhs.ReferencedOffset) &&
				(Desc == rhs.Desc) &&
				(Values == rhs.Values);
		}

		friend std::ostream& operator<<(std::ostream& out, const AdditionalInformationEntry& _myStruct)
		{
			out << " Handle: " << _myStruct.ReferencedHandle;
			out << " Offset: " << _myStruct.ReferencedOffset + 0;
			out << " String: " << _myStruct.Desc;
			out << " Values: ";
			for (const auto& Value : _myStruct.Values)
			{
				out << std::setw(2) << std::setfill('0') << std::hex << Value << " ";
			}
			return out;
		}

		friend std::wostream& operator<<(std::wostream& out, const AdditionalInformationEntry& _myStruct)
		{
			out << L" Handle: " << _myStruct.ReferencedHandle;
			out << L" Offset: " << _myStruct.ReferencedOffset + 0;
			out << L" String: " << _myStruct.Desc.c_str();
			out << L" Values: ";
			for (const auto& Value : _myStruct.Values)
			{
				out << std::setw(2) << std::setfill(L'0') << std::hex << Value << " ";
			}
			return out;
		}
	};
#pragma endregion

#pragma region Type41
	struct ExtendedDeviceTypeField
	{
		enum class DeviceTypeStatus
		{
			Enable = 1,
			Disable = 0
		};
		DeviceTypeStatus Status;
		std::string DeviceType;

		ExtendedDeviceTypeField() : Status(DeviceTypeStatus::Disable), DeviceType{}{}
		bool operator == (const ExtendedDeviceTypeField& rhs) const
		{
			return this->Status == rhs.Status && (DeviceType == rhs.DeviceType);
		}

		friend std::ostream& operator<<(std::ostream& out, const ExtendedDeviceTypeField& _myStruct)
		{
			if (_myStruct.Status == DeviceTypeStatus::Enable)
			{
				out << "Enable";
			}
			else
			{
				out << "Disable";
			}
			out << _myStruct.DeviceType;
			return out;
		}

		friend std::wostream& operator<<(std::wostream& out, const ExtendedDeviceTypeField& _myStruct)
		{
			out << _myStruct.DeviceType.c_str() << L" - ";
			if (_myStruct.Status == DeviceTypeStatus::Enable)
			{
				out << L"Enable";
			}
			else
			{
				out << L"Disable";
			}
			return out;
		}
	};
#pragma endregion

#pragma region Type42
	struct ProtocolRecordDataField
	{
		std::string Type;
		uint8_t Length;
		std::vector<uint8_t> ProtocolDatas;

		ProtocolRecordDataField() = default;
		bool operator == (const ProtocolRecordDataField& rhs) const
		{
			return this->Type == rhs.Type &&
				(Length == rhs.Length) &&
				(ProtocolDatas == rhs.ProtocolDatas);
		}

		friend std::ostream& operator<<(std::ostream& out, const ProtocolRecordDataField& _myStruct)
		{
			out << " Type: " << _myStruct.Type;
			out << " Length: " << _myStruct.Length + 0;
			out << " ProtocolDatas: ";
			for (const auto& Value : _myStruct.ProtocolDatas)
			{
				out << std::setw(2) << std::setfill('0') << std::hex << Value << " ";
			}
			return out;
		}

		friend std::wostream& operator<<(std::wostream& out, const ProtocolRecordDataField& _myStruct)
		{
			out << L" Type: " << _myStruct.Type.c_str();
			out << L" Length: " << _myStruct.Length + 0;
			out << L" ProtocolDatas: ";
			for (const auto& Value : _myStruct.ProtocolDatas)
			{
				out << std::setw(2) << std::setfill(L'0') << std::hex << Value << L" ";
			}
			return out;
		}
	};
#pragma endregion

#pragma region Type43
	struct TPMDeviceCharacteristicsField
	{
		union
		{
			struct
			{
				uint64_t Reserved0 : 1;
				uint64_t Reserved1 : 1;
				uint64_t TPMDeviceCharacteristicsAreNotSupported : 1;
				uint64_t FamilyConfigurableViaFirmwareUpdate : 1;
				uint64_t FamilyConfigurableViaPlatformSoftwareUpdate : 1;
				uint64_t FamilyConfigurableViaOEMMechanismUpdate : 1;
				uint64_t Reserved : 58;
			};
			uint64_t Number;
		};
		bool operator == (const TPMDeviceCharacteristicsField& rhs) const {
			return this->Number == rhs.Number;
		}
		TPMDeviceCharacteristicsField() = default;
		explicit TPMDeviceCharacteristicsField(const uint64_t& Num) : Number(Num) {}
	};
#pragma endregion

#pragma region Type44
	struct ProcessorSpecificField
	{
		std::string Type;
		std::vector<uint8_t> Values;

		ProcessorSpecificField() = default;
		bool operator == (const ProcessorSpecificField& rhs) const
		{
			return (this->Type == rhs.Type) &&
				(Values == rhs.Values);
		}

		friend std::ostream& operator<<(std::ostream& out, const ProcessorSpecificField& _myStruct)
		{
			out << " Processor Type: " << _myStruct.Type;
			out << " Values: ";
			for (const auto& Value : _myStruct.Values)
			{
				out << std::setw(2) << std::setfill('0') << std::hex << Value << " ";
			}
			return out;
		}
		friend std::wostream& operator<<(std::wostream& out, const ProcessorSpecificField& _myStruct)
		{
			out << L" Processor Type: " << _myStruct.Type.c_str();
			out << L" Values: ";
			for (const auto& Value : _myStruct.Values)
			{
				out << std::setw(2) << std::setfill(L'0') << std::hex << Value << L" ";
			}
			return out;
		}
	};
#pragma endregion
#pragma pack(pop)
}