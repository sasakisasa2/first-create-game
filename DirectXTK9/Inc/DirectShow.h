#pragma once

#include "..\DirectShow\MediaRenderer.h"
#include <DirectXHelpers.h>
#include <exception>

namespace DX9 {
	namespace MediaRenderer {
		inline std::unique_ptr<CMediaRenderer> CreateFromFile(
			ID3D12Device* device,
			const D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor,
			LPCWSTR		  mediafile
		)
		{
			std::unique_ptr<CMediaRenderer> media = std::make_unique<CMediaRenderer>();
			if (FAILED(media->LoadFromFile(device, mediafile)))
				throw std::exception();
			DirectX::CreateShaderResourceView(device, media->GetResource(), srvDescriptor);

			return media;
		}

		inline std::unique_ptr<CMediaRenderer> CreateFromFile(
			IDirect3DDevice9* device,
			LPCWSTR			  mediafile
		)
		{
			std::unique_ptr<CMediaRenderer> media = std::make_unique<CMediaRenderer>();
			if(FAILED(media->LoadFromFile(device, mediafile)))
				throw std::exception();

			return media;
		}
	}

	typedef std::unique_ptr<CMediaRenderer> MEDIARENDERER;
};
