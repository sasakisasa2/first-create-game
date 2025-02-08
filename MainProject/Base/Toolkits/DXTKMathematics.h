#pragma once

#include <SimpleMath.h>

namespace DirectXTK {
	// XMVLOAT3 Helpers
	inline XMFLOAT3 operator+ (const XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator+ (const XMFLOAT3& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator- (const XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator- (const XMFLOAT3& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator* (const XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorMultiply(V1, V2);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator* (const XMFLOAT3& v1, const XMMATRIX& m) noexcept
	{
		const XMVECTOR V = XMLoadFloat3(&v1);
		const XMVECTOR X = XMVector3TransformCoord(V, m);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator* (const XMFLOAT3& v1, const float s) noexcept
	{
		const XMVECTOR V = XMLoadFloat3(&v1);
		const XMVECTOR X = XMVectorScale(V, s);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator* (const float s, const XMFLOAT3& v1) noexcept
	{
		const XMVECTOR V = XMLoadFloat3(&v1);
		const XMVECTOR X = XMVectorScale(V, s);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator/ (const XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorDivide(V1, V2);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3 operator/ (const XMFLOAT3& v1, const float d) noexcept
	{
		const XMVECTOR V = XMLoadFloat3(&v1);
		const XMVECTOR X = XMVectorScale(V, 1.0f / d);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3& operator+= (XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMStoreFloat3(&v1, X);
		return v1;
	}

	inline XMFLOAT3& operator-= (XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMStoreFloat3(&v1, X);
		return v1;
	}

	inline XMFLOAT3& operator*= (XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorMultiply(V1, V2);
		XMStoreFloat3(&v1, X);
		return v1;
	}

	inline XMFLOAT3& operator*= (XMFLOAT3& v, const XMMATRIX& m) noexcept
	{
		const XMVECTOR V = XMLoadFloat3(&v);
		const XMVECTOR X = XMVector3TransformCoord(V, m);
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3& operator*= (XMFLOAT3& v, const float s) noexcept
	{
		const XMVECTOR V = XMLoadFloat3(&v);
		const XMVECTOR X = XMVectorScale(V, s);
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3& operator/= (XMFLOAT3& v1, const XMFLOAT3& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat3(&v2);
		const XMVECTOR X  = XMVectorDivide(V1, V2);
		XMStoreFloat3(&v1, X);
		return v1;
	}

	inline XMFLOAT3& operator/= (XMFLOAT3& v1, const float d) noexcept
	{
		const XMVECTOR V = XMLoadFloat3(&v1);
		const XMVECTOR X = XMVectorScale(V, 1.0f / d);
		XMFLOAT3 v;
		XMStoreFloat3(&v, X);
		return v;
	}

	inline XMFLOAT3& operator+= (XMFLOAT3& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMStoreFloat3(&v1, X);
		return v1;
	}

	inline XMFLOAT3& operator-= (XMFLOAT3& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat3(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMStoreFloat3(&v1, X);
		return v1;
	}

	// XMVLOAT2 Helpers
	inline XMFLOAT2 operator+ (const XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2 operator- (const XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2 operator* (const XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorMultiply(V1, V2);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2 operator* (const XMFLOAT2& v1, const float s) noexcept
	{
		const XMVECTOR V = XMLoadFloat2(&v1);
		const XMVECTOR X = XMVectorScale(V, s);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2 operator* (const float s, const XMFLOAT2& v1) noexcept
	{
		const XMVECTOR V = XMLoadFloat2(&v1);
		const XMVECTOR X = XMVectorScale(V, s);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2 operator* (const XMFLOAT2& v1, const XMMATRIX& m) noexcept
	{
		const XMVECTOR V = XMLoadFloat2(&v1);
		const XMVECTOR X = XMVector2TransformCoord(V, m);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2 operator/ (const XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorDivide(V1, V2);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2 operator/ (const XMFLOAT2& v1, const float d) noexcept
	{
		const XMVECTOR V = XMLoadFloat2(&v1);
		const XMVECTOR X = XMVectorScale(V, 1.0f / d);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2& operator+= (XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMStoreFloat2(&v1, X);
		return v1;
	}

	inline XMFLOAT2& operator-= (XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMStoreFloat2(&v1, X);
		return v1;
	}

	inline XMFLOAT2& operator*= (XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorMultiply(V1, V2);
		XMStoreFloat2(&v1, X);
		return v1;
	}

	inline XMFLOAT2& operator*= (XMFLOAT2& v, const XMMATRIX& m) noexcept
	{
		const XMVECTOR V = XMLoadFloat2(&v);
		const XMVECTOR X = XMVector2TransformCoord(V, m);
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2& operator*= (XMFLOAT2& v, const float s) noexcept
	{
		const XMVECTOR V = XMLoadFloat2(&v);
		const XMVECTOR X = XMVectorScale(V, s);
		XMStoreFloat2(&v, X);
		return v;
	}

	inline XMFLOAT2& operator/= (XMFLOAT2& v1, const XMFLOAT2& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat2(&v1);
		const XMVECTOR V2 = XMLoadFloat2(&v2);
		const XMVECTOR X  = XMVectorDivide(V1, V2);
		XMStoreFloat2(&v1, X);
		return v1;
	}

	inline XMFLOAT2& operator/= (XMFLOAT2& v1, const float d) noexcept
	{
		const XMVECTOR V = XMLoadFloat2(&v1);
		const XMVECTOR X = XMVectorScale(V, 1.0f / d);
		XMFLOAT2 v;
		XMStoreFloat2(&v, X);
		return v;
	}

	// XMVLOAT4 Helpers
	inline XMFLOAT4 operator+ (const XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4 operator- (const XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4 operator* (const XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorMultiply(V1, V2);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4 operator* (const XMFLOAT4& v1, const XMMATRIX& m) noexcept
	{
		const XMVECTOR V = XMLoadFloat4(&v1);
		const XMVECTOR X = XMVector4Transform(V, m);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4 operator* (const XMFLOAT4& v1, const float s) noexcept
	{
		const XMVECTOR V = XMLoadFloat4(&v1);
		const XMVECTOR X = XMVectorScale(V, s);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4 operator* (const float s, const XMFLOAT4& v1) noexcept
	{
		const XMVECTOR V = XMLoadFloat4(&v1);
		const XMVECTOR X = XMVectorScale(V, s);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4 operator/ (const XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorDivide(V1, V2);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4 operator/ (const XMFLOAT4& v1, const float d) noexcept
	{
		const XMVECTOR V = XMLoadFloat4(&v1);
		const XMVECTOR X = XMVectorScale(V, 1.0f / d);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4& operator+= (XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorAdd(V1, V2);
		XMStoreFloat4(&v1, X);
		return v1;
	}

	inline XMFLOAT4& operator-= (XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorSubtract(V1, V2);
		XMStoreFloat4(&v1, X);
		return v1;
	}

	inline XMFLOAT4& operator*= (XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorMultiply(V1, V2);
		XMStoreFloat4(&v1, X);
		return v1;
	}

	inline XMFLOAT4& operator*= (XMFLOAT4& v, const XMMATRIX& m) noexcept
	{
		const XMVECTOR V = XMLoadFloat4(&v);
		const XMVECTOR X = XMVector4Transform(V, m);
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4& operator*= (XMFLOAT4& v, const float s) noexcept
	{
		const XMVECTOR V = XMLoadFloat4(&v);
		const XMVECTOR X = XMVectorScale(V, s);
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMFLOAT4& operator/= (XMFLOAT4& v1, const XMFLOAT4& v2) noexcept
	{
		const XMVECTOR V1 = XMLoadFloat4(&v1);
		const XMVECTOR V2 = XMLoadFloat4(&v2);
		const XMVECTOR X  = XMVectorDivide(V1, V2);
		XMStoreFloat4(&v1, X);
		return v1;
	}

	inline XMFLOAT4& operator/= (XMFLOAT4& v1, const float d) noexcept
	{
		const XMVECTOR V = XMLoadFloat4(&v1);
		const XMVECTOR X = XMVectorScale(V, 1.0f / d);
		XMFLOAT4 v;
		XMStoreFloat4(&v, X);
		return v;
	}

	inline XMMATRIX XM_CALLCONV XMMatrixWorld(
		const XMFLOAT3& position, const XMFLOAT3& forward, const XMFLOAT3& up
	) noexcept
	{
		const XMVECTOR zaxis = XMVector3Normalize(XMLoadFloat3(&forward));
		XMVECTOR yaxis = XMLoadFloat3(&up);
		const XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
		yaxis = XMVector3Cross(zaxis, xaxis);

		XMMATRIX R = XMMatrixIdentity();
		R.r[0] = xaxis;
		R.r[1] = yaxis;
		R.r[2] = zaxis;
		R.r[3] = XMLoadFloat3(&position);

		return R;
	}

	inline XMMATRIX XM_CALLCONV XMMatrixLookAt(
		const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& up
	) noexcept
	{
		const XMFLOAT3 forward = target - position;
		return XMMatrixWorld(position, forward, up);
	}

	//inline XMMATRIX& operator* (const XMMATRIX& M1, const XMMATRIX& M2) noexcept
	//{
	//	XMMATRIX X = XMMatrixMultiply(M1, M2);
	//	return X;
	//}

	//inline XMMATRIX& operator* (const DirectX::SimpleMath::Matrix& M1, const XMMATRIX& M2) noexcept
	//{
	//	const XMMATRIX m1 = XMLoadFloat4x4(&M1);
	//	XMMATRIX X = XMMatrixMultiply(m1, M2);
	//	return X;
	//}

	//inline XMMATRIX& operator* (const XMMATRIX& M1, const DirectX::SimpleMath::Matrix& M2) noexcept
	//{
	//	const XMMATRIX m2 = XMLoadFloat4x4(&M2);
	//	XMMATRIX X = XMMatrixMultiply(M1, m2);
	//	return X;
	//}
}

namespace DirectX {
	inline XMFLOAT3 XMFloat3Normalize(XMFLOAT3 v1) noexcept
	{
		XMFLOAT3 v;
		XMStoreFloat3(&v, XMVector3Normalize(XMLoadFloat3(&v1)));
		return v;
	}

	inline XMFLOAT2 XMFloat2Normalize(XMFLOAT2 v1) noexcept
	{
		XMFLOAT2 v;
		XMStoreFloat2(&v, XMVector2Normalize(XMLoadFloat2(&v1)));
		return v;
	}

	inline XMFLOAT4 XMFloat4Normalize(XMFLOAT4 v1) noexcept
	{
		XMFLOAT4 v;
		XMStoreFloat4(&v, XMVector4Normalize(XMLoadFloat4(&v1)));
		return v;
	}
}

#pragma warning(push)
#pragma warning(disable:4324 4820)	// C4324/4820: padding warnings
namespace HLSL {
	using uint   = unsigned int;
	using dword  = unsigned long;
	using matrix = XMMATRIX;

	__declspec(align(16)) struct float3 : public XMFLOAT3
	{
		float3() = default;

		float3(const float3&) = default;
		float3& operator=(const float3&) = default;

		float3(float3&&) = default;
		float3& operator=(float3&&) = default;

		constexpr float3(XMFLOAT3&& f) : XMFLOAT3(f.x, f.y, f.z) {}
		constexpr float3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
		explicit float3(_In_reads_(3) const float* pArray) : XMFLOAT3(pArray) {}
	};

	__declspec(align(16)) struct float4 : public XMFLOAT4
	{
		float4() = default;

		float4(const float4&) = default;
		float4& operator=(const float4&) = default;

		float4(float4&&) = default;
		float4& operator=(float4&&) = default;

		constexpr float4(XMFLOAT4&& f) : XMFLOAT4(f.x, f.y, f.z, f.w) {}
		constexpr float4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		explicit float4(_In_reads_(4) const float* pArray) : XMFLOAT4(pArray) {}
	};

	// 3x4 Column-major Matrix: 32 bit floating point components aligned on a 16 byte boundary
	__declspec(align(16)) struct float3x4 : public XMFLOAT3X4
	{
		float3x4() = default;

		float3x4(const float3x4&) = default;
		float3x4& operator=(const float3x4&) = default;

		float3x4(float3x4&&) = default;
		float3x4& operator=(float3x4&&) = default;

		constexpr float3x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23) :
			XMFLOAT3X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23) {}
		explicit float3x4(_In_reads_(12) const float* pArray) : XMFLOAT3X4(pArray) {}
		explicit float3x4(XMMATRIX&& m) : XMFLOAT3X4()
		{ XMStoreFloat3x4((XMFLOAT3X4*)this, m); }
	};
}
#pragma warning(pop)

namespace Mathf {
	constexpr float Epsilon = 1.192092896e-07F;
	constexpr float PI      = XM_PI;
	constexpr float Deg2Rad = (2.0f * XM_PI) / 360.0f;
	constexpr float Rad2Deg = 360.0f / (2.0f * XM_PI);
}
