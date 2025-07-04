#include "MapEditor.h"
#include "imgui.h"

MapEditor::MapEditor(int width, int height)
    : m_map(width, height)
{
    m_map.SetMapName("NewMap");
}

void MapEditor::DrawUI()
{
    ImGui::Begin("Map Editor");

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


void MapEditor::ExportMap(const std::string& dnsFolder, const std::string& dmapFile)
{
    // Export all tiles to .dns files
    std::vector<Tile> allTiles;
    for (int y = 0; y < m_map.GetHeight(); ++y)
    {
        for (int x = 0; x < m_map.GetWidth(); ++x)
        {
            Tile* tile = m_map.GetTile(x, y);
            if (tile)
                allTiles.push_back(*tile);
        }
    }

    if (!DnsExporter::ExportTilesToDns(allTiles, dnsFolder))
    {
        // Handle error
    }

    // Export the .dmap file
    if (!DmapExporter::ExportDmap(m_map, dmapFile, dnsFolder))
    {
        // Handle error
    }
}
