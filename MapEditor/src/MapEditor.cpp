// MapEditor.cpp
#include "MapEditor.h"
#include "imgui.h"
#include "DmapLoader.h"
#include "DnsExporter.h"
#include "DmapExporter.h"
#include "TileExporter.h"
#include <filesystem>
#include <iostream>
#include "../vendor/libigl-stb/stb_image.h"

inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x + b.x, a.y + b.y); }

MapEditor::MapEditor(int width, int height)
    : m_map(width, height)
{
    m_map.SetMapName("NewMap");
}

void MapEditor::DrawUI()
{
    // === DockSpace ===
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(2);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
    ImGui::End();

    // === Controls Panel ===
    ImGui::Begin("Controls");

    static char imagePath[256] = "assets/background.jpg";
    ImGui::InputText("Background Image", imagePath, IM_ARRAYSIZE(imagePath));
    if (ImGui::Button("Load Background"))
        LoadBackgroundImage(imagePath);

    static std::string dmapPath = "maps/map1.dmap";
    static std::string dnsFolder = "maps/map1_dns";
    char dmapBuf[256]; strcpy(dmapBuf, dmapPath.c_str());
    char dnsBuf[256]; strcpy(dnsBuf, dnsFolder.c_str());

    if (ImGui::InputText("DMAP File", dmapBuf, sizeof(dmapBuf))) dmapPath = dmapBuf;
    if (ImGui::InputText("DNS Folder", dnsBuf, sizeof(dnsBuf))) dnsFolder = dnsBuf;

    if (ImGui::Button("Load Map")) LoadMap(dmapPath, dnsFolder);
    if (ImGui::Button("Import Image As Tiles")) ImportImageAsTiles(imagePath, 32);
    if (ImGui::Button("Export Map (.dns + .dmap)")) ExportMap("exported_dns", "exported_map.dmap");
    if (ImGui::Button("Export Walkability as PNG")) TileExporter::ExportTilesAsImages(m_map, "tiles_png");

    ImGui::SliderInt("View Width", &m_viewWidth, 5, 100);
    ImGui::SliderInt("View Height", &m_viewHeight, 5, 100);
    ImGui::InputInt("Scroll X", &m_viewOffsetX);
    ImGui::InputInt("Scroll Y", &m_viewOffsetY);
    ImGui::End();

    // === Viewport ===
    ImGui::Begin("Viewport");
    const float tileSizePx = 32.0f;
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize(tileSizePx * m_viewWidth, tileSizePx * m_viewHeight);
    ImVec2 canvasMax = canvasPos + canvasSize;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(canvasPos, canvasMax, IM_COL32(30, 30, 30, 255));

    if (m_backgroundImage && m_backgroundImage->GetSRV())
    {
        // Use the texture, for example:
        drawList->AddImage(m_backgroundImage->GetSRV(), canvasPos, canvasMax);
    }
    else
    {
        // لا تستخدم الخلفية أو أظهر رسالة / لون افتراضي
        // مثلاً تلوين الخلفية بلون رمادي فاتح:
        drawList->AddRectFilled(canvasPos, canvasMax, IM_COL32(100, 100, 100, 255));
    }


    for (int y = 0; y < m_viewHeight; ++y)
    {
        for (int x = 0; x < m_viewWidth; ++x)
        {
            int mapX = x + m_viewOffsetX;
            int mapY = y + m_viewOffsetY;

            ImVec2 tileMin = canvasPos + ImVec2(x * tileSizePx, y * tileSizePx);
            ImVec2 tileMax = tileMin + ImVec2(tileSizePx, tileSizePx);

            const Tile* tile = m_map.GetTile(mapX, mapY);
            if (tile)
            {
                uint32_t color = (tile->GetFlags() & 1) ? IM_COL32(200, 60, 60, 255) : IM_COL32(100, 200, 100, 255);
                drawList->AddRectFilled(tileMin, tileMax, color);
            }

            drawList->AddRect(tileMin, tileMax, IM_COL32(255, 255, 255, 40));
        }
    }

    // Mouse selection
    ImVec2 mousePos = ImGui::GetMousePos();
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
    {
        for (int y = 0; y < m_viewHeight; ++y)
        {
            for (int x = 0; x < m_viewWidth; ++x)
            {
                ImVec2 tileMin = canvasPos + ImVec2(x * tileSizePx, y * tileSizePx);
                ImVec2 tileMax = tileMin + ImVec2(tileSizePx, tileSizePx);
                if (mousePos.x >= tileMin.x && mousePos.x < tileMax.x &&
                    mousePos.y >= tileMin.y && mousePos.y < tileMax.y)
                {
                    m_selectedX = m_viewOffsetX + x;
                    m_selectedY = m_viewOffsetY + y;
                }
            }
        }
    }

    // Highlight selected
    if (m_selectedX >= 0 && m_selectedY >= 0)
    {
        int lx = m_selectedX - m_viewOffsetX;
        int ly = m_selectedY - m_viewOffsetY;
        if (lx >= 0 && ly >= 0 && lx < m_viewWidth && ly < m_viewHeight)
        {
            ImVec2 selMin = canvasPos + ImVec2(lx * tileSizePx, ly * tileSizePx);
            ImVec2 selMax = selMin + ImVec2(tileSizePx, tileSizePx);
            drawList->AddRect(selMin, selMax, IM_COL32(255, 255, 0, 255), 0, 0, 3.0f);
        }
    }

    ImGui::Dummy(canvasSize);
    ImGui::End();

    // === Tile Inspector ===
    if (Tile* tile = m_map.GetTile(m_selectedX, m_selectedY))
    {
        ImGui::Begin("Tile Inspector");
        uint32_t flags = tile->GetFlags();
        uint16_t height = tile->GetHeight();
        uint16_t texIndex = tile->GetTextureIndex();
        uint32_t overlay = tile->GetOverlay();

        ImGui::Text("Editing tile at (%d,%d)", m_selectedX, m_selectedY);
        ImGui::InputScalar("Flags", ImGuiDataType_U32, &flags);
        ImGui::InputScalar("Height", ImGuiDataType_U16, &height);
        ImGui::InputScalar("Texture", ImGuiDataType_U16, &texIndex);
        ImGui::InputScalar("Overlay", ImGuiDataType_U32, &overlay);
        if (ImGui::Button("Apply Changes"))
        {
            tile->SetFlags(flags);
            tile->SetHeight(height);
            tile->SetTextureIndex(texIndex);
            tile->SetOverlay(overlay);
        }
        ImGui::End();
    }
}

