#pragma once

#pragma warning(push)
#pragma warning(disable:4996)

#include <streams.h>
#include <atlcomcli.h>

#pragma warning(pop)

#include <d3d9.h>
#include <wrl/client.h>

struct __declspec(uuid("{1014D2F8-9589-48E6-B536-0E20E51A019E}")) CLSID_TextureRenderer;

namespace DX9
{
	class CTextureRenderer : public CBaseVideoRenderer {
	public:
		CTextureRenderer(IDirect3DDevice9* pD3DDevice, HRESULT* pHR);
		virtual ~CTextureRenderer() {}

		HRESULT CheckMediaType(const CMediaType* pMediaFormat);
		HRESULT SetMediaType  (const CMediaType* pMediaFormat);
		HRESULT DoRenderSample(IMediaSample* pMediaSample);

		inline void OnReceiveFirstSample(IMediaSample* pMediaSample)
		{ DoRenderSample(pMediaSample); }

		inline IDirect3DTexture9* GetTexture() const
		{ return m_renderTexture.Get(); }

		inline int GetWidth () const { return m_width;  }
		inline int GetHeight() const { return m_height; }

	private:
		Microsoft::WRL::ComPtr<IDirect3DDevice9>  m_device;
		Microsoft::WRL::ComPtr<IDirect3DTexture9> m_renderTexture;
		Microsoft::WRL::ComPtr<IDirect3DTexture9> m_decodeTexture;

		int m_width;
		int m_height;
		int m_pitch;
	};
};