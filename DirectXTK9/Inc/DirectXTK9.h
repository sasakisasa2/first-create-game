#pragma once

#include <d3dx9.h>

#include "TextureLoader.h"
#include "SpriteFont.h"
#include "SpriteBatch9.h"

#include "Camera.h"
#include "Model.h"
#include "SkinnedModel.h"

#include "Shader.h"

namespace DX9 {
	inline D3DVECTOR VectorSet(const float x, const float y, const float z)
	{ return D3DXVECTOR3(x, y, z); }

	struct VertexScreen {
		DirectX::XMFLOAT3 position;
		float             rhw;
		D3DCOLOR          color;
		DirectX::XMFLOAT2 uv;

		VertexScreen()
			: position{ 0.0f, 0.0f, 0.0f }, rhw(1.0f), color(0), uv{ 0.0f, 0.0f }
		{}
		static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	};

	struct VertexColor {
		DirectX::XMFLOAT3 position;
		D3DCOLOR          color;
		DirectX::XMFLOAT2 uv;

		VertexColor()
			: position{ 0.0f, 0.0f, 0.0f }, color(0), uv{ 0.0f, 0.0f }
		{}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	};

	struct VertexNormal {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;

		VertexNormal()
			: position{ 0.0f, 0.0f, 0.0f }, normal{ 0.0f, 0.0f, 0.0f }, uv{ 0.0f, 0.0f }
		{}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	};

namespace Colors {
	enum Colors {
		AliceBlue            = 0xFFF0F8FF,
		AntiqueWhite         = 0xFFFAEBD7,
		Aqua                 = 0xFF00FFFF,
		Aquamarine           = 0xFF7FFFD4,
		Azure                = 0xFFF0FFFF,
		Beige                = 0xFFF5F5DC,
		Bisque               = 0XFFFFE4C4,
		Black                = 0xFF000000,
		BlanchedAlmond       = 0xFFFFEBCD,
		Blue                 = 0xFF0000FF,
		BlueViolet           = 0xFF8A2BE2,
		Brown                = 0xFFA52A2A,
		BurlyWood            = 0xFFDEB887,
		CadetBlue            = 0xFF5F9EA0,
		Chartreuse           = 0xFF7FFF00,
		Chocolate            = 0xFFD2691E,
		Coral                = 0xFFFF7F50,
		CornflowerBlue       = 0xFF6495ED,
		Cornsilk             = 0xFFFFF8DC,
		Crimson              = 0xFFDC143C,
		Cyan                 = 0xFF00FFFF,
		DarkBlue             = 0xFF00008B,
		DarkCyan             = 0xFF008B8B,
		DarkGoldenrod        = 0xFFB8860B,
		DarkGray             = 0xFFA9A9A9,
		DarkGrey             = 0xFFA9A9A9,
		DarkGreen            = 0xFF006400,
		DarkKhaki            = 0xFFBDB76B,
		DarkMagenta          = 0xFF8B008B,
		DarkOliveGreen       = 0xFF556B2F,
		DarkOrange           = 0xFFFF8C00,
		DarkOrchid           = 0xFF9932CC,
		DarkRed              = 0xFF8B0000,
		DarkSalmon           = 0xFFE9967A,
		DarkSeaGreen         = 0xFF8FBC8F,
		DarkSlateBlue        = 0xFF483D8B,
		DarkSlateGray        = 0xFF2F4F4F,
		DarkSlateGrey        = 0xFF2F4F4F,
		DarkTurquoise        = 0xFF00CED1,
		DarkViolet           = 0xFF9400D3,
		DeepPink             = 0xFFFF1493,
		DeepSkyBlue          = 0xFF00BFFF,
		DimGray              = 0xFF696969,
		DimGrey              = 0xFF696969,
		DodgerBlue           = 0xFF1E90FF,
		FireBrick            = 0xFFB22222,
		FloralWhite          = 0xFFFFFAF0,
		ForestGreen          = 0xFF228B22,
		Fuchsia              = 0xFFFF00FF,
		Gainsboro            = 0xFFDCDCDC,
		GhostWhite           = 0xFFF8F8FF,
		Gold                 = 0xFFFFD700,
		Goldenrod            = 0xFFDAA520,
		Gray                 = 0xFF808080,
		Grey                 = 0xFF808080,
		Green                = 0xFF008000,
		GreenYellow          = 0xFFADFF2F,
		Honeydew             = 0xFFF0FFF0,
		HotPink              = 0xFFFF69B4,
		IndianRed            = 0xFFCD5C5C,
		Indigo               = 0xFF4B0082,
		Ivory                = 0xFFFFFFF0,
		Khaki                = 0xFFF0E68C,
		Lavender             = 0xFFE6E6FA,
		LavenderBlush        = 0xFFFFF0F5,
		LawnGreen            = 0xFF7CFC00,
		LemonChiffon         = 0xFFFFFACD,
		LightBlue            = 0xFFADD8E6,
		LightCoral           = 0xFFF08080,
		LightCyan            = 0xFFE0FFFF,
		LightGoldenrodYellow = 0xFFFAFAD2,
		LightGreen           = 0xFF90EE90,
		LightGray            = 0xFFD3D3D3,
		LightGrey            = 0xFFD3D3D3,
		LightPink            = 0xFFFFB6C1,
		LightSalmon          = 0xFFFFA07A,
		LightSeaGreen        = 0xFF20B2AA,
		LightSkyBlue         = 0xFF87CEFA,
		LightSlateGray       = 0xFF778899,
		LightSlateGrey       = 0xFF778899,
		LightSteelBlue       = 0xFFB0C4DE,
		LightYellow          = 0xFFFFFFE0,
		Lime                 = 0xFF00FF00,
		LimeGreen            = 0xFF32CD32,
		Linen                = 0xFFFAF0E6,
		Magenta              = 0xFFFF00FF,
		Maroon               = 0xFF800000,
		MediumAquamarine     = 0xFF66CDAA,
		MediumBlue           = 0xFF0000CD,
		MediumOrchid         = 0xFFBA55D3,
		MediumPurple         = 0xFF9370DB,
		MediumSeaGreen       = 0xFF3CB371,
		MediumSlateBlue      = 0xFF7B68EE,
		MediumSpringGreen    = 0xFF00FA9A,
		MediumTurquoise      = 0xFF48D1CC,
		MediumVioletRed      = 0xFFC71585,
		MidnightBlue         = 0xFF191970,
		MintCream            = 0xFFF5FFFA,
		MistyRose            = 0xFFFFE4E1,
		Moccasin             = 0xFFFFE4B5,
		NavajoWhite          = 0xFFFFDEAD,
		Navy                 = 0xFF000080,
		OldLace              = 0xFFFDF5E6,
		Olive                = 0xFF808000,
		OliveDrab            = 0xFF6B8E23,
		Orange               = 0xFFFFA500,
		OrangeRed            = 0xFFFF4500,
		Orchid               = 0xFFDA70D6,
		PaleGoldenrod        = 0xFFEEE8AA,
		PaleGreen            = 0xFF98FB98,
		PaleTurquoise        = 0xFFAFEEEE,
		PaleVioletRed        = 0xFFDB7093,
		PapayaWhip           = 0xFFFFEFD5,
		PeachPuff            = 0xFFFFDAB9,
		Peru                 = 0xFFCD853F,
		Pink                 = 0xFFFFC0CB,
		Plum                 = 0xFFDDA0DD,
		PowderBlue           = 0xFFB0E0E6,
		Purple               = 0xFF800080,
		Red                  = 0xFFFF0000,
		RosyBrown            = 0xFFBC8F8F,
		RoyalBlue            = 0xFF4169E1,
		SaddleBrown          = 0xFF8B4513,
		Salmon               = 0xFFFA8072,
		SandyBrown           = 0xFFF4A460,
		SeaGreen             = 0xFF2E8B57,
		Seashell             = 0xFFFFF5EE,
		Sienna               = 0xFFA0522D,
		Silver               = 0xFFC0C0C0,
		SkyBlue              = 0xFF87CEEB,
		SlateBlue            = 0xFF6A5ACD,
		SlateGray            = 0xFF708090,
		SlateGrey            = 0xFF708090,
		Snow                 = 0xFFFFFAFA,
		SpringGreen          = 0xFF00FF7F,
		SteelBlue            = 0xFF4682B4,
		Tan                  = 0xFFD2B48C,
		Teal                 = 0xFF008080,
		Thistle              = 0xFFD8BFD8,
		Tomato               = 0xFFFF6347,
		Turquoise            = 0xFF40E0D0,
		Violet               = 0xFFEE82EE,
		Wheat                = 0xFFF5DEB3,
		White                = 0xFFFFFFFF,
		WhiteSmoke           = 0xFFF5F5F5,
		Yellow               = 0xFFFFFF00,
		YellowGreen          = 0xFF9ACD32
	};

