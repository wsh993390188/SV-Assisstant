#include "stdafx.h"
#include "Superio.h"
#include "SioChip.h"
#include "..\driver\Driverdll.h"

class SuperIOInterface
{
public:
	typedef std::pair<std::map<USHORT, std::vector<USHORT>>, std::map< USHORT, std::map<USHORT, std::string> > > SioPageDB;
	SuperIOInterface() : Sio{ std::make_shared<SuperIoEntry>() }, SioChip{ nullptr }
	{
#ifdef ZX_OutputLog
		Logger::Instance()->OutputLogInfo(el::Level::Debug, "***** SuperIo info *****");
		std::string ttt = boost::str(boost::format("0x%lx") % Sio->ChipID.first);
		Logger::Instance()->OutputLogInfo(el::Level::Debug, std::string("Chip ID:") + ttt + std::string(" Chip Name:") + Sio->ChipID.second);
#endif // ZX_OutputLog
		if (Sio->IsFintek())
			SioChip = std::make_shared<F718XX>(Sio->ChipID, Sio->LPCBASE());
		else if (Sio->IsIte())
			SioChip = std::make_shared<IT87XX>(Sio->ChipID, Sio->LPCBASE());
		else if (Sio->IsNuvoton())
			SioChip = std::make_shared<NCT677X>(Sio->ChipID, Sio->LPCBASE());
		else if (Sio->IsSMSC())
			SioChip = std::make_shared<SMSCXX>(Sio->ChipID, Sio->LPCBASE());
		else if (Sio->IsWinbond())
			SioChip = std::make_shared<W836XX>(Sio->ChipID, Sio->LPCBASE());
		else
			SioChip = std::make_shared<_SioSensor>();
		InitSioDB();
		SioChip->UpdateData();
#ifdef ZX_OutputLog
		Logger::Instance()->OutputLogInfo(el::Level::Debug, "***** End SuperIo info *****\n");
#endif
	}

	SIO_Model GetSuperIoChipId() const
	{
		return (SIO_Model)Sio->ChipID.first;
	}

	static SuperIOInterface* Instance()
	{
		if (!temp.get())
			temp = std::make_shared<SuperIOInterface>();
		return temp.get();
	}

	const SioSensor & GetSioSensor() const
	{
		return SioChip->GetSensordata();
	}

	void UpdateData()
	{
		SioChip->UpdateData();
	}

	const std::vector<USHORT>& GetSioPage(IN int Chipid)
	{
		if (pageDB.first.count(Chipid) > 0)
		{
			const auto& temp = pageDB.first.at(Chipid);
			return temp;
		}
		else
		{
			const auto& temp = pageDB.first.at(0);
			return temp;
		}

	}

	void InitSioDB()
	{
		#pragma region Unknown
		pageDB.first[0] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
		for(int i = 0; i < 0x20;i++)
		pageDB.second[0][i] = "Logicial Device " + std::to_string(i);
#pragma endregion
		#pragma region W83627HF
		pageDB.first[0x5200] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B };
		pageDB.second[0x5200][0] = "FDC";
		pageDB.second[0x5200][1] = "Parallel Port";
		pageDB.second[0x5200][2] = "UART A";
		pageDB.second[0x5200][3] = "UART B";
		pageDB.second[0x5200][5] = "KBC";
		pageDB.second[0x5200][6] = "CIR";
		pageDB.second[0x5200][7] = "Game Port, MIDI Port, GPIO Port 1";
		pageDB.second[0x5200][8] = "GPIO Port 2";
		pageDB.second[0x5200][9] = "GPIO Port 3, VSB powered";
		pageDB.second[0x5200][10] = "ACPI";
		pageDB.second[0x5200][11] = "Hardware Monitor";
#pragma endregion
		#pragma region W83627THF
		pageDB.first[0x8280] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x07,0x08,0x09,0x0A,0x0B };
		pageDB.second[0x8280][0] = "FDC";
		pageDB.second[0x8280][1] = "Parallel Port";
		pageDB.second[0x8280][2] = "UART A";
		pageDB.second[0x8280][3] = "UART B";
		pageDB.second[0x8280][5] = "KBC";
		pageDB.second[0x8280][7] = "Game Port, MIDI Port, GPIO Port 1, 5";
		pageDB.second[0x8280][8] = "GPIO Port 2";
		pageDB.second[0x8280][9] = "GPIO Port 3, 4";
		pageDB.second[0x8280][10] = "ACPI";
		pageDB.second[0x8280][11] = "Hardware Monitor";
