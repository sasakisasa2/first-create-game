#include "MediaRenderer.h"

#pragma comment(lib, "winmm.lib")

DX9::CMediaRenderer::~CMediaRenderer()
{
    if (m_mediaControl)
        m_mediaControl->Stop();

	if(m_soundRenderer)
    	m_graphBuilder->RemoveFilter(m_soundRenderer);
    m_graphBuilder->RemoveFilter(m_videoRenderer);

    m_basicAudio  .Release();
    m_mediaSeeking.Release();
    m_mediaControl.Release();

    m_soundRenderer.Release();
    m_videoRenderer.Release();

    m_textureRenderer .reset();
    m_resourceRenderer.reset();

    m_graphBuilder.Release();
}


HRESULT DX9::CMediaRenderer::LoadFromFile(ID3D12Device* device, LPCWSTR filename)
{
    try {
        HRESULT hr;

        hr = m_graphBuilder.CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC);
        if (hr != S_OK)
            throw hr;

        m_resourceRenderer = std::make_unique<CResourceRenderer>(device, &hr);
        if (hr != S_OK)
            throw hr;

        IBaseFilter* base_filter;
        hr = m_resourceRenderer->QueryInterface(IID_IBaseFilter, (void**)&base_filter);
        if (hr != S_OK)
            throw hr;
        hr = m_graphBuilder->AddFilter(base_filter, L"ResourceRenderer");
        if (hr != S_OK)
            throw hr;

        m_videoRenderer = base_filter;
//      base_filter->AddRef();  // AddFilter
//      base_filter->Release(); // QueryInterface

        hr = m_soundRenderer.CoCreateInstance(CLSID_DSoundRender, nullptr, CLSCTX_INPROC);
        if (hr != S_OK)
            throw hr;
        hr = m_graphBuilder->AddFilter(m_soundRenderer, L"DSoundRenderer");
        //if (hr != S_OK)
        //    throw hr;

        hr = m_graphBuilder->RenderFile(filename, nullptr);
        if (hr != S_OK)
            throw hr;

        hr = m_graphBuilder.QueryInterface(&m_mediaControl);
        if (hr != S_OK)
            throw hr;
        hr = m_graphBuilder.QueryInterface(&m_mediaSeeking);
        if (hr != S_OK)
            throw hr;
        hr = m_graphBuilder.QueryInterface(&m_basicAudio);
        if (hr != S_OK)
            throw hr;
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

    return S_OK;
}

HRESULT DX9::CMediaRenderer::LoadFromFile(IDirect3DDevice9* device, LPCWSTR filename)
{
	try {
		HRESULT hr;

        hr = m_graphBuilder.CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC);
		if (hr != S_OK)
			throw hr;

        m_textureRenderer = std::make_unique<CTextureRenderer>(device, &hr);
        if (hr != S_OK)
            throw hr;

        IBaseFilter* base_filter;
        hr = m_textureRenderer->QueryInterface(IID_IBaseFilter, (void**)&base_filter);
        if (hr != S_OK)
            throw hr;
        hr = m_graphBuilder->AddFilter(base_filter, L"TextureRenderer");
        if (hr != S_OK)
            throw hr;

        m_videoRenderer = base_filter;
//      base_filter->AddRef();  // AddFilter
//      base_filter->Release(); // QueryInterface

        hr = m_soundRenderer.CoCreateInstance(CLSID_DSoundRender, nullptr, CLSCTX_INPROC);
        if (hr == S_OK) 
            m_graphBuilder->AddFilter(m_soundRenderer, L"DSoundRenderer");

        hr = m_graphBuilder->RenderFile(filename, nullptr);
        if (hr != S_OK)
            throw hr;

        hr = m_graphBuilder.QueryInterface(&m_mediaControl);
        if (hr != S_OK)
            throw hr;
        hr = m_graphBuilder.QueryInterface(&m_mediaSeeking);
        if (hr != S_OK)
            throw hr;
        hr = m_graphBuilder.QueryInterface(&m_basicAudio);
        if (hr != S_OK)
            throw hr;
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

	return S_OK;
}