	inline DWORD RGBA(const int r, const int g, const int b, const int a)
	{ return D3DCOLOR_RGBA(r, g, b, a); }

	inline DWORD Alpha(const int a)
	{ return D3DCOLOR_RGBA(0xff, 0xff, 0xff, a); }

	inline DWORD Alpha(const float a)
	{ return D3DCOLOR_RGBA(0xff, 0xff, 0xff, (int)(255.0f * a)); }

	inline D3DXCOLOR Value(const float r, const float g, const float b, const float a)
	{
		return D3DXCOLOR(r, g, b, a);
	}
};

enum class AlphaBlendMode {
	Modulate,
	Add,
	Subtract,
	SourceColor,
	ZeroColor
};

#pragma region renderstate
	// Rendering State
	#define Lighting_Enable                        D3DRS_LIGHTING,(DWORD)TRUE
	#define Lighting_Disable                       D3DRS_LIGHTING,(DWORD)FALSE

	#define DepthBuffer_Enable                     D3DRS_ZENABLE,(DWORD)TRUE
	#define DepthBuffer_Disable                    D3DRS_ZENABLE,(DWORD)FALSE

	#define DepthBufferWrite_Enable                D3DRS_ZWRITEENABLE,(DWORD)TRUE
	#define DepthBufferWrite_Disable               D3DRS_ZWRITEENABLE,(DWORD)FALSE

