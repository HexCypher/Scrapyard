// clipboard.h - version .002

#ifndef CLIPBOARD_H
#define CLIPBOARD_H
#include <iostream>
#include <string>
#include <WinUser.h>

// Declaration of the function to set clipboard text
void SetClipboardText(const std::string& text);

#endif // CLIPBOARD_H
