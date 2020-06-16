#pragma once

#include "math_public.h"

#include "vector3.h"

namespace h5
{
	struct quat
		: public DirectX::XMFLOAT4
	{
		static const quat identity;

		__forceinline quat()
			: XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
		{}

		__forceinline quat(const quat &other)
			: XMFLOAT4(other)
		{}

		__forceinline quat(quat &&other) noexcept
			: XMFLOAT4(std::move(other))
		{}

		__forceinline quat(const vector3 &axis, float rad)
		{
			DirectX::XMStoreFloat4(this, DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&axis), rad));
		}

		__forceinline quat(float x, float y, float z, float w)
			: XMFLOAT4(x, y, z, w)
		{}

		__forceinline ~quat() {}

		__forceinline bool operator==(const quat &other) const
		{
			return DirectX::XMVector4Equal(XMLoadFloat4(this), XMLoadFloat4(&other));
		}

		__forceinline bool operator!=(const quat &other) const
		{
			return DirectX::XMVector4NotEqual(XMLoadFloat4(this), XMLoadFloat4(&other));
		}

		__forceinline quat &operator=(const quat &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		__forceinline quat &operator*=(const quat &other)
		{
			XMStoreFloat4(this, DirectX::XMQuaternionMultiply(XMLoadFloat4(&other), XMLoadFloat4(this)));
			return *this;
		}		
	};

	__forceinline quat operator*(const quat &q1, const quat &q2)
	{
		quat result;
		XMStoreFloat4(&result, DirectX::XMQuaternionMultiply(XMLoadFloat4(&q2), XMLoadFloat4(&q1)));
		return result;
	}
}
