#pragma once
#include "Map.h"
#include "DnsExporter.h"
#include "DmapExporter.h"
#include "Texture2D.h" // <- your wrapper class for ImGui+DirectX textures



// Other includes...

class MapEditor
{
public:
    MapEditor(int width, int height);

    void DrawUI();

    void ExportMap(const std::string& dnsFolder, const std::string& dmapFile);
    void LoadMap(const std::string& dmapFile, const std::string& dnsFolder);
    void LoadBackgroundImage(const std::string& path);

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
    std::shared_ptr<Texture2D> m_backgroundImage;
    std::string m_backgroundPath;

    int m_tileSize = 32;  // Typical tile size in pixels
};
