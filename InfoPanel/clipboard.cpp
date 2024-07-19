// clipboard.cpp - version .004

#include "clipboard.h"

// Definition of the function to set clipboard text
void SetClipboardText(const std::string& text) {
    // Open the clipboard
    if (!OpenClipboard(nullptr)) {
        std::cerr << "Error: Unable to open clipboard." << std::endl;
        return;
    }

    // Clear the clipboard
    if (!EmptyClipboard()) {
        std::cerr << "Error: Unable to empty clipboard." << std::endl;
        CloseClipboard();
        return;
    }

    // Allocate global memory for the text
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hGlobal) {
        std::cerr << "Error: Unable to allocate global memory." << std::endl;
        CloseClipboard();
        return;
    }

    // Lock the global memory and check for success
    LPVOID pGlobal = GlobalLock(hGlobal);
    if (!pGlobal) {
        std::cerr << "Error: Unable to lock global memory." << std::endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        return;
    }

    // Copy the text to the global memory
    memcpy(pGlobal, text.c_str(), text.size() + 1);
    GlobalUnlock(hGlobal);

    // Set the clipboard data
    if (!SetClipboardData(CF_TEXT, hGlobal)) {
        std::cerr << "Error: Unable to set clipboard data." << std::endl;
        GlobalFree(hGlobal); // Free the memory if setting clipboard data fails
    }

    // Close the clipboard
    CloseClipboard();

    // Debug output to verify the clipboard text
    //std::cout << "Clipboard set to: " << text << std::endl;
}
