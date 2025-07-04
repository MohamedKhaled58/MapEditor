// src/DmapExporter.h
#pragma once
#include <string>
#include "Map.h"

class DmapExporter
{
public:
    // Export full map as .dmap file with sector references and metadata
    static bool ExportDmap(const Map& map, const std::string& outputFile, const std::string& dnsFolder);
};
