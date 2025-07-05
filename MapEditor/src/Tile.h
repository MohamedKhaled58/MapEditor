#pragma once
#include <cstdint>

class Tile
{
public:
    Tile(int x = 0, int y = 0);

    int GetX() const;
    int GetY() const;

    uint32_t GetFlags() const;
    void SetFlags(uint32_t flags);

    uint16_t GetHeight() const;
    void SetHeight(uint16_t height);

    uint16_t GetTextureIndex() const;
    void SetTextureIndex(uint16_t index);

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
