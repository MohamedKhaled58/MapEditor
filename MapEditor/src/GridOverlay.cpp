#include "GridOverlay.h"
#include "imgui.h"


std::vector<Tile> GridOverlay::GetFlatTiles() const {
    std::vector<Tile> flatTiles;
    for (int y = 0; y < tiles.size(); ++y) {
        for (int x = 0; x < tiles[y].size(); ++x) {
            Tile t;
            t.x = x;
            t.y = y;
            t.walkable = tiles[y][x].walkable;
            flatTiles.push_back(t);
        }
    }
    return flatTiles;
}

void GridOverlay::Draw(ImVec2 origin, int tileSize, int imgWidth, int imgHeight)
{
    int cols = imgWidth / tileSize;
    int rows = imgHeight / tileSize;

    if (tiles.size() != cols || (cols > 0 && tiles[0].size() != rows)) {
        tiles.resize(cols, std::vector<GridTile>(rows, GridTile{ true }));
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int x = 0; x < cols; ++x)
    {
        for (int y = 0; y < rows; ++y)
        {
            ImVec2 tl = ImVec2(origin.x + x * tileSize, origin.y + y * tileSize);
            ImVec2 br = ImVec2(tl.x + tileSize, tl.y + tileSize);
            ImU32 color = tiles[x][y].walkable ? IM_COL32(0, 255, 0, 50) : IM_COL32(255, 0, 0, 120);
            draw_list->AddRectFilled(tl, br, color);
            draw_list->AddRect(tl, br, IM_COL32(255, 255, 255, 40));
        }
    }
}

void GridOverlay::HandleMouse(ImVec2 origin, int tileSize, int imgWidth, int imgHeight)
{
    ImVec2 mouse = ImGui::GetIO().MousePos;
    if (!ImGui::IsMouseDown(0))
        return;

    int x = (int)((mouse.x - origin.x) / tileSize);
    int y = (int)((mouse.y - origin.y) / tileSize);

    int cols = imgWidth / tileSize;
    int rows = imgHeight / tileSize;
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        tiles[x][y].walkable = false;
    }
}

std::vector<std::vector<GridTile>>& GridOverlay::GetTiles() {
    return tiles;
}