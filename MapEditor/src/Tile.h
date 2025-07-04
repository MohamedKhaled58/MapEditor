#pragma once
#include <cstdint>

class Tile
{
public:
    Tile();
    Tile(int x, int y);

    int GetX() const;
    int GetY() const;

    // Walkability flags: bitmask
    uint32_t GetFlags() const;
    void SetFlags(uint32_t flags);

    // Height of the tile (0 - 255)
    uint16_t GetHeight() const;
    void SetHeight(uint16_t height);

    // Texture or tile type index
    uint16_t GetTextureIndex() const;
    void SetTextureIndex(uint16_t textureIndex);

    // Overlay data (optional)
    uint32_t GetOverlay() const;
    void SetOverlay(uint32_t overlay);

private:
    int m_x;
    int m_y;

    uint32_t m_flags = 0;
    uint16_t m_height = 0;
    uint16_t m_textureIndex = 0;
    uint32_t m_overlay = 0;
};
