#pragma once

#include "math_public.h"

namespace h5
{
	struct vector2
		: public DirectX::XMFLOAT2
	{
		static const vector2 zero, right, down;

		__forceinline vector2()
			: XMFLOAT2(0.0f, 0.0f)
		{}

		__forceinline vector2(const vector2 &other)
			: XMFLOAT2(other)
		{}

		__forceinline vector2(vector2 &&other) noexcept
			: XMFLOAT2(std::move(other))
		{}

		__forceinline vector2(float x, float y)
			: XMFLOAT2(x, y)
		{}

		__forceinline ~vector2() {}

		__forceinline bool operator==(const vector2 &other) const
		{
			return DirectX::XMVector2Equal(XMLoadFloat2(this), XMLoadFloat2(&other));
		}

		__forceinline bool operator!=(const vector2 &other) const
		{
			return DirectX::XMVector2NotEqual(XMLoadFloat2(this), XMLoadFloat2(&other));
		}

		__forceinline vector2 &operator=(const vector2 &other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}

		__forceinline vector2 &operator+=(const vector2 &other)
		{
			XMStoreFloat2(this, DirectX::XMVectorAdd(XMLoadFloat2(this), XMLoadFloat2(&other)));
			return *this;
		}

		__forceinline vector2 &operator-=(const vector2 &other)
		{
			XMStoreFloat2(this, DirectX::XMVectorSubtract(XMLoadFloat2(this), XMLoadFloat2(&other)));
			return *this;
		}

		__forceinline vector2 &operator*=(const vector2 &other)
		{
			XMStoreFloat2(this, DirectX::XMVectorMultiply(XMLoadFloat2(this), XMLoadFloat2(&other)));
			return *this;
		}

		__forceinline vector2 &operator*=(float scalar)
		{
			XMStoreFloat2(this, DirectX::XMVectorScale(XMLoadFloat2(this), scalar));
			return *this;
		}

		__forceinline vector2 &operator/=(float scalar)
		{
			assert(scalar != 0.0f);
			XMStoreFloat2(this, DirectX::XMVectorScale(XMLoadFloat2(this), 1.0f / scalar));
			return *this;
		}

		__forceinline vector2 operator+() const
		{
			return *this;
		}

		__forceinline vector2 operator-() const
		{
			return vector2(-x, -y);
		}
	};

	__forceinline vector2 operator+(const vector2 &v1, const vector2 &v2)
	{
		vector2 result;
		XMStoreFloat2(&result, DirectX::XMVectorAdd(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
		return result;
	}

	__forceinline vector2 operator-(const vector2 &v1, const vector2 &v2)
	{
		vector2 result;
		XMStoreFloat2(&result, DirectX::XMVectorSubtract(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
		return result;
	}

	__forceinline vector2 operator*(const vector2 &v1, const vector2 &v2)
	{
		vector2 result;
		XMStoreFloat2(&result, DirectX::XMVectorMultiply(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
		return result;
	}

	__forceinline vector2 operator*(const vector2 &v, float scalar)
	{
		vector2 result;
		XMStoreFloat2(&result, DirectX::XMVectorScale(XMLoadFloat2(&v), scalar));
		return result;
	}

	__forceinline vector2 operator/(const vector2 &v1, const vector2 &v2)
	{
		vector2 result;
		XMStoreFloat2(&result, DirectX::XMVectorDivide(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
		return result;
	}

	__forceinline vector2 operator*(float scalar, const vector2 &v)
	{
		vector2 result;
		XMStoreFloat2(&result, DirectX::XMVectorScale(XMLoadFloat2(&v), scalar));
		return result;
	}
}
