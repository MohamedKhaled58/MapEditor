#pragma once
#include <array>
#include "Tile.h"

constexpr int SECTOR_SIZE = 16;

class Sector
{
public:
    Sector(int sectorX, int sectorY);

    int GetSectorX() const;
    int GetSectorY() const;

    // Get a tile by local coords within sector (0-15)
    Tile& GetTile(int localX, int localY);

    // Set tile data
    void SetTile(int localX, int localY, const Tile& tile);

private:
    int m_sectorX;
    int m_sectorY;

    // 16x16 tiles per sector
    std::array<Tile, SECTOR_SIZE* SECTOR_SIZE> m_tiles;
};
