#include "SkinnedModel.h"
#include <exception>

#ifdef NDEBUG
	#pragma comment(lib, "d3dx9.lib")
#else
	#pragma comment(lib, "d3dx9d.lib")
#endif

DX9::SkinnedModel::SkinnedModel(IDirect3DDevice9* device) :
	m_device(device),
	m_frameRoot(nullptr),
	m_maxVertexBlendMatrices(0), m_maxVertexBlendMatrixIndex(0),
	m_boneOffsetMatrices(nullptr), m_numBoneOffsetMatrices(0),
	m_position(),
	m_scale(1.0f, 1.0f, 1.0f),
	m_rotation(DirectX::XMMatrixIdentity())
{
	m_position.v = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	D3DCAPS9 caps{};
	m_device->GetDeviceCaps(&caps);
	m_maxVertexBlendMatrices    = caps.MaxVertexBlendMatrices;
	m_maxVertexBlendMatrixIndex = caps.MaxVertexBlendMatrixIndex;
}

DX9::SkinnedModel::~SkinnedModel()
{
	if (m_frameRoot) {
		CDX9AllocateHierarchy allocater(this);
		D3DXFrameDestroy(m_frameRoot, &allocater);
	}
}

void DX9::SkinnedModel::Draw()
{
//	auto world_transform = GetWorldTransform();
//	D3DXMATRIX world((float*)&world_transform);
//	UpdateFrameMatrices(m_frameRoot, &world);
	D3DXMATRIX world(GetWorldMatrix());
	UpdateFrameMatrices(m_frameRoot, &world);
	DrawFrame(m_frameRoot);
}

void DX9::SkinnedModel::UpdateFrameMatrices(D3DXFRAME* frameRoot, D3DXMATRIX* parentMatrix)
{
	DX9FRAME* frame = (DX9FRAME*)frameRoot;
	if (parentMatrix) {
		D3DXMatrixMultiply(
			&frame->CombinedTransformationMatrix,
			&frame->TransformationMatrix,
			parentMatrix
		);
	} else {
		frame->CombinedTransformationMatrix = frame->TransformationMatrix;
	}

	if (frame->pFrameSibling)
		UpdateFrameMatrices(frame->pFrameSibling, parentMatrix);

	if (frame->pFrameFirstChild)
		UpdateFrameMatrices(frame->pFrameFirstChild, &frame->CombinedTransformationMatrix);
}

void DX9::SkinnedModel::DrawFrame(D3DXFRAME* frame)
{
	D3DXMESHCONTAINER* mesh_container = frame->pMeshContainer;

	while (mesh_container) {
		DrawMeshContainer(mesh_container, frame);
		mesh_container = mesh_container->pNextMeshContainer;
	}

	if (frame->pFrameSibling)
		DrawFrame(frame->pFrameSibling);

	if (frame->pFrameFirstChild)
		DrawFrame(frame->pFrameFirstChild);
}

void DX9::SkinnedModel::DrawMeshContainer(D3DXMESHCONTAINER* meshContainerBase, D3DXFRAME* frameBase)
{
	DX9MESHCONTAINER* meshContainer = (DX9MESHCONTAINER*)meshContainerBase;
	DX9FRAME*         frame         = (DX9FRAME*)frameBase;

	if (meshContainer->pSkinInfo) {
		DrawMeshContainerIndexed(meshContainer, frame);
	} else {
		ID3DXMesh* mesh = meshContainer->MeshData.pMesh;
		m_device->SetTransform(D3DTS_WORLD, &frame->CombinedTransformationMatrix);
		for (DWORD i = 0; i < meshContainer->NumMaterials; i++) {
			m_device->SetMaterial(&meshContainer->pMaterials[i].MatD3D);
			m_device->SetTexture (0, meshContainer->textures.at(i).Get());
			mesh->DrawSubset(i);
		}
	}
}

