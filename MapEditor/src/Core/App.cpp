#include "App.h"
#include "UI/ImGuiSetup.h"
#include "Utils/TextureLoader.h"
#include "Core/GridOverlay.h"
#include "Export/DMapFile.h"
#include <tchar.h>
#include <string>
#include <imgui_impl_win32.h>
#include <algorithm>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Global variables simplified
static ID3D11ShaderResourceView * g_Texture = nullptr;
static int g_ImageWidth = 0, g_ImageHeight = 0;
static int g_TileSize = 32;
static GridOverlay g_Grid;
static char g_ImagePath[256] = "Assets/sample_map.jpg";
static char g_SavePath[256] = "Assets/export.dmap";
static bool g_ImageLoaded = false;
static ID3D11Device* g_Device = nullptr;

extern ID3D11Device* GetD3DDevice();


bool LoadMapTexture() {
    if (g_Texture) g_Texture->Release();
    g_Texture = nullptr;
    return LoadTextureFromFile(g_ImagePath, g_Device, &g_Texture, &g_ImageWidth, &g_ImageHeight);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
            ImGuiLayer::Resize(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool App::CreateMainWindow(HINSTANCE hInstance) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, hInstance, NULL, NULL, NULL, NULL, _T("MapEditorClass"), NULL };
    RegisterClassEx(&wc);
    hWnd = CreateWindow(wc.lpszClassName, _T("Map Editor"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, hInstance, NULL);
    return hWnd != nullptr;
}

bool App::Initialize(HINSTANCE hInstance) {
    if (!CreateMainWindow(hInstance)) return false;
    if (!ImGuiLayer::Init(hWnd)) return false;
    g_Device = ImGuiLayer::GetD3DDevice();
    g_ImageLoaded = LoadMapTexture();
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    return true;
}

int App::Run() {
    MainLoop();
    Cleanup();
    return 0;
}

void App::MainLoop() {
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGuiLayer::BeginFrame();
        ImGui::Begin("Map Editor");
        ImGui::InputText("Image Path", g_ImagePath, IM_ARRAYSIZE(g_ImagePath));
        if (ImGui::Button("Load Image")) g_ImageLoaded = LoadMapTexture();
        ImGui::InputInt("Tile Size", &g_TileSize);
        g_TileSize = std::clamp(g_TileSize, 4, 256);
        ImGui::InputText("Save Path", g_SavePath, IM_ARRAYSIZE(g_SavePath));
        if (ImGui::Button("Export .dmap")) DMapFile::Save(g_SavePath, g_Grid.GetTiles());

        if (g_ImageLoaded && g_Texture) {
            ImGui::Text("Map Size: %d x %d", g_ImageWidth, g_ImageHeight);
            ImVec2 image_pos = ImGui::GetCursorScreenPos();
            ImGui::Image((void*)g_Texture, ImVec2((float)g_ImageWidth, (float)g_ImageHeight));
            g_Grid.HandleMouse(image_pos, g_TileSize, g_ImageWidth, g_ImageHeight);
            g_Grid.Draw(image_pos, g_TileSize, g_ImageWidth, g_ImageHeight);
        }

        ImGui::End();
        ImGuiLayer::EndFrame();
    }
}

void App::Cleanup() {
    if (g_Texture) g_Texture->Release();
    ImGuiLayer::Shutdown();
    DestroyWindow(hWnd);
    UnregisterClass(_T("MapEditorClass"), GetModuleHandle(NULL));
}
