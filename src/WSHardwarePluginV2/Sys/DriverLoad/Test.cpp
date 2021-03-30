// DriverLoad.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include "stdafx.h"
#include "WinRing0.h"
#include <iostream>
#include <iomanip>

using namespace Hardware;
int main()
{
	/*
	{
		DWORD Iodata = 0;
		for (int j = 0; j <= 0xF; ++j)
			std::cout << std::setw(2) << std::setfill('0') << std::hex << j << " ";
		for (int i = 0; i <= 0xFF; ++i)
		{
			WinRing0::Instance().RdIOPort(i, 1, Iodata);
			if (i % 16 == 0)
				std::cout << std::endl;
			std::cout << std::setw(2) << std::setfill('0') << std::hex << Iodata << " ";
		}
		getchar();
		system("cls");
	}
	{
		DWORD Pcidata = 0;
		std::cout << "PciB:0, D:0,F:0" << std::endl;
		for (int j = 0; j <= 0xF; ++j)
			std::cout << std::setw(2) << std::setfill('0') << std::hex << j << " ";
		for (int i = 0; i <= 0xFF; ++i)
		{
			WinRing0::Instance().ReadPci(0, 0, 0, i, 1, Pcidata);
			if (i % 16 == 0)
				std::cout << std::endl;
			std::cout << std::setw(2) << std::setfill('0') << std::hex << Pcidata << " ";
		}
		getchar();
		system("cls");
	}*/
	{
		DWORD Memorydata = 0;
		DWORD MemoryBase = 0xE000'0000;
		std::cout << "Memory" << std::endl;
		for (int j = 0; j <= 0xF; ++j)
			std::cout << std::setw(2) << std::setfill('0') << std::hex << j << " ";
		for (int i = 0; i <= 0xFF; ++i)
		{
			WinRing0::Instance().RdMemory(MemoryBase + i, 1, Memorydata);
			if (i % 16 == 0)
				std::cout << std::endl;
			std::cout << std::setw(2) << std::setfill('0') << std::hex << Memorydata << " ";
		}
		getchar();
		system("cls");
	}

	DWORD64 MsrData = 0;
	WinRing0::Instance().RdMsr(0x250, MsrData);

	{
		double res = 0.0;
		int i = 10;
		while (i--)
		{
			WinRing0::Instance().RdTsc(MsrData);
			res += (MsrData / 1000000.0);
		}
		
		std::cout << "Tsc: " << res / 2300.0 * 10.0 << "Mhz" << std::endl;
	}

}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���:
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