#pragma once

#include <Windows.h>
#include <DirectXMath.h>

namespace DX9 {
	class Camera {
	public:
		Camera();
		virtual ~Camera() {}

		void SetView(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& rotation
		);
		void SetViewLookAt(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& at,
			const DirectX::XMFLOAT3& up
		);
		void SetViewLookTo(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& direction,
			const DirectX::XMFLOAT3& up
		);

		inline void SetPerspectiveFieldOfView(
			const float fovY,  const float aspect,
			const float nearZ, const float farZ
		)
		{
			m_projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
		}

		inline void SetOrthographic(
			const float width, const float height,
			const float nearZ, const float farZ
		)
		{
			m_projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
		}

		inline void SetOrthographicOffCenter(
			const float left,   const float right,
			const float bottom, const float top,
			const float nearZ,  const float farZ
		)
		{
			m_projection = DirectX::XMMatrixOrthographicOffCenterLH(
				left, right, bottom, top, nearZ, farZ
			);
		}

		void SetFieldOfViewY(const float fovY);

		inline DirectX::XMMATRIX GetViewMatrix() const
		{ 
			const DirectX::XMMATRIX view = DirectX::XMMatrixInverse(nullptr, m_world);
			return view;
		}

		inline DirectX::XMMATRIX GetProjectionMatrix() const
		{ return m_projection; }

		inline DirectX::XMMATRIX GetViewProjectionMatrix() const
		{
			const auto view = GetViewMatrix();
			return view * m_projection;
		}

		inline void SetPosition(const DirectX::XMFLOAT3& position)
		{ CopyMemory(&m_world.r[3], &position, sizeof(float) * 3); }
		inline void SetPosition(const float x, const float y, const float z)
		{ m_world.r[3] = DirectX::XMVectorSet(x, y, z, 1.0f); }

		inline DirectX::XMFLOAT3 GetPosition() const
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMStoreFloat3(&position, m_world.r[3]);
			return position;
		}

		void SetRotation(const float x, const float y, const float z);
		inline void SetRotation(const DirectX::XMFLOAT3& rotation)
		{ SetRotation(rotation.x, rotation.y, rotation.z); }

		DirectX::XMFLOAT3 GetRotation() const;

		inline DirectX::XMFLOAT3 GetRightVector() const
		{
			DirectX::XMFLOAT3 right;
			DirectX::XMStoreFloat3(&right, m_world.r[0]);
			return right;
		}

		inline DirectX::XMFLOAT3 GetUpVector() const
		{
			DirectX::XMFLOAT3 up;
			DirectX::XMStoreFloat3(&up, m_world.r[1]);
			return up;
		}

		inline DirectX::XMFLOAT3 GetForwardVector() const
		{
			DirectX::XMFLOAT3 forward;
			DirectX::XMStoreFloat3(&forward, m_world.r[2]);
			return forward;
		}

		void Move(const float x, const float y, const float z);
		inline void Move(const DirectX::XMFLOAT3& move)
		{ Move(move.x, move.y, move.z); }

		void MoveForward(const float z);

		void Rotate(const float x, const float y, const float z);
		inline void Rotate(const DirectX::XMFLOAT3& rotation)
		{ Rotate(rotation.x, rotation.y, rotation.z); }

		void RotateAxis(const DirectX::XMFLOAT3& axis, const float angle);

		__declspec(property(get = GetViewMatrix))		    DirectX::XMMATRIX ViewMatrix;
		__declspec(property(get = GetProjectionMatrix))     DirectX::XMMATRIX ProjectionMatrix;
		__declspec(property(get = GetViewProjectionMatrix)) DirectX::XMMATRIX ViewProjectionMatrix;

		__declspec(property(get = GetPosition))				DirectX::XMFLOAT3 Position;
		__declspec(property(get = GetRotation))				DirectX::XMFLOAT3 Rotation;

		__declspec(property(get = GetRightVector ))			DirectX::XMFLOAT3 RightVector;
		__declspec(property(get = GetUpVector    ))			DirectX::XMFLOAT3 UpVector;
		__declspec(property(get = GetForwardVector))		DirectX::XMFLOAT3 ForwardVector;

		inline Camera* operator->() { return this; }

	private:
		DirectX::XMMATRIX m_world;
		DirectX::XMMATRIX m_projection;
	};

	typedef Camera CAMERA;
}
