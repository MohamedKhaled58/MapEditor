#pragma once
#include <string>
#include <d3d11.h>
#include <wrl/client.h>

class Texture2D
{
public:
    Texture2D();
    ~Texture2D();

    bool LoadFromFile(const std::string& path);
    bool IsValid() const { return m_srv != nullptr; }
    ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
};