void DX9::SkinnedModel::DrawMeshContainerIndexed(DX9MESHCONTAINER* meshContainer, DX9FRAME* frame)
{
	if (meshContainer->useSoftwareVP)
		m_device->SetSoftwareVertexProcessing(TRUE);

	if (meshContainer->numInfl == 1)
		m_device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
	else
		m_device->SetRenderState(D3DRS_VERTEXBLEND, meshContainer->numInfl - 1);

	if (meshContainer->numInfl > 0)
		m_device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);

	D3DXMATRIXA16        mat;
	D3DXBONECOMBINATION* bone_comb = (D3DXBONECOMBINATION*)meshContainer->boneCombinationBuf->GetBufferPointer();
	ID3DXMesh*           mesh      = meshContainer->MeshData.pMesh;
	for (UINT iAttrib = 0; iAttrib < meshContainer->numAttributeGroups; iAttrib++) {
		for (UINT iPaletteEntry = 0; iPaletteEntry < meshContainer->numPaletteEntries; iPaletteEntry++) {
			const UINT matrix_index = bone_comb[iAttrib].BoneId[iPaletteEntry];
			if (matrix_index != UINT_MAX) {
				D3DXMatrixMultiply(
					&mat,
					&meshContainer->boneOffsetMatrices.at(matrix_index),
					 meshContainer->boneMatrixPtrs.at(matrix_index)
				);
				m_device->SetTransform(D3DTS_WORLDMATRIX(iPaletteEntry), &mat);
			}
		}

		m_device->SetMaterial(&meshContainer->pMaterials[bone_comb[iAttrib].AttribId].MatD3D);
		m_device->SetTexture(0, meshContainer->textures.at(bone_comb[iAttrib].AttribId).Get());
		mesh->DrawSubset(iAttrib);
	}

	m_device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	m_device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);

	if (meshContainer->useSoftwareVP)
		m_device->SetSoftwareVertexProcessing(FALSE);
}

void DX9::SkinnedModel::SetMaterial(D3DXFRAME* frame, const D3DMATERIAL9& material)
{
	DX9MESHCONTAINER* mesh_container = (DX9MESHCONTAINER*)frame->pMeshContainer;
	while (mesh_container) {
		for (DWORD i = 0; i < mesh_container->NumMaterials; ++i) {
			mesh_container->pMaterials[i].MatD3D = material;
		}

		mesh_container = (DX9MESHCONTAINER*)mesh_container->pNextMeshContainer;
	}

	if (frame->pFrameSibling)
		SetMaterial(frame->pFrameSibling, material);

	if (frame->pFrameFirstChild)
		SetMaterial(frame->pFrameFirstChild, material);
}

DirectX::XMFLOAT3 DX9::SkinnedModel::GetRotation() const
{
	DirectX::XMFLOAT3X3 axis;
	DirectX::XMStoreFloat3x3(&axis, m_rotation);

	DirectX::XMFLOAT3 rotation;
	rotation.x = -asinf(axis._32);
	rotation.y =  atan2f(axis._12, axis._22);
	rotation.z =  atan2f(axis._31, axis._33);

	return rotation;
}

void DX9::SkinnedModel::Move(const float x, const float y, const float z)
{
	const auto right  = DirectX::XMVectorScale(m_rotation.r[0], x);
	const auto up     = DirectX::XMVectorScale(m_rotation.r[1], y);
	const auto foward = DirectX::XMVectorScale(m_rotation.r[2], z);

	m_position.v = DirectX::XMVectorAdd(m_position.v, right);
	m_position.v = DirectX::XMVectorAdd(m_position.v, up);
	m_position.v = DirectX::XMVectorAdd(m_position.v, foward);
}

void DX9::SkinnedModel::Rotate(const float x, const float y, const float z)
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

void DX9::SkinnedModel::SetTrackLoopTime(const UINT track, const float loopTime) const
{
	Microsoft::WRL::ComPtr<ID3DXAnimationSet> anime_set;
	if(m_animationController->GetAnimationSet(track, anime_set.GetAddressOf()) != D3D_OK)
		return;
	m_animationController->SetTrackSpeed(track, (float)anime_set->GetPeriod() / loopTime);
}

