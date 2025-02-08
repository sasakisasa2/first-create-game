#include "SpriteFont.h"

#ifdef NDEBUG
	#pragma comment(lib, "d3dx9.lib")
#else
	#pragma comment(lib, "d3dx9d.lib")
#endif

Microsoft::WRL::ComPtr<ID3DXFont> DX9::SpriteFont::CreateDefaultFont(IDirect3DDevice9* inDevice)
{
	LOGFONT log_font;
	GetObject((HFONT)GetStockObject(SYSTEM_FONT), sizeof(log_font), &log_font);

	D3DXFONT_DESC font_desc;
	font_desc.Height          = log_font.lfHeight;
	font_desc.Width           = log_font.lfWidth;
	font_desc.Weight          = log_font.lfWeight;
	font_desc.MipLevels       = D3DX_DEFAULT;
	font_desc.Italic          = log_font.lfItalic;
	font_desc.CharSet         = log_font.lfCharSet;
	font_desc.OutputPrecision = log_font.lfOutPrecision;
	font_desc.Quality         = log_font.lfQuality;
	font_desc.PitchAndFamily  = log_font.lfPitchAndFamily;
	const auto ret_val = lstrcpyn(font_desc.FaceName, log_font.lfFaceName, LF_FACESIZE);

	return CreateFromFontDesc(inDevice, font_desc);
}

Microsoft::WRL::ComPtr<ID3DXFont> DX9::SpriteFont::CreateFromFontName(
	IDirect3DDevice9* inDevice,
	LPCWSTR			  inFontName,
	const int		  inSize
)
{
	const auto hwindow  = GetDesktopWindow();
	const auto hdc      = GetDC(hwindow);
	const auto lfHeight = -MulDiv(inSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(hwindow, hdc);

	D3DXFONT_DESC font_desc;
	font_desc.Height          = lfHeight;
	font_desc.Width           = 0;
	font_desc.Weight          = 0;
	font_desc.MipLevels       = D3DX_DEFAULT;
	font_desc.Italic          = FALSE;
#ifdef UNICODE
	font_desc.CharSet         = DEFAULT_CHARSET;
#else
	font_desc.CharSet         = SHIFTJIS_CHARSET;
#endif
	font_desc.OutputPrecision = OUT_TT_PRECIS;	// OUT_DEFAULT_PRECIS;
	font_desc.Quality         = DEFAULT_QUALITY;// PROOF_QUALITY;
	font_desc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	const auto ret_val = lstrcpyn(font_desc.FaceName, inFontName, LF_FACESIZE);

	return CreateFromFontDesc(inDevice, font_desc);
}
