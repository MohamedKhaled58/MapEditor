#pragma once

#include <string>
#include "Map.h"

class DnsExporter
{
public:
    static bool ExportMapToDns(const Map& map, const std::string& outputFolder);
};
