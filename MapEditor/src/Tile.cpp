#include "Tile.h"

Tile::Tile(int x, int y) : m_x(x), m_y(y) {}

int Tile::GetX() const { return m_x; }
int Tile::GetY() const { return m_y; }

uint32_t Tile::GetFlags() const { return m_flags; }
void Tile::SetFlags(uint32_t flags) { m_flags = flags; }

uint16_t Tile::GetHeight() const { return m_height; }
void Tile::SetHeight(uint16_t height) { m_height = height; }

uint16_t Tile::GetTextureIndex() const { return m_textureIndex; }
void Tile::SetTextureIndex(uint16_t index) { m_textureIndex = index; }

uint32_t Tile::GetOverlay() const { return m_overlay; }
void Tile::SetOverlay(uint32_t overlay) { m_overlay = overlay; }
