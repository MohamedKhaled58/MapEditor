#pragma once
#include "Map.h"
#include "Export/DnsExporter.h"
#include "DmapExporter.h"

// Other includes...

class MapEditor
{
public:
    MapEditor(int width, int height);

    void DrawUI();

    void ExportMap(const std::string& dnsFolder, const std::string& dmapFile);

    Map& GetMap() { return m_map; }

private:
    Map m_map;
    // Add to private section:
    int m_viewOffsetX = 0;  // Pan/scroll offset in tiles
    int m_viewOffsetY = 0;
    int m_viewWidth = 20;   // How many tiles to show horizontally
    int m_viewHeight = 15;  // Vertically
    int m_selectedX = -1;
    int m_selectedY = -1;

    // Additional editor state variables...

    int m_tileSize = 32;  // Typical tile size in pixels
};
