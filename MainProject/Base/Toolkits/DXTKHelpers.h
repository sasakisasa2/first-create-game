#pragma once

namespace DirectXTK {
	// Graphics
	using DescriptorHeap       = std::unique_ptr<DirectX::DescriptorHeap>;
	using SpriteFont           = std::unique_ptr<DirectX::SpriteFont>;
	using SpriteBatch          = std::unique_ptr<DirectX::SpriteBatch>;
	using GeometricPrimitive   = std::unique_ptr<DirectX::GeometricPrimitive>;
	using Model                = std::unique_ptr<DirectX::Model>;
	using CommonStates         = std::unique_ptr<DirectX::CommonStates>;
	using BasicEffect          = std::unique_ptr<DirectX::BasicEffect>;
	using NormalMapEffect      = std::unique_ptr<DirectX::NormalMapEffect>;
	using IEffectFactory       = std::unique_ptr<DirectX::IEffectFactory>;
	using EffectFactory        = std::unique_ptr<DirectX::EffectFactory>;
	using EffectTextureFactory = std::unique_ptr<DirectX::EffectTextureFactory>;

	template<typename TVertex>
	using PrimitiveBatch = std::unique_ptr<DirectX::PrimitiveBatch<TVertex>>;

	template<typename TVertex>
	inline DirectXTK::PrimitiveBatch<TVertex> CreatePrimitiveBatch(ID3D12Device* device)
	{ return std::make_unique<DirectX::PrimitiveBatch<TVertex>>(device); }

	struct Sprite {
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_GPU_DESCRIPTOR_HANDLE            handle;
		DirectX::XMUINT2                       size;
	};

	struct Texture {
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_GPU_DESCRIPTOR_HANDLE            handle;
	};

	inline DirectXTK::DescriptorHeap CreateDescriptorHeap(
		ID3D12Device* device, size_t count
	)
	{ return std::make_unique<DirectX::DescriptorHeap>(device, count); }

	inline DirectXTK::SpriteBatch CreateSpriteBatch(
		ID3D12Device* device,
		DirectX::ResourceUploadBatch& upload,
		const DirectX::SpriteBatchPipelineStateDescription& psoDesc,
		const D3D12_VIEWPORT* viewport = nullptr
	)
	{ return std::make_unique<DirectX::SpriteBatch>(device, upload, psoDesc, viewport); }

	inline Sprite CreateSpriteSRV(
		ID3D12Device* device, const wchar_t* szFileName,
		DirectX::ResourceUploadBatch& resourceUpload,
		const DirectXTK::DescriptorHeap& descripterHeap, const size_t index
	)
	{
		Sprite sprite;

		const auto hr = DirectX::CreateWICTextureFromFileEx(
			device, resourceUpload, szFileName,
			0, D3D12_RESOURCE_FLAG_NONE, WIC_LOADER_FORCE_SRGB,
			sprite.resource.GetAddressOf()
		);
		if (FAILED(hr)) {
			DX::ThrowIfFailed(CreateDDSTextureFromFileEx(
				device, resourceUpload, szFileName,
				0, D3D12_RESOURCE_FLAG_NONE, DDS_LOADER_FORCE_SRGB,
				sprite.resource.GetAddressOf()
			));
		}
		DirectX::CreateShaderResourceView(
			device, sprite.resource.Get(),
			descripterHeap->GetCpuHandle(index)
		);
		sprite.handle = descripterHeap->GetGpuHandle(index);
		sprite.size   = DirectX::GetTextureSize(sprite.resource.Get());

		return sprite;
	}

	inline Texture CreateTextureSRV(
		ID3D12Device* device, const wchar_t* szFileName,
		DirectX::ResourceUploadBatch& resourceUpload,
		const DirectXTK::DescriptorHeap& descripterHeap, const size_t index
	)
	{
		Texture texture;

		const auto hr = DirectX::CreateWICTextureFromFileEx(
			device, resourceUpload, szFileName,
			0, D3D12_RESOURCE_FLAG_NONE, WIC_LOADER_FORCE_SRGB,
			texture.resource.GetAddressOf()
		);
		if (FAILED(hr)) {
			DX::ThrowIfFailed(CreateDDSTextureFromFileEx(
				device, resourceUpload, szFileName,
				0, D3D12_RESOURCE_FLAG_NONE, DDS_LOADER_FORCE_SRGB,
				texture.resource.GetAddressOf()
			));
		}
		DirectX::CreateShaderResourceView(
			device, texture.resource.Get(),
			descripterHeap->GetCpuHandle(index)
		);
		texture.handle = descripterHeap->GetGpuHandle(index);

		return texture;
	}