//void DX9::SkinnedModel::SetTrackLoopMode(const UINT track, const D3DXPLAYBACK_TYPE loopType) const
//{
//	Microsoft::WRL::ComPtr<ID3DXAnimationSet> anime_set;
//	if (m_animationController->GetAnimationSet(track, anime_set.GetAddressOf()) != D3D_OK)
//		return;
//
//	HRESULT hr;
//
//	Microsoft::WRL::ComPtr<ID3DXKeyframedAnimationSet> keyframe_anime_set;
//	hr = anime_set->QueryInterface(
//		IID_ID3DXKeyframedAnimationSet, 
//		(LPVOID*)keyframe_anime_set.GetAddressOf()
//	);
//	if (hr != S_OK)
//		return;
//
//	if (keyframe_anime_set->GetPlaybackType() == loopType)
//		return;
//
//	LPCSTR       NAME       = keyframe_anime_set->GetName();
//	const double TPS        = keyframe_anime_set->GetSourceTicksPerSecond();
//	const UINT   ANIMATIONS = keyframe_anime_set->GetNumAnimations();
//
//	Microsoft::WRL::ComPtr<ID3DXKeyframedAnimationSet> new_Keyframed_anime_set;
//	hr = D3DXCreateKeyframedAnimationSet(
//		NAME, TPS, loopType, ANIMATIONS,
//		0, nullptr, new_Keyframed_anime_set.GetAddressOf()
//	);
//	if (hr != S_OK)
//		return;
//
//	for (UINT i = 0; i < ANIMATIONS; ++i) {
//		const UINT SCALE_KEYS = keyframe_anime_set->GetNumScaleKeys(i);
//		std::vector<D3DXKEY_VECTOR3> scale_keys(SCALE_KEYS);
//		if (SCALE_KEYS > 0)
//			keyframe_anime_set->GetScaleKeys(i, scale_keys.data());
//
//		const UINT ROTATE_KEYS = keyframe_anime_set->GetNumRotationKeys(i);
//		std::vector<D3DXKEY_QUATERNION> rotate_keys(ROTATE_KEYS);
//		if (ROTATE_KEYS > 0)
//			keyframe_anime_set->GetRotationKeys(i, rotate_keys.data());
//
//		const UINT TRANS_KEYS = keyframe_anime_set->GetNumTranslationKeys(i);
//		std::vector<D3DXKEY_VECTOR3> trans_keys(TRANS_KEYS);
//		if (TRANS_KEYS > 0)
//			keyframe_anime_set->GetTranslationKeys(i, trans_keys.data());
//
//		LPCSTR anime_name;
//		keyframe_anime_set->GetAnimationNameByIndex(i, &anime_name);
//
//		new_Keyframed_anime_set->RegisterAnimationSRTKeys(
//			anime_name, SCALE_KEYS, ROTATE_KEYS, TRANS_KEYS,
//			scale_keys.data(), rotate_keys.data(), trans_keys.data(),
//			nullptr
//		);
//	}
//
//	m_animationController->UnregisterAnimationSet(keyframe_anime_set.Get());
//	if (m_animationController->RegisterAnimationSet(
//			new_Keyframed_anime_set.Get()
//		) != S_OK) {
//		m_animationController->RegisterAnimationSet(keyframe_anime_set.Get());
//		m_animationController->SetTrackAnimationSet(track, keyframe_anime_set.Get());
//		return;
//	}
//
//	m_animationController->SetTrackAnimationSet(track, new_Keyframed_anime_set.Get());
//}

void DX9::SkinnedModel::ChangeAnimation(
	const UINT track1, const UINT track2,
	const double duration, const D3DXTRANSITION_TYPE transition
)
{
	const double GAT = m_animationController->GetTime();

	m_animationController->UnkeyAllTrackEvents(track1);
	m_animationController->KeyTrackWeight(track1, 1.0f, GAT, duration, transition);
	m_animationController->SetTrackEnable(track1, TRUE);

	m_animationController->UnkeyAllTrackEvents(track2);
	m_animationController->KeyTrackWeight(track2, 0.0f, GAT, duration, transition);
	m_animationController->KeyTrackEnable(track2, FALSE, GAT + duration);
}

void DX9::SkinnedModel::ChangeAnimation(
	const UINT track, const double duration,
	const D3DXTRANSITION_TYPE transition
)
{
	const double  GAT = m_animationController->GetTime();
	const UINT TRACKS = m_animationController->GetMaxNumTracks();
	for (UINT i = 0; i < TRACKS; ++i) {
		D3DXTRACK_DESC desc;
		m_animationController->GetTrackDesc(i, &desc);
		if (desc.Enable) {
			m_animationController->UnkeyAllTrackEvents(i);
			m_animationController->KeyTrackWeight(i, 0.0f, GAT, duration, transition);
			m_animationController->KeyTrackEnable(i, FALSE, GAT + duration);
		}
	}

	m_animationController->UnkeyAllTrackEvents(track);
	m_animationController->KeyTrackWeight(track, 1.0f, GAT, duration, transition);
	m_animationController->SetTrackEnable(track, TRUE);
}

