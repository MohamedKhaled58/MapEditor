#pragma once
#include "GridOverlay.h"
#include <string>

namespace DMapFile {
    bool Save(const std::string& filename, const std::vector<std::vector<GridTile>>& tiles);
}