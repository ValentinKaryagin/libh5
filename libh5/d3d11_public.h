#pragma once

#include "commons.h"

#define D3D11_SAFE_RELEASE(ptr) if (ptr) { ptr->Release(); ptr = nullptr; }
#define D3D11_SAFE_DELETE(ptr) if (ptr) { delete ptr; ptr = nullptr; }

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11InputLayout;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;
struct ID3D10Blob;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;

namespace DirectX
{
	class ScratchImage;
	class SpriteBatch;
	class SpriteFont;
}
