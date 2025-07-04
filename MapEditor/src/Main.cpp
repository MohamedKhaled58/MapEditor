
#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    App app;
    if (!app.Initialize(hInstance))
        return -1;

    return app.Run();
}