	inline Sprite CreateDefaultSpriteSRV(
		ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		const DirectXTK::DescriptorHeap& descripterHeap, const size_t index
	)
	{
		Sprite sprite;

		const uint32_t s_pixel = 0xffffffff;
		D3D12_SUBRESOURCE_DATA initData = { &s_pixel, sizeof(uint32_t), 0 };
		DX::ThrowIfFailed(
			DirectX::CreateTextureFromMemory(
				device, resourceUpload,
				1u, 1u, DXGI_FORMAT_R8G8B8A8_UNORM, initData,
				sprite.resource.ReleaseAndGetAddressOf()
			)
		);
		DirectX::CreateShaderResourceView(
			device, sprite.resource.Get(),
			descripterHeap->GetCpuHandle(index)
		);

		sprite.handle = descripterHeap->GetGpuHandle(index);
		sprite.size   = DirectX::GetTextureSize(sprite.resource.Get());

		return sprite;
	}

	inline Texture CreateDefaultTextureSRV(
		ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		const DirectXTK::DescriptorHeap& descripterHeap, const size_t index
	)
	{
		Texture texture;

		const uint32_t s_pixel = 0xffffffff;
		D3D12_SUBRESOURCE_DATA initData = { &s_pixel, sizeof(uint32_t), 0 };
		DX::ThrowIfFailed(
			DirectX::CreateTextureFromMemory(
				device, resourceUpload,
				1u, 1u, DXGI_FORMAT_R8G8B8A8_UNORM, initData,
				texture.resource.ReleaseAndGetAddressOf()
			)
		);
		DirectX::CreateShaderResourceView(
			device, texture.resource.Get(),
			descripterHeap->GetCpuHandle(index)
		);

		texture.handle = descripterHeap->GetGpuHandle(index);

		return texture;
	}

	inline DirectXTK::SpriteFont CreateSpriteFont(
		ID3D12Device* device,
		wchar_t const* fileName,
		DirectX::ResourceUploadBatch& resourceUpload,
		const DirectXTK::DescriptorHeap& descripterHeap, const size_t index
	)
	{
		return std::make_unique<DirectX::SpriteFont>(
			device, resourceUpload, fileName,
			descripterHeap->GetCpuHandle(index),
			descripterHeap->GetGpuHandle(index)
		);
	}

	inline DirectXTK::CommonStates CreateCommonStates(ID3D12Device* device)
	{ return std::make_unique<DirectX::CommonStates>(device); }

	inline DirectXTK::BasicEffect CreateBasicEffect(
		ID3D12Device* device, uint32_t effectFlags,
		const EffectPipelineStateDescription& pipelineDescription
	)
	{ return std::make_unique<DirectX::BasicEffect>(device, effectFlags, pipelineDescription); }

	inline DirectXTK::NormalMapEffect CreateNormalMapEffect(
		ID3D12Device* device, uint32_t effectFlags,
		const EffectPipelineStateDescription& pipelineDescription
	)
	{ return std::make_unique<DirectX::NormalMapEffect>(device, effectFlags, pipelineDescription); }

	inline DirectXTK::EffectFactory CreateEffectFactory(
		ID3D12DescriptorHeap* textureDescriptors,
		ID3D12DescriptorHeap* samplerDescriptors
	)
	{ return std::make_unique<DirectX::EffectFactory>(textureDescriptors, samplerDescriptors); }

	// Sound
	using Sound         = std::unique_ptr<DirectX::SoundEffect>;
	using SoundInstance = std::unique_ptr<DirectX::SoundEffectInstance>;
	using SoundStream   = std::unique_ptr<DirectX::SoundStreamInstance>;
	using WaveBank      = std::unique_ptr<DirectX::WaveBank>;

	inline DirectXTK::Sound CreateSound(
		DirectX::AudioEngine* audioEngine, const wchar_t* waveFileName
	)
	{ return std::make_unique<DirectX::SoundEffect>(audioEngine, waveFileName); }

	inline DirectXTK::WaveBank CreateWaveBank(
		DirectX::AudioEngine* audioEngine, const wchar_t* wbFileName
	)
	{ return std::make_unique<DirectX::WaveBank>(audioEngine, wbFileName); }
}

struct Rect : public RECT {
	Rect()
	{
		left   = 0;
		top    = 0;
		right  = 0;
		bottom = 0;
	}

	Rect(const int l, const int t, const int r, const int b)
	{
		left   = l;
		top    = t;
		right  = r;
		bottom = b;
	}

	Rect(const RECT& rect)
	{
		left   = rect.left;
		top    = rect.top;
		right  = rect.right;
		bottom = rect.bottom;
	}

	inline Rect& operator =(const RECT& rect)
	{
		left   = rect.left;
		top    = rect.top;
		right  = rect.right;
		bottom = rect.bottom;
		return *this;
	}

	inline bool Intersect(const RECT& rect)
	{
		if (right < rect.left || left   > rect.right ||
			top > rect.bottom || bottom < rect.top)
			return false;
		return true;
	}

	inline bool Intersect(const POINT& pt)
	{
		if (left > pt.x || top > pt.y || right < pt.x || bottom < pt.y)
			return false;
		return true;
	}
};

struct RectWH : public Rect {
	RectWH() : Rect()
	{
	}

	RectWH(const int l, const int t, const int w, const int h)
	{
		left   = l;
		top    = t;
		right  = left + w;
		bottom = top  + h;
	}
};
