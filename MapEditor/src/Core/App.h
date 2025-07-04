// Refactored structure for MapEditor

// Directory: src/Core/App.h
#pragma once
#include <Windows.h>

class App {
public:
    bool Initialize(HINSTANCE hInstance);
    int Run();

private:
    HWND hWnd;
    bool CreateMainWindow(HINSTANCE hInstance);
    void MainLoop();
    void Cleanup();
};