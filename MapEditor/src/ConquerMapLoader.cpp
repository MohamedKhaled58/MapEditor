#include "ConquerMapLoader.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <sstream>

bool ConquerMapLoader::LoadDMap(Map& map, const std::string& dmapPath, const std::string& dnsFolder)
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

    std::cout << "Loading DMAP: " << width << "x" << height << " tiles\n";
    map.Resize(width, height);

    // Read sector headers
    while (dmapFile.peek() != EOF)
    {
        int sectorX = 0, sectorY = 0;
        dmapFile.read(reinterpret_cast<char*>(&sectorX), sizeof(int));
        dmapFile.read(reinterpret_cast<char*>(&sectorY), sizeof(int));

        char nameBuffer[128] = {};
        dmapFile.read(nameBuffer, sizeof(nameBuffer));
        std::string sectorFile = std::string(nameBuffer);

        std::string fullDnsPath = dnsFolder + "/" + sectorFile;
        ConquerSector sector;
        sector.x = sectorX;
        sector.y = sectorY;
        
        if (LoadDNSFile(fullDnsPath, sector))
        {
            // Convert Conquer tiles to Map tiles
            for (int y = 0; y < sector.height; ++y)
            {
                for (int x = 0; x < sector.width; ++x)
                {
                    int tileIndex = y * sector.width + x;
                    if (tileIndex < sector.tiles.size())
                    {
                        const ConquerTile& conquerTile = sector.tiles[tileIndex];
                        
                        Tile tile(sectorX * sector.width + x, sectorY * sector.height + y);
                        tile.SetFlags(conquerTile.flags);
                        tile.SetHeight(conquerTile.height);
                        tile.SetTextureIndex(conquerTile.texture);
                        tile.SetOverlay(conquerTile.overlay);
                        
                        map.SetTile(sectorX * sector.width + x, sectorY * sector.height + y, tile);
                    }
                }
            }
        }
    }

    return true;
}

bool ConquerMapLoader::LoadPK(Map& map, const std::string& pkPath)
{
    std::vector<ConquerSector> sectors;
    if (!LoadPKFile(pkPath, sectors))
    {
        return false;
    }

    // Calculate map dimensions from sectors
    int minX = INT_MAX, minY = INT_MAX;
    int maxX = INT_MIN, maxY = INT_MIN;
    
    for (const auto& sector : sectors)
    {
        minX = std::min(minX, sector.x);
        minY = std::min(minY, sector.y);
        maxX = std::max(maxX, sector.x + sector.width);
        maxY = std::max(maxY, sector.y + sector.height);
    }

    int mapWidth = maxX - minX;
    int mapHeight = maxY - minY;
    
    std::cout << "Loading PK: " << mapWidth << "x" << mapHeight << " tiles\n";
    map.Resize(mapWidth, mapHeight);

    // Convert sectors to map tiles
    for (const auto& sector : sectors)
    {
        for (int y = 0; y < sector.height; ++y)
        {
            for (int x = 0; x < sector.width; ++x)
            {
                int tileIndex = y * sector.width + x;
                if (tileIndex < sector.tiles.size())
                {
                    const ConquerTile& conquerTile = sector.tiles[tileIndex];
                    
                    Tile tile(sector.x + x - minX, sector.y + y - minY);
                    tile.SetFlags(conquerTile.flags);
                    tile.SetHeight(conquerTile.height);
                    tile.SetTextureIndex(conquerTile.texture);
                    tile.SetOverlay(conquerTile.overlay);
                    
                    map.SetTile(sector.x + x - minX, sector.y + y - minY, tile);
                }
            }
        }
    }

    return true;
}

bool ConquerMapLoader::LoadDDS(Map& map, const std::string& ddsPath)
{
    std::vector<uint8_t> data;
    int width, height;
    
    if (!LoadDDSFile(ddsPath, data, width, height))
    {
        return false;
    }

    std::cout << "Loading DDS: " << width << "x" << height << " pixels\n";
    map.Resize(width, height);

    // Convert DDS data to tiles (simplified - you might want to add texture support)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Tile tile(x, y);
            // For DDS files, we'll set basic flags based on pixel data
            // This is a simplified approach - you might want more sophisticated analysis
            tile.SetFlags(0); // Default to walkable
            tile.SetHeight(0);
            tile.SetTextureIndex(0);
            tile.SetOverlay(0);
            
            map.SetTile(x, y, tile);
        }
    }

    return true;
}

