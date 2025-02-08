#pragma once

#include <d3dx9.h>
#include <wrl/client.h>
#include <exception>

namespace DX9
{
	namespace SpriteFont {
		Microsoft::WRL::ComPtr<ID3DXFont> CreateDefaultFont(
			IDirect3DDevice9* inDevice
		);

		Microsoft::WRL::ComPtr<ID3DXFont> CreateFromFontName(
			IDirect3DDevice9* inDevice,
			LPCWSTR			  inFontName,
			const int		  inSize
		);

		inline Microsoft::WRL::ComPtr<ID3DXFont> CreateFromFontDesc(
			IDirect3DDevice9*    inDevice,
			const D3DXFONT_DESC& inFontDesc
		)
		{
			Microsoft::WRL::ComPtr<ID3DXFont> font;
			const HRESULT hr = D3DXCreateFontIndirect(
				inDevice, &inFontDesc, font.GetAddressOf()
			);
			if (hr != D3D_OK)
				throw std::exception();

			font->PreloadCharacters(0x20, 0x7e);
			font->PreloadGlyphs(0x20, 0x7e);

			return font;
		}

		inline Microsoft::WRL::ComPtr<ID3DXFont> CreateFromFontFile(
			IDirect3DDevice9* inDevice,
			LPCWSTR			  inFontFileName,
			LPCWSTR			  inFontName,
			const int		  inSize
		)
		{
			AddFontResourceEx(inFontFileName, FR_PRIVATE, nullptr);
			return CreateFromFontName(inDevice, inFontName, inSize);
		}
	}

	typedef Microsoft::WRL::ComPtr<ID3DXFont> SPRITEFONT;
};