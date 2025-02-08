#include "Model.h"
#include <exception>
#include <map>
#include <string>
#include <algorithm>

DX9::Model::Model(
	IDirect3DDevice9* device,
	ID3DXMesh*        mesh,
	const DWORD       meshes
) : m_device(device), m_mesh(mesh), 
	m_meshes(meshes), m_textures(meshes, nullptr),
	m_position(),
	m_scale   (1.0f, 1.0f, 1.0f),
	m_rotation(DirectX::XMMatrixIdentity())
{
	m_position.v = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	D3DCOLORVALUE initial{ 0.0f, 0.0f, 0.0f, 1.0f };
	D3DMATERIAL9  material;
	material.Diffuse  = initial;
	material.Ambient  = initial;
	material.Specular = initial;
	material.Emissive = initial;
	material.Power    = 0.0f;
	m_materials.resize(meshes, material);
}

void DX9::Model::Draw()
{
	const DirectX::XMMATRIX world = GetWorldTransform();
	m_device->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&world);

	for (DWORD index = 0; index < m_meshes; ++index) {
		m_device->SetMaterial(&m_materials.at(index));
		m_device->SetTexture(0, m_textures.at(index).Get());
		m_mesh  ->DrawSubset(index);
	}
}

void DX9::Model::SetMaterial(const D3DMATERIAL9& material)
{
	for (auto& materials : m_materials)
		materials = material;
}

void DX9::Model::SetTexture(LPCWSTR texture)
{
	Microsoft::WRL::ComPtr<IDirect3DTexture9> tex;
	D3DXCreateTextureFromFile(m_device.Get(), texture, tex.GetAddressOf());
	for (auto& textures : m_textures)
		textures = tex;
}

void DX9::Model::SetTexture(IDirect3DTexture9* texture)
{
	for (auto& textures : m_textures)
		textures = texture;
}

DirectX::XMFLOAT3 DX9::Model::GetRotation() const
{
	DirectX::XMFLOAT3X3 axis;
	DirectX::XMStoreFloat3x3(&axis, m_rotation);

	DirectX::XMFLOAT3 rotation;
	rotation.x = -asinf (axis._32);
	rotation.y =  atan2f(axis._12, axis._22);
	rotation.z =  atan2f(axis._31, axis._33);

	return rotation;
}

void DX9::Model::Move(const float x, const float y, const float z)
{
	const auto right  = DirectX::XMVectorScale(m_rotation.r[0], x);
	const auto up     = DirectX::XMVectorScale(m_rotation.r[1], y);
	const auto foward = DirectX::XMVectorScale(m_rotation.r[2], z);

	m_position.v = DirectX::XMVectorAdd(m_position.v, right);
	m_position.v = DirectX::XMVectorAdd(m_position.v, up);
	m_position.v = DirectX::XMVectorAdd(m_position.v, foward);
}

void DX9::Model::Rotate(const float x, const float y, const float z)
{
	if (z != 0.0f) {
		const auto rotation = DirectX::XMMatrixRotationAxis(m_rotation.r[2], z);
		m_rotation = DirectX::XMMatrixMultiply(m_rotation, rotation);
	}

	if (x != 0.0f) {
		const auto rotation = DirectX::XMMatrixRotationAxis(m_rotation.r[0], x);
		m_rotation = DirectX::XMMatrixMultiply(m_rotation, rotation);
	}

	if (y != 0.0f) {
		const auto rotation = DirectX::XMMatrixRotationAxis(m_rotation.r[1], y);
		m_rotation = DirectX::XMMatrixMultiply(m_rotation, rotation);
	}
}

