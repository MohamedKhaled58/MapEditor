#include "DMapFile.h"
#include <fstream>
#include <filesystem>

#include "DnsExporter.h"
#include "GridOverlay.h"



void DMapFile::ExportDnsTiles(const std::string& directory, const std::vector<std::vector<GridTile>>& tiles)
{
    std::filesystem::create_directories(directory);

    for (size_t x = 0; x < tiles.size(); ++x) {
        for (size_t y = 0; y < tiles[x].size(); ++y) {
            std::string filename = directory + "/tile_" + std::to_string(x) + "_" + std::to_string(y) + ".dns";
            std::ofstream out(filename, std::ios::binary);
            if (out) {
                uint8_t walkable = tiles[x][y].walkable ? 1 : 0;
                out.write((char*)&x, sizeof(uint32_t));
                out.write((char*)&y, sizeof(uint32_t));
                out.write((char*)&walkable, sizeof(uint8_t));
            }
        }
    }
}


bool DMapFile::Save(const std::string& filename, const std::vector<std::vector<GridTile>>& tiles)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    uint16_t width = (uint16_t)tiles.size();
    uint16_t height = width > 0 ? (uint16_t)tiles[0].size() : 0;
    file.write(reinterpret_cast<const char*>(&width), sizeof(uint16_t));
    file.write(reinterpret_cast<const char*>(&height), sizeof(uint16_t));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            uint8_t val = tiles[x][y].walkable ? 1 : 0;
            file.write(reinterpret_cast<const char*>(&val), sizeof(uint8_t));
        }
    }

    file.close();
    return true;
}