	#define AlphaTest_Enable                       D3DRS_ALPHATESTENABLE,(DWORD)TRUE
	#define AlphaTest_Disable                      D3DRS_ALPHATESTENABLE,(DWORD)FALSE

	//#define Dither_Enable                          D3DRS_DITHERENABLE,(DWORD)TRUE
	//#define Dither_Disable                         D3DRS_DITHERENABLE,(DWORD)FALSE

	#define Specular_Enable                        D3DRS_SPECULARENABLE,(DWORD)TRUE
	#define Specular_Disable                       D3DRS_SPECULARENABLE,(DWORD)FALSE

	#define Stencil_Enable                         D3DRS_STENCILENABLE,(DWORD)TRUE
	#define Stencil_Disable                        D3DRS_STENCILENABLE(DWORD),FALSE

	#define Clipping_Enable                        D3DRS_CLIPPING,(DWORD)TRUE
	#define Clipping_Disable                       D3DRS_CLIPPING,(DWORD)FALSE

	//#define LocalViewer_Enable                     D3DRS_LOCALVIEWER,(DWORD)TRUE
	//#define LocalViewer_Disable                    D3DRS_LOCALVIEWER,(DWORD)FALSE

	#define ScissorTest_Enable                     D3DRS_SCISSORTESTENABLE,(DWORD)TRUE
	#define ScissorTest_Disable                    D3DRS_SCISSORTESTENABLE,(DWORD)FALSE

	//#define SeparateAlphaBlend_Enable              D3DRS_SEPARATEALPHABLENDENABLE,(DWORD)TRUE
	//#define SeparateAlphaBlend_Disable             D3DRS_SEPARATEALPHABLENDENABLE,(DWORD)FALSE

	#define NormalizeNormals_Enable                D3DRS_NORMALIZENORMALS,(DWORD)TRUE
	#define NormalizeNormals_Disable               D3DRS_NORMALIZENORMALS,(DWORD)FALSE

	#define Fog_Enable                             D3DRS_FOGENABLE,(DWORD)TRUE
	#define Fog_Disable                            D3DRS_FOGENABLE,(DWORD)FALSE

	#define FogColor(r,g,b)                        D3DRS_FOGCOLOR,D3DCOLOR_XRGB(r,g,b)

	#define FogStart                               D3DRS_FOGSTART
	#define FogEnd                                 D3DRS_FOGEND
	#define FogDensity                             D3DRS_FOGDENSITY

	#define RangeFog_Enable                        D3DRS_RANGEFOGENABLE,(DWORD)TRUE
	#define RangeFog_Disable                       D3DRS_RANGEFOGENABLE,(DWORD)FALSE

