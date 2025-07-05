// MapEditor.h
#pragma once

#include "Map.h"
#include "Texture2D.h"
#include <string>
#include <memory>

class MapEditor
{
public:
    MapEditor(int width, int height);

    void DrawUI();
    void LoadMap(const std::string& dmapFile, const std::string& dnsFolder);
    void LoadBackgroundImage(const std::string& path);
    void ExportMap(const std::string& dnsFolder, const std::string& dmapFile);
    void ImportImageAsTiles(const std::string& path, int tileSize);

private:
    Map m_map;
    std::shared_ptr<Texture2D> m_backgroundImage;
    std::string m_backgroundPath;

    int m_viewWidth = 32;
    int m_viewHeight = 32;
    int m_viewOffsetX = 0;
    int m_viewOffsetY = 0;
    int m_selectedX = -1;
    int m_selectedY = -1;
};
