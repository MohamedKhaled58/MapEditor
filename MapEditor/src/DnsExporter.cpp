#include "DnsExporter.h"
#include <fstream>
#include <filesystem>

void DnsExporter::ExportTilesToDns(const std::vector<Tile>& tiles, const std::string& folderPath, int tileSize) {
    std::filesystem::create_directories(folderPath);

    for (const auto& tile : tiles) {
        std::string fileName = folderPath + "/tile_" + std::to_string(tile.x) + "_" + std::to_string(tile.y) + ".dns";
        std::ofstream out(fileName, std::ios::binary);

        if (!out.is_open())
            continue;

        uint32_t header = 0x444E5301; // 'DNS\x01' magic
        out.write(reinterpret_cast<const char*>(&header), sizeof(header));

        out.write(reinterpret_cast<const char*>(&tile.x), sizeof(int));
        out.write(reinterpret_cast<const char*>(&tile.y), sizeof(int));
        uint8_t walkableFlag = tile.walkable ? 1 : 0;
        out.write(reinterpret_cast<const char*>(&walkableFlag), sizeof(uint8_t));

        out.close();
    }
}
