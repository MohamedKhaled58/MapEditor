// src/DmapExporter.cpp
#include "DmapExporter.h"
#include <fstream>
#include <iostream>
#include <filesystem>

bool DmapExporter::ExportDmap(const Map& map, const std::string& outputFile, const std::string& dnsFolder)
{
    std::ofstream file(outputFile, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open dmap output file: " << outputFile << "\n";
        return false;
    }

    // Write map metadata
    // This is a simplified example: real .dmap files contain map name, width/height, and sector info

    // Example header:
    // 32 bytes: Map name (padded with zero)
    // 4 bytes: map width (tiles)
    // 4 bytes: map height (tiles)
    // 4 bytes: sector count x
    // 4 bytes: sector count y

    char mapNameBuffer[32] = {};
    std::string mapName = map.GetMapName();
    if (!mapName.empty())
        std::copy(mapName.begin(), mapName.begin() + std::min<size_t>(32, mapName.size()), mapNameBuffer);

    file.write(mapNameBuffer, sizeof(mapNameBuffer));

    int width = map.GetWidth();
    int height = map.GetHeight();
    int sectorCountX = map.GetSectorCountX();
    int sectorCountY = map.GetSectorCountY();

    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    file.write(reinterpret_cast<const char*>(&sectorCountX), sizeof(sectorCountX));
    file.write(reinterpret_cast<const char*>(&sectorCountY), sizeof(sectorCountY));

    // Write sector data references (for simplicity, write sector coords and reference to dns files)
    for (int y = 0; y < sectorCountY; ++y)
    {
        for (int x = 0; x < sectorCountX; ++x)
        {
            // For each sector, write sector coordinates
            file.write(reinterpret_cast<const char*>(&x), sizeof(x));
            file.write(reinterpret_cast<const char*>(&y), sizeof(y));

            // You can add sector flags or metadata here as needed

            // In a full implementation, you might also write sector checksums or pointers to .dns files
        }
    }

    file.close();

    std::cout << "Map exported successfully to " << outputFile << "\n";
    return true;
}

