// DnsTile.h
#pragma once
#include <string>

struct DnsTile
{
    char FileName[260] = "tile.dds";
    char IndexName[128] = "tile";
    int CellX = 0;
    int CellY = 0;
    unsigned int Width = 32;
    unsigned int Height = 32;
    int OffsetX = 0;
    int OffsetY = 0;
    unsigned int FrameInterval = 100;
};
