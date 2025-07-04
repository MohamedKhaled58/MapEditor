// src/Map.h
#pragma once
#include <map>
#include <memory>
#include <string>
#include "Sector.h"
#include <vector>

class Map
{
public:
    Map(int widthInTiles, int heightInTiles);

    int GetWidth() const;   // In tiles
    int GetHeight() const;

    void Resize(int width, int height);

    int GetSectorCountX() const;
    int GetSectorCountY() const;

    // Get sector by sector coordinates
    Sector* GetSector(int sectorX, int sectorY);

    // Create or get sector at sector coordinates
    Sector& CreateOrGetSector(int sectorX, int sectorY);

    // Get tile by absolute map coordinates
    Tile* GetTile(int x, int y);
    const Tile* GetTile(int x, int y) const;      // ✅ new const-overload

    // Set tile by absolute map coordinates
    void SetTile(int x, int y, const Tile& tile);

    // Map metadata
    void SetMapName(const std::string& name);
    const std::string& GetMapName() const;

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<std::vector<Tile>> m_tiles;


    int m_sectorCountX;
    int m_sectorCountY;

    std::map<std::pair<int, int>, std::unique_ptr<Sector>> m_sectors;

    std::string m_mapName;
};
