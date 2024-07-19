#pragma once
#pragma comment(lib, "advapi32.lib")
#include <vector>
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <stdio.h>
#include <memoryapi.h>
#include <cstdio>
#include <fstream>
#include <string>

BOOL SetPrivilege(
    HANDLE hToken,          // token handle
    LPCTSTR Privilege,      // Privilege to enable/disable
    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
);

int enableDebug();
void CreateAndAttachConsole();
HANDLE GetHandle(const wchar_t* procName);
DWORD GetProcId(const wchar_t* procName);
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
uintptr_t FindDMAaddress(HANDLE hProc, uintptr_t ptr, const std::vector<unsigned int>& offsets);
byte GetCombatFlag(HANDLE hProc, uintptr_t combatFlagAddr);

float getX(uintptr_t moduleBase, HANDLE handle);
float getY(uintptr_t moduleBase, HANDLE handle);
float getZ(uintptr_t moduleBase, HANDLE handle);