DirectX::BoundingSphere DX9::SkinnedModel::GetBoundingSphere()
{
	DirectX::BoundingSphere sphere;

	D3DXFrameCalculateBoundingSphere(
		m_frameRoot, (D3DXVECTOR3*)&sphere.Center, &sphere.Radius
	);

	sphere.Center.x += m_position.f[0];
	sphere.Center.y += m_position.f[1];
	sphere.Center.z += m_position.f[2];

	float scale = (std::max)({ m_scale.x, m_scale.y, m_scale.z });
	sphere.Radius *= scale;

	return sphere;
}

DirectX::BoundingBox DX9::SkinnedModel::GetBoundingBox()
{
	D3DXVECTOR3 pos_min(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 pos_max(FLT_MIN, FLT_MIN, FLT_MIN);
	ComputeBoundingBoxPoint(m_frameRoot, pos_min, pos_max);

	DirectX::BoundingBox box;

	const DirectX::XMVECTOR pt1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&pos_min);
	const DirectX::XMVECTOR pt2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&pos_max);
	DirectX::BoundingBox::CreateFromPoints(box, pt1, pt2);

	const DirectX::XMMATRIX world(GetWorldTransform());
	box.Transform(box, world);

	return box;
}

DirectX::BoundingOrientedBox DX9::SkinnedModel::GetBoundingOrientedBox()
{
	D3DXVECTOR3 pos_min(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 pos_max(FLT_MIN, FLT_MIN, FLT_MIN);
	ComputeBoundingBoxPoint(m_frameRoot, pos_min, pos_max);

	DirectX::BoundingBox box;
	const DirectX::XMVECTOR pt1 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&pos_min);
	const DirectX::XMVECTOR pt2 = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&pos_max);
	DirectX::BoundingBox::CreateFromPoints(box, pt1, pt2);

	DirectX::BoundingOrientedBox obb;
	DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, box);

	const DirectX::XMMATRIX world(GetWorldTransform());
	obb.Transform(obb, world);

	return obb;
}

void DX9::SkinnedModel::ComputeBoundingBoxPoint(D3DXFRAME* frame, D3DXVECTOR3& ptmin, D3DXVECTOR3& ptmax)
{
	DX9MESHCONTAINER* mesh_container = (DX9MESHCONTAINER*)frame->pMeshContainer;
	while (mesh_container) {
		BYTE* vertices;
		if (mesh_container->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&vertices) != D3D_OK)
			return;

		const int NumVertices = mesh_container->MeshData.pMesh->GetNumVertices();
		const int STRIDE      = mesh_container->MeshData.pMesh->GetNumBytesPerVertex();
		for (int i = 0; i < NumVertices; ++i) {
			D3DXVECTOR3& pos = *(D3DXVECTOR3*)vertices;
			if (pos.x < ptmin.x)	ptmin.x = pos.x;
			if (pos.x > ptmax.x)	ptmax.x = pos.x;
			if (pos.y < ptmin.y)	ptmin.y = pos.y;
			if (pos.y > ptmax.y)	ptmax.y = pos.y;
			if (pos.z < ptmin.z)	ptmin.z = pos.z;
			if (pos.z > ptmax.z)	ptmax.z = pos.z;
			vertices += STRIDE;
		}

		mesh_container->MeshData.pMesh->UnlockVertexBuffer();

		mesh_container = (DX9MESHCONTAINER*)mesh_container->pNextMeshContainer;
	}

	if (frame->pFrameSibling)
		ComputeBoundingBoxPoint(frame->pFrameSibling, ptmin, ptmax);

	if (frame->pFrameFirstChild)
		ComputeBoundingBoxPoint(frame->pFrameFirstChild, ptmin, ptmax);
}

