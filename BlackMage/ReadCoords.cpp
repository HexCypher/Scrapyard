#include "ReadCoords.h"
#include "BaseAddr.h"
#include "Process.h"
#include "Handle.h"
#include <iostream>

float getX(const wchar_t* procName)
{
	DWORD procId = GetProcId(procName);

	HANDLE hProcess = getHandle(procName);

	uintptr_t moduleBase = GetModuleBaseAddress(procId, procName);

	std::cout << moduleBase << std::endl;


	uintptr_t xBaseAddr = moduleBase + 0x1DBC1A0;
	std::cout << xBaseAddr << std::endl;
	
	float x = 000.00;

	uintptr_t realAddr = 0x7ff76077c1a0;
	std::cout << realAddr << std::endl;

	ReadProcessMemory(hProcess, (BYTE*)xBaseAddr, &x, sizeof(x), nullptr);

	return x;
}