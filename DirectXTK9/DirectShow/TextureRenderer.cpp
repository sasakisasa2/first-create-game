#include "TextureRenderer.h"
#include <d3dx9.h>

#ifdef NDEBUG
	#pragma comment(lib, "strmbase.lib")
	#pragma comment(lib, "d3dx9.lib")
#else
	#pragma comment(lib, "strmiids.lib")
	#pragma comment(lib, "d3dx9d.lib")
#endif

DX9::CTextureRenderer::CTextureRenderer(IDirect3DDevice9* pD3DDevice, HRESULT* pHR)
	: CBaseVideoRenderer(
		__uuidof(CLSID_TextureRenderer),
		NAME("TextureRenderer"), nullptr, pHR
	),
	m_device(pD3DDevice),
	m_width(0), m_height(0), m_pitch(0)
{
}

HRESULT DX9::CTextureRenderer::CheckMediaType(const CMediaType* pMediaFormat)
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

HRESULT DX9::CTextureRenderer::SetMediaType(const CMediaType* pMediaFormat)
{
//#ifdef DEBUG
//	if (pMediaFormat == NULL)
//		return E_POINTER;
//#endif

	// ビデオ情報取得
	const VIDEOINFOHEADER*  pVideoInfoHeader  = (VIDEOINFOHEADER*)pMediaFormat->Format();
	const BITMAPINFOHEADER* pBitmapInfoHeader = &(pVideoInfoHeader->bmiHeader);

	m_width  = pBitmapInfoHeader->biWidth;
	m_height = pBitmapInfoHeader->biHeight;
	m_pitch  = (int)((((size_t)m_width * pBitmapInfoHeader->biBitCount / 8) + 0x03) & ~0x03);

	// テクスチャ生成
	int height = m_height;
	if (height < 0)
		height *= -1;

	HRESULT hr;

	hr = D3DXCreateTexture(
		m_device.Get(),
		(UINT)m_width, (UINT)height, 1, 0, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, m_renderTexture.ReleaseAndGetAddressOf()
	);
	if (hr != D3D_OK)
		return hr;

	hr = ::D3DXCreateTexture(
		m_device.Get(),
		(UINT)m_width, (UINT)height, 1, 0, D3DFMT_X8R8G8B8,
		D3DPOOL_SYSTEMMEM, m_decodeTexture.ReleaseAndGetAddressOf()
	);
	if (hr != D3D_OK) {
		m_renderTexture.Reset();
		return hr;
	}

	return S_OK;
}

HRESULT DX9::CTextureRenderer::DoRenderSample(IMediaSample* pMediaSample)
{
//#ifdef DEBUG
//	if (pMediaSample == NULL)
//		return E_POINTER;
//#endif

	D3DLOCKED_RECT lock;
	if (m_decodeTexture->LockRect(0, &lock, 0, 0) != D3D_OK)
		return E_FAIL;

	BYTE* pTxtImage = (BYTE*)lock.pBits;
	const INT TXT_PITCH = lock.Pitch;

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

	m_decodeTexture->UnlockRect(0);
	m_device->UpdateTexture(m_decodeTexture.Get(), m_renderTexture.Get());

	return S_OK;
}
