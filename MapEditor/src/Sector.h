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

    Tile& GetTile(int localX, int localY);
    const Tile& GetTile(int localX, int localY) const;


    void SetTile(int localX, int localY, const Tile& tile);

private:
    int m_sectorX;
    int m_sectorY;
    std::array<Tile, SECTOR_SIZE* SECTOR_SIZE> m_tiles;
};
