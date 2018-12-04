#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class Texture
{
public:
    static Texture& GetEmpty();
    static std::unique_ptr<Texture> CreateFromFile(const wchar_t* filePath);

    Texture(const BYTE* buffer, int width, int height);
    ID3D11Texture2D& GetTexture();
    DirectX::XMUINT2 GetSize();
    void Attach(int slot);

private:
    DirectX::XMUINT2 size;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};