std::unique_ptr<DX9::SkinnedModel> DX9::SkinnedModel::CreateFromFile(
	IDirect3DDevice9* device, LPCWSTR file
)
{
	wchar_t fullPath[MAX_PATH];
	const auto retvalue = _wfullpath(fullPath, file, MAX_PATH);

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir  [_MAX_DIR  ];
	_wsplitpath_s(fullPath, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

	wchar_t currentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDirectory);

	wchar_t filePath[MAX_PATH];
	wsprintf(filePath, L"%s%s", drive, dir);
	SetCurrentDirectory(filePath);

	std::unique_ptr<DX9::SkinnedModel> model = std::make_unique<DX9::SkinnedModel>(device);

	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DXAnimationController> animeController;
	CDX9AllocateHierarchy allocater(model.get());
	hr = D3DXLoadMeshHierarchyFromX(
		fullPath, 0, device, &allocater,
		nullptr, &model->m_frameRoot, animeController.GetAddressOf()
	);
	if (hr != D3D_OK)
		throw std::exception();

	SetCurrentDirectory(currentDirectory);

	UINT tracks     = animeController->GetMaxNumTracks();
	UINT anime_sets = animeController->GetMaxNumAnimationSets();
	if (tracks < anime_sets)
		tracks = anime_sets;

	hr = animeController->CloneAnimationController(
		animeController->GetMaxNumAnimationOutputs(),
		animeController->GetMaxNumAnimationSets(),
		tracks,
		animeController->GetMaxNumEvents(),
		model->m_animationController.GetAddressOf()
	);
	if (hr != S_OK)
		throw std::exception();

	animeController.Reset();

	if (anime_sets >= 2) {
		D3DXTRACK_DESC desc;
		desc.Priority = D3DXPRIORITY_LOW;
		desc.Weight   = 1.0f;
		desc.Speed    = 1.0f;
		desc.Position = 0.0;
		desc.Enable   = FALSE;

		for (UINT i = 1; i < anime_sets; ++i) {
			Microsoft::WRL::ComPtr<ID3DXAnimationSet> anime_set;
			if (model->m_animationController->GetAnimationSet(
					i, anime_set.GetAddressOf()
				) != S_OK)
				throw std::exception();

			hr = model->m_animationController->SetTrackAnimationSet(i, anime_set.Get());
			anime_set.Reset();
			if (hr != S_OK)
				throw std::exception();
			if (model->m_animationController->SetTrackDesc(i, &desc) != S_OK)
				throw std::exception();
		}
	}

	if(!model->SetupBoneMatrixPointers(model->m_frameRoot))
		throw std::exception();
	model->SetBoneOffsetMatrixPointer(model->m_frameRoot);

	model->m_animationController->AdvanceTime(0.0, nullptr);

	return model;
}

HRESULT DX9::SkinnedModel::GenerateSkinnedMesh(DX9MESHCONTAINER* meshContainer)
{
	if (!meshContainer->pSkinInfo)
		return S_OK;

	HRESULT hr;

	Microsoft::WRL::ComPtr<IDirect3DIndexBuffer9> index_buffer;
	hr = meshContainer->mesh->GetIndexBuffer(index_buffer.GetAddressOf());
	if (hr != D3D_OK)
		return hr;

	meshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	DWORD num_max_faceinfl;
	hr = meshContainer->pSkinInfo->GetMaxFaceInfluences(
		index_buffer.Get(),
		meshContainer->mesh->GetNumFaces(),
		&num_max_faceinfl
	);
	index_buffer.Reset();
	if (hr != D3D_OK)
		return hr;

	num_max_faceinfl = min(num_max_faceinfl, 12);

	DWORD flags = D3DXMESHOPT_VERTEXCACHE;
	if (m_maxVertexBlendMatrixIndex == 0 || m_maxVertexBlendMatrixIndex + 1 < num_max_faceinfl) {
		meshContainer->useSoftwareVP     = true;
		meshContainer->numPaletteEntries = min(meshContainer->pSkinInfo->GetNumBones(), 256);
		flags |= D3DXMESH_SYSTEMMEM;
	} else {
		meshContainer->useSoftwareVP     = false;
		//meshContainer->numPaletteEntries = min(
		//	(m_maxVertexBlendMatrixIndex + 1) / 2,
		//	meshContainer->pSkinInfo->GetNumBones()
		//);
		meshContainer->numPaletteEntries = max(meshContainer->numPaletteEntries, num_max_faceinfl);
	}

	// ƒƒbƒVƒ…•ÏŠ·
	hr = meshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(
		meshContainer->mesh.Get(),
		flags,
		meshContainer->numPaletteEntries,
		meshContainer->pAdjacency,
		nullptr, nullptr, nullptr,
		&meshContainer->numInfl,
		&meshContainer->numAttributeGroups,
		 meshContainer->boneCombinationBuf.GetAddressOf(),
		&meshContainer->MeshData.pMesh);
	if (hr != D3D_OK)
		return hr;
	
	std::unique_ptr<DWORD[]> adjacency = std::make_unique<DWORD[]>(
		meshContainer->MeshData.pMesh->GetNumFaces() * 3
	);
	meshContainer->MeshData.pMesh->GenerateAdjacency(FLT_EPSILON, adjacency.get());
	meshContainer->MeshData.pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		adjacency.get(), nullptr, nullptr, nullptr
	);

	return S_OK;
}

