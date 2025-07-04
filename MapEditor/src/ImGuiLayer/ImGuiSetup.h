#pragma once

#include <Windows.h>
#include <d3d11.h>

namespace ImGuiLayer {
    bool Init(HWND hwnd);
    void BeginFrame();
    void EndFrame();
    void Shutdown();
    void Resize(UINT width, UINT height);
    ID3D11Device* GetD3DDevice();
}