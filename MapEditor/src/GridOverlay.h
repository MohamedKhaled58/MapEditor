#pragma once
#include "imgui.h"
#include <vector>

struct GridTile {
    bool walkable = true;
};

class GridOverlay {
public:
    void Draw(ImVec2 origin, int tileSize, int imgWidth, int imgHeight);
    void HandleMouse(ImVec2 origin, int tileSize, int imgWidth, int imgHeight);
    std::vector<std::vector<GridTile>>& GetTiles();

private:
    std::vector<std::vector<GridTile>> tiles;
};