#include "../CPU/CPU.h"


int main()
{
	CPUID_Data* Data = new CPUID_Data;
	CPU_RES* Res = new CPU_RES;
	ULONG Data_size = 0;
	CCPU* info = Instantiate();
	info->Excute(Data, Data_size);
	info->Get_CPUID(Res);
	cout << sizeof(Data) << "	" << sizeof(Res);
	Free(info);

	//cout << sizeof(BOOL) << " " << sizeof(bool);
	return 0;
}
