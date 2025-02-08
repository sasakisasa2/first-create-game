#pragma once

#include <d3dx9.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include <memory>
#include <vector>

namespace DX9
{
    static constexpr float SpriteDepthMostTop    = -10000.0f;
    static constexpr float SpriteDepthMostBottom =  10000.0f;

    enum SpriteSortMode : DWORD
    {
        SpriteSortMode_Texture     = D3DXSPRITE_SORT_TEXTURE,
        SpriteSortMode_BackToFront = D3DXSPRITE_SORT_DEPTH_BACKTOFRONT,
        SpriteSortMode_FrontToBack = D3DXSPRITE_SORT_DEPTH_FRONTTOBACK
    };

	class SpriteBatch9 {
	public:
        SpriteBatch9(IDirect3DDevice9* inDevice);
        virtual ~SpriteBatch9() {}

        void Begin(const DWORD sortMode = D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);

        inline void End()   const { m_sprite->End(); }
        inline void Flush() const { m_sprite->Flush(); }

        inline void ResetTransform() const
        { m_sprite->SetTransform((D3DXMATRIX*)&MatrixIdentity); }

        inline void DrawSimple(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            RECT const* sourceRectangle = nullptr,
            const D3DCOLOR color = 0xFFFFFFFF,
            DirectX::XMFLOAT3 const* origin = nullptr
        )
        {
            m_sprite->Draw(texture, sourceRectangle, (D3DXVECTOR3*)origin, (D3DXVECTOR3*)&position, color);
        }

        inline void DrawSimple(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            RECT const& sourceRectangle,
            const D3DCOLOR color = 0xFFFFFFFF,
            DirectX::XMFLOAT3 const& origin = Float3Zero
        )
        {
            m_sprite->Draw(texture, &sourceRectangle, (D3DXVECTOR3*)&origin, (D3DXVECTOR3*)&position, color);
        }

        void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            RECT const* sourceRectangle = nullptr,
            const D3DCOLOR color = 0xFFFFFFFF,
            const float rotation = 0.0f,
            DirectX::XMFLOAT3 const* origin = nullptr,
            const float scale = 1.0f
        );

        inline void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            RECT const& sourceRectangle,
            const D3DCOLOR color = 0xFFFFFFFF,
            const float rotation = 0.0f,
            DirectX::XMFLOAT3 const& origin = Float3Zero,
            const float scale = 1.0f
        )
        { Draw(texture, position, &sourceRectangle, color, rotation, &origin, scale); }

        inline void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            RECT const& sourceRectangle,
            const D3DCOLOR color,
            DirectX::XMFLOAT2 const& scale,
            DirectX::XMFLOAT3 const& origin = Float3Zero
        )
        { Draw(texture, position, &sourceRectangle, color, Float3Zero, origin, scale); }

        inline void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            const D3DCOLOR color,
            const float rotation,
            DirectX::XMFLOAT3 const& origin = Float3Zero
        )
        { Draw(texture, position, nullptr, color, rotation, &origin, 1.0f); }

        inline void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            const D3DCOLOR color,
            DirectX::XMFLOAT2 const& scale,
            DirectX::XMFLOAT3 const& origin = Float3Zero
        )
        { Draw(texture, position, nullptr, color, Float3Zero, origin, scale); }

        inline void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            const D3DCOLOR color,
            const float rotation,
            DirectX::XMFLOAT3 const& origin = Float3Zero,
            const float scale = 1.0f
        )
        { Draw(texture, position, nullptr, color, rotation, &origin, scale); }

        void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            RECT const* sourceRectangle,
            const D3DCOLOR color,
            DirectX::XMFLOAT3 const& rotation,
            DirectX::XMFLOAT3 const& origin,
            DirectX::XMFLOAT2 const& scale
        );

        inline void Draw(
            IDirect3DTexture9* texture,
            DirectX::XMFLOAT3 const& position,
            RECT const& sourceRectangle,
            const D3DCOLOR color,
            DirectX::XMFLOAT3 const& rotation,
            DirectX::XMFLOAT3 const& origin,
            DirectX::XMFLOAT2 const& scale
        )
        { Draw(texture, position, &sourceRectangle, color, rotation, origin, scale); }

        void DrawString(
            ID3DXFont* font,
            LPCWSTR text,
            const DirectX::XMFLOAT2& position,
            const D3DCOLOR color,
            const DirectX::XMMATRIX& transform = MatrixIdentity
        );

        inline void DrawString(
            ID3DXFont* font,
            const DirectX::XMFLOAT2& position,
            const D3DCOLOR color,
            LPCWSTR text, ...
        )
        {
            va_list args;
            va_start(args, text);

            const int LENGTH = _vscwprintf(text, args) + 1;
            std::vector<wchar_t> format(LENGTH);
            vswprintf_s(format.data(), LENGTH, text, args);

            va_end(args);

            DrawString(font, format.data(), position, color, MatrixIdentity);
        }

        inline void DrawString(
            ID3DXFont* font,
            const DirectX::XMFLOAT2& position,
            const D3DCOLOR color,
            const DirectX::XMMATRIX& transform,
            LPCWSTR text, ...
        )
        {
            va_list args;
            va_start(args, text);

            const int LENGTH = _vscwprintf(text, args) + 1;
            std::vector<wchar_t> format(LENGTH);
            vswprintf_s(format.data(), LENGTH, text, args);

            va_end(args);

            DrawString(font, format.data(), position, color, transform);
        }

        SpriteBatch9(SpriteBatch9&& moveFrom) noexcept { UNREFERENCED_PARAMETER(moveFrom); }
        SpriteBatch9& operator= (SpriteBatch9&& moveFrom) noexcept { UNREFERENCED_PARAMETER(moveFrom); }

        SpriteBatch9(SpriteBatch9 const&) = delete;
        SpriteBatch9& operator= (SpriteBatch9 const&) = delete;

	private:
        Microsoft::WRL::ComPtr<IDirect3DDevice9> m_device;
        Microsoft::WRL::ComPtr<ID3DXSprite>      m_sprite;

        static const DirectX::XMFLOAT2 Float2Zero;
        static const DirectX::XMFLOAT3 Float3Zero;
        static const DirectX::XMMATRIX MatrixIdentity;
    };

    extern std::unique_ptr<SpriteBatch9> SpriteBatch;
};