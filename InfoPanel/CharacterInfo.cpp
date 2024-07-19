// InfoPanel.cpp : Defines the entry point for the application.
//
#include <sstream>
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "framework.h"
#include "InfoPanel.h"
#include "TextLoop.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
DWORD procID;
HANDLE handle;
uintptr_t moduleBase;
WNDCLASSEX wcex;
MSG msg;
const wchar_t* procName = L"WoW.exe";
wchar_t xText[100];
wchar_t yText[100];
wchar_t zText[100];
int textIndex = 0;


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
    enableDebug();
    procID = GetProcId(procName);
    handle = GetHandle(procName);
    moduleBase = GetModuleBaseAddress(procID, procName);

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
            std::cout << "WM_QUIT received. Exiting message loop." << std::endl;
            break;
        }
        else  // An error occurred
        {
            std::cerr << "GetMessage failed with error code: " << GetLastError() << std::endl;
            break;
        }
    }

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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static UINT_PTR timerID;

    switch (message)
    {
    case WM_CREATE:
        // Set a timer to update the text
        timerID = SetTimer(hWnd, 1, 1000, nullptr);
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