	#define FogVertexMode_None                     D3DRS_FOGVERTEXMODE,(DWORD)D3DFOG_NONE
	#define FogVertexMode_Linear                   D3DRS_FOGVERTEXMODE,(DWORD)D3DFOG_LINEAR
	#define FogVertexMode_Exponent                 D3DRS_FOGVERTEXMODE,(DWORD)D3DFOG_EXP
	#define FogVertexMode_ExponentSquared          D3DRS_FOGVERTEXMODE,(DWORD)D3DFOG_EXP2

	#define FogPixelMode_None                      D3DRS_FOGTABLEMODE,(DWORD)D3DFOG_NONE
	#define FogPixelMode_Linear                    D3DRS_FOGTABLEMODE,(DWORD)D3DFOG_LINEAR
	#define FogPixelMode_Exponent                  D3DRS_FOGTABLEMODE,(DWORD)D3DFOG_EXP
	#define FogPixelMode_ExponentSquared           D3DRS_FOGTABLEMODE,(DWORD)D3DFOG_EXP2

	#define PointSize                              D3DRS_POINTSIZE
	#define PointSizeMin                           D3DRS_POINTSIZE_MIN
	#define PointSizeMax                           D3DRS_POINTSIZE_MAX

	#define PointSprite_Enable                     D3DRS_POINTSPRITEENABLE,(DWORD)TRUE
	#define PointSprite_Disable                    D3DRS_POINTSPRITEENABLE,(DWORD)FALSE

	#define PointScale_Enable                      D3DRS_POINTSCALEENABLE,(DWORD)TRUE
	#define PointScale_Disable                     D3DRS_POINTSCALEENABLE,(DWORD)FALSE

	//#define MultiSampleAntiAlias_Enable            D3DRS_MULTISAMPLEANTIALIAS,(DWORD)TRUE
	//#define MultiSampleAntiAlias_Disable           D3DRS_MULTISAMPLEANTIALIAS,(DWORD)FALSE

	//#define MultiSampleMask                        D3DRS_MULTISAMPLEMASK

	#define FillMode_Point                         D3DRS_FILLMODE,(DWORD)D3DFILL_POINT
	#define FillMode_WireFrame                     D3DRS_FILLMODE,(DWORD)D3DFILL_WIREFRAME
	#define FillMode_Solid                         D3DRS_FILLMODE,(DWORD)D3DFILL_SOLID

	#define ShadeMode_Flat                         D3DRS_SHADEMODE,(DWORD)D3DSHADE_FLAT
	#define ShadeMode_Gouraud                      D3DRS_SHADEMODE,(DWORD)D3DSHADE_GOURAUD

	#define CullMode_CullClockwiseFace             D3DRS_CULLMODE,(DWORD)D3DCULL_CW
	#define CullMode_CullCounterClockwiseFace      D3DRS_CULLMODE,(DWORD)D3DCULL_CCW
	#define CullMode_None                          D3DRS_CULLMODE,(DWORD)D3DCULL_NONE

	#define AmbientLightColor(r,g,b)               D3DRS_AMBIENT,D3DCOLOR_XRGB(r,g,b)

	//#define DepthBias                              D3DRS_DEPTHBIAS

	#define AlphaBlend_Enable                      D3DRS_ALPHABLENDENABLE,(DWORD)TRUE
	#define AlphaBlend_Disable                     D3DRS_ALPHABLENDENABLE,(DWORD)FALSE

	//#define BlendFactor(r,g,b)                     D3DRS_BLENDFACTOR,D3DCOLOR_XRGB(r,g,b)

	#define BlendOperation_Add                     D3DRS_BLENDOP,(DWORD)D3DBLENDOP_ADD
	#define BlendOperation_Max                     D3DRS_BLENDOP,(DWORD)D3DBLENDOP_MAX
	#define BlendOperation_Min                     D3DRS_BLENDOP,(DWORD)D3DBLENDOP_MIN
	#define BlendOperation_ReverseSubtract         D3DRS_BLENDOP,(DWORD)D3DBLENDOP_REVSUBTRACT
	#define BlendOperation_Subtract                D3DRS_BLENDOP,(DWORD)D3DBLENDOP_SUBTRACT

