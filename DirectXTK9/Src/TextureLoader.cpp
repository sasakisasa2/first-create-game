#include "TextureLoader.h"
#include <wincodec.h>
#include <exception>

#ifdef NDEBUG
	#pragma comment(lib, "d3dx9.lib")
#else
	#pragma comment(lib, "d3dx9d.lib")
#endif
#pragma comment(lib, "windowscodecs.lib")

Microsoft::WRL::ComPtr<IDirect3DTexture9> DX9::Texture::CreateFromFile(
	IDirect3DDevice9*	inDevice,
	LPCTSTR				inFileName,
	const D3DCOLOR		inColorKey
)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<IDirect3DTexture9> texture;
	hr = D3DXCreateTextureFromFileEx(
		inDevice, inFileName,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, inColorKey,
		nullptr, nullptr,
		texture.GetAddressOf()
	);
	if (hr == D3D_OK)
		return texture;

	try {
		Microsoft::WRL::ComPtr<IWICImagingFactory> pWICFactory;
		hr = CoCreateInstance(
			CLSID_WICImagingFactory, nullptr,
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pWICFactory.GetAddressOf())
		);
		if (hr != S_OK)
			throw(hr);

		Microsoft::WRL::ComPtr<IWICBitmapDecoder> pWICDecoder;
		hr = pWICFactory->CreateDecoderFromFilename(
			inFileName, nullptr, GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			pWICDecoder.GetAddressOf()
		);
		if (hr != S_OK)
			throw(hr);

		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> pWICFrameDec;
		hr = pWICDecoder->GetFrame(0, pWICFrameDec.GetAddressOf());
		if (hr != S_OK)
			throw(hr);

		Microsoft::WRL::ComPtr<IWICFormatConverter> pWICConverter;
		hr = pWICFactory->CreateFormatConverter(pWICConverter.GetAddressOf());
		if (hr != S_OK)
			throw(hr);

		hr = pWICConverter->Initialize(
			pWICFrameDec.Get(), GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone, nullptr, 0.0f,
			WICBitmapPaletteTypeMedianCut
		);
		if (hr != S_OK)
			throw(hr);

		UINT wic_width, wic_height;
		pWICConverter->GetSize(&wic_width, &wic_height);

		Microsoft::WRL::ComPtr<IDirect3DTexture9> pMemTexture;
		hr = D3DXCreateTexture(
			inDevice,
			wic_width, wic_height, 1, 0, D3DFMT_A8R8G8B8,
			D3DPOOL_SYSTEMMEM, pMemTexture.GetAddressOf()
		);
		if (hr != S_OK)
			throw(hr);

		D3DSURFACE_DESC desc;
		pMemTexture->GetLevelDesc(0, &desc);

		WICRect   copy_rect;
		copy_rect.X = 0;
		copy_rect.Y = 0;

		if (wic_width <= desc.Width)
			copy_rect.Width = wic_width;
		else
			copy_rect.Width = desc.Width;

		if (wic_width <= desc.Height)
			copy_rect.Height = wic_height;
		else
			copy_rect.Height = desc.Height;

		D3DLOCKED_RECT   lock_rect;
		pMemTexture->LockRect(0, &lock_rect, nullptr, 0);
		hr = pWICConverter->CopyPixels(
			&copy_rect,
			lock_rect.Pitch, lock_rect.Pitch * desc.Height,
			(BYTE*)lock_rect.pBits
		);
		pMemTexture->UnlockRect(0);
		if (hr != S_OK)
			throw(hr);

		pWICConverter.Reset();
		pWICFrameDec .Reset();
		pWICDecoder  .Reset();
		pWICFactory  .Reset();

		hr = D3DXCreateTexture(
			inDevice,
			wic_width, wic_height, 1, 0, D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, texture.ReleaseAndGetAddressOf()
		);
		if (hr != S_OK)
			throw(hr);

		IDirect3DSurface9* srcSurface;
		pMemTexture->GetSurfaceLevel(0, &srcSurface);

		IDirect3DSurface9* destSurface;
		texture->GetSurfaceLevel(0, &destSurface);

		D3DXLoadSurfaceFromSurface(
			destSurface, nullptr, nullptr,
			srcSurface, nullptr, nullptr,
			D3DX_FILTER_POINT, inColorKey
		);
	}
	catch (const HRESULT hr) {
#ifdef _DEBUG
		LPVOID string;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&string), 0, nullptr
		);
		OutputDebugString(reinterpret_cast<LPTSTR>(string));
		LocalFree(string);
#endif

		throw std::exception();
	}

	return texture;
}