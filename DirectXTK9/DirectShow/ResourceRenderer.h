#pragma once

#pragma warning(push)
#pragma warning(disable:4996)

#include <streams.h>
#include <atlcomcli.h>

#pragma warning(pop)

#include <d3d12.h>
#include <wrl/client.h>
#include <memory>

struct __declspec(uuid("{88DD3865-7277-4B7A-9C20-56BAEF1EA9AD}")) CLSID_ResourceRenderer;

namespace DX9
{
	class CResourceRenderer : public CBaseVideoRenderer {
	public:
		CResourceRenderer(ID3D12Device* pDevice, HRESULT* pHR);
		virtual ~CResourceRenderer() {}

		HRESULT CheckMediaType(const CMediaType* pMediaFormat);
		HRESULT SetMediaType  (const CMediaType* pMediaFormat);
		HRESULT DoRenderSample(IMediaSample* pMediaSample);

		inline void OnReceiveFirstSample(IMediaSample* pMediaSample)
		{ DoRenderSample(pMediaSample); }

		inline ID3D12Resource* GetResource() const
		{ return m_renderResource.Get(); }

		inline int GetWidth()  const { return m_width; }
		inline int GetHeight() const { return m_height; }

	private:
		Microsoft::WRL::ComPtr<ID3D12Device>       m_device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		Microsoft::WRL::ComPtr<ID3D12Resource>     m_renderResource;
		std::unique_ptr<BYTE[]>                    m_decodeBuffer;

		int                    m_width;
		int                    m_height;
		int                    m_pitch;
		D3D12_SUBRESOURCE_DATA m_subResource;
	};
};