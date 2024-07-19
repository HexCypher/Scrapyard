// Version: .001
#pragma once

#ifndef TEXTLOOP_H
#define TEXTLOOP_H

#include <windows.h>
#include <tchar.h>

// Global variables
extern HINSTANCE hInst;
extern HWND hLabel;
extern TCHAR szText[];
extern int textIndex;

// Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void UpdateText();

#endif // TEXTLOOP_H