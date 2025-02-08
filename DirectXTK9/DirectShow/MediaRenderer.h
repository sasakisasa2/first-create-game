#pragma once

#include "ResourceRenderer.h"
#include "TextureRenderer.h"
#include <memory>

namespace DX9
{
	class CMediaRenderer {
	public:
		CMediaRenderer() : m_suspend(false) {}
		virtual ~CMediaRenderer();

		HRESULT LoadFromFile(ID3D12Device*     device, LPCWSTR filename);
		HRESULT LoadFromFile(IDirect3DDevice9* device, LPCWSTR filename);

		inline IDirect3DTexture9* Get() const
		{ return m_textureRenderer->GetTexture(); }

		inline ID3D12Resource* GetResource() const
		{ return m_resourceRenderer->GetResource(); }

		inline int GetWidth()  const
		{
			return m_resourceRenderer.get() != nullptr ? 
				m_resourceRenderer->GetWidth() : m_textureRenderer->GetWidth();
		}
		
		inline int GetHeight() const
		{
			return m_resourceRenderer.get() != nullptr ?
				m_resourceRenderer->GetHeight() : m_textureRenderer->GetHeight();
		}

		inline void Play()  { m_mediaControl->Run();   }
		inline void Stop()  { m_mediaControl->Stop();  }
		inline void Pause() { m_mediaControl->Pause(); }

		inline void Replay()
		{
			LONGLONG   pos = 0;
			m_mediaSeeking->SetPositions(
				&pos, AM_SEEKING_AbsolutePositioning,
				nullptr, AM_SEEKING_NoPositioning
			);
			m_mediaControl->Run();
		}

		inline OAFilterState GetState()
		{
			OAFilterState  state;
			const HRESULT hr = m_mediaControl->GetState(INFINITE, &state);
			if (hr == E_FAIL)
				return State_Stopped;
			return state;
		}

		inline bool isComplete()
		{
			LONGLONG duration = 0;
			m_mediaSeeking->GetDuration(&duration);
			LONGLONG pos = 0;
			m_mediaSeeking->GetCurrentPosition(&pos);

			if (pos < duration)
				return false;
			return true;
		}

		inline LONGLONG GetLength()
		{
			LONGLONG duration = 0;
			m_mediaSeeking->GetDuration(&duration);
			return duration;
		}

		inline LONGLONG GetPosition()
		{
			LONGLONG pos = 0;
			m_mediaSeeking->GetCurrentPosition(&pos);
			return pos;
		}

		inline double GetRate()
		{
			double rate = 0.0;
			m_mediaSeeking->GetRate(&rate);
			return rate;
		}

		inline GUID GetTimeFormat()
		{
			GUID format;
			m_mediaSeeking->GetTimeFormat(&format);
			return format;
		}

		inline long GetVolume()
		{
			long volume = -10000;
			m_basicAudio->get_Volume(&volume);
			return volume;
		}

		inline long GetBalance()
		{
			long balance = 0;
			m_basicAudio->get_Balance(&balance);
			return balance;
		}

		inline void SetPosition(LONGLONG position)
		{
			m_mediaSeeking->SetPositions(
				&position, AM_SEEKING_AbsolutePositioning,
				nullptr, AM_SEEKING_NoPositioning
			);
		}

		inline void SetRate(const double rate)
		{
			m_mediaSeeking->SetRate(rate);
		}

		inline void SetTimeFormat(const GUID& format)
		{
			m_mediaSeeking->SetTimeFormat(&format);
		}

		inline void SetRelativePosition(LONGLONG position)
		{
			m_mediaSeeking->SetPositions(
				&position, AM_SEEKING_RelativePositioning,
				nullptr, AM_SEEKING_NoPositioning
			);
		}

		inline void SetVolume(long volume)
		{
			m_basicAudio->put_Volume(volume);
		}

		inline void SetBalance(long balance)
		{
			m_basicAudio->put_Balance(balance);
		}

		inline void Suspend()
		{
			OAFilterState  state = State_Stopped;
			m_mediaControl->GetState(0, &state);
			if (state == State_Running) {
				m_suspend = true;
				m_mediaControl->Stop();
			}
		}

		inline void Resume()
		{
			if (!m_suspend) {
				m_suspend = false;
				m_mediaControl->Run();
			}
		}

	private:
		CComPtr<IGraphBuilder> m_graphBuilder;
		CComPtr<IBaseFilter>   m_videoRenderer;
		CComPtr<IBaseFilter>   m_soundRenderer;

		CComPtr<IMediaControl> m_mediaControl;
		CComPtr<IMediaSeeking> m_mediaSeeking;

		CComPtr<IBasicAudio>   m_basicAudio;

		std::unique_ptr<CResourceRenderer> m_resourceRenderer;
		std::unique_ptr<CTextureRenderer>  m_textureRenderer;

		bool m_suspend;
	};
};