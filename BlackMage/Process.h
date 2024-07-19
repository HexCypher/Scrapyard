#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(const wchar_t* procName);

uintptr_t FindDMAaddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
