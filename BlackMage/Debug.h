#pragma once
#pragma comment(lib, "advapi32.lib")
#include <Windows.h>
#include <stdio.h>


BOOL SetPrivilege(
    HANDLE hToken,          // token handle
    LPCTSTR Privilege,      // Privilege to enable/disable
    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
);

int enableDebug();