bool ConquerMapLoader::LoadDNSFile(const std::string& dnsPath, ConquerSector& sector)
{
    std::ifstream dnsFile(dnsPath, std::ios::binary);
    if (!dnsFile)
    {
        std::cerr << "Missing DNS: " << dnsPath << "\n";
        return false;
    }

    dnsFile.read(reinterpret_cast<char*>(&sector.width), sizeof(int));
    dnsFile.read(reinterpret_cast<char*>(&sector.height), sizeof(int));

    sector.tiles.resize(sector.width * sector.height);
    
    for (int y = 0; y < sector.height; ++y)
    {
        for (int x = 0; x < sector.width; ++x)
        {
            int index = y * sector.width + x;
            dnsFile.read(reinterpret_cast<char*>(&sector.tiles[index]), sizeof(ConquerTile));
        }
    }

    return true;
}

bool ConquerMapLoader::LoadPKFile(const std::string& pkPath, std::vector<ConquerSector>& sectors)
{
    std::ifstream pkFile(pkPath, std::ios::binary);
    if (!pkFile)
    {
        std::cerr << "Failed to open PK: " << pkPath << "\n";
        return false;
    }

    // PK file format parsing (this is a simplified version)
    // You might need to adjust based on actual PK format specification
    int sectorCount = 0;
    pkFile.read(reinterpret_cast<char*>(&sectorCount), sizeof(int));

    sectors.resize(sectorCount);
    
    for (int i = 0; i < sectorCount; ++i)
    {
        ConquerSector& sector = sectors[i];
        pkFile.read(reinterpret_cast<char*>(&sector.x), sizeof(int));
        pkFile.read(reinterpret_cast<char*>(&sector.y), sizeof(int));
        pkFile.read(reinterpret_cast<char*>(&sector.width), sizeof(int));
        pkFile.read(reinterpret_cast<char*>(&sector.height), sizeof(int));

        sector.tiles.resize(sector.width * sector.height);
        
        for (int j = 0; j < sector.width * sector.height; ++j)
        {
            pkFile.read(reinterpret_cast<char*>(&sector.tiles[j]), sizeof(ConquerTile));
        }
    }

    return true;
}

bool ConquerMapLoader::LoadDDSFile(const std::string& ddsPath, std::vector<uint8_t>& data, int& width, int& height)
{
    std::ifstream ddsFile(ddsPath, std::ios::binary);
    if (!ddsFile)
    {
        std::cerr << "Failed to open DDS: " << ddsPath << "\n";
        return false;
    }

    // Read DDS header
    char magic[4];
    ddsFile.read(magic, 4);
    if (strncmp(magic, "DDS ", 4) != 0)
    {
        std::cerr << "Invalid DDS file format\n";
        return false;
    }

    // Skip DDS header (128 bytes)
    ddsFile.seekg(128);

    // For simplicity, we'll just read the raw data
    // In a real implementation, you'd want to decode the DDS format properly
    ddsFile.seekg(0, std::ios::end);
    size_t fileSize = ddsFile.tellg();
    ddsFile.seekg(128); // Skip header

    data.resize(fileSize - 128);
    ddsFile.read(reinterpret_cast<char*>(data.data()), data.size());

    // Set default dimensions (you might want to parse the actual DDS header)
    width = 1024;  // Default
    height = 1024; // Default

    return true;
}

bool ConquerMapLoader::IsWalkable(uint16_t flags)
{
    // Conquer Online walkability flags
    // This is a simplified version - you might need to adjust based on actual flag meanings
    return (flags & 0x0001) == 0; // Assuming bit 0 indicates blocked
}

bool ConquerMapLoader::IsBlocked(uint16_t flags)
{
    return !IsWalkable(flags);
}

std::string ConquerMapLoader::GetTileTypeDescription(uint16_t flags)
{
    std::stringstream ss;
    ss << "Flags: 0x" << std::hex << std::setw(4) << std::setfill('0') << flags;
    
    if (IsWalkable(flags))
        ss << " (Walkable)";
    else
        ss << " (Blocked)";
    
    return ss.str();
} 