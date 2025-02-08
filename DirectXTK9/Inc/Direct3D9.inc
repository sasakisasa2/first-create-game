class direct3d9 {
public:
	friend dxtk;

	direct3d9() : m_gameBase(nullptr), m_d3dDevice9(nullptr) {}
	~direct3d9() = default;

	inline void CreateShaderResourceView(const D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor) const
	{ m_gameBase->D3D9CreateShaderResourceView(srvDescriptor); }

	inline D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceView(
		DirectX::DescriptorHeap* descripterHeap, const size_t index
	) const
	{
		m_gameBase->D3D9CreateShaderResourceView(descripterHeap->GetCpuHandle(index));
		return descripterHeap->GetGpuHandle(index);
	}

	inline void UpdateResource() const
	{ m_gameBase->D3D9UpdateResource(); }

	inline void WaitUpdate() const
	{ m_gameBase->D3D9WaitUpdate(); }

	inline void Clear(const D3DCOLOR color)
	{
		m_d3dDevice9->Clear(
			0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			color, 1.0f, 0
		);
	}

	inline void BeginScene() const
	{ m_d3dDevice9->BeginScene(); }

	inline void EndScene() const
	{ m_d3dDevice9->EndScene(); }

	inline void DrawInstanced(
		const D3DPRIMITIVETYPE PrimitiveType,
		const UINT             PrimitiveCount,
		const void*            pVertexStreamZeroData,
		const DWORD            fvf,
		const UINT             VertexStreamZeroStride = 0
	) const
	{
		const UINT STRIDE = VertexStreamZeroStride == 0 ? 
			D3DXGetFVFVertexSize(fvf) : VertexStreamZeroStride;

		m_d3dDevice9->SetFVF(fvf);
		m_d3dDevice9->DrawPrimitiveUP(
			PrimitiveType, PrimitiveCount,
			pVertexStreamZeroData, STRIDE
		);
	}

	inline void SetTransform(const D3DTRANSFORMSTATETYPE state, const D3DMATRIX& matrix) const
	{ m_d3dDevice9->SetTransform(state, &matrix); }

	inline void SetWorldTransform(const D3DMATRIX& matrix) const
	{ m_d3dDevice9->SetTransform(D3DTS_WORLD, &matrix); }

	inline void SetViewTransform(const D3DMATRIX& matrix) const
	{ m_d3dDevice9->SetTransform(D3DTS_VIEW, &matrix); }

	inline void SetProjectionTransform(const D3DMATRIX& matrix) const
	{ m_d3dDevice9->SetTransform(D3DTS_PROJECTION, &matrix); }

	inline void SetCamera(const DX9::Camera& camera) const
	{
		const DirectX::XMMATRIX& view = camera.ViewMatrix;
		const DirectX::XMMATRIX& proj = camera.ProjectionMatrix;
		m_d3dDevice9->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);
		m_d3dDevice9->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&proj);
	}

	inline void SetRenderState(const D3DRENDERSTATETYPE state, const DWORD value) const
	{ m_d3dDevice9->SetRenderState(state, value); }

	inline void SetRenderState(const D3DRENDERSTATETYPE state, const float value) const
	{ m_d3dDevice9->SetRenderState(state, *((DWORD*)&value)); }

	inline void SetLight(
		const DWORD index, const D3DLIGHT9& light, const bool enable = true
	) const
	{
		m_d3dDevice9->SetLight   (index, &light);
		m_d3dDevice9->LightEnable(index, (BOOL)enable);
	}

	inline void LightEnable(const DWORD index, const bool enable) const
	{ m_d3dDevice9->LightEnable(index, (BOOL)enable); }

	inline void SetMaterial(const D3DMATERIAL9& material) const
	{ m_d3dDevice9->SetMaterial(&material); }

	inline void SetTexture(const DWORD stage, IDirect3DTexture9* texture) const
	{ m_d3dDevice9->SetTexture(stage, texture); }

	inline void SetAlphaBlendMode(const DX9::AlphaBlendMode mode) const
	{
		switch (mode) {
		case DX9::AlphaBlendMode::Modulate:
			m_d3dDevice9->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
			m_d3dDevice9->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
			m_d3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;

		case DX9::AlphaBlendMode::Add:
			m_d3dDevice9->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
			m_d3dDevice9->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
			m_d3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		case DX9::AlphaBlendMode::Subtract:
			m_d3dDevice9->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_SUBTRACT);
			m_d3dDevice9->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
			m_d3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;

		case DX9::AlphaBlendMode::SourceColor:
			m_d3dDevice9->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
			m_d3dDevice9->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
			m_d3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			break;

		case DX9::AlphaBlendMode::ZeroColor:
			m_d3dDevice9->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
			m_d3dDevice9->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
			m_d3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		}
	}

	inline void AlphaBendEnable(
		const bool enable,
		const DX9::AlphaBlendMode blend = DX9::AlphaBlendMode::Modulate
	) const
	{
		if (enable) {
            m_d3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            m_d3dDevice9->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
            m_d3dDevice9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
            SetAlphaBlendMode(blend);
        } else {
            m_d3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            m_d3dDevice9->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
            m_d3dDevice9->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);
        }
	}

	DirectX::XMFLOAT3 WorldToScreenPoint(const DirectX::XMFLOAT3& position, const DX9::Camera& camera)
	{
		D3DVIEWPORT9 viewport;
		m_d3dDevice9->GetViewport(&viewport);

		DirectX::SimpleMath::Matrix vpm = DirectX::SimpleMath::Matrix::Identity;
		vpm._11 = viewport.Width / 2.0f;
		vpm._22 = -static_cast<float>(viewport.Height) / 2.0f;
		vpm._33 = viewport.MaxZ - viewport.MinZ;
		vpm._41 = viewport.X + viewport.Width  / 2.0f;
		vpm._42 = viewport.Y + viewport.Height / 2.0f;
		vpm._43 = viewport.MinZ;
		vpm._44 = 1.0f;

		DirectX::SimpleMath::Vector3 screen(position);
		screen = DirectX::XMVector3TransformCoord(screen, camera.ViewMatrix);
		screen = DirectX::XMVector3TransformCoord(screen, camera.ProjectionMatrix);
		screen = DirectX::XMVector3TransformCoord(screen, vpm);

		return screen;
	}

	// position.zには、ワールド座標のzを設定しておく
	DirectX::XMFLOAT3 ScreenToWorldPoint(const DirectX::XMFLOAT3& position, const DX9::Camera& camera)
	{
		D3DVIEWPORT9 viewport;
		m_d3dDevice9->GetViewport(&viewport);

		DirectX::SimpleMath::Matrix vpm = DirectX::SimpleMath::Matrix::Identity;
		vpm._11 = viewport.Width / 2.0f;
		vpm._22 = -static_cast<float>(viewport.Height) / 2.0f;
		vpm._33 = viewport.MaxZ - viewport.MinZ;
		vpm._41 = viewport.X + viewport.Width  / 2.0f;
		vpm._42 = viewport.Y + viewport.Height / 2.0f;
		vpm._43 = viewport.MinZ;
		vpm._44 = 1.0f;
		vpm = vpm.Invert();

		DirectX::SimpleMath::Matrix inv_proj = DirectX::XMMatrixInverse(nullptr, camera.ProjectionMatrix);
		DirectX::SimpleMath::Matrix inv_view = DirectX::XMMatrixInverse(nullptr, camera.ViewMatrix);

		DirectX::SimpleMath::Vector3 world(position.x, position.y, 0.0f);
		world = DirectX::XMVector3TransformCoord(world, vpm);
		world = DirectX::XMVector3TransformCoord(world, inv_proj);
		world = DirectX::XMVector3TransformCoord(world, inv_view);

		world.x = (world.x - inv_view.m[3][0]) * (position.z - world.z) + inv_view.m[3][0];
		world.y = (world.y - inv_view.m[3][1]) * (position.z - world.z) + inv_view.m[3][1];
		world.z = position.z;

		return world;
	}

private:
	GameBase*           m_gameBase;
	IDirect3DDevice9Ex* m_d3dDevice9;

	inline void Attach(GameBase* gamebase)
	{ 
		m_gameBase   = gamebase;
		m_d3dDevice9 = gamebase->m_d3d9Device.Get();
	}
};