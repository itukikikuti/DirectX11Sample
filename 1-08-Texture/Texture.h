#pragma once
#include "Library.h"

class Texture
{
public:
    Texture(const void* buffer, int width, int height);
    ID3D11Texture2D& GetTexture() const;
    DirectX::XMUINT2 GetSize() const;
    void Attach(int slot) const;

    static Texture& GetEmpty();
    static std::unique_ptr<Texture> CreateFromFile(const std::wstring& filePath);

private:
    DirectX::XMUINT2 size;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState = nullptr;
};