	#define SourceBlend_Default                      D3DRS_SRCBLEND,(DWORD)D3DBLEND_ONE
	#define SourceBlend_Zero                         D3DRS_SRCBLEND,(DWORD)D3DBLEND_ZERO
	#define SourceBlend_One                          D3DRS_SRCBLEND,(DWORD)D3DBLEND_ONE
	#define SourceBlend_SourceColor                  D3DRS_SRCBLEND,(DWORD)D3DBLEND_SRCCOLOR
	#define SourceBlend_InverseSourceColor           D3DRS_SRCBLEND,(DWORD)D3DBLEND_INVSRCCOLOR
	#define SourceBlend_SourceAlpha                  D3DRS_SRCBLEND,(DWORD)D3DBLEND_SRCALPHA
	#define SourceBlend_InverseSourceAlpha           D3DRS_SRCBLEND,(DWORD)D3DBLEND_INVSRCALPHA
	#define SourceBlend_DestinationAlpha             D3DRS_SRCBLEND,(DWORD)D3DBLEND_DESTALPHA
	#define SourceBlend_InverseDestinationAlpha      D3DRS_SRCBLEND,(DWORD)D3DBLEND_INVDESTALPHA
	#define SourceBlend_DestinationColor             D3DRS_SRCBLEND,(DWORD)D3DBLEND_DESTCOLOR
	#define SourceBlend_InverseDestinationColor      D3DRS_SRCBLEND,(DWORD)D3DBLEND_INVDESTCOLOR
	#define SourceBlend_SourceAlphaSaturation        D3DRS_SRCBLEND,(DWORD)D3DBLEND_SRCALPHASAT
	#define SourceBlend_BothSourceAlpha              D3DRS_SRCBLEND,(DWORD)D3DBLEND_BOTHSRCALPHA
	#define SourceBlend_BothInverseSourceAlpha       D3DRS_SRCBLEND,(DWORD)D3DBLEND_BOTHINVSRCALPHA
	#define SourceBlend_BlendFactor                  D3DRS_SRCBLEND,(DWORD)D3DBLEND_BLENDFACTOR
	#define SourceBlend_InverseBlendFactor           D3DRS_SRCBLEND,(DWORD)D3DBLEND_INVBLENDFACTOR

	#define DestinationBlend_Default                 D3DRS_DESTBLEND,(DWORD)D3DBLEND_ZERO
	#define DestinationBlend_Zero                    D3DRS_DESTBLEND,(DWORD)D3DBLEND_ZERO
	#define DestinationBlend_One                     D3DRS_DESTBLEND,(DWORD)D3DBLEND_ONE
	#define DestinationBlend_SourceColor             D3DRS_DESTBLEND,(DWORD)D3DBLEND_SRCCOLOR
	#define DestinationBlend_InverseSourceColor      D3DRS_DESTBLEND,(DWORD)D3DBLEND_INVSRCCOLOR
	#define DestinationBlend_SourceAlpha             D3DRS_DESTBLEND,(DWORD)D3DBLEND_SRCALPHA
	#define DestinationBlend_InverseSourceAlpha      D3DRS_DESTBLEND,(DWORD)D3DBLEND_INVSRCALPHA
	#define DestinationBlend_DestinationAlpha        D3DRS_DESTBLEND,(DWORD)D3DBLEND_DESTALPHA
	#define DestinationBlend_InverseDestinationAlpha D3DRS_DESTBLEND,(DWORD)D3DBLEND_INVDESTALPHA
	#define DestinationBlend_DestinationColor        D3DRS_DESTBLEND,(DWORD)D3DBLEND_DESTCOLOR
	#define DestinationBlend_InverseDestinationColor D3DRS_DESTBLEND,(DWORD)D3DBLEND_INVDESTCOLOR
	#define DestinationBlend_SourceAlphaSaturation   D3DRS_DESTBLEND,(DWORD)D3DBLEND_SRCALPHASAT
	#define DestinationBlend_BothSourceAlpha         D3DRS_DESTBLEND,(DWORD)D3DBLEND_BOTHSRCALPHA
	#define DestinationBlend_BothInverseSourceAlpha  D3DRS_DESTBLEND,(DWORD)D3DBLEND_BOTHINVSRCALPHA
	#define DestinationBlend_BlendFactor             D3DRS_DESTBLEND,(DWORD)D3DBLEND_BLENDFACTOR
	#define DestinationBlend_InverseBlendFactor      D3DRS_DESTBLEND,(DWORD)D3DBLEND_INVBLENDFACTOR

