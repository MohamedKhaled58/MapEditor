#pragma once
#include <string>
#include "Map.h"

class DmapLoader
{
public:
    static bool LoadMap(Map& map, const std::string& dmapPath, const std::string& dnsFolder);
};
