// src/DnsExporter.cpp
#include "DnsExporter.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <Map.h>

bool DnsExporter::ExportTilesToDns(const std::vector<Tile>& tiles, const std::string& outputFolder)
{
    namespace fs = std::filesystem;
    if (!fs::exists(outputFolder))
    {
        if (!fs::create_directories(outputFolder))
        {
            std::cerr << "Failed to create output directory: " << outputFolder << "\n";
            return false;
        }
    }

    for (const Tile& tile : tiles)
    {
        std::string filename = outputFolder + "/tile_" + std::to_string(tile.GetX()) + "_" + std::to_string(tile.GetY()) + ".dns";
        if (!WriteDnsFile(tile, filename))
        {
            std::cerr << "Failed to write tile file: " << filename << "\n";
            return false;
        }
    }
    return true;
}

bool DnsExporter::WriteDnsFile(const Tile& tile, const std::string& filePath)
{
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open())
        return false;

    // Write tile data in binary format per Conquer Online .dns spec

    // Example binary layout (little endian):
    // 4 bytes: Flags
    // 2 bytes: Height
    // 2 bytes: TextureIndex
    // 4 bytes: Overlay

    uint32_t flags = tile.GetFlags();
    uint16_t height = tile.GetHeight();
    uint16_t texIndex = tile.GetTextureIndex();
    uint32_t overlay = tile.GetOverlay();

    file.write(reinterpret_cast<const char*>(&flags), sizeof(flags));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    file.write(reinterpret_cast<const char*>(&texIndex), sizeof(texIndex));
    file.write(reinterpret_cast<const char*>(&overlay), sizeof(overlay));

    file.close();
    return true;
}




bool DnsExporter::ExportMapToDns(const Map& map, const std::string& outputFolder)
{
    std::filesystem::create_directories(outputFolder);
    const int sectorSize = 32;

    int sectorsX = (map.GetWidth() + sectorSize - 1) / sectorSize;
    int sectorsY = (map.GetHeight() + sectorSize - 1) / sectorSize;

    for (int sy = 0; sy < sectorsY; ++sy)
    {
        for (int sx = 0; sx < sectorsX; ++sx)
        {
            std::string filename = outputFolder + "/sector_" + std::to_string(sx) + "_" + std::to_string(sy) + ".dns";
            std::ofstream out(filename, std::ios::binary);
            if (!out) continue;

            out.write(reinterpret_cast<const char*>(&sectorSize), sizeof(int));
            out.write(reinterpret_cast<const char*>(&sectorSize), sizeof(int));

            Tile empty{};
            for (int y = 0; y < sectorSize; ++y)
            {
                for (int x = 0; x < sectorSize; ++x)
                {
                    int mapX = sx * sectorSize + x;
                    int mapY = sy * sectorSize + y;
                    const Tile* tile = map.GetTile(mapX, mapY); // use const Tile*

                    out.write(reinterpret_cast<const char*>(tile ? tile : &empty), sizeof(Tile));
                }
            }

        }
    }

    return true;
}