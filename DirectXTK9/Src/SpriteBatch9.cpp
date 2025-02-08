#include "SpriteBatch9.h"
#include <exception>

#ifdef NDEBUG
	#pragma comment(lib, "d3dx9.lib")
#else
	#pragma comment(lib, "d3dx9d.lib")
#endif

namespace DX9
{
	std::unique_ptr<SpriteBatch9> SpriteBatch;
};

const DirectX::XMFLOAT2 DX9::SpriteBatch9::Float2Zero(0.0f, 0.0f);
const DirectX::XMFLOAT3 DX9::SpriteBatch9::Float3Zero(0.0f, 0.0f, 0.0f);
const DirectX::XMMATRIX DX9::SpriteBatch9::MatrixIdentity = DirectX::XMMatrixIdentity();

DX9::SpriteBatch9::SpriteBatch9(IDirect3DDevice9* inDevice)
{
	if(D3DXCreateSprite(inDevice, m_sprite.GetAddressOf()) != D3D_OK)
		throw std::exception();
	m_device = inDevice;
}

void DX9::SpriteBatch9::Begin(const DWORD sortMode)
{
	m_sprite->Begin(sortMode);
	m_sprite->SetTransform((D3DXMATRIX*)&MatrixIdentity);

	D3DXMATRIX   proj;
	m_device->GetTransform(D3DTS_PROJECTION, &proj);
	proj._33 = 1.0f / (SpriteDepthMostBottom - SpriteDepthMostTop);
	proj._43 = SpriteDepthMostTop / (SpriteDepthMostTop - SpriteDepthMostBottom);
	m_device->SetTransform(D3DTS_PROJECTION, &proj);
}

void DX9::SpriteBatch9::Draw(
	IDirect3DTexture9* texture,
	DirectX::XMFLOAT3 const& position,
	RECT const* sourceRectangle,
	const D3DCOLOR color,
	const float rotation,
	DirectX::XMFLOAT3 const* origin,
	const float scale
)
{
	DirectX::XMMATRIX transform = DirectX::XMMatrixScaling(scale, scale, 1.0f);
	transform *= DirectX::XMMatrixRotationZ(rotation);
	CopyMemory(&transform.r[3], &position, sizeof(float) * 3);

	m_sprite->SetTransform((D3DXMATRIX*)&transform);
	m_sprite->Draw(texture, sourceRectangle, (D3DXVECTOR3*)origin, nullptr, color);
}

void DX9::SpriteBatch9::Draw(
	IDirect3DTexture9* texture,
	DirectX::XMFLOAT3 const& position,
	RECT const* sourceRectangle,
	const D3DCOLOR color,
	DirectX::XMFLOAT3 const& rotation,
	DirectX::XMFLOAT3 const& origin,
	DirectX::XMFLOAT2 const& scale
)
{
	DirectX::XMMATRIX transform = DirectX::XMMatrixScaling(scale.x, scale.y, 1.0f);
	transform *= DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	CopyMemory(&transform.r[3], &position, sizeof(float) * 3);

	m_sprite->SetTransform((D3DXMATRIX*)&transform);
	m_sprite->Draw(texture, sourceRectangle, (D3DXVECTOR3*)&origin, nullptr, color);
}

void DX9::SpriteBatch9::DrawString(
	ID3DXFont* font,
	LPCWSTR text,
	const DirectX::XMFLOAT2& position,
	const D3DCOLOR color,
	const DirectX::XMMATRIX& transform
)
{
	RECT dest;
	dest.left   = static_cast<long>(position.x + 0.5f);
	dest.top    = static_cast<long>(position.y + 0.5f);
	dest.right  = dest.left + 1;
	dest.bottom = dest.top  + 1;

	m_sprite->SetTransform((D3DXMATRIX*)&transform);
	font->DrawText(m_sprite.Get(), text, -1, &dest, DT_NOCLIP | DT_SINGLELINE, color);
}

