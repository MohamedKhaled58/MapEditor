#include "DMapFile.h"
#include <fstream>

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