	#define AlphaSourceBlend_Default                 D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_ONE
	#define AlphaSourceBlend_Zero                    D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_ZERO
	#define AlphaSourceBlend_One                     D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_ONE
	#define AlphaSourceBlend_SourceColor             D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_SRCCOLOR
	#define AlphaSourceBlend_InverseSourceColor      D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_INVSRCCOLOR
	#define AlphaSourceBlend_SourceAlpha             D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_SRCALPHA
	#define AlphaSourceBlend_InverseSourceAlpha      D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_INVSRCALPHA
	#define AlphaSourceBlend_DestinationAlpha        D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_DESTALPHA
	#define AlphaSourceBlend_InverseDestinationAlpha D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_INVDESTALPHA
	#define AlphaSourceBlend_DestinationColor        D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_DESTCOLOR
	#define AlphaSourceBlend_InverseDestinationColor D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_INVDESTCOLOR
	#define AlphaSourceBlend_SourceAlphaSaturation   D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_SRCALPHASAT
	#define AlphaSourceBlend_BothSourceAlpha         D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_BOTHSRCALPHA
	#define AlphaSourceBlend_BothInverseSourceAlpha  D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_BOTHINVSRCALPHA
	#define AlphaSourceBlend_BlendFactor             D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_BLENDFACTOR
	#define AlphaSourceBlend_InverseBlendFactor      D3DRS_SRCBLENDALPHA,(DWORD)D3DBLEND_INVBLENDFACTOR

	#define AlphaDestinationBlend_Default                 D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_ZERO
	#define AlphaDestinationBlend_Zero                    D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_ZERO
	#define AlphaDestinationBlend_One                     D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_ONE
	#define AlphaDestinationBlend_SourceColor             D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_SRCCOLOR
	#define AlphaDestinationBlend_InverseSourceColor      D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_INVSRCCOLOR
	#define AlphaDestinationBlend_SourceAlpha             D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_SRCALPHA
	#define AlphaDestinationBlend_InverseSourceAlpha      D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_INVSRCALPHA
	#define AlphaDestinationBlend_DestinationAlpha        D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_DESTALPHA
	#define AlphaDestinationBlend_InverseDestinationAlpha D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_INVDESTALPHA
	#define AlphaDestinationBlend_DestinationColor        D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_DESTCOLOR
	#define AlphaDestinationBlend_InverseDestinationColor D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_INVDESTCOLOR
	#define AlphaDestinationBlend_SourceAlphaSaturation   D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_SRCALPHASAT
	#define AlphaDestinationBlend_BothSourceAlpha         D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_BOTHSRCALPHA
	#define AlphaDestinationBlend_BothInverseSourceAlpha  D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_BOTHINVSRCALPHA
	#define AlphaDestinationBlend_BlendFactor             D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_BLENDFACTOR
	#define AlphaDestinationBlend_InverseBlendFactor      D3DRS_DESTBLENDALPHA,(DWORD)D3DBLEND_INVBLENDFACTOR

	#define AlphaBlendOperation_Add                D3DRS_BLENDOPALPHA,(DWORD)D3DBLENDOP_ADD
	#define AlphaBlendOperation_Max                D3DRS_BLENDOPALPHA,(DWORD)D3DBLENDOP_MAX
	#define AlphaBlendOperation_Min                D3DRS_BLENDOPALPHA,(DWORD)D3DBLENDOP_MIN
	#define AlphaBlendOperation_ReverseSubtract    D3DRS_BLENDOPALPHA,(DWORD)D3DBLENDOP_REVSUBTRACT
	#define AlphaBlendOperation_Subtract           D3DRS_BLENDOPALPHA,(DWORD)D3DBLENDOP_SUBTRACT

	//#define ReferenceAlpha                         D3DRS_ALPHAREF
	//#define ReferenceStencil                       D3DRS_STENCILREF

