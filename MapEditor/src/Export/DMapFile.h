#pragma once
#include "GridOverlay.h"
#include <string>

namespace DMapFile {

    void SaveAsDns(const std::string& path, const GridOverlay& grid);

    bool Save(const std::string& filename, const std::vector<std::vector<GridTile>>& tiles);
    void ExportDnsTiles(const std::string& directory, const std::vector<std::vector<GridTile>>& tiles);
}