#pragma once

#include <d3dx9.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>

namespace DX9 {
	class Model {
	public:
		Model() = default;
		Model(
			IDirect3DDevice9* device,
			ID3DXMesh*        mesh,
			const DWORD       meshes
		);
		virtual ~Model() {}

		void Draw();

		inline void SetMaterial(const D3DMATERIAL9& material, const DWORD index)
		{ m_materials[index] = material; }
		void SetMaterial(const D3DMATERIAL9& material);

		inline void SetTexture(LPCWSTR texture, const DWORD index)
		{
			D3DXCreateTextureFromFile(
				m_device.Get(), texture,
				m_textures[index].ReleaseAndGetAddressOf()
			);
		}
		void SetTexture(LPCWSTR texture);

		inline void SetTexture(IDirect3DTexture9* texture, const DWORD index)
		{ m_textures.at(index) = texture; }
		void SetTexture(IDirect3DTexture9* texture);

		inline void SetPosition(const float x, const float y, const float z)
		{ m_position.f[0] = x; m_position.f[1] = y; m_position.f[2] = z; }
		inline void SetPosition(const DirectX::XMFLOAT3& position)
		{ CopyMemory(m_position.f, &position, sizeof(float) * 3); }

		inline void SetScale(const float x, const float y, const float z)
		{ m_scale.x = x; m_scale.y = y; m_scale.z = z; }
		inline void SetScale(const DirectX::XMFLOAT3& scale)
		{ m_scale = scale; }
		inline void SetScale(const float scale)
		{ m_scale.x = scale; m_scale.y = scale; m_scale.z = scale; }

		inline void SetRotation(const float x, const float y, const float z)
		{ m_rotation = DirectX::XMMatrixRotationRollPitchYaw(x, y, z); }
		inline void SetRotation(const DirectX::XMFLOAT3& rotation)
		{ SetRotation(rotation.x, rotation.y, rotation.z); }

		inline void SetRotationMatrix(const DirectX::XMMATRIX rotation)
		{ m_rotation = rotation; }
		inline void SetRotationQuaternion(const DirectX::XMFLOAT4& quaternion)
		{
			DirectX::XMVECTOR v = DirectX::XMLoadFloat4(&quaternion);
			m_rotation = DirectX::XMMatrixRotationQuaternion(v);
		}

		inline DirectX::XMFLOAT3 GetPosition() const
		{ return DirectX::XMFLOAT3(m_position.f[0], m_position.f[1], m_position.f[2]); }
		inline DirectX::XMFLOAT3 GetScale() const { return m_scale; }

		DirectX::XMFLOAT3 GetRotation() const;
		inline DirectX::XMMATRIX GetRotationMatrix() const { return m_rotation; }
		inline DirectX::XMFLOAT4 GetRotationQuaternion() const
		{
			DirectX::XMVECTOR v = DirectX::XMQuaternionRotationMatrix(m_rotation);
			DirectX::XMFLOAT4 quaternion;
			DirectX::XMStoreFloat4(&quaternion, v);
			return quaternion;
		}

		inline DirectX::XMMATRIX GetWorldTransform() const
		{
			DirectX::XMMATRIX world(
				DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)
			);
			world     *= m_rotation;
			world.r[3] = m_position.v;
			return world;
		}

		inline DirectX::XMFLOAT3 GetRightVector() const
		{
			DirectX::XMFLOAT3 right;
			DirectX::XMStoreFloat3(&right, m_rotation.r[0]);
			return right;
		}

		inline DirectX::XMFLOAT3 GetUpVector() const
		{
			DirectX::XMFLOAT3 up;
			DirectX::XMStoreFloat3(&up, m_rotation.r[1]);
			return up;
		}

		inline DirectX::XMFLOAT3 GetForwardVector() const
		{
			DirectX::XMFLOAT3 foward;
			DirectX::XMStoreFloat3(&foward, m_rotation.r[2]);
			return foward;
		}

		inline D3DMATERIAL9 GetMaterial(const DWORD index = 0) const
		{ return m_materials[index]; }

		void Move(const float x, const float y, const float z);
		inline void Move(const DirectX::XMFLOAT3& move)
		{ Move(move.x, move.y, move.z); }

		void Rotate(const float x, const float y, const float z);
		inline void Rotate(const DirectX::XMFLOAT3& rotation)
		{ Rotate(rotation.x, rotation.y, rotation.z); }

		bool IntersectRay(
			const DirectX::XMFLOAT3& raypos,
			const DirectX::XMFLOAT3& raydir,
			float*			 distance = nullptr,
			DirectX::XMFLOAT3* normal = nullptr
		);

		inline bool RayCast(
			const DirectX::XMFLOAT3& origin,
			const DirectX::XMFLOAT3& direction,
			float*			 distance = nullptr,
			DirectX::XMFLOAT3* normal = nullptr
		)
		{ return IntersectRay(origin, direction, distance, normal); }

		DirectX::BoundingSphere      GetBoundingSphere();
		DirectX::BoundingBox         GetBoundingBox();
		DirectX::BoundingOrientedBox GetBoundingOrientedBox();

		std::unique_ptr<Model> Clone();

		__declspec(property(get = GetPosition))			DirectX::XMFLOAT3 Position;
		__declspec(property(get = GetRotation))			DirectX::XMFLOAT3 Rotation;

		__declspec(property(get = GetRightVector))		DirectX::XMFLOAT3 RightVector;
		__declspec(property(get = GetUpVector))			DirectX::XMFLOAT3 UpVector;
		__declspec(property(get = GetForwardVector))	DirectX::XMFLOAT3 ForwardVector;

		static std::unique_ptr<Model> CreateFromFile(
			IDirect3DDevice9* device, LPCWSTR file
		);

		static std::unique_ptr<Model> CreateBox(
			IDirect3DDevice9* device,
			const float width, const float height, const float depth
		);

		static std::unique_ptr<Model> CreateCylinder(
			IDirect3DDevice9* device,
			const float radius1, const float radius2,
			const float length,
			const UINT  slices,  const UINT stacks
		);

		static std::unique_ptr<Model> CreatePolygon(
			IDirect3DDevice9* device,
			const float length, const UINT slices
		);

		static std::unique_ptr<Model> CreateSphere(
			IDirect3DDevice9* device,
			const float radius, const UINT slices, const UINT stacks
		);

		static std::unique_ptr<Model> CreateTorus(
			IDirect3DDevice9* device,
			const float innnerRadius, const float outerRadius,
			const UINT  slides, const UINT rings
		);

		static std::unique_ptr<Model> CreateTeapot(IDirect3DDevice9* device);

	private:
		Microsoft::WRL::ComPtr<IDirect3DDevice9> m_device;
		Microsoft::WRL::ComPtr<ID3DXMesh>        m_mesh;

		DWORD                                    m_meshes;

		std::vector<D3DMATERIAL9>                m_materials;
		std::vector<Microsoft::WRL::ComPtr<IDirect3DTexture9>> m_textures;

		DirectX::XMVECTORF32                     m_position;
		DirectX::XMFLOAT3                        m_scale;
		DirectX::XMMATRIX                        m_rotation;
	};

	typedef std::unique_ptr<Model> MODEL;
}