#pragma endregion
		#pragma region W83627DHG
		pageDB.first[0xA020] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C };
		pageDB.second[0xA020][0] = "FDC";
		pageDB.second[0xA020][1] = "Parallel Port";
		pageDB.second[0xA020][2] = "UART A";
		pageDB.second[0xA020][3] = "UART B";
		pageDB.second[0xA020][5] = "KBC";
		pageDB.second[0xA020][6] = "Serial Peripheral Interface";
		pageDB.second[0xA020][7] = "GPIO6";
		pageDB.second[0xA020][8] = "WDTO, PLED";
		pageDB.second[0xA020][9] = "GPIO2, GPIO3, GPIO4, GPIO5";
		pageDB.second[0xA020][10] = "ACPI";
		pageDB.second[0xA020][11] = "Hardware Monitor";
		pageDB.second[0xA020][12] = "PECI, SST";
#pragma endregion
		#pragma region W83627DHG-P
		pageDB.first[0xB070] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C };
		pageDB.second[0xB070][0] = "FDC";
		pageDB.second[0xB070][1] = "Parallel Port";
		pageDB.second[0xB070][2] = "UART A";
		pageDB.second[0xB070][3] = "UART B";
		pageDB.second[0xB070][5] = "KBC";
		pageDB.second[0xB070][6] = "Serial Peripheral Interface";
		pageDB.second[0xB070][7] = "GPIO6";
		pageDB.second[0xB070][8] = "WDTO, PLED";
		pageDB.second[0xB070][9] = "GPIO2, GPIO3, GPIO4, GPIO5";
		pageDB.second[0xB070][10] = "ACPI";
		pageDB.second[0xB070][11] = "Hardware Monitor";
		pageDB.second[0xB070][12] = "PECI, SST";
#pragma endregion
		#pragma region W83627EHF
		pageDB.first[0x8800] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x07,0x08,0x09,0x0A,0x0B };
		pageDB.second[0x8800][0] = "FDC";
		pageDB.second[0x8800][1] = "Parallel Port";
		pageDB.second[0x8800][2] = "UART A";
		pageDB.second[0x8800][3] = "UART B";
		pageDB.second[0x8800][5] = "KBC";
		pageDB.second[0x8800][7] = "GPIO1, GPIO6, Game Port, MIDI Port";
		pageDB.second[0x8800][8] = "WDTO, PLED";
		pageDB.second[0x8800][9] = "GPIO2, GPIO3, GPIO4, GPIO5, SUSLED";
		pageDB.second[0x8800][10] = "ACPI";
		pageDB.second[0x8800][11] = "Hardware Monitor";
#pragma endregion
		#pragma region W83667HG
		pageDB.first[0xA510] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F };
		pageDB.second[0xA510][0] = "FDC";
		pageDB.second[0xA510][1] = "Parallel Port";
		pageDB.second[0xA510][2] = "UART A";
		pageDB.second[0xA510][3] = "UART B";
		pageDB.second[0xA510][5] = "KBC";
		pageDB.second[0xA510][6] = "Serial Peripheral Interface & CIR";
		pageDB.second[0xA510][7] = "GPIO6, GPIO7, GPIO8, GPIO9";
		pageDB.second[0xA510][8] = "WDTO# & GPIO1";
		pageDB.second[0xA510][9] = "GPIO2, GPIO3, GPIO4, GPIO5";
		pageDB.second[0xA510][10] = "ACPI";
		pageDB.second[0xA510][11] = "Hardware Monitor & SB-TSI"; 
		pageDB.second[0xA510][12] = "PECI, SST";
		pageDB.second[0xA510][13] = "VID, BUSSEL";
		pageDB.second[0xA510][14] = "CIR WAKE-UP";
		pageDB.second[0xA510][15] = "GPIO Push-Pull/OD Select";
