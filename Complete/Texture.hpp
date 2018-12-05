#pragma once
#include <memory>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

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
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};