bool DX9::SkinnedModel::SetupBoneMatrixPointers(D3DXFRAME* frame)
{
	if (frame->pMeshContainer) {
		if (!SetupBoneMatrixPointersOnMesh(frame->pMeshContainer))
			return false;
	}

	if (frame->pFrameSibling) {
		if (!SetupBoneMatrixPointers(frame->pFrameSibling))
			return false;
	}

	if (frame->pFrameFirstChild) {
		if (!SetupBoneMatrixPointers(frame->pFrameFirstChild))
			return false;
	}

	return true;
}

bool DX9::SkinnedModel::SetupBoneMatrixPointersOnMesh(D3DXMESHCONTAINER* meshContainer)
{
	DX9MESHCONTAINER* mesh_container = (DX9MESHCONTAINER*)meshContainer;
	if (mesh_container->pSkinInfo) {
		const UINT BONES = mesh_container->pSkinInfo->GetNumBones();
		mesh_container->boneMatrixPtrs.resize(BONES, nullptr);

		for (UINT i = 0; i < BONES; i++) {
			DX9FRAME* pFrame = (DX9FRAME*)D3DXFrameFind(m_frameRoot, mesh_container->pSkinInfo->GetBoneName(i));
			if (!pFrame) {
				mesh_container->boneMatrixPtrs.clear();
				return false;
			}
			mesh_container->boneMatrixPtrs.at(i) = &pFrame->CombinedTransformationMatrix;
		}
	}

	return true;
}

void DX9::SkinnedModel::SetBoneOffsetMatrixPointer(D3DXFRAME* frame)
{
	if (frame->pMeshContainer) {
		DX9MESHCONTAINER* pMeshContainer = (DX9MESHCONTAINER*)frame->pMeshContainer;
		if (pMeshContainer->boneOffsetMatrices.size() > 0) {
			m_boneOffsetMatrices    = pMeshContainer->boneOffsetMatrices.data();
			m_numBoneOffsetMatrices = pMeshContainer->pSkinInfo->GetNumBones();
			return;
		}
	}

	if (frame->pFrameSibling) {
		SetBoneOffsetMatrixPointer(frame->pFrameSibling);
		if (m_boneOffsetMatrices)
			return;
	}

	if (frame->pFrameFirstChild)
		SetBoneOffsetMatrixPointer(frame->pFrameFirstChild);
}

HRESULT DX9::SkinnedModel::CDX9AllocateHierarchy::CreateFrame(LPCSTR name, LPD3DXFRAME* ppNewFrame)
{
	*ppNewFrame = nullptr;

	DX9FRAME* frame = new DX9FRAME;
	if (!frame)
		return E_OUTOFMEMORY;

	frame->pMeshContainer   = nullptr;
	frame->pFrameSibling    = nullptr;
	frame->pFrameFirstChild = nullptr;

	frame->SetName(name);

	D3DXMatrixIdentity(&frame->TransformationMatrix);
	D3DXMatrixIdentity(&frame->CombinedTransformationMatrix);

	*ppNewFrame = frame;

	return D3D_OK;
}