#pragma endregion
		#pragma region W83667HG-B
		pageDB.first[0xB350] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0F };
		pageDB.second[0xB350][0] = "FDC";
		pageDB.second[0xB350][1] = "Parallel Port";
		pageDB.second[0xB350][2] = "UART A";
		pageDB.second[0xB350][3] = "UART B";
		pageDB.second[0xB350][5] = "KBC";
		pageDB.second[0xB350][6] = "Serial Peripheral Interface & CIR";
		pageDB.second[0xB350][7] = "GPIO6, GPIO7, GPIO8, GPIO9";
		pageDB.second[0xB350][8] = "WDTO# & GPIO1";
		pageDB.second[0xB350][9] = "GPIO2, GPIO3, GPIO4, GPIO5";
		pageDB.second[0xB350][10] = "ACPI";
		pageDB.second[0xB350][11] = "Hardware Monitor & SB-TSI";
		pageDB.second[0xB350][12] = "PECI";
		pageDB.second[0xB350][13] = "VID, BUSSEL";
		pageDB.second[0xB350][15] = "GPIO Push-Pull/OD Select";
#pragma endregion

		#pragma region NCT6771F
		pageDB.first[0xB470] = std::vector<USHORT>{ 0x00, 0x01,0x02,0x03,0x05,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F };
		pageDB.second[0xB470][0] = "FDC";
		pageDB.second[0xB470][1] = "Parallel Port";
		pageDB.second[0xB470][2] = "UART A";
		pageDB.second[0xB470][3] = "IR";
		pageDB.second[0xB470][5] = "KBC";
		pageDB.second[0xB470][6] = "CIR";
		pageDB.second[0xB470][7] = "GPIO6, GPIO7, GPIO8, GPIO9";
		pageDB.second[0xB470][8] = "WDT1, GPIO0, GPIO1";
		pageDB.second[0xB470][9] = "GPIO2, GPIO3, GPIO4, GPIO5";
		pageDB.second[0xB470][10] = "ACPI";
		pageDB.second[0xB470][11] = "Hardware Monitor, SB-TSI, Front Panel LED, Deep S5";
		pageDB.second[0xB470][12] = "PECI, SST";
		pageDB.second[0xB470][13] = "VID, VIDDAC, BUSSEL";
		pageDB.second[0xB470][14] = "CIR WAKEUP";
		pageDB.second[0xB470][15] = "GPIO Push-Pull/OD Select";
#pragma endregion
		#pragma region NCT6776F
		pageDB.first[0xC330] = std::vector<USHORT>{ 0,1,2,3,5,7,8,9,10,11,13,14,15,20,22,23 };
		pageDB.second[0xC330][0] = "FDC";
		pageDB.second[0xC330][1] = "Parallel Port";
		pageDB.second[0xC330][2] = "UART A";
		pageDB.second[0xC330][3] = "UART B, IR";
		pageDB.second[0xC330][5] = "KBC";
		pageDB.second[0xC330][6] = "CIR";
		pageDB.second[0xC330][7] = "GPIO6, GPIO7, GPIO8, GPIO9";
		pageDB.second[0xC330][8] = "WDT1, GPIO0, GPIOA";
		pageDB.second[0xC330][9] = "GPIO1,2,3,4,5,6,7";
		pageDB.second[0xC330][10] = "ACPI";
		pageDB.second[0xC330][11] = "Hardware Monitor, Front Panel LED";
		pageDB.second[0xC330][13] = "VID";
		pageDB.second[0xC330][14] = "CIR WAKEUP";
		pageDB.second[0xC330][15] = "GPIO";
		pageDB.second[0xC330][20] = "SVID";
		pageDB.second[0xC330][22] = "Deep Sleep";
		pageDB.second[0xC330][23] = "GPIOA";
