#pragma once
#include <string>
#include <d3d11.h>
#include <wrl/client.h>

class Texture2D
{
public:
    Texture2D();
    ~Texture2D();

    bool LoadFromFile(const std::string& filename);
    bool IsValid() const;
    ID3D11ShaderResourceView* GetSRV() const;

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
};
