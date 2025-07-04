#pragma once
#include <d3d11.h>

bool LoadTextureFromFile(const char* filename, ID3D11Device* device, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);