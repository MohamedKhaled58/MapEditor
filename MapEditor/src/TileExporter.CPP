#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/libigl-stb/stb_image_write.h"
#include "TileExporter.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <Tile.h>

bool TileExporter::ExportTilesAsImages(const Map& map, const std::string& outputFolder)
{
    namespace fs = std::filesystem;
    fs::create_directories(outputFolder);

    const int size = 32; // size of each tile image in pixels
    const int channels = 3;

    int width = map.GetWidth();
    int height = map.GetHeight();
    std::cout << "Exporting " << width << "x" << height << " tiles\n";

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const Tile* tile = map.GetTile(x, y);
            if (!tile)
            {
                std::cerr << "??  Null tile at (" << x << ", " << y << ")\n";
                continue;
            }

            std::vector<unsigned char> image(size * size * channels, 255); // default white

            bool isBlocked = (tile->GetFlags() & 1);
            for (int py = 0; py < size; ++py)
            {
                for (int px = 0; px < size; ++px)
                {
                    int idx = (py * size + px) * channels;
                    if (isBlocked)
                    {
                        image[idx + 0] = 200;
                        image[idx + 1] = 60;
                        image[idx + 2] = 60;
                    }
                    else
                    {
                        image[idx + 0] = 255;
                        image[idx + 1] = 255;
                        image[idx + 2] = 255;
                    }
                }
            }

            char filename[256];
            sprintf_s(filename, "%s/tile_%03d_%03d.png", outputFolder.c_str(), x, y);
            if (!stbi_write_png(filename, size, size, channels, image.data(), size * channels))
            {
                std::cerr << "? Failed to write tile image: " << filename << "\n";
            }
        }
    }

    std::cout << "? Walkability tiles exported as PNG.\n";
    return true;
}