HRESULT DX9::SkinnedModel::CDX9AllocateHierarchy::CreateMeshContainer(
	LPCSTR                    name,
	CONST D3DXMESHDATA*       pMeshData,
	CONST D3DXMATERIAL*       pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD                     inNumMaterials,
	CONST DWORD*              pAdjacency,
	LPD3DXSKININFO            pSkinInfo,
	LPD3DXMESHCONTAINER*      ppNewMeshContainer
)
{
	*ppNewMeshContainer = nullptr;

	Microsoft::WRL::ComPtr<IDirect3DDevice9> device;
	DX9MESHCONTAINER* mesh_container = nullptr;
	try {
		ID3DXMesh* mesh = pMeshData->pMesh;
		if (pMeshData->Type != D3DXMESHTYPE_MESH)
			throw E_FAIL;

		if (mesh->GetFVF() == 0)
			throw E_FAIL;

		mesh->GetDevice(device.GetAddressOf());

		mesh_container = new DX9MESHCONTAINER;
		if (!mesh_container)
			throw E_OUTOFMEMORY;

		ZeroMemory(mesh_container, sizeof(DX9MESHCONTAINER));
		mesh_container->SetName(name);

		mesh_container->MeshData.Type  = D3DXMESHTYPE_MESH;
		mesh_container->MeshData.pMesh = mesh;

		const UINT NumFaces = mesh->GetNumFaces();
		mesh_container->pAdjacency = new DWORD[NumFaces * 3];
		if (!mesh_container->pAdjacency)
			throw E_OUTOFMEMORY;
		if (mesh_container->MeshData.pMesh->GenerateAdjacency(FLT_EPSILON, mesh_container->pAdjacency) != D3D_OK)
			ZeroMemory(mesh_container->pAdjacency, sizeof(DWORD) * NumFaces * 3);

		mesh_container->NumMaterials = max(inNumMaterials, 1);
		mesh_container->pMaterials   = new D3DXMATERIAL[mesh_container->NumMaterials];
		if (!mesh_container->pMaterials)
			throw E_OUTOFMEMORY;

		mesh_container->textures.resize(mesh_container->NumMaterials, nullptr);

		if (inNumMaterials > 0) {
			CopyMemory(mesh_container->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * inNumMaterials);
			for (DWORD i = 0; i < inNumMaterials; i++) {
				if (mesh_container->pMaterials[i].pTextureFilename) {
					if (D3DXCreateTextureFromFileA(
							device.Get(),
							mesh_container->pMaterials[i].pTextureFilename,
							mesh_container->textures.at(i).GetAddressOf()
						)
						!= D3D_OK)
						mesh_container->textures.at(i) = nullptr;
				}
			}
		} else {
			mesh_container->pMaterials[0].MatD3D.Diffuse   = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			mesh_container->pMaterials[0].MatD3D.Ambient   = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
			mesh_container->pMaterials[0].MatD3D.Specular  = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			mesh_container->pMaterials[0].MatD3D.Emissive  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			mesh_container->pMaterials[0].MatD3D.Power     = 1.0f;
			mesh_container->pMaterials[0].pTextureFilename = nullptr;
		}

		if (pSkinInfo) {
			mesh_container->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();

			mesh_container->mesh = mesh_container->MeshData.pMesh;

			const UINT BONES = pSkinInfo->GetNumBones();
			mesh_container->boneOffsetMatrices.resize(BONES);
			for (UINT i = 0; i < BONES; i++) {
				mesh_container->boneOffsetMatrices.at(i) = *(mesh_container->pSkinInfo->GetBoneOffsetMatrix(i));
			}

			const HRESULT hr = m_skinnedModel->GenerateSkinnedMesh(mesh_container);
			if (FAILED(hr))
				throw hr;
		}

		*ppNewMeshContainer = mesh_container;
	}
	catch (const HRESULT hr) {
		if (mesh_container)
			DestroyMeshContainer(mesh_container);

		return hr;
	}

	return D3D_OK;
}

HRESULT DX9::SkinnedModel::CDX9AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrame)
{
	delete pFrame;
	pFrame = nullptr;

	return S_OK;
}

HRESULT DX9::SkinnedModel::CDX9AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DX9MESHCONTAINER* mesh_container = (DX9MESHCONTAINER*)pMeshContainerBase;

	DeleteArray(mesh_container->pAdjacency);
	DeleteArray(mesh_container->pMaterials);

	for (DWORD i = 0; i < mesh_container->NumMaterials; i++)
		mesh_container->textures.at(i).Reset();

	mesh_container->boneMatrixPtrs.clear();

	mesh_container->boneCombinationBuf.Reset();
	SafeRelease(mesh_container->MeshData.pMesh);
	SafeRelease(mesh_container->pSkinInfo);
	mesh_container->mesh.Reset();

	delete mesh_container;
	mesh_container = nullptr;

	return S_OK;
}