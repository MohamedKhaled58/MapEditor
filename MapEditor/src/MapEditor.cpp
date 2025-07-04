#include "MapEditor.h"
#include "imgui.h"
#include "DmapLoader.h"
#include <filesystem>
#include <iostream>

MapEditor::MapEditor(int width, int height)
    : m_map(width, height)
{
    m_map.SetMapName("NewMap");
}

void MapEditor::DrawUI()
{
    ImGui::Begin("Map Editor");


    static char imagePath[256] = "assets/download.jpg";

    ImGui::InputText("Background Image", imagePath, IM_ARRAYSIZE(imagePath));
    if (ImGui::Button("Load Background"))
    {
        LoadBackgroundImage(imagePath); // Call your image loader
    }


    static std::string dmapPath = "maps/map1.dmap";
    static std::string dnsFolder = "maps/map1_dns";

    char dmapBuf[256]; strcpy(dmapBuf, dmapPath.c_str());
    char dnsBuf[256]; strcpy(dnsBuf, dnsFolder.c_str());

    if (ImGui::InputText("DMAP File", dmapBuf, sizeof(dmapBuf)))
        dmapPath = dmapBuf;
    if (ImGui::InputText("DNS Folder", dnsBuf, sizeof(dnsBuf)))
        dnsFolder = dnsBuf;

    if (ImGui::Button("Load Map"))
    {
        LoadMap(dmapPath, dnsFolder);
    }




    ImGui::Text("Map: %s", m_map.GetMapName().c_str());

    ImGui::SliderInt("View Width", &m_viewWidth, 5, 100);
    ImGui::SliderInt("View Height", &m_viewHeight, 5, 100);

    ImGui::InputInt("Scroll X", &m_viewOffsetX);
    ImGui::InputInt("Scroll Y", &m_viewOffsetY);

    const float tileSizePx = 32.0f;
    ImVec2 canvasSize = ImVec2(tileSizePx * m_viewWidth, tileSizePx * m_viewHeight);
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(50, 50, 50, 255));

    // Draw grid + tiles
    for (int y = 0; y < m_viewHeight; ++y)
    {
        for (int x = 0; x < m_viewWidth; ++x)
        {
            int mapX = x + m_viewOffsetX;
            int mapY = y + m_viewOffsetY;

            ImVec2 tileMin = ImVec2(canvasPos.x + x * tileSizePx, canvasPos.y + y * tileSizePx);
            ImVec2 tileMax = ImVec2(tileMin.x + tileSizePx, tileMin.y + tileSizePx);

            Tile* tile = m_map.GetTile(mapX, mapY);
            if (tile)
            {
                uint32_t color = tile->GetFlags() & 1 ? IM_COL32(200, 60, 60, 255) : IM_COL32(60, 200, 60, 255);
                drawList->AddRectFilled(tileMin, tileMax, color);
            }

            drawList->AddRect(tileMin, tileMax, IM_COL32(255, 255, 255, 80));
        }
    }

    // Tile click logic
    ImVec2 mousePos = ImGui::GetMousePos();
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
    {
        for (int y = 0; y < m_viewHeight; ++y)
        {
            for (int x = 0; x < m_viewWidth; ++x)
            {
                ImVec2 tileMin = ImVec2(canvasPos.x + x * tileSizePx, canvasPos.y + y * tileSizePx);
                ImVec2 tileMax = ImVec2(tileMin.x + tileSizePx, tileMin.y + tileSizePx);

                if (mousePos.x >= tileMin.x && mousePos.x < tileMax.x &&
                    mousePos.y >= tileMin.y && mousePos.y < tileMax.y)
                {
                    m_selectedX = m_viewOffsetX + x;
                    m_selectedY = m_viewOffsetY + y;
                }
            }
        }
    }

    // Draw selection highlight
    if (m_selectedX >= 0 && m_selectedY >= 0)
    {
        int localX = m_selectedX - m_viewOffsetX;
        int localY = m_selectedY - m_viewOffsetY;
        if (localX >= 0 && localX < m_viewWidth && localY >= 0 && localY < m_viewHeight)
        {
            ImVec2 selMin = ImVec2(canvasPos.x + localX * tileSizePx, canvasPos.y + localY * tileSizePx);
            ImVec2 selMax = ImVec2(selMin.x + tileSizePx, selMin.y + tileSizePx);
            drawList->AddRect(selMin, selMax, IM_COL32(255, 255, 0, 255), 0, 0, 3.0f);
        }
    }

    ImGui::Dummy(canvasSize); // Reserve space

    // Selected tile info and editing
    if (Tile* tile = m_map.GetTile(m_selectedX, m_selectedY))
    {
        uint32_t flags = tile->GetFlags();
        uint16_t height = tile->GetHeight();
        uint16_t texIndex = tile->GetTextureIndex();
        uint32_t overlay = tile->GetOverlay();

        ImGui::Separator();
        ImGui::Text("Editing tile at (%d,%d)", m_selectedX, m_selectedY);

        ImGui::InputScalar("Flags (uint32)", ImGuiDataType_U32, &flags);
        ImGui::InputScalar("Height", ImGuiDataType_U16, &height);
        ImGui::InputScalar("Texture Index", ImGuiDataType_U16, &texIndex);
        ImGui::InputScalar("Overlay", ImGuiDataType_U32, &overlay);

        if (ImGui::Button("Apply Changes"))
        {
            tile->SetFlags(flags);
            tile->SetHeight(height);
            tile->SetTextureIndex(texIndex);
            tile->SetOverlay(overlay);
        }
    }

    if (ImGui::Button("Export Map (.dns + .dmap)"))
    {
        ExportMap("exported_dns", "exported_map.dmap");
    }

    ImGui::End();
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
    m_backgroundPath = path;
    m_backgroundImage = std::make_shared<Texture2D>();
    if (!m_backgroundImage->LoadFromFile(path))
    {
        m_backgroundImage.reset();
        std::cerr << "Failed to load background image: " << path << "\n";
    }
}



void MapEditor::ExportMap(const std::string& dnsFolder, const std::string& dmapFile)
{
    if (!DnsExporter::ExportMapToDns(m_map, dnsFolder))
    {
        std::cerr << "❌ Failed to export DNS tiles.\n";
        return;
    }

    if (!DmapExporter::ExportDmap(m_map, dmapFile, dnsFolder))
    {
        std::cerr << "❌ Failed to export DMAP.\n";
        return;
    }

    std::cout << "✅ Map export complete: " << dmapFile << "\n";
}

