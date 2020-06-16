#pragma once

#include "math_public.h"

namespace h5
{
	struct vector3
		: public DirectX::XMFLOAT3
	{
		static const vector3 zero, forward, right, up, identity;

		__forceinline vector3()
			: XMFLOAT3(0.0f, 0.0f, 0.0f)
		{}

		__forceinline vector3(const vector3 &other)
			: XMFLOAT3(other)
		{}

		__forceinline vector3(vector3 &&other) noexcept
			: XMFLOAT3(std::move(other))
		{}

		__forceinline vector3(float x, float y, float z)
			: XMFLOAT3(x, y, z)
		{}

		__forceinline ~vector3() {}

		__forceinline bool operator==(const vector3 &other) const
		{
			return DirectX::XMVector3Equal(XMLoadFloat3(this), XMLoadFloat3(&other));
		}

		__forceinline bool operator!=(const vector3 &other) const
		{
			return DirectX::XMVector3NotEqual(XMLoadFloat3(this), XMLoadFloat3(&other));
		}

		__forceinline vector3 &operator=(const vector3 &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		__forceinline vector3 &operator+=(const vector3 &other)
		{
			XMStoreFloat3(this, DirectX::XMVectorAdd(XMLoadFloat3(this), XMLoadFloat3(&other)));
			return *this;
		}

		__forceinline vector3 &operator-=(const vector3 &other)
		{
			XMStoreFloat3(this, DirectX::XMVectorSubtract(XMLoadFloat3(this), XMLoadFloat3(&other)));
			return *this;
		}

		__forceinline vector3 &operator*=(const vector3 &other)
		{
			XMStoreFloat3(this, DirectX::XMVectorMultiply(XMLoadFloat3(this), XMLoadFloat3(&other)));
			return *this;
		}

		__forceinline vector3 &operator*=(float scalar)
		{
			XMStoreFloat3(this, DirectX::XMVectorScale(XMLoadFloat3(this), scalar));
			return *this;
		}

		__forceinline vector3 &operator/=(float scalar)
		{
			assert(scalar != 0.0f);
			XMStoreFloat3(this, DirectX::XMVectorScale(XMLoadFloat3(this), 1.0f / scalar));
			return *this;
		}

		__forceinline vector3 operator+() const
		{
			return *this;
		}

		__forceinline vector3 operator-() const
		{
			return vector3(-x, -y, -z);
		}
	};

	__forceinline vector3 operator+(const vector3 &v1, const vector3 &v2)
	{
		vector3 result;
		XMStoreFloat3(&result, DirectX::XMVectorAdd(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
		return result;
	}

	__forceinline vector3 operator-(const vector3 &v1, const vector3 &v2)
	{
		vector3 result;
		XMStoreFloat3(&result, DirectX::XMVectorSubtract(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
		return result;
	}

	__forceinline vector3 operator*(const vector3 &v1, const vector3 &v2)
	{
		vector3 result;
		XMStoreFloat3(&result, DirectX::XMVectorMultiply(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
		return result;
	}

	__forceinline vector3 operator*(const vector3 &v, float scalar)
	{
		vector3 result;
		XMStoreFloat3(&result, DirectX::XMVectorScale(XMLoadFloat3(&v), scalar));
		return result;
	}

	__forceinline vector3 operator/(const vector3 &v1, const vector3 &v2)
	{
		vector3 result;
		XMStoreFloat3(&result, DirectX::XMVectorDivide(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
		return result;
	}

	__forceinline vector3 operator*(float scalar, const vector3 &v)
	{
		vector3 result;
		XMStoreFloat3(&result, DirectX::XMVectorScale(XMLoadFloat3(&v), scalar));
		return result;
	}
}
