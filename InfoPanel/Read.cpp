#include <iostream>
#include <Windows.h>
#include <vector>
#include "Read.h"


//
//  SetPrivilege enables/disables process token privilege.
//
BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
)
{
    TOKEN_PRIVILEGES tp{};
    LUID luid;

    if (!LookupPrivilegeValue(
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid))        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}


int enableDebug()
{

    HANDLE hProcess = GetCurrentProcess();
    HANDLE hToken;

    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
        CloseHandle(hToken);
    }
    return 0;
}

static void DebugOutput(const char* message) {
    OutputDebugStringA(message);
}

static void DebugOutput(const wchar_t* message) {
    OutputDebugStringW(message);
}

void CreateAndAttachConsole() {
    AllocConsole();

    FILE* fp;
    if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0) {
        std::cerr << "Failed to redirect stdout." << std::endl;
    }
    if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0) {
        std::cerr << "Failed to redirect stderr." << std::endl;
    }
    if (freopen_s(&fp, "CONIN$", "r", stdin) != 0) {
        std::cerr << "Failed to redirect stdin." << std::endl;
    }
}

HANDLE GetHandle(const wchar_t* procName)
{
    DWORD procId = GetProcId(procName);

    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    return hProcess;
}


DWORD GetProcId(const wchar_t* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry{};
        procEntry.dwSize = sizeof(procEntry);
        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;

}


uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* procName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry{};
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {

            do
            {
                if (!_wcsicmp(modEntry.szModule, procName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

//Pointer resolution
uintptr_t FindDMAaddress(HANDLE hProc, uintptr_t baseAddr, const std::vector<unsigned int>& offsets) {
    uintptr_t addr = baseAddr;
     // std::cout << "Base Address: " << std::hex << addr << std::endl;

    for (unsigned int i = 0; i < offsets.size(); ++i) {
        DWORD tempAddr;  // Using DWORD to read 4 bytes as Cheat Engine does
        if (!ReadProcessMemory(hProc, (LPCVOID)addr, &tempAddr, sizeof(tempAddr), nullptr)) {
            DWORD error = GetLastError();
            std::cerr << "Error reading memory at address: " << std::hex << addr
                << std::dec << " (Error Code: " << error << ")" << std::endl;
            if (error == ERROR_NOACCESS) {
                std::cerr << "ERROR_NOACCESS: Invalid access to memory location." << std::endl;
            }
            return 0; // Return 0 or handle the error as appropriate
        }

        // std::cout << "Read memory at address: " << std::hex << addr << " with value: " << tempAddr << " and offset: " << offsets[i] << " Read->FindDMAAddress" << std::endl;

        addr = (uintptr_t)tempAddr + offsets[i]; // Move to the next address in the chain
    }
    return addr;
}


byte GetCombatFlag(HANDLE handle, uintptr_t combatFlagAddr) {
    // std::cout << "combatFlagAddr: " << combatFlagAddr << " Read->GetCombatFlag" << std::endl;
    byte flag = 0;  // Initialize flag
    SIZE_T bytesRead;
    if (!ReadProcessMemory(handle, (LPCVOID)combatFlagAddr, &flag, sizeof(flag), &bytesRead)) {
        std::cerr << "Error reading memory at address: " << std::hex << combatFlagAddr
            << std::dec << " (Error Code: " << GetLastError() << ")" << std::endl;
        return 0; // Return 0 or handle the error as appropriate
    }

    if (bytesRead != sizeof(flag)) {
        std::cerr << "Error: Not all bytes read. Expected: " << sizeof(flag) << " Actual: " << bytesRead << std::endl;
        return 0;
    }
    return flag;
}

float getX(uintptr_t moduleBase, HANDLE handle)
{
    float x = 0000.00;
    uintptr_t xBaseAddr = moduleBase + 0x87B544;
    ReadProcessMemory(handle, (BYTE*)xBaseAddr, &x, sizeof(x), nullptr);
    return x;
}

float getY(uintptr_t moduleBase, HANDLE handle)
{
    float y = 0000.00;
    uintptr_t yBaseAddr = moduleBase + 0x87B548;
    ReadProcessMemory(handle, (BYTE*)yBaseAddr, &y, sizeof(y), nullptr);
    return y;
}

float getZ(uintptr_t moduleBase, HANDLE handle)
{
    float z = 0000.00;
    uintptr_t zBaseAddr = moduleBase + 0x87B54C;
    ReadProcessMemory(handle, (BYTE*)zBaseAddr, &z, sizeof(z), nullptr);
    return z;
}

static void writeToFile(const std::string& filePath, int value) {
    // Ensure the value is within the valid range
    if (value != 0 && value != 1 && value != 2) {
        std::cerr << "Error: Invalid value. Only 0, 1, or 2 are allowed." << std::endl;
        return;
    }

    // Try to open the file and handle potential errors
    try {
        std::ofstream outFile(filePath, std::ios::trunc); // Open file in truncate mode to overwrite
        if (!outFile) {
            throw std::ios_base::failure("Failed to open the file.");
        }

        // Write the value to the file
        outFile << value;
        outFile.close();

        std::cout << "Successfully wrote " << value << " to " << filePath << std::endl;
    }
    catch (const std::ios_base::failure& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

//Pointer Addresses
/*    // Set the file path
    std::string filePath = "output.txt";

    // Example values to write to the file
    int combatState = 1; // This value would be retrieved from your game memory

    // Call the function to write to the file
    writeToFile(filePath, combatState);
*/


