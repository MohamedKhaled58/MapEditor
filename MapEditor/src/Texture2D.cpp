#include "Texture2D.h"
#include <wincodec.h>
#include <wrl.h>
#include <d3d11.h>
#include <iostream>
#include <vector>

extern ID3D11Device* g_pd3dDevice; // Declare globally from main.cpp

Texture2D::Texture2D() = default;
Texture2D::~Texture2D() = default;

bool Texture2D::IsValid() const
{
    return m_srv != nullptr;
}

ID3D11ShaderResourceView* Texture2D::GetSRV() const
{
    return m_srv.Get();
}

bool Texture2D::LoadFromFile(const std::string& filename)
{
    using Microsoft::WRL::ComPtr;

    ComPtr<IWICImagingFactory> wicFactory;
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&wicFactory));
    if (FAILED(hr)) return false;

    ComPtr<IWICBitmapDecoder> decoder;
    hr = wicFactory->CreateDecoderFromFilename(
        std::wstring(filename.begin(), filename.end()).c_str(),
        nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) return false;

    ComPtr<IWICBitmapFrameDecode> frame;
    decoder->GetFrame(0, &frame);

    ComPtr<IWICFormatConverter> converter;
    wicFactory->CreateFormatConverter(&converter);
    converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

    UINT width, height;
    converter->GetSize(&width, &height);
    std::vector<uint8_t> pixels(width * height * 4);
    converter->CopyPixels(nullptr, width * 4, pixels.size(), pixels.data());

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.SampleDesc.Count = 1;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels.data();
    initData.SysMemPitch = width * 4;

    ComPtr<ID3D11Texture2D> texture;
    hr = g_pd3dDevice->CreateTexture2D(&desc, &initData, &texture);
    if (FAILED(hr)) return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    return SUCCEEDED(g_pd3dDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &m_srv));
}
