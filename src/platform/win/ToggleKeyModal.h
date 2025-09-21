#pragma once
#include <windows.h>

class AutoClicker; // Forward declaration

// The function to show the modal dialog.
void ShowToggleKeyModal(HWND parent, HINSTANCE hInstance, AutoClicker* app);