	//#define AlphaFunction_Default                  D3DRS_ALPHAFUNC,(DWORD)D3DCMP_ALWAYS
	//#define AlphaFunction_Never                    D3DRS_ALPHAFUNC,(DWORD)D3DCMP_NEVER
	//#define AlphaFunction_Always                   D3DRS_ALPHAFUNC,(DWORD)D3DCMP_ALWAYS
	//#define AlphaFunction_Equal                    D3DRS_ALPHAFUNC,(DWORD)D3DCMP_EQUAL
	//#define AlphaFunction_NotEqual                 D3DRS_ALPHAFUNC,(DWORD)D3DCMP_NOTEQUAL
	//#define AlphaFunction_Greater                  D3DRS_ALPHAFUNC,(DWORD)D3DCMP_GREATER
	//#define AlphaFunction_GreaterEqual             D3DRS_ALPHAFUNC,(DWORD)D3DCMP_GREATEREQUAL
	//#define AlphaFunction_Less                     D3DRS_ALPHAFUNC,(DWORD)D3DCMP_LESS
	//#define AlphaFunction_LessEqual                D3DRS_ALPHAFUNC,(DWORD)D3DCMP_LESSEQUAL

	//#define DepthBufferFunction_Default            D3DRS_ZFUNC,(DWORD)D3DCMP_LESSEQUAL
	//#define DepthBufferFunction_Never              D3DRS_ZFUNC,(DWORD)D3DCMP_NEVER
	//#define DepthBufferFunction_Always             D3DRS_ZFUNC,(DWORD)D3DCMP_ALWAYS
	//#define DepthBufferFunction_Equal              D3DRS_ZFUNC,(DWORD)D3DCMP_EQUAL
	//#define DepthBufferFunction_NotEqual           D3DRS_ZFUNC,(DWORD)D3DCMP_NOTEQUAL
	//#define DepthBufferFunction_Greater            D3DRS_ZFUNC,(DWORD)D3DCMP_GREATER
	//#define DepthBufferFunction_GreaterEqual       D3DRS_ZFUNC,(DWORD)D3DCMP_GREATEREQUAL
	//#define DepthBufferFunction_Less               D3DRS_ZFUNC,(DWORD)D3DCMP_LESS
	//#define DepthBufferFunction_LessEqual          D3DRS_ZFUNC,(DWORD)D3DCMP_LESSEQUAL

	//#define StencilFunction_Default                D3DRS_STENCILFUNC,(DWORD)D3DCMP_ALWAYS
	//#define StencilFunction_Never                  D3DRS_STENCILFUNC,(DWORD)D3DCMP_NEVER
	//#define StencilFunction_Always                 D3DRS_STENCILFUNC,(DWORD)D3DCMP_ALWAYS
	//#define StencilFunction_Equal                  D3DRS_STENCILFUNC,(DWORD)D3DCMP_EQUAL
	//#define StencilFunction_NotEqual               D3DRS_STENCILFUNC,(DWORD)D3DCMP_NOTEQUAL
	//#define StencilFunction_Greater                D3DRS_STENCILFUNC,(DWORD)D3DCMP_GREATER
	//#define StencilFunction_GreaterEqual           D3DRS_STENCILFUNC,(DWORD)D3DCMP_GREATEREQUAL
	//#define StencilFunction_Less                   D3DRS_STENCILFUNC,(DWORD)D3DCMP_LESS
	//#define StencilFunction_LessEqual              D3DRS_STENCILFUNC,(DWORD)D3DCMP_LESSEQUAL

	//#define TwoSidedStencilMode_Enable             D3DRS_TWOSIDEDSTENCILMODE,(DWORD)TRUE
	//#define TwoSidedStencilMode_Disable            D3DRS_TWOSIDEDSTENCILMODE,(DWORD)FALSE

	//#define StencilFailOperation_Default                        D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_KEEP
	//#define StencilFailOperation_Keep                           D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_KEEP
	//#define StencilFailOperation_Zero                           D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_ZERO
	//#define StencilFailOperation_Replace                        D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_REPLACE
	//#define StencilFailOperation_IncrementSaturation            D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_INCRSAT
	//#define StencilFailOperation_DecrementSaturation            D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_DECRSAT
	//#define StencilFailOperation_Invert                         D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_INVERT
	//#define StencilFailOperation_Increment                      D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_INCR
	//#define StencilFailOperation_Decrement                      D3DRS_STENCILFAIL,(DWORD)D3DSTENCILOP_DECR