bool DX9::Model::IntersectRay(
	const DirectX::XMFLOAT3& raypos,
	const DirectX::XMFLOAT3& raydir,
	float*             distance,
	DirectX::XMFLOAT3* normal
)
{
	DirectX::XMMATRIX world = GetWorldTransform();
	world = DirectX::XMMatrixInverse(nullptr, world);

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(raypos.x, raypos.y, raypos.z, 1.0f);
	pos = DirectX::XMVector3TransformCoord(pos, world);

	DirectX::XMVECTOR dir = DirectX::XMVectorSet(raydir.x, raydir.y, raydir.z, 1.0f);
	dir = DirectX::XMVector3TransformNormal(dir, world);

	BOOL hit = FALSE;
	if (!normal) {
		D3DXIntersect(
			m_mesh.Get(), (D3DXVECTOR3*)&pos, (D3DXVECTOR3*)&dir, &hit,
			nullptr, nullptr, nullptr, distance, nullptr, nullptr
		);
	} else {
		DWORD face;
		D3DXIntersect(
			m_mesh.Get(), (D3DXVECTOR3*)&pos, (D3DXVECTOR3*)&dir, &hit,
			&face, nullptr, nullptr, distance, nullptr, nullptr
		);
		if (hit != FALSE) {
			Microsoft::WRL::ComPtr<IDirect3DIndexBuffer9> index_buffer;
			m_mesh->GetIndexBuffer(index_buffer.GetAddressOf());
			
			D3DINDEXBUFFER_DESC index_desc;
			index_buffer->GetDesc(&index_desc);

			index_buffer.Reset();

			DWORD index[3];
			auto set_index = [&](auto& pointer)
			{
				// 衝突面インデックス取得
				index[0] = pointer[face * 3 + 0];
				index[1] = pointer[face * 3 + 1];
				index[2] = pointer[face * 3 + 2];
			};

			if (index_desc.Format & D3DFMT_INDEX16) {
				WORD* indices;
				if (m_mesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&indices) != D3D_OK) {
					*normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					return hit;
				}
				set_index(indices);
				m_mesh->UnlockIndexBuffer();
			} else {
				DWORD* indices;
				if (m_mesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&indices) != D3D_OK) {
					*normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					return hit;
				}
				set_index(indices);
				m_mesh->UnlockIndexBuffer();
			}

			const DWORD STRIDE = m_mesh->GetNumBytesPerVertex();

			BYTE* vertices;
			if (m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&vertices) != D3D_OK) {
				*normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
				return hit;
			}

			// 衝突面頂点座標取得
			DirectX::XMVECTOR face[3]{};
			CopyMemory(&face[0], vertices + index[0] * STRIDE, sizeof(DirectX::XMFLOAT3));
			CopyMemory(&face[1], vertices + index[1] * STRIDE, sizeof(DirectX::XMFLOAT3));
			CopyMemory(&face[2], vertices + index[2] * STRIDE, sizeof(DirectX::XMFLOAT3));

			m_mesh->UnlockVertexBuffer();

			// 法線設定
			const DirectX::XMVECTOR face10 = DirectX::XMVectorSubtract(face[1], face[0]);
			const DirectX::XMVECTOR face20 = DirectX::XMVectorSubtract(face[2], face[0]);
			auto cross = DirectX::XMVector3Cross(face10, face20);
			cross = DirectX::XMVector3Normalize(cross);
			DirectX::XMStoreFloat3(normal, cross);
		}
	}

	return (bool)hit;
}

std::unique_ptr<DX9::Model> DX9::Model::Clone()
{
	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(
		m_device.Get(), m_mesh.Get(), m_meshes
	);

	for (DWORD i = 0; i < m_meshes; ++i) {
		model->m_materials[i] = m_materials[i];
		model->m_textures [i] = m_textures [i];
	}

	model->m_position = m_position;
	model->m_scale    = m_scale;
	model->m_rotation = m_rotation;

	return model;
}

std::unique_ptr<DX9::Model> DX9::Model::CreateFromFile(
	IDirect3DDevice9* device, LPCWSTR file
)
{
	Microsoft::WRL::ComPtr<ID3DXMesh>   mesh;
	Microsoft::WRL::ComPtr<ID3DXBuffer> material;
	DWORD subset;
	if (D3DXLoadMeshFromX(
			file, 0, device,
			nullptr, material.GetAddressOf(), nullptr,
			&subset, mesh    .GetAddressOf()
		) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(device, mesh.Get(), subset);

	wchar_t fullpath[MAX_PATH];
	const auto max_length = _wfullpath(fullpath, file, MAX_PATH);

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir  [_MAX_DIR];
	_wsplitpath_s(fullpath, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

	D3DXMATERIAL* materials = (D3DXMATERIAL*)material->GetBufferPointer();
	std::map<std::string, IDirect3DTexture9*, std::less<>> texture_map;
	for (DWORD i = 0; i < subset; ++i) {
		model->SetMaterial(materials[i].MatD3D, i);

		if (!materials[i].pTextureFilename)
			continue;

		decltype(texture_map)::iterator it = texture_map.find(materials[i].pTextureFilename);
		if (it == texture_map.end()) {
			wchar_t wfile[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, materials[i].pTextureFilename, -1, wfile, MAX_PATH);
			wsprintf(fullpath, L"%s%s%s", drive, dir, wfile);

			Microsoft::WRL::ComPtr<IDirect3DTexture9> texture;
			D3DXCreateTextureFromFile(
				device, fullpath, texture.GetAddressOf()
			);
			model->SetTexture(texture.Get(), i);

			texture_map.insert(std::make_pair(materials[i].pTextureFilename, texture.Get()));
		} else {
			model->SetTexture(it->second, i);
		}
	}

	return model;
}

DirectX::BoundingSphere DX9::Model::GetBoundingSphere()
{
	DirectX::BoundingSphere sphere;

	void* pVertices;
	if (m_mesh->LockVertexBuffer(0, &pVertices) != D3D_OK) {
		return sphere;
	}

	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		m_mesh->GetNumVertices(), m_mesh->GetNumBytesPerVertex(),
		(D3DXVECTOR3*)&sphere.Center, &sphere.Radius
	);

	m_mesh->UnlockVertexBuffer();

	sphere.Center.x += m_position.f[0];
	sphere.Center.y += m_position.f[1];
	sphere.Center.z += m_position.f[2];

	float scale = (std::max)({ m_scale.x, m_scale.y, m_scale.z });
	sphere.Radius *= scale;

	return sphere;
}

DirectX::BoundingBox DX9::Model::GetBoundingBox()
{
	DirectX::BoundingBox box;

	void* pVertices;
	if (m_mesh->LockVertexBuffer(0, &pVertices) != D3D_OK) {
		return box;
	}

	D3DXVECTOR3 min, max;
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,
		m_mesh->GetNumVertices(), m_mesh->GetNumBytesPerVertex(),
		&min, &max
	);

	m_mesh->UnlockVertexBuffer();

	const DirectX::XMVECTOR pt1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&min);
	const DirectX::XMVECTOR pt2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&max);
	DirectX::BoundingBox::CreateFromPoints(box, pt1, pt2);

	const DirectX::XMMATRIX world(GetWorldTransform());
	box.Transform(box, world);

	return box;
}

