// src/Map.cpp
#include "Map.h"

Map::Map(int widthInTiles, int heightInTiles)
    : m_width(widthInTiles), m_height(heightInTiles)
{
    m_sectorCountX = (widthInTiles + SECTOR_SIZE - 1) / SECTOR_SIZE;
    m_sectorCountY = (heightInTiles + SECTOR_SIZE - 1) / SECTOR_SIZE;
}

int Map::GetWidth() const { return m_width; }
int Map::GetHeight() const { return m_height; }

int Map::GetSectorCountX() const { return m_sectorCountX; }
int Map::GetSectorCountY() const { return m_sectorCountY; }

Sector* Map::GetSector(int sectorX, int sectorY)
{
    auto it = m_sectors.find({ sectorX, sectorY });
    if (it != m_sectors.end())
        return it->second.get();
    return nullptr;
}

Sector& Map::CreateOrGetSector(int sectorX, int sectorY)
{
    auto it = m_sectors.find({ sectorX, sectorY });
    if (it != m_sectors.end())
        return *it->second;

    auto sector = std::make_unique<Sector>(sectorX, sectorY);
    Sector& ref = *sector;
    m_sectors[{sectorX, sectorY}] = std::move(sector);
    return ref;
}

Tile* Map::GetTile(int x, int y)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return nullptr;

    int sectorX = x / SECTOR_SIZE;
    int sectorY = y / SECTOR_SIZE;
    int localX = x % SECTOR_SIZE;
    int localY = y % SECTOR_SIZE;

    Sector* sector = GetSector(sectorX, sectorY);
    if (!sector) return nullptr;

    return &sector->GetTile(localX, localY);
}

void Map::SetTile(int x, int y, const Tile& tile)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;

    int sectorX = x / SECTOR_SIZE;
    int sectorY = y / SECTOR_SIZE;
    int localX = x % SECTOR_SIZE;
    int localY = y % SECTOR_SIZE;

    Sector& sector = CreateOrGetSector(sectorX, sectorY);
    sector.SetTile(localX, localY, tile);
}

void Map::SetMapName(const std::string& name)
{
    m_mapName = name;
}

const std::string& Map::GetMapName() const
{
    return m_mapName;
}