	//#define StencilDepthBufferFailOperation_Default             D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_KEEP
	//#define StencilDepthBufferFailOperation_Keep                D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_KEEP
	//#define StencilDepthBufferFailOperation_Zero                D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_ZERO
	//#define StencilDepthBufferFailOperation_Replace             D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_REPLACE
	//#define StencilDepthBufferFailOperation_IncrementSaturation D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_INCRSAT
	//#define StencilDepthBufferFailOperation_DecrementSaturation D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_DECRSAT
	//#define StencilDepthBufferFailOperation_Invert              D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_INVERT
	//#define StencilDepthBufferFailOperation_Increment           D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_INCR
	//#define StencilDepthBufferFailOperation_Decrement           D3DRS_STENCILZFAIL,(DWORD)D3DSTENCILOP_DECR

	//#define StencilPassOperation_Default                        D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_KEEP
	//#define StencilPassOperation_Keep                           D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_KEEP
	//#define StencilPassOperation_Zero                           D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_ZERO
	//#define StencilPassOperation_Replace                        D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_REPLACE
	//#define StencilPassOperation_IncrementSaturation            D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_INCRSAT
	//#define StencilPassOperation_DecrementSaturation            D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_DECRSAT
	//#define StencilPassOperation_Invert                         D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_INVERT
	//#define StencilPassOperation_Increment                      D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_INCR
	//#define StencilPassOperation_Decrement                      D3DRS_STENCILPASS,(DWORD)D3DSTENCILOP_DECR

	//#define StencilMask                            D3DRS_STENCILMASK
	//#define StencilWriteMask                       D3DRS_STENCILWRITEMASK

	//#define CounterClockwiseStencilFail            D3DRS_CCW_STENCILFAIL
	//#define CounterClockwiseStencilDepthBufferFail D3DRS_CCW_STENCILZFAIL
	//#define CounterClockwiseStencilPass            D3DRS_CCW_STENCILPASS
	//#define CounterClockwiseStencilFunction        D3DRS_CCW_STENCILFUNC

	//#define Wrap0                                  D3DRS_WRAP0
	//#define Wrap1                                  D3DRS_WRAP1
	//#define Wrap2                                  D3DRS_WRAP2
	//#define Wrap3                                  D3DRS_WRAP3
	//#define Wrap4                                  D3DRS_WRAP4
	//#define Wrap5                                  D3DRS_WRAP5
	//#define Wrap6                                  D3DRS_WRAP6
	//#define Wrap7                                  D3DRS_WRAP7
	//#define Wrap8                                  D3DRS_WRAP8
	//#define Wrap9                                  D3DRS_WRAP9
	//#define Wrap10                                 D3DRS_WRAP10
	//#define Wrap11                                 D3DRS_WRAP11
	//#define Wrap12                                 D3DRS_WRAP12
	//#define Wrap13                                 D3DRS_WRAP13
	//#define Wrap14                                 D3DRS_WRAP14
	//#define Wrap15                                 D3DRS_WRAP15

	//#define TextureWrapCoordinates_None            0
	//#define TextureWrapCoordinates_Zero            D3DWRAPCOORD_0
	//#define TextureWrapCoordinates_One             D3DWRAPCOORD_1
	//#define TextureWrapCoordinates_Two             D3DWRAPCOORD_2
	//#define TextureWrapCoordinates_Three           D3DWRAPCOORD_3

	//#define TextureWrap_None                       0
	//#define TextureWrap_U                          D3DWRAP_U
	//#define TextureWrap_V                          D3DWRAP_V
	//#define TextureWrap_W                          D3DWRAP_W

	//#define ColorWriteChannels                     D3DRS_COLORWRITEENABLE
	//#define ColorWriteChannels1                    D3DRS_COLORWRITEENABLE1
	//#define ColorWriteChannels2                    D3DRS_COLORWRITEENABLE2
	//#define ColorWriteChannels3                    D3DRS_COLORWRITEENABLE3
	//#define ColorWrite_None                        0
	//#define ColorWrite_Red                         D3DCOLORWRITEENABLE_RED
	//#define ColorWrite_Green                       D3DCOLORWRITEENABLE_GREEN
	//#define ColorWrite_Blue                        D3DCOLORWRITEENABLE_BLUE
	//#define ColorWrite_Alpha                       D3DCOLORWRITEENABLE_ALPHA
	//#define ColorWrite_All                        (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA)

#pragma endregion
};