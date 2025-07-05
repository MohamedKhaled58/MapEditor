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

    std::cout << "Loading texture from: " << path << std::endl;

    Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&factory));
    if (FAILED(hr))
    {
        std::cerr << "Failed to create WIC factory. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    hr = factory->CreateDecoderFromFilename(std::wstring(path.begin(), path.end()).c_str(), nullptr,
        GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create WIC decoder from file. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        std::cerr << "File path: " << path << std::endl;
        return false;
    }

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get frame from decoder. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
    hr = factory->CreateFormatConverter(&converter);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create WIC format converter. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
    if (FAILED(hr))
    {
        std::cerr << "Failed to initialize WIC format converter. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    UINT width, height;
    frame->GetSize(&width, &height);

    std::unique_ptr<BYTE[]> buffer(new BYTE[width * height * 4]);
    hr = converter->CopyPixels(nullptr, width * 4, width * height * 4, buffer.get());
    if (FAILED(hr))
    {
        std::cerr << "Failed to copy pixels from WIC converter. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

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
    hr = g_pd3dDevice->CreateTexture2D(&texDesc, &initData, &texture);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create Direct3D texture. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    hr = g_pd3dDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &m_srv);
    if (FAILED(hr)) {
        std::cerr << "Failed to create shader resource view. HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    std::cout << "? Loaded texture from: " << path << " (" << width << "x" << height << ")\n";


    m_texture = texture;
    return true;
}