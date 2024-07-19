// InfoPanel.cpp - version .004 

#include "InfoPanel.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
DWORD procID;
HANDLE handle;
uintptr_t combatFlagAddr;
uintptr_t moduleBase;
uintptr_t playerBaseAddr = 0x00C7BCD4;
std::vector<unsigned int> CombatFlagOffsets = { 0x88, 0x28, 0x8, 0xBA };
WNDCLASSEX wcex;
MSG msg;
const wchar_t* procName = L"REDACTED";
wchar_t xText[100];
wchar_t yText[100];
wchar_t zText[100];
wchar_t combatText[100];
wchar_t vxText[100];
wchar_t vyText[100];
wchar_t vzText[100];
int textIndex = 0;
int combatFlag;
float playerX;
float playerY;
float playerZ;
Camera* camera;

// Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    //CreateAndAttachConsole();

    enableDebug();
    procID = GetProcId(procName);
    handle = GetHandle(procName);
    moduleBase = GetModuleBaseAddress(procID, procName);

    combatFlagAddr = FindDMAaddress(handle, playerBaseAddr, CombatFlagOffsets);
    if (combatFlagAddr != 0) {
        uintptr_t finalValue;
        if (ReadProcessMemory(handle, (LPCVOID)combatFlagAddr, &finalValue, sizeof(finalValue), nullptr)) {
            // std::cout << "Final address: " << std::hex << combatFlagAddr << " Value at final address: " << std::hex << finalValue << " InfoPanel->combatFlagAddr" << std::endl;
        }
        else {
            std::cerr << "Failed to read value at final address (Error Code: " << GetLastError() << ")" << std::endl;
        }
    }
    else {
        std::cerr << "Failed to find final address." << std::endl;
    }

    // Initialize the Camera
    camera = new Camera(handle);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INFOPANEL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INFOPANEL));

    // Main message loop:
    // .001
    while (true)
    {
        BOOL bRet = GetMessage(&msg, nullptr, 0, 0);
        if (bRet > 0)  // A message is received
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else if (bRet == 0)  // WM_QUIT is received
        {
            std::cerr << "WM_QUIT received. Exiting message loop." << std::endl;
            break;
        }
        else  // An error occurred
        {
            std::cerr << "GetMessage failed with error code: " << GetLastError() << std::endl;
            break;
        }
    }

    delete camera;  // Clean up the Camera instance

    return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INFOPANEL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    // Set the window position and size
    SetWindowPos(hWnd, HWND_TOP, 100, 100, 400, 300, SWP_NOZORDER);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static UINT_PTR timerID;

    switch (message)
    {
    case WM_CREATE:
        // Set a timer to update the text
        timerID = SetTimer(hWnd, 1, 250, nullptr);  // Update every 250ms
        break;

    case WM_TIMER:
        if (wParam == 1)
        {
            // Get the XYZ coordinates
            float x = getX(moduleBase, handle);
            float y = getY(moduleBase, handle);
            float z = getZ(moduleBase, handle);

            if (!x || !y || !z)
            {
                swprintf(xText, 100, L"X: nil");
                swprintf(yText, 100, L"Y: nil");
                swprintf(zText, 100, L"Z: nil");
            }
            else
            {
                swprintf(xText, 100, L"X: %f", x);
                swprintf(yText, 100, L"Y: %f", y);
                swprintf(zText, 100, L"Z: %f", z);
            }

            // Get Combat Flag
            combatFlag = GetCombatFlag(handle, combatFlagAddr);

            if (!combatFlag)
                swprintf(combatText, 100, L"No Combat");
            else
                swprintf(combatText, 100, L"In Combat");

            // Update the clipboard with the combat flag state
            std::string combatState = std::to_string(combatFlag);
            SetClipboardText(combatState);

            // Read yaw value
            DWORD CameraYawAddress = 0x00C7B708;  // Replace with the actual address
            camera->readYaw(CameraYawAddress);
            float currentYaw = camera->getYaw();

            // Waypoint coordinates (example values, replace with actual waypoints)
            float waypointX = 1342.167725f;
            float waypointY = -4360.056152f;

            // Check if facing the waypoint with a tolerance of 15 degrees
            bool facingWaypoint = isFacingWaypoint(currentYaw, waypointX, waypointY, x, y, 15.0f);
            wchar_t facingText[100];
            swprintf(facingText, 100, facingWaypoint ? L"Facing Waypoint" : L"Not Facing Waypoint");

            swprintf(vxText, 100, L"Yaw: %f", currentYaw);
            swprintf(vyText, 100, L"%s", facingText);

            // Force the window to repaint
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT: // v.002
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        wchar_t procIDText[100];
        wchar_t handleText[100];
        wchar_t moduleBaseText[100];

        swprintf(procIDText, 100, L"ProcID: %d", procID);
        TextOut(hdc, 10, 0, procIDText, lstrlen(procIDText));

        swprintf(handleText, 100, L"Handle: %p", (void*)handle);
        TextOut(hdc, 10, 15, handleText, lstrlen(handleText));

        swprintf(moduleBaseText, 100, L"Base Address: %p", (void*)(UINT_PTR)moduleBase);
        TextOut(hdc, 10, 30, moduleBaseText, lstrlen(moduleBaseText));

        TextOut(hdc, 10, 45, xText, lstrlen(xText));
        TextOut(hdc, 10, 60, yText, lstrlen(yText));
        TextOut(hdc, 10, 75, zText, lstrlen(zText));

        TextOut(hdc, 10, 90, combatText, lstrlen(combatText));

        TextOut(hdc, 10, 105, vxText, lstrlen(vxText));
        TextOut(hdc, 10, 120, vyText, lstrlen(vyText));
        TextOut(hdc, 10, 135, vzText, lstrlen(vzText));

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
