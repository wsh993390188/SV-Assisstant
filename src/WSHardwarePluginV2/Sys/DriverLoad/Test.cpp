// DriverLoad.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件