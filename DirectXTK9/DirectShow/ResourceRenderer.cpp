#include "ResourceRenderer.h"

#include <ResourceUploadBatch.h>
#include <DirectXHelpers.h>
#include <d3dx12.h>

#ifdef NDEBUG
	#pragma comment(lib, "strmbase.lib")
#else
	#pragma comment(lib, "strmiids.lib")
#endif

DX9::CResourceRenderer::CResourceRenderer(ID3D12Device* pDevice, HRESULT* pHR)
	: CBaseVideoRenderer(
		__uuidof(CLSID_ResourceRenderer),
		NAME("ResourceRenderer"), nullptr, pHR
	),
	m_device(pDevice),
	m_width(0), m_height(0), m_pitch(0),
	m_subResource{}
{
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
	if (FAILED(
		m_device->CreateCommandQueue(
			&queueDesc,
			IID_PPV_ARGS(m_commandQueue.GetAddressOf())
		)
	)) {
		throw std::exception();
	}
}

HRESULT DX9::CResourceRenderer::CheckMediaType(const CMediaType* pMediaFormat)
{
//#ifdef _DEBUG
//	if (pMediaFormat == NULL)
//		return E_POINTER;
//#endif

	if (::IsEqualGUID(*pMediaFormat->FormatType(), FORMAT_VideoInfo) == false)
		return VFW_E_NO_TYPES;

	if (::IsEqualGUID(*pMediaFormat->Type(), MEDIATYPE_Video) == false)
		return VFW_E_INVALIDMEDIATYPE;

	if (::IsEqualGUID(*pMediaFormat->Subtype(), MEDIASUBTYPE_RGB24) == false)
		return VFW_E_INVALIDSUBTYPE;

	return S_OK;
}

HRESULT DX9::CResourceRenderer::SetMediaType(const CMediaType* pMediaFormat)
{
//#ifdef DEBUG
//	if (pMediaFormat == NULL)
//		return E_POINTER;
//#endif

	// ビデオ情報取得
	const VIDEOINFOHEADER*  pVideoInfoHeader  =  (VIDEOINFOHEADER*)pMediaFormat->Format();
	const BITMAPINFOHEADER* pBitmapInfoHeader = &(pVideoInfoHeader->bmiHeader);

	m_width  = pBitmapInfoHeader->biWidth;
	m_height = pBitmapInfoHeader->biHeight;
	m_pitch  = (int)((((size_t)m_width * pBitmapInfoHeader->biBitCount / 8) + 0x03) & ~0x03);

	// リソース生成
	int height = m_height;
	if (height < 0)
		height *= -1;

	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		static_cast<UINT64>(m_width), static_cast<UINT>(height)
	);
	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);

	HRESULT hr;

	hr = m_device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_GRAPHICS_PPV_ARGS(m_renderResource.ReleaseAndGetAddressOf()));
	if (FAILED(hr))
		return hr;

	m_decodeBuffer = std::make_unique<BYTE[]>(m_width * height * 4);

	m_subResource.pData      = (void*)m_decodeBuffer.get();
	m_subResource.RowPitch   = (LONG_PTR)m_width * 4;
	m_subResource.SlicePitch = m_subResource.RowPitch * height;

	return S_OK;
}

HRESULT DX9::CResourceRenderer::DoRenderSample(IMediaSample* pMediaSample)
{
//#ifdef DEBUG
//	if (pMediaSample == NULL)
//		return E_POINTER;
//#endif

	BYTE* pTxtImage = (BYTE*)m_subResource.pData;
	const LONG_PTR TXT_PITCH = m_subResource.RowPitch;

	BYTE* pBmpImage;
	pMediaSample->GetPointer(&pBmpImage);

	const int WIDTH = m_width / 4;
	LPDWORD   pTXT, pBMP;
	LPBYTE    pRMN;
	int       bmp_pitch, height;
	int       x, y;

	if (m_height >= 0) {
		// ボトムアップ
		pBmpImage +=  m_pitch * (m_height - 1);
		bmp_pitch  = -m_pitch;
		height     =  m_height;
	}
	else {
		// トップダウン
		bmp_pitch =  m_pitch;
		height    = -m_height;
	}

	for (y = 0; y < height; y++) {
		pTXT = (LPDWORD)pTxtImage;
		pBMP = (LPDWORD)pBmpImage;

		for (x = 0; x < WIDTH; x++) {
			pTXT[0] = pBMP[0] | 0xFF000000;
			pTXT[1] = ((pBMP[1] <<  8) | 0xFF000000) | (pBMP[0] >> 24);
			pTXT[2] = ((pBMP[2] << 16) | 0xFF000000) | (pBMP[1] >> 16);
			pTXT[3] = 0xFF000000 | (pBMP[2] >> 8);
			pTXT += 4;
			pBMP += 3;
		}

		// we might have remaining (misaligned) bytes here
		pRMN = (LPBYTE)pBMP;
		for (x = 0; x < m_width % 4; x++) {
			*pTXT = 0xFF000000 | (pRMN[2] << 16) | (pRMN[1] << 8) | pRMN[0];
			pTXT++;
			pRMN += 3;
		}

		pTxtImage += TXT_PITCH;
		pBmpImage += bmp_pitch;
	}	// for(y)

	DirectX::ResourceUploadBatch resourceUploadBatch(m_device.Get());
	resourceUploadBatch.Begin();

	resourceUploadBatch.Upload(m_renderResource.Get(), 0, &m_subResource, 1);
	resourceUploadBatch.Transition(
		m_renderResource.Get(), 
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COMMON
	);

	auto uploadResourcesFinished = resourceUploadBatch.End(m_commandQueue.Get());
	uploadResourcesFinished.wait();

	return S_OK;
}