#pragma endregion

		#pragma region F71862
		pageDB.first[0x0601] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8,10};
		pageDB.second[0x0601][0] = "FDC";
		pageDB.second[0x0601][1] = "UART 1";
		pageDB.second[0x0601][2] = "UART 2";
		pageDB.second[0x0601][3] = "Parallel Port";
		pageDB.second[0x0601][4] = "Hardware Monitor";
		pageDB.second[0x0601][5] = "KBC";
		pageDB.second[0x0601][6] = "GPIO";
		pageDB.second[0x0601][7] = "VID";
		pageDB.second[0x0601][8] = "SPI";
		pageDB.second[0x0601][10] = "PME & ACPI";
#pragma endregion
		#pragma region F71869A
		pageDB.first[0x1007] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8,10 };
		pageDB.second[0x1007][0] = "FDC";
		pageDB.second[0x1007][1] = "UART 1";
		pageDB.second[0x1007][2] = "UART 2";
		pageDB.second[0x1007][3] = "Parallel Port";
		pageDB.second[0x1007][4] = "Hardware Monitor";
		pageDB.second[0x1007][5] = "KBC";
		pageDB.second[0x1007][6] = "GPIO";
		pageDB.second[0x1007][7] = "WDT";
		pageDB.second[0x1007][8] = "CIR";
		pageDB.second[0x1007][10] = "PME,ACPI,ERP Power Saving";
#pragma endregion
		#pragma region F71882
		pageDB.first[0x0541] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8,10 };
		pageDB.second[0x0541][0] = "FDC";
		pageDB.second[0x0541][1] = "UART 1";
		pageDB.second[0x0541][2] = "UART 2";
		pageDB.second[0x0541][3] = "Parallel Port";
		pageDB.second[0x0541][4] = "Hardware Monitor";
		pageDB.second[0x0541][5] = "KBC";
		pageDB.second[0x0541][6] = "GPIO";
		pageDB.second[0x0541][7] = "VID";
		pageDB.second[0x0541][8] = "SPI";
		pageDB.second[0x0541][10] = "PME & ACPI";
#pragma endregion
		#pragma region F71889AD
		pageDB.first[0x1005] = std::vector<USHORT>{ 1,2,3,4,5,6,7,8,10,11 };
		pageDB.second[0x1005][1] = "UART 1";
		pageDB.second[0x1005][2] = "UART 2";
		pageDB.second[0x1005][3] = "Parallel Port";
		pageDB.second[0x1005][4] = "Hardware Monitor";
		pageDB.second[0x1005][5] = "KBC";
		pageDB.second[0x1005][6] = "GPIO";
		pageDB.second[0x1005][7] = "WDT";
		pageDB.second[0x1005][8] = "SPI";
		pageDB.second[0x1005][10] = "PME,ACPI,ERP Power Saving";
		pageDB.second[0x1005][11] = "VREF";
#pragma endregion
		#pragma region F71808E
		pageDB.first[0x0901] = std::vector<USHORT>{1,4,5,6,7,8,10 };
		pageDB.second[0x0901][1] = "UART 1";
		pageDB.second[0x0901][4] = "Hardware Monitor";
		pageDB.second[0x0901][5] = "KBC";
		pageDB.second[0x0901][6] = "GPIO";
		pageDB.second[0x0901][7] = "WDT";
		pageDB.second[0x0901][8] = "CIR";
		pageDB.second[0x0901][10] = "PME,ACPI,Power Saving";
