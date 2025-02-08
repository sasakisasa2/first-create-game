#pragma once

#include <d3dx9.h>
#include <wrl/client.h>
#include <string>
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include <DirectXCollision.h>

namespace DX9 {
	class SkinnedModel {
	public:
		struct DX9FRAME;
		struct DX9MESHCONTAINER;

	public:
		SkinnedModel() = default;
		SkinnedModel(IDirect3DDevice9* device);

		virtual ~SkinnedModel();

		void Draw();

		inline void AdvanceTime(const double elapsedTime) const
		{ m_animationController->AdvanceTime(elapsedTime, nullptr); }

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

		void Move(const float x, const float y, const float z);
		inline void Move(const DirectX::XMFLOAT3& move)
		{ Move(move.x, move.y, move.z); }

		void Rotate(const float x, const float y, const float z);
		inline void Rotate(const DirectX::XMFLOAT3& rotation)
		{ Rotate(rotation.x, rotation.y, rotation.z); }

		__declspec(property(get = GetPosition))			DirectX::XMFLOAT3 Position;
		__declspec(property(get = GetRotation))			DirectX::XMFLOAT3 Rotation;

		__declspec(property(get = GetRightVector))		DirectX::XMFLOAT3 RightVector;
		__declspec(property(get = GetUpVector))			DirectX::XMFLOAT3 UpVector;
		__declspec(property(get = GetForwardVector))	DirectX::XMFLOAT3 ForwardVector;

		inline void SetMaterial(const D3DMATERIAL9& material)
		{ SetMaterial(m_frameRoot, material); }

		inline double GetTrackPosition(const UINT track) const
		{
			D3DXTRACK_DESC   desc;
			m_animationController->GetTrackDesc(track, &desc);
			return desc.Position;
		}

		inline void SetTrackEnable(const UINT track, const BOOL enable) const
		{ m_animationController->SetTrackEnable(track, enable); }

		inline void SetTrackWeight(const UINT track, const float weight) const
		{ m_animationController->SetTrackWeight(track, weight); }

		void SetTrackLoopTime(const UINT track, const float loopTime) const;

		void SetTrackPosition(const UINT track, const double position) const
		{ m_animationController->SetTrackPosition(track, position); }

		//void SetTrackLoopMode(const UINT track, const D3DXPLAYBACK_TYPE loopType) const;

		void ChangeAnimation(
			const UINT track1, const UINT track2,
			const double duration, const D3DXTRANSITION_TYPE transition
		);
		void ChangeAnimation(
			const UINT track, const double duration,
			const D3DXTRANSITION_TYPE transition
		);

		DirectX::BoundingSphere      GetBoundingSphere();
		DirectX::BoundingBox         GetBoundingBox();
		DirectX::BoundingOrientedBox GetBoundingOrientedBox();

		static std::unique_ptr<SkinnedModel> CreateFromFile(
			IDirect3DDevice9* device, LPCWSTR file
		);

	private:
		static constexpr UINT MAX_BONES = 26;

		Microsoft::WRL::ComPtr<IDirect3DDevice9> m_device;

		D3DXFRAME*                               m_frameRoot;

		DWORD                                    m_maxVertexBlendMatrices;
		DWORD                                    m_maxVertexBlendMatrixIndex;

		Microsoft::WRL::ComPtr<ID3DXAnimationController> m_animationController;

		D3DXMATRIX*                              m_boneOffsetMatrices;
		DWORD                                    m_numBoneOffsetMatrices;

		DirectX::XMVECTORF32                     m_position;
		DirectX::XMFLOAT3                        m_scale;
		DirectX::XMMATRIX                        m_rotation;

		inline D3DXMATRIX GetWorldMatrix() const
		{
			D3DXMATRIX world;
			D3DXMatrixScaling(&world, m_scale.x, m_scale.y, m_scale.z);
			world *= *(D3DXMATRIX*)&m_rotation;
			CopyMemory(&world.m[3], &m_position.v, sizeof(float) * 4);
			return world;
		}

		HRESULT GenerateSkinnedMesh(DX9MESHCONTAINER* meshContainer);

		bool SetupBoneMatrixPointers(D3DXFRAME* frame);
		bool SetupBoneMatrixPointersOnMesh(D3DXMESHCONTAINER* meshContainer);
		void SetBoneOffsetMatrixPointer(D3DXFRAME* frame);

		void UpdateFrameMatrices(D3DXFRAME* frameRoot, D3DXMATRIX* parentMatrix);
		void DrawFrame(D3DXFRAME* frame);
		void DrawMeshContainer(D3DXMESHCONTAINER* meshContainerBase, D3DXFRAME* frameBase);
		void DrawMeshContainerIndexed(DX9MESHCONTAINER* meshContainer, DX9FRAME* frame);

		void SetMaterial(D3DXFRAME* frame, const D3DMATERIAL9& material);
		void ComputeBoundingBoxPoint(D3DXFRAME* frame, D3DXVECTOR3& ptmin, D3DXVECTOR3& ptmax);

	public:
		struct DX9FRAME : public D3DXFRAME {
		public:
			void SetName(LPCSTR name)
			{
				if (name)	frameName = name;
				else		frameName = "";
				Name = (LPSTR)frameName.c_str();
			}

		public:
			D3DXMATRIX  CombinedTransformationMatrix;
		private:
			std::string frameName;
		};

		struct DX9MESHCONTAINER : public D3DXMESHCONTAINER {
		public:
			void SetName(LPCSTR name)
			{
				if (name)	ContainerName = name;
				else		ContainerName = "";
				Name = (LPSTR)ContainerName.c_str();
			}

		public:
			Microsoft::WRL::ComPtr<ID3DXMesh>   mesh;

			std::vector<D3DXATTRIBUTERANGE>     attributeTable;
			DWORD                               numAttributeGroups;
			DWORD                               numInfl;

			Microsoft::WRL::ComPtr<ID3DXBuffer> boneCombinationBuf;
			std::vector<D3DXMATRIX*>            boneMatrixPtrs;
			std::vector<D3DXMATRIX>             boneOffsetMatrices;

			DWORD                               numPaletteEntries;
			bool                                useSoftwareVP;
			DWORD                               attributeSW;

			std::vector<Microsoft::WRL::ComPtr<IDirect3DTexture9>> textures;

		private:
			std::string                         ContainerName;
		};

		class CDX9AllocateHierarchy : public ID3DXAllocateHierarchy {
		public:
			CDX9AllocateHierarchy(SkinnedModel* pAnimeModel)
				: m_skinnedModel(pAnimeModel) {}

			STDMETHOD(CreateFrame)(THIS_ LPCSTR name, LPD3DXFRAME* ppNewFrame);

			STDMETHOD(CreateMeshContainer)(THIS_
				LPCSTR                    inName,
				CONST D3DXMESHDATA*       pMeshData,
				CONST D3DXMATERIAL*       pMaterials,
				CONST D3DXEFFECTINSTANCE* pEffectInstances,
				DWORD                     inNumMaterials,
				CONST DWORD*              pAdjacency,
				LPD3DXSKININFO            pSkinInfo,
				LPD3DXMESHCONTAINER*      ppNewMeshContainer);

			STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrame);
			STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

		public:
			SkinnedModel* m_skinnedModel;

		private:
			template <class    T> inline void SafeRelease(T& x) { if (x) { x->Release(); x = nullptr; } }
			template <typename T> inline void DeleteArray(T* x) { delete[] x; x = nullptr; }
		};
	};

	typedef std::unique_ptr<SkinnedModel> SKINNEDMODEL;
}