void MapEditor::LoadMap(const std::string& dmapFile, const std::string& dnsFolder)
{
    if (DmapLoader::LoadMap(m_map, dmapFile, dnsFolder))
    {
        m_selectedX = m_selectedY = -1;
        m_map.SetMapName(std::filesystem::path(dmapFile).stem().string());
    }
}

void MapEditor::LoadBackgroundImage(const std::string& path)
{
    std::cerr << "Trying to load background image from: " << std::filesystem::absolute(path) << "\n";
    m_backgroundPath = path;
    m_backgroundImage = std::make_shared<Texture2D>();
    if (!m_backgroundImage->LoadFromFile(path))
    {
        m_backgroundImage.reset();
        std::cerr << "Failed to load background image: " << path << "\n";
        std::cerr << "STB error: " << stbi_failure_reason() << "\n";
    }
    else
    {
        std::cout << "Background image loaded successfully: " << path << "\n";
    }

}



void MapEditor::ExportMap(const std::string& dnsFolder, const std::string& dmapFile)
{
    if (!DnsExporter::ExportMapToDns(m_map, dnsFolder))
        std::cerr << "❌ Failed to export DNS.\n";
    if (!DmapExporter::ExportDmap(m_map, dmapFile, dnsFolder))
        std::cerr << "❌ Failed to export DMAP.\n";
}

void MapEditor::ImportImageAsTiles(const std::string& path, int tileSize)
{
    int w, h, n;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &n, 0);
    if (!data)
    {
        std::cerr << "Failed to load image for tiles: " << path << "\n";
        return;
    }

    m_map.Resize(w / tileSize, h / tileSize);
    for (int y = 0; y < m_map.GetHeight(); ++y)
    {
        for (int x = 0; x < m_map.GetWidth(); ++x)
        {
            int pixelX = x * tileSize;
            int pixelY = y * tileSize;
            int index = (pixelY * w + pixelX) * n;

            bool blocked = (data[index] < 128); // red channel
            Tile* tile = m_map.GetTile(x, y);
            if (tile)
                tile->SetFlags(blocked ? 1 : 0);
        }
    }

    stbi_image_free(data);
}
