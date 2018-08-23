#pragma once

struct Super_IO_Data
{
	unsigned char SuperioData[256];
};

enum SIO_Model
{
	SIO_Unknown = 0,

	ATK0110 = 0x0110,

	F71858 = 0x0507,//none
	F71862 = 0x0601,
	F71869 = 0x0814,//none
	F71869A = 0x1007,
	F71882 = 0x0541,
	F71889AD = 0x1005,
	F71889ED = 0x0909,//none
	F71889F = 0x0723,//none
	F71808E = 0x0901,

	IT8620E = 0x8620,//none
	IT8628E = 0x8628,//none
	IT8665E = 0x8665,//none
	IT8705F = 0x8705,
	IT8712F = 0x8712,
	IT8716F = 0x8716,
	IT8718F = 0x8718,
	IT8720F = 0x8720,
	IT8721F = 0x8721,//none
	IT8726F = 0x8726,
	IT8728F = 0x8728,//none
	IT8733E = 0x8733,//none
	IT8771E = 0x8771,//none
	IT8772E = 0x8772,//none

	NCT6771F = 0xB470,
	NCT6776F = 0xC330,
	NCT610X = 0xC452,//none
	NCT6779D = 0xC560,//none
	NCT6791D = 0xC803,//none

	W83627DHG = 0xA020,
	W83627DHGP = 0xB070,
	W83627EHF = 0x8800,
	W83627HF = 0x5200,
	W83627THF = 0x8280,
	W83667HG = 0xA510,
	W83667HGB = 0xB350,
	W83687THF = 0x8541//none
};

struct SioSensor
{
	SioSensor();
	std::string ChipName;
	std::vector<float> Voltages;
	std::vector<float> Temperatures;
	std::vector<float> Fans;
	std::vector<float> Controls;
};