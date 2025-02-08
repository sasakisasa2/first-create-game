#pragma once

#include <d3dx9.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include <memory>
#include <exception>

namespace DX9
{
	class Shader {
	public:
		Shader() = default;
		Shader(
			IDirect3DDevice9* device,
			ID3DXEffect*      effect
		);
		virtual ~Shader() {}

		UINT Begin() const;
		inline void End() const { m_effect->End(); }

		inline HRESULT BeginPass(const UINT pass) const
		{ return m_effect->BeginPass(pass); }
		inline void EndPass() const
		{ m_effect->EndPass(); }

		inline D3DXHANDLE GetTechniqueByName(LPCSTR technique) const
		{ return m_effect->GetTechniqueByName(technique); }
		inline D3DXHANDLE GetParameterHandleByName(LPCSTR parameter) const
		{ return m_effect->GetParameterByName(nullptr, parameter); }

		inline void SetTechnique(const D3DXHANDLE parameter) const
		{ m_effect->SetTechnique(parameter); }
		inline void SetData(const D3DXHANDLE parameter, LPCVOID data, const UINT bytes) const
		{ m_effect->SetValue(parameter, data, bytes); }

		inline void SetIntArray(const D3DXHANDLE parameter, const int array[], UINT count) const
		{ m_effect->SetIntArray(parameter, array, count); }
		inline void SetFloatArray(const D3DXHANDLE parameter, const float array[], UINT count) const
		{ m_effect->SetFloatArray(parameter, array, count); }
		inline void SetVectorArray(const D3DXHANDLE parameter, const DirectX::XMFLOAT4 array[], UINT count) const
		{ m_effect->SetVectorArray(parameter, (D3DXVECTOR4*)array, count); }
		inline void SetMatrixArray(const D3DXHANDLE parameter, const DirectX::XMMATRIX array[], UINT count) const
		{ m_effect->SetMatrixArray(parameter, (D3DXMATRIX*)array, count); }

		template<class T> inline void SetParameter(const D3DXHANDLE parameter, const T& data) const
		{ SetData(parameter, &data, sizeof(data)); }

		void RetisterMaterialByName(
			LPCSTR diffuse,  LPCSTR ambient, LPCSTR specular,
			LPCSTR emissive, LPCSTR power
		);
		void SetMaterial(D3DMATERIAL9& material) const;

		inline void SetTexture(const D3DXHANDLE parameter, IDirect3DBaseTexture9* texture) const
		{ m_effect->SetTexture(parameter, texture); }
		inline void SetTexture(const DWORD stage, IDirect3DBaseTexture9* texture) const
		{ m_device->SetTexture(stage, texture); }

		inline void CommitChanges() const { m_effect->CommitChanges(); }

		static std::unique_ptr<Shader> CreateFromFile(
			IDirect3DDevice9* device, LPCWSTR file
		);

	private:
		Microsoft::WRL::ComPtr<IDirect3DDevice9> m_device;
		Microsoft::WRL::ComPtr<ID3DXEffect>      m_effect;

		struct SHADERMATERIAL {
			D3DXHANDLE   Diffuse;
			D3DXHANDLE   Ambient;
			D3DXHANDLE   Specular;
			D3DXHANDLE   Emissive;
			D3DXHANDLE   Power;

			SHADERMATERIAL() :
				Diffuse(nullptr), Ambient(nullptr), Specular(nullptr),
				Emissive(nullptr), Power(nullptr)
			{
			}
		};
		SHADERMATERIAL m_material;
	};

	typedef std::unique_ptr<Shader> SHADER;
};