#include "DXTKCamera.h"
#include <Windows.h>

DirectXTK::Camera::Camera()
{
	m_world      = DirectX::XMMatrixIdentity();
	m_projection = DirectX::XMMatrixIdentity();
}

void DirectXTK::Camera::SetView(
	const DirectX::XMFLOAT3& position,
	const DirectX::XMFLOAT3& rotation
)
{
	m_world      = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	m_world.r[3] = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);
}

void DirectXTK::Camera::SetViewLookAt(
	const DirectX::XMFLOAT3& position,
	const DirectX::XMFLOAT3& at,
	const DirectX::XMFLOAT3& up
)
{
	const DirectX::XMVECTOR eye   = DirectX::XMLoadFloat3(&position);
	const DirectX::XMVECTOR focus = DirectX::XMLoadFloat3(&at);
	const DirectX::XMVECTOR updir = DirectX::XMLoadFloat3(&up);
	m_world = DirectX::XMMatrixLookAtLH(eye, focus, updir);
	m_world = DirectX::XMMatrixInverse(nullptr, m_world);
}

void DirectXTK::Camera::SetViewLookTo(
	const DirectX::XMFLOAT3& position,
	const DirectX::XMFLOAT3& direction,
	const DirectX::XMFLOAT3& up
)
{
	const DirectX::XMVECTOR eye   = DirectX::XMLoadFloat3(&position);
	const DirectX::XMVECTOR dir   = DirectX::XMLoadFloat3(&direction);
	const DirectX::XMVECTOR updir = DirectX::XMLoadFloat3(&up);
	m_world = DirectX::XMMatrixLookToLH(eye, dir, updir);
	m_world = DirectX::XMMatrixInverse(nullptr, m_world);
}

void DirectXTK::Camera::SetFieldOfViewY(const float fovY)
{
	DirectX::XMFLOAT3 m1, m2;
	DirectX::XMStoreFloat3(&m1, m_projection.r[0]);
	DirectX::XMStoreFloat3(&m2, m_projection.r[1]);

	m1.x = m2.y / m1.x;
	m2.y = 1.0f / tanf(fovY * 0.5f);
	m1.x = m2.y / m1.x;

	m_projection.r[0] = DirectX::XMLoadFloat3(&m1);
	m_projection.r[1] = DirectX::XMLoadFloat3(&m2);
}

void DirectXTK::Camera::SetRotation(const float x, const float y, const float z)
{
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	//CopyMemory(m_world.r, rotation.r, sizeof(float) * 4 * 3);
	m_world.r[0] = rotation.r[0];
	m_world.r[1] = rotation.r[1];
	m_world.r[2] = rotation.r[2];
}

DirectX::XMFLOAT3 DirectXTK::Camera::GetRotation() const
{
	DirectX::XMFLOAT3X3 axis;
	DirectX::XMStoreFloat3x3(&axis, m_world);

	//DirectX::XMFLOAT3 rotation;
	//rotation.x = -asinf (axis._32);
	//rotation.y =  atan2f(axis._12, axis._22);
	//rotation.z =  atan2f(axis._31, axis._33);

	//return rotation;
	const float cy =  sqrtf(axis._33 * axis._33 + axis._31 * axis._31);
	const float cx = atan2f(-axis._32, cy);
	if (cy <= 16.f * FLT_EPSILON)
		return DirectX::XMFLOAT3(cx, 0.0f, atan2f(-axis._21, axis._11));

	return DirectX::XMFLOAT3(cx, atan2f(axis._31, axis._33), atan2f(axis._12, axis._22));
}

void DirectXTK::Camera::Move(const float x, const float y, const float z)
{
	const auto right   = DirectX::XMVectorScale(m_world.r[0], x);
	const auto up      = DirectX::XMVectorScale(m_world.r[1], y);
	const auto forward = DirectX::XMVectorScale(m_world.r[2], z);

	m_world.r[3] = DirectX::XMVectorAdd(m_world.r[3], right);
	m_world.r[3] = DirectX::XMVectorAdd(m_world.r[3], up);
	m_world.r[3] = DirectX::XMVectorAdd(m_world.r[3], forward);
}

void DirectXTK::Camera::MoveForward(const float z)
{
	DirectX::XMVECTORF32 forward;
	forward.v    = m_world.r[2];
	forward.f[1] = 0.0f;
	forward.v    = DirectX::XMVector3Normalize(forward.v);
	forward.v    = DirectX::XMVectorScale     (forward.v, z);
	m_world.r[3] = DirectX::XMVectorAdd       (m_world.r[3], forward);
}

void DirectXTK::Camera::Rotate(const float x, const float y, const float z)
{
	const DirectX::XMVECTOR position = m_world.r[3];
	m_world.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	if (z != 0.0f) {
		const auto rotation = DirectX::XMMatrixRotationAxis(m_world.r[2], z);
		m_world = DirectX::XMMatrixMultiply(m_world, rotation);
	}

	if (x != 0.0f) {
		const auto rotation = DirectX::XMMatrixRotationAxis(m_world.r[0], x);
		m_world = DirectX::XMMatrixMultiply(m_world, rotation);
	}

	if (y != 0.0f) {
		const auto rotation = DirectX::XMMatrixRotationAxis(m_world.r[1], y);
		m_world = DirectX::XMMatrixMultiply(m_world, rotation);
	}

	m_world.r[3] = position;
}

void DirectXTK::Camera::RotateAxis(const DirectX::XMFLOAT3& axis, const float angle)
{
	const DirectX::XMVECTOR axis_v   = DirectX::XMLoadFloat3(&axis);
	const DirectX::XMVECTOR position = m_world.r[3];
	m_world.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	const auto rotation = DirectX::XMMatrixRotationAxis(axis_v, angle);
	m_world = DirectX::XMMatrixMultiply(m_world, rotation);

	m_world.r[3] = position;
}