DirectX::BoundingOrientedBox DX9::Model::GetBoundingOrientedBox()
{
	DirectX::BoundingOrientedBox obb;
	DirectX::BoundingBox         box;

	void* vertices;
	if (m_mesh->LockVertexBuffer(0, &vertices) != D3D_OK)
		return obb;

	D3DXVECTOR3 min, max;
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)vertices,
		m_mesh->GetNumVertices(), m_mesh->GetNumBytesPerVertex(),
		&min, &max
	);

	m_mesh->UnlockVertexBuffer();

	const DirectX::XMVECTOR pt1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&min);
	const DirectX::XMVECTOR pt2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&max);
	DirectX::BoundingBox::CreateFromPoints(box, pt1, pt2);
	DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, box);

	const DirectX::XMMATRIX world(GetWorldTransform());
	obb.Transform(obb, world);

	return obb;
}

std::unique_ptr<DX9::Model> DX9::Model::CreateBox(
	IDirect3DDevice9* device,
	const float width, const float height, const float depth
)
{
	Microsoft::WRL::ComPtr<ID3DXMesh> mesh;
	if (D3DXCreateBox(
			device,
			width, height, depth,
			mesh.GetAddressOf(), nullptr
		) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(device, mesh.Get(), 1);
	return model;
}

std::unique_ptr<DX9::Model> DX9::Model::CreateCylinder(
	IDirect3DDevice9* device,
	const float radius1, const float radius2,
	const float length,
	const UINT  slices, const UINT stacks
)
{
	Microsoft::WRL::ComPtr<ID3DXMesh> mesh;
	if (D3DXCreateCylinder(
			device,
			radius1, radius2, length, slices, stacks,
			mesh.GetAddressOf(), nullptr
		) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(device, mesh.Get(), 1);
	return model;
}

std::unique_ptr<DX9::Model> DX9::Model::CreatePolygon(
	IDirect3DDevice9* device,
	const float length, const UINT slices
)
{
	Microsoft::WRL::ComPtr<ID3DXMesh> mesh;
	if (D3DXCreatePolygon(
			device,
			length, slices,
			mesh.GetAddressOf(), nullptr
		) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(device, mesh.Get(), 1);
	return model;
}

std::unique_ptr<DX9::Model> DX9::Model::CreateSphere(
	IDirect3DDevice9* device,
	const float radius, const UINT slices, const UINT stacks
)
{
	Microsoft::WRL::ComPtr<ID3DXMesh> mesh;
	if (D3DXCreateSphere(
		device,
		radius, slices, stacks,
		mesh.GetAddressOf(), nullptr
	) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(device, mesh.Get(), 1);
	return model;
}

std::unique_ptr<DX9::Model> DX9::Model::CreateTorus(
	IDirect3DDevice9* device,
	const float innnerRadius, const float outerRadius,
	const UINT  slides, const UINT rings
)
{
	Microsoft::WRL::ComPtr<ID3DXMesh> mesh;
	if (D3DXCreateTorus(
		device,
		innnerRadius, outerRadius,
		slides, rings,
		mesh.GetAddressOf(), nullptr
	) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(device, mesh.Get(), 1);
	return model;
}

std::unique_ptr<DX9::Model> DX9::Model::CreateTeapot(IDirect3DDevice9* device)
{
	Microsoft::WRL::ComPtr<ID3DXMesh> mesh;
	if (D3DXCreateTeapot(device, mesh.GetAddressOf(), nullptr) != D3D_OK)
		throw std::exception();

	std::unique_ptr<DX9::Model> model = std::make_unique<DX9::Model>(device, mesh.Get(), 1);
	return model;
}