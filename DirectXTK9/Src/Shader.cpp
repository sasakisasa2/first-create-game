#include "Shader.h"

#ifdef NDEBUG
	#pragma comment(lib, "d3dx9.lib")
#else
	#pragma comment(lib, "d3dx9d.lib")
#endif

DX9::Shader::Shader(IDirect3DDevice9* device, ID3DXEffect* effect) :
	m_device(device), m_effect(effect), m_material{}
{
}

UINT DX9::Shader::Begin() const
{
	UINT pass;
	if (m_effect->Begin(&pass, 0) != D3D_OK)
		return 0;

	return pass;
}

std::unique_ptr<DX9::Shader> DX9::Shader::CreateFromFile(
	IDirect3DDevice9* device, LPCWSTR file
)
{
	Microsoft::WRL::ComPtr<ID3DXEffect> effect;
	if (D3DXCreateEffectFromFile(
			device, file,
			nullptr, nullptr,
			D3DXFX_NOT_CLONEABLE, nullptr,
			effect.GetAddressOf(), nullptr
		) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Shader> shader = std::make_unique<DX9::Shader>(device, effect.Get());
	return shader;
}

void DX9::Shader::RetisterMaterialByName(
	LPCSTR diffuse,  LPCSTR ambient, LPCSTR specular,
	LPCSTR emissive, LPCSTR power
)
{
	if (diffuse)
		m_material.Diffuse  = m_effect->GetParameterByName(nullptr, diffuse);
	if (ambient)
		m_material.Ambient  = m_effect->GetParameterByName(nullptr, ambient);
	if (specular)
		m_material.Specular = m_effect->GetParameterByName(nullptr, specular);
	if (emissive)
		m_material.Emissive = m_effect->GetParameterByName(nullptr, emissive);
	if (power)
		m_material.Power    = m_effect->GetParameterByName(nullptr, power);
}

void DX9::Shader::SetMaterial(D3DMATERIAL9& material) const
{
	if (m_material.Diffuse)
		m_effect->SetVector(m_material.Diffuse,  (D3DXVECTOR4*)&material.Diffuse);
	if (m_material.Ambient)
		m_effect->SetVector(m_material.Ambient,  (D3DXVECTOR4*)&material.Ambient);
	if (m_material.Specular)
		m_effect->SetVector(m_material.Specular, (D3DXVECTOR4*)&material.Specular);
	if (m_material.Emissive)
		m_effect->SetVector(m_material.Emissive, (D3DXVECTOR4*)&material.Emissive);
	if (m_material.Power)
		m_effect->SetFloat (m_material.Power, material.Power);
}
