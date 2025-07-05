#include "Texture2D.h"
#include <wincodec.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <iostream>

extern ID3D11Device* g_pd3dDevice;

Texture2D::Texture2D() {}
Texture2D::~Texture2D() {}

bool Texture2D::LoadFromFile(const std::string& path)
{

    if (!g_pd3dDevice)
    {
        std::cerr << "Error: Direct3D device not initialized!\n";
        return false;
    }
    Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&factory))))
        return false;

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    if (FAILED(factory->CreateDecoderFromFilename(std::wstring(path.begin(), path.end()).c_str(), nullptr,
        GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder)))
        return false;

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    if (FAILED(decoder->GetFrame(0, &frame)))
        return false;

    Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
    if (FAILED(factory->CreateFormatConverter(&converter)))
        return false;

    if (FAILED(converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom)))
        return false;

    UINT width, height;
    frame->GetSize(&width, &height);

    std::unique_ptr<BYTE[]> buffer(new BYTE[width * height * 4]);
    if (FAILED(converter->CopyPixels(nullptr, width * 4, width * height * 4, buffer.get())))
        return false;

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = buffer.get();
    initData.SysMemPitch = width * 4;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    if (FAILED(g_pd3dDevice->CreateTexture2D(&texDesc, &initData, &texture)))
        return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    if (FAILED(g_pd3dDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &m_srv))) {
        std::cerr << "? Failed to create shader resource view\n";
        return false;
    }

    std::cout << "? Loaded texture from: " << path << " (" << width << "x" << height << ")\n";


    m_texture = texture;
    return true;
}