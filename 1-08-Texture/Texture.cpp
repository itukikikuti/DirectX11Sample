#include "Library.h"

Texture::Texture(const void* buffer, int width, int height)
{
    InitializeLibrary();

    size = DirectX::XMUINT2(width, height);

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = size.x;
    textureDesc.Height = size.y;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

    D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
    textureSubresourceData.pSysMem = buffer;
    textureSubresourceData.SysMemPitch = size.x * 4;
    textureSubresourceData.SysMemSlicePitch = size.x * size.y * 4;
    Graphics::GetDevice().CreateTexture2D(&textureDesc, &textureSubresourceData, texture.GetAddressOf());

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    Graphics::GetDevice().CreateShaderResourceView(texture.Get(), &shaderResourceViewDesc, shaderResourceView.GetAddressOf());

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    Graphics::GetDevice().CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
}

ID3D11Texture2D& Texture::GetTexture() const
{
    return *texture.Get();
}

DirectX::XMUINT2 Texture::GetSize() const
{
    return size;
}

void Texture::Attach(int slot) const
{
    Graphics::GetContext().PSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
    Graphics::GetContext().PSSetSamplers(slot, 1, samplerState.GetAddressOf());
}

Texture& Texture::GetEmpty()
{
    static std::unique_ptr<Texture> texture = nullptr;

    if (texture == nullptr)
    {
        std::uint32_t buffer = 0x00000000;
        texture = std::make_unique<Texture>(&buffer, 1, 1);
    }

    return *texture.get();
}

std::unique_ptr<Texture> Texture::CreateFromFile(const std::wstring& filePath)
{
    InitializeLibrary();

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder = nullptr;
    Graphics::GetImageFactory().CreateDecoderFromFilename(filePath.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame = nullptr;
    decoder->GetFrame(0, &frame);

    UINT width, height;
    frame->GetSize(&width, &height);

    WICPixelFormatGUID pixelFormat;
    frame->GetPixelFormat(&pixelFormat);
    std::unique_ptr<BYTE[]> buffer(new BYTE[width * height * 4]);

    if (pixelFormat != GUID_WICPixelFormat32bppBGRA)
    {
        Microsoft::WRL::ComPtr<IWICFormatConverter> formatConverter = nullptr;
        Graphics::GetImageFactory().CreateFormatConverter(&formatConverter);

        formatConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);

        formatConverter->CopyPixels(0, width * 4, width * height * 4, buffer.get());
    }
    else
    {
        frame->CopyPixels(0, width * 4, width * height * 4, buffer.get());
    }

    return std::make_unique<Texture>(buffer.get(), width, height);
}
