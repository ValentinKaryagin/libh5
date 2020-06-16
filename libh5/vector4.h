#pragma once

#include "math_public.h"

namespace h5
{
	struct vector4
		: public DirectX::XMFLOAT4
	{
		static const vector4 zero;

		__forceinline vector4()
			: XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
		{}

		__forceinline vector4(const vector4 &other)
			: XMFLOAT4(other)
		{}

		__forceinline vector4(vector4 &&other) noexcept
			: XMFLOAT4(std::move(other))
		{}

		__forceinline vector4(float x, float y, float z, float w)
			: XMFLOAT4(x, y, z, w)
		{}

		__forceinline ~vector4() {}

		__forceinline bool operator==(const vector4 &other) const
		{
			DirectX::XMVECTOR v1 = XMLoadFloat4(this);
			DirectX::XMVECTOR v2 = XMLoadFloat4(&other);
			return DirectX::XMVector4Equal(v1, v2);
		}

		__forceinline bool operator!=(const vector4 &other) const
		{
			DirectX::XMVECTOR v1 = XMLoadFloat4(this);
			DirectX::XMVECTOR v2 = XMLoadFloat4(&other);
			return DirectX::XMVector4NotEqual(v1, v2);
		}

		__forceinline vector4 &operator=(const vector4 &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		__forceinline vector4 &operator+=(const vector4 &other)
		{
			DirectX::XMVECTOR v1 = XMLoadFloat4(this);
			DirectX::XMVECTOR v2 = XMLoadFloat4(&other);
			DirectX::XMVECTOR x = DirectX::XMVectorAdd(v1, v2);
			XMStoreFloat4(this, x);
			return *this;
		}

		__forceinline vector4 &operator-=(const vector4 &other)
		{
			DirectX::XMVECTOR v1 = XMLoadFloat4(this);
			DirectX::XMVECTOR v2 = XMLoadFloat4(&other);
			DirectX::XMVECTOR x = DirectX::XMVectorSubtract(v1, v2);
			XMStoreFloat4(this, x);
			return *this;
		}

		__forceinline vector4 &operator*=(const vector4 &other)
		{
			DirectX::XMVECTOR v1 = XMLoadFloat4(this);
			DirectX::XMVECTOR v2 = XMLoadFloat4(&other);
			DirectX::XMVECTOR x = DirectX::XMVectorMultiply(v1, v2);
			XMStoreFloat4(this, x);
			return *this;
		}

		__forceinline vector4 &operator*=(float scalar)
		{
			DirectX::XMVECTOR v1 = XMLoadFloat4(this);
			DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, scalar);
			XMStoreFloat4(this, X);
			return *this;
		}

		__forceinline vector4 &operator/=(float scalar)
		{
			assert(scalar != 0.0f);
			DirectX::XMVECTOR v1 = XMLoadFloat4(this);
			DirectX::XMVECTOR x = DirectX::XMVectorScale(v1, 1.0f / scalar);
			XMStoreFloat4(this, x);
			return *this;
		}

		__forceinline vector4 operator+() const
		{
			return *this;
		}

		__forceinline vector4 operator-() const
		{
			return vector4(-x, -y, -z, -w);
		}
	};

	__forceinline vector4 operator+(const vector4 &v1, const vector4 &v2)
	{
		DirectX::XMVECTOR _v1 = XMLoadFloat4(&v1);
		DirectX::XMVECTOR _v2 = XMLoadFloat4(&v2);
		DirectX::XMVECTOR x = DirectX::XMVectorAdd(_v1, _v2);
		vector4 result;
		XMStoreFloat4(&result, x);
		return result;
	}

	__forceinline vector4 operator-(const vector4 &v1, const vector4 &v2)
	{
		DirectX::XMVECTOR _v1 = XMLoadFloat4(&v1);
		DirectX::XMVECTOR _v2 = XMLoadFloat4(&v2);
		DirectX::XMVECTOR x = DirectX::XMVectorSubtract(_v1, _v2);
		vector4 result;
		XMStoreFloat4(&result, x);
		return result;
	}

	__forceinline vector4 operator*(const vector4 &v1, const vector4 &v2)
	{
		DirectX::XMVECTOR _v1 = XMLoadFloat4(&v1);
		DirectX::XMVECTOR _v2 = XMLoadFloat4(&v2);
		DirectX::XMVECTOR x = DirectX::XMVectorMultiply(_v1, _v2);
		vector4 result;
		XMStoreFloat4(&result, x);
		return result;
	}

	__forceinline vector4 operator*(const vector4 &v, float scalar)
	{
		DirectX::XMVECTOR v1 = XMLoadFloat4(&v);
		DirectX::XMVECTOR x = DirectX::XMVectorScale(v1, scalar);
		vector4 result;
		XMStoreFloat4(&result, x);
		return result;
	}

	__forceinline vector4 operator/(const vector4 &v1, const vector4 &v2)
	{
		DirectX::XMVECTOR _v1 = XMLoadFloat4(&v1);
		DirectX::XMVECTOR _v2 = XMLoadFloat4(&v2);
		DirectX::XMVECTOR x = DirectX::XMVectorDivide(_v1, _v2);
		vector4 result;
		XMStoreFloat4(&result, x);
		return result;
	}

	__forceinline vector4 operator*(float scalar, const vector4 &v)
	{
		DirectX::XMVECTOR v1 = XMLoadFloat4(&v);
		DirectX::XMVECTOR x = DirectX::XMVectorScale(v1, scalar);
		vector4 result;
		XMStoreFloat4(&result, x);
		return result;
	}
}