#pragma endregion

		#pragma region IT8705F
		pageDB.first[0x8705] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8 };
		pageDB.second[0x8705][0] = "FDC";
		pageDB.second[0x8705][1] = "Serial Port 1";
		pageDB.second[0x8705][2] = "Serial Port 2";
		pageDB.second[0x8705][3] = "Parallel Port";
		pageDB.second[0x8705][4] = "Environment Controller";
		pageDB.second[0x8705][5] = "GPIO";
		pageDB.second[0x8705][6] = "Game Port";
		pageDB.second[0x8705][7] = "Consumer IR";
		pageDB.second[0x8705][8] = "MIDI Port";
#pragma endregion	
		#pragma region IT8712F
		pageDB.first[0x8712] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8,9,10 };
		pageDB.second[0x8712][0] = "FDC";
		pageDB.second[0x8712][1] = "Serial Port 1";
		pageDB.second[0x8712][2] = "Serial Port 2";
		pageDB.second[0x8712][3] = "Parallel Port";
		pageDB.second[0x8712][4] = "Environment Controller";
		pageDB.second[0x8712][5] = "KBC (Keyboard)";
		pageDB.second[0x8712][6] = "KBC (Mouse)";
		pageDB.second[0x8712][7] = "GPIO";
		pageDB.second[0x8712][8] = "MIDI Port";
		pageDB.second[0x8712][9] = "Game Port";
		pageDB.second[0x8712][10] = "Consumer IR";
#pragma endregion	
		#pragma region IT8716F
		pageDB.first[0x8716] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8,9,10 };
		pageDB.second[0x8716][0] = "FDC";
		pageDB.second[0x8716][1] = "Serial Port 1";
		pageDB.second[0x8716][2] = "Serial Port 2";
		pageDB.second[0x8716][3] = "Parallel Port";
		pageDB.second[0x8716][4] = "Environment Controller";
		pageDB.second[0x8716][5] = "Keyboard";
		pageDB.second[0x8716][6] = "Mouse";
		pageDB.second[0x8716][7] = "GPIO";
		pageDB.second[0x8716][8] = "Game Port";
		pageDB.second[0x8716][9] = "Consumer IR";
		pageDB.second[0x8716][10] = "MIDI Port";
#pragma endregion	
		#pragma region IT8718F
		pageDB.first[0x8718] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,10 };
		pageDB.second[0x8718][0] = "FDC";
		pageDB.second[0x8718][1] = "Serial Port 1";
		pageDB.second[0x8718][2] = "Serial Port 2";
		pageDB.second[0x8718][3] = "Parallel Port";
		pageDB.second[0x8718][4] = "Environment Controller";
		pageDB.second[0x8718][5] = "KBC (Keyboard)";
		pageDB.second[0x8718][6] = "KBC (Mouse)";
		pageDB.second[0x8718][7] = "GPIO";
		pageDB.second[0x8718][10] = "Consumer IR";
#pragma endregion	
		#pragma region IT8720F
		pageDB.first[0x8720] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,10 };
		pageDB.second[0x8720][0] = "FDC";
		pageDB.second[0x8720][1] = "Serial Port 1";
		pageDB.second[0x8720][2] = "Serial Port 2";
		pageDB.second[0x8720][3] = "Parallel Port";
		pageDB.second[0x8720][4] = "Environment Controller";
		pageDB.second[0x8720][5] = "Keyboard";
		pageDB.second[0x8720][6] = "Mouse";
		pageDB.second[0x8720][7] = "GPIO";
		pageDB.second[0x8720][10] = "Consumer IR";
