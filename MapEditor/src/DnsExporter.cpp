#include "DnsExporter.h"
#include <filesystem>
#include <fstream>
#include <iostream>

bool DnsExporter::ExportMapToDns(const Map& map, const std::string& outputFolder)
{
    namespace fs = std::filesystem;
    fs::create_directories(outputFolder);

    const int sectorSize = 16;

    for (int sy = 0; sy < map.GetSectorCountY(); ++sy)
    {
        for (int sx = 0; sx < map.GetSectorCountX(); ++sx)
        {
            const Sector* sector = map.GetSector(sx, sy);
            if (!sector) continue;

            char filename[256];
            sprintf_s(filename, "%s/Tile_%03d_%03d.dns", outputFolder.c_str(), sx, sy);

            std::ofstream out(filename, std::ios::binary);
            if (!out.is_open())
            {
                std::cerr << "? Failed to open: " << filename << "\n";
                continue;
            }

            for (int y = 0; y < 16; ++y)
            {
                for (int x = 0; x < 16; ++x)
                {
                    const Tile& tile = sector->GetTile(x, y);
                    uint32_t flag = tile.GetFlags();
                    uint16_t height = tile.GetHeight();
                    uint16_t texIndex = tile.GetTextureIndex();
                    uint32_t overlay = tile.GetOverlay();

                    out.write(reinterpret_cast<char*>(&flag), sizeof(flag));
                    out.write(reinterpret_cast<char*>(&height), sizeof(height));
                    out.write(reinterpret_cast<char*>(&texIndex), sizeof(texIndex));
                    out.write(reinterpret_cast<char*>(&overlay), sizeof(overlay));
                }
            }

            out.close();
        }
    }

    std::cout << "? DNS export complete.\n";
    return true;
}
