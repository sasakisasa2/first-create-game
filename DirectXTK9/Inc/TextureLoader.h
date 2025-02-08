#pragma once

#include <d3dx9.h>
#include <wrl/client.h>

namespace DX9
{
	namespace Texture {
		Microsoft::WRL::ComPtr<IDirect3DTexture9> CreateFromFile(
			IDirect3DDevice9* inDevice,
			LPCTSTR			  inFileName,
			const D3DCOLOR	  inColorKey = 0
		);
	}

	namespace Sprite {
		inline Microsoft::WRL::ComPtr<IDirect3DTexture9> CreateFromFile(
			IDirect3DDevice9* inDevice,
			LPCTSTR			  inFileName,
			const D3DCOLOR	  inColorKey = 0
		)
		{
			return Texture::CreateFromFile(inDevice, inFileName, inColorKey);
		}
	}

	typedef Microsoft::WRL::ComPtr<IDirect3DTexture9> TEXTURE;
	typedef Microsoft::WRL::ComPtr<IDirect3DTexture9> SPRITE;
};