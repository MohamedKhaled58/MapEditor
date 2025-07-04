#pragma once
#include <vector>
#include <string>

struct Tile {
    int x, y;
    bool walkable;
};

class DnsExporter {
public:
    static void ExportTilesToDns(const std::vector<Tile>& tiles, const std::string& folderPath, int tileSize);
};
