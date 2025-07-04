#pragma once
#include "imgui.h"
#include <vector>
#include <DnsExporter.h>

struct GridTile {
    bool walkable;
};


class GridOverlay {
public:
    std::vector<Tile> GetFlatTiles() const;

    void Draw(ImVec2 origin, int tileSize, int imgWidth, int imgHeight);
    void HandleMouse(ImVec2 origin, int tileSize, int imgWidth, int imgHeight);
    std::vector<std::vector<GridTile>>& GetTiles();

private:
    std::vector<std::vector<GridTile>> tiles;
};