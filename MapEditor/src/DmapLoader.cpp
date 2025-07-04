#include "DmapLoader.h"
#include <fstream>
#include <filesystem>
#include <iostream>

bool DmapLoader::LoadMap(Map& map, const std::string& dmapPath, const std::string& dnsFolder)
{
    std::ifstream dmapFile(dmapPath, std::ios::binary);
    if (!dmapFile)
    {
        std::cerr << "Failed to open DMAP: " << dmapPath << "\n";
        return false;
    }

    int width = 0, height = 0;
    dmapFile.read(reinterpret_cast<char*>(&width), sizeof(int));
    dmapFile.read(reinterpret_cast<char*>(&height), sizeof(int));

    map.Resize(width, height);

    // Read sector headers (not hardcoded size)
    while (dmapFile.peek() != EOF)
    {
        int sectorX = 0, sectorY = 0;
        dmapFile.read(reinterpret_cast<char*>(&sectorX), sizeof(int));
        dmapFile.read(reinterpret_cast<char*>(&sectorY), sizeof(int));

        char nameBuffer[128] = {};
        dmapFile.read(nameBuffer, sizeof(nameBuffer));
        std::string sectorFile = std::string(nameBuffer);

        std::string fullDnsPath = dnsFolder + "/" + sectorFile;
        std::ifstream dnsFile(fullDnsPath, std::ios::binary);
        if (!dnsFile)
        {
            std::cerr << "Missing DNS: " << fullDnsPath << "\n";
            continue;
        }

        int sectorWidth = 0, sectorHeight = 0;
        dnsFile.read(reinterpret_cast<char*>(&sectorWidth), sizeof(int));
        dnsFile.read(reinterpret_cast<char*>(&sectorHeight), sizeof(int));

        for (int y = 0; y < sectorHeight; ++y)
        {
            for (int x = 0; x < sectorWidth; ++x)
            {
                Tile tile;
                dnsFile.read(reinterpret_cast<char*>(&tile), sizeof(Tile));
                map.SetTile(sectorX * sectorWidth + x, sectorY * sectorHeight + y, tile);
            }
        }
    }

    return true;
}
