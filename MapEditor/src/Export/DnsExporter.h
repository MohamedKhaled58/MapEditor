// src/DnsExporter.h
#pragma once
#include <string>
#include <vector>
#include "Tile.h"

class DnsExporter
{
public:
    // Export tiles to .dns files in folder
    // tileSize: dimension of tile (usually 32)
    static bool ExportTilesToDns(const std::vector<Tile>& tiles, const std::string& outputFolder);

private:
    // Write one tile to .dns file
    static bool WriteDnsFile(const Tile& tile, const std::string& filePath);
};
