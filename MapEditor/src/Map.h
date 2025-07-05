// Map.h
#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Sector.h"
#include "Tile.h"
#include "PairHash.h"


class Map
{
public:
    Map(int widthInTiles, int heightInTiles);

    int GetWidth() const;
    int GetHeight() const;

    int GetSectorCountX() const;
    int GetSectorCountY() const;

    // لإصدار بدون تعديل
    Sector* GetSector(int sectorX, int sectorY);

    // إصدار const لاستخدام داخل دوال const
    const Sector* GetSector(int sectorX, int sectorY) const;

    Sector& CreateOrGetSector(int sectorX, int sectorY);

    Tile* GetTile(int x, int y);
    const Tile* GetTile(int x, int y) const;


    void SetTile(int x, int y, const Tile& tile);

    void Resize(int newWidth, int newHeight);

    void SetMapName(const std::string& name);
    const std::string& GetMapName() const;

private:
    int m_width;
    int m_height;
    int m_sectorCountX;
    int m_sectorCountY;

    std::string m_mapName;

    std::unordered_map<std::pair<int, int>, std::unique_ptr<Sector>, PairHash> m_sectors;
};
