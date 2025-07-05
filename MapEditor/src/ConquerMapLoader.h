#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Map.h"

// Conquer Online map format structures
struct ConquerTile {
    uint16_t flags;      // Walkability flags
    uint16_t height;     // Tile height
    uint16_t texture;    // Texture index
    uint16_t overlay;    // Overlay information
};

struct ConquerSector {
    int x, y;           // Sector coordinates
    int width, height;  // Sector dimensions
    std::vector<ConquerTile> tiles;
};

class ConquerMapLoader
{
public:
    // Load different Conquer Online map formats
    static bool LoadDMap(Map& map, const std::string& dmapPath, const std::string& dnsFolder);
    static bool LoadPK(Map& map, const std::string& pkPath);
    static bool LoadDDS(Map& map, const std::string& ddsPath);
    
    // Utility functions
    static bool IsWalkable(uint16_t flags);
    static bool IsBlocked(uint16_t flags);
    static std::string GetTileTypeDescription(uint16_t flags);
    
private:
    // Helper functions for different formats
    static bool LoadDNSFile(const std::string& dnsPath, ConquerSector& sector);
    static bool LoadPKFile(const std::string& pkPath, std::vector<ConquerSector>& sectors);
    static bool LoadDDSFile(const std::string& ddsPath, std::vector<uint8_t>& data, int& width, int& height);
}; 