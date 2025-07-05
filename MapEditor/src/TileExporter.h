#pragma once
#include "Map.h" 
#include <string>

class TileExporter
{
public:
    static bool ExportTilesAsImages(const Map& map, const std::string& outputFolder);
};
