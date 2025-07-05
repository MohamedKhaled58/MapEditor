// Sector.cpp
#include "Sector.h"

Sector::Sector(int sectorX, int sectorY)
    : m_sectorX(sectorX), m_sectorY(sectorY)
{
    for (int y = 0; y < SECTOR_SIZE; ++y)
    {
        for (int x = 0; x < SECTOR_SIZE; ++x)
        {
            int absX = sectorX * SECTOR_SIZE + x;
            int absY = sectorY * SECTOR_SIZE + y;
            m_tiles[y * SECTOR_SIZE + x] = Tile(absX, absY);
        }
    }
}

int Sector::GetSectorX() const { return m_sectorX; }
int Sector::GetSectorY() const { return m_sectorY; }

Tile& Sector::GetTile(int localX, int localY)
{
    return m_tiles[localY * SECTOR_SIZE + localX];
}

const Tile& Sector::GetTile(int localX, int localY) const
{
    return m_tiles[localY * SECTOR_SIZE + localX];
}


void Sector::SetTile(int localX, int localY, const Tile& tile)
{
    m_tiles[localY * SECTOR_SIZE + localX] = tile;
}