#pragma endregion	
		#pragma region IT8726F
		pageDB.first[0x8726] = std::vector<USHORT>{ 0,1,2,3,4,5,6,7,8,9,10 };
		pageDB.second[0x8726][0] = "FDC";
		pageDB.second[0x8726][1] = "Serial Port 1";
		pageDB.second[0x8726][2] = "Serial Port 2";
		pageDB.second[0x8726][3] = "Parallel Port";
		pageDB.second[0x8726][4] = "Environment Controller";
		pageDB.second[0x8726][5] = "Keyboard";
		pageDB.second[0x8726][6] = "Mouse";
		pageDB.second[0x8726][7] = "GPIO";
		pageDB.second[0x8726][8] = "MIDI Port";
		pageDB.second[0x8726][9] = "Game Port";
		pageDB.second[0x8726][10] = "Consumer IR";
#pragma endregion	
	}

	BOOL ReadSioPage(std::vector<Super_IO_Data>& Siodata, const std::vector<USHORT>& Pages)
	{
		if (Sio->IsIte())
		{
			for (const auto& page : Pages)
			{
				Super_IO_Data data = {};
				Sio->ReadITEPage(page, data);
				Siodata.emplace_back(data);
			}
		}
		else if (Sio->IsFintek() || Sio->IsNuvoton() || Sio->IsWinbond())
		{
			for (const auto& page : Pages)
			{
				Super_IO_Data data = {};
				Sio->ReadWinbondFintekPage(page, data);
				Siodata.emplace_back(data);
			}
		}
		else if (Sio->IsSMSC())
		{
			for (const auto& page : Pages)
			{
				Super_IO_Data data = {};
				Sio->ReadSMSCPage(page, data);
				Siodata.emplace_back(data);
			}
		}
		else
			return FALSE;
		return TRUE;
	}
private:
	explicit SuperIOInterface(const SuperIOInterface& that);
	SuperIOInterface operator=(const SuperIOInterface& that);
	static std::shared_ptr<SuperIOInterface> temp;
	std::shared_ptr<SuperIoEntry> Sio;
	std::shared_ptr<_SioSensor> SioChip;
	SioPageDB pageDB;
};

std::shared_ptr<SuperIOInterface> SuperIOInterface::temp = nullptr;

SIO_Model SV_ASSIST::SUPERIO::GetSuperIoChipId()
{
	return SuperIOInterface::Instance()->GetSuperIoChipId();
}

BOOL SV_ASSIST::SUPERIO::ReadSioPage(std::vector<Super_IO_Data>& Siodata, const std::vector<USHORT>& Pages)
{
	return SuperIOInterface::Instance()->ReadSioPage(Siodata, Pages);
}

const SioSensor & SV_ASSIST::SUPERIO::GetSioSensor()
{
	return SuperIOInterface::Instance()->GetSioSensor();
}

const std::vector<USHORT>& SV_ASSIST::SUPERIO::GetSioPage(IN int Chipid)
{
	return SuperIOInterface::Instance()->GetSioPage(Chipid);
}

void SV_ASSIST::SUPERIO::UpdateData()
{
	SuperIOInterface::Instance()->UpdateData();
}

float ZX_CoreVoltage()
{
	float VCore = INFINITY;
	DWORD64 msrdata = 0;
	if (SV_ASSIST::Ring0::RdMsr(0x198, msrdata) == 1 && msrdata)
	{
		BYTE VID = (msrdata & 0xFF);
		VCore = 1.5f - VID * 0.0125f;
		if (VCore < 0.0f)
			VCore = 0.0f;
	}
	return VCore;
}

float SV_ASSIST::CPU::GetCpuVCore(IN DWORD VendorID)
{
	auto Sioinfo = SuperIOInterface::Instance()->GetSioSensor();
	float temp = INFINITY;
	if (!Sioinfo.Voltages.empty() && temp > Sioinfo.Voltages.at(0))
		temp = Sioinfo.Voltages[0];
	else
	{
		switch (VendorID)
		{
		case 0x8086:
			break;
		case 0x1022:
		case 0x1002:
			break;
		case 0x1106:
			temp = ZX_CoreVoltage();
			break;
		default:
			break;
		}
	}
	return temp;
}

