
#pragma once

#include <Windows.h>

class App {
public:
    bool Initialize(HINSTANCE hInstance);
    int Run();

private:
    HWND hWnd = nullptr;
    bool CreateMainWindow(HINSTANCE hInstance);
    void MainLoop();
    void Cleanup();
};
