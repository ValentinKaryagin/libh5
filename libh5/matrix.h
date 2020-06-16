#pragma once

#include "math_public.h"

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace h5
{
	struct matrix
		: public DirectX::XMFLOAT4X4
	{
		static const matrix identity;

		__forceinline matrix()
			: XMFLOAT4X4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f)
		{}

		__forceinline matrix(const matrix &other)
			: XMFLOAT4X4(other)
		{}

		__forceinline matrix(matrix &&other) noexcept
			: XMFLOAT4X4(std::move(other))
		{}

		__forceinline matrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: XMFLOAT4X4(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33)
		{}

		__forceinline void look_at(const vector3 &eye, const vector3 &target, const vector3 &up)
		{			
			DirectX::XMStoreFloat4x4(
				this,
				DirectX::XMMatrixLookAtLH(
					DirectX::XMLoadFloat3(&eye),
					DirectX::XMLoadFloat3(&target),
					DirectX::XMLoadFloat3(&up)));
		}

		__forceinline void perspective(float fov, float aspect_ratio, float near_plane_distance, float far_plane_distance)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixPerspectiveFovLH(fov, aspect_ratio, near_plane_distance, far_plane_distance));
		}

		__forceinline void translation(const vector3 &translation)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&translation)));
		}

		__forceinline void rotation(const quat &rotation)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation)));
		}

		__forceinline void scaling(const vector3 &scaling)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scaling)));
		}		

		__forceinline void add_translation(const vector3 &translation)
		{
			DirectX::XMStoreFloat4x4(
				this,
				DirectX::XMMatrixMultiply(
					DirectX::XMLoadFloat4x4(this),
					DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&translation))
				)
			);
		}

		__forceinline void add_rotation(const quat &rotation)
		{
			DirectX::XMStoreFloat4x4(
				this,
				DirectX::XMMatrixMultiply(
					DirectX::XMLoadFloat4x4(this),
					DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&rotation))
				)
			);
		}

		__forceinline void add_scaling(const vector3 &scaling)
		{
			DirectX::XMStoreFloat4x4(
				this,
				DirectX::XMMatrixMultiply(
					DirectX::XMLoadFloat4x4(this),
					DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scaling))
				)
			);
		}

		__forceinline void transpose()
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixTranspose(XMLoadFloat4x4(this)));
		}

		__forceinline matrix get_transposed() const
		{
			matrix result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(this)));
			return result;
		}

		__forceinline bool operator==(const matrix &m) const
		{
			return (DirectX::XMVector4Equal(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_11)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._11)))
				&& DirectX::XMVector4Equal(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_21)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._21)))
				&& DirectX::XMVector4Equal(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_31)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._31)))
				&& DirectX::XMVector4Equal(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_41)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._41)))) != 0;
		}

		__forceinline bool operator!=(const matrix &m) const
		{
			return (DirectX::XMVector4NotEqual(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_11)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._11)))
				&& DirectX::XMVector4NotEqual(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_21)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._21)))
				&& DirectX::XMVector4NotEqual(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_31)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._31)))
				&& DirectX::XMVector4NotEqual(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_41)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._41)))) != 0;
		}

		__forceinline matrix &operator=(const matrix &other)
		{
			m[0][0] = other.m[0][0]; m[0][1] = other.m[0][1]; m[0][2] = other.m[0][2]; m[0][3] = other.m[0][3];
			m[1][0] = other.m[1][0]; m[1][1] = other.m[1][1]; m[1][2] = other.m[1][2]; m[1][3] = other.m[1][3];
			m[2][0] = other.m[2][0]; m[2][1] = other.m[2][1]; m[2][2] = other.m[2][2]; m[2][3] = other.m[2][3];
			m[3][0] = other.m[3][0]; m[3][1] = other.m[3][1]; m[3][2] = other.m[3][2]; m[3][3] = other.m[3][3];
			return *this;
		}

		__forceinline matrix &operator+=(const matrix &m)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._11))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._21))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._31))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._41))));
			return *this;
		}

		__forceinline matrix &operator-=(const matrix &M)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11), DirectX::XMVectorSubtract(XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11)), XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&M._11))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21), DirectX::XMVectorSubtract(XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21)), XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&M._21))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31), DirectX::XMVectorSubtract(XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31)), XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&M._31))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41), DirectX::XMVectorSubtract(XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41)), XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&M._41))));
			return *this;
		}

		__forceinline matrix &operator*=(const matrix &m)
		{
			XMStoreFloat4x4(this, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(this), DirectX::XMLoadFloat4x4(&m)));
			return *this;
		}

		__forceinline matrix &operator*=(float scalar)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11)), scalar));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21)), scalar));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31)), scalar));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41)), scalar));
			return *this;
		}

		__forceinline matrix &operator/=(float scalar)
		{
			assert(scalar != 0.0f);
			const float rs = 1.0f / scalar;
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11)), rs));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21)), rs));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31)), rs));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41)), rs));
			return *this;
		}

		__forceinline matrix &operator/=(const matrix &m)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&_11), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_11)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._11))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&_21), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_21)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._21))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&_31), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_31)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._31))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&_41), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&_41)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._41))));
			return *this;
		}

		__forceinline matrix operator-() const
		{
			matrix result;
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._11), DirectX::XMVectorNegate(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_11))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._21), DirectX::XMVectorNegate(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_21))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._31), DirectX::XMVectorNegate(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_31))));
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._41), DirectX::XMVectorNegate(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&_41))));
			return result;
		}
	};

	__forceinline matrix operator+(const matrix &m1, const matrix &m2)
	{
		matrix result;
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._11), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._11)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._11))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._21), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._21)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._21))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._31), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._31)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._31))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._41), DirectX::XMVectorAdd(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._41)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._41))));
		return result;
	}

	__forceinline matrix operator-(const matrix &m1, const matrix &m2)
	{
		matrix result;
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._11), DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._11)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._11))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._21), DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._21)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._21))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._31), DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._31)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._31))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._41), DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._41)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._41))));
		return result;
	}

	__forceinline matrix operator*(const matrix &m1, const matrix &m2)
	{
		matrix result;
		XMStoreFloat4x4(&result, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&m1), DirectX::XMLoadFloat4x4(&m2)));
		return result;
	}

	__forceinline matrix operator*(const matrix &m, float scalar)
	{
		matrix result;
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._11), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._11)), scalar));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._21), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._21)), scalar));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._31), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._31)), scalar));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._41), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._41)), scalar));
		return result;
	}

	__forceinline matrix operator/(const matrix &m, float scalar)
	{
		assert(scalar != 0.0f);
		float rs = 1.0f / scalar;
		matrix result;
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._11), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._11)), rs));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._21), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._21)), rs));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._31), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._31)), rs));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._41), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._41)), rs));
		return result;
	}

	__forceinline matrix operator/(const matrix &m1, const matrix &m2)
	{
		matrix result;
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._11), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._11)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._11))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._21), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._21)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._21))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._31), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._31)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._31))));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._41), DirectX::XMVectorDivide(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m1._41)), DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m2._41))));
		return result;
	}

	__forceinline matrix operator*(float scalar, const matrix &m)
	{
		matrix result;
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._11), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._11)), scalar));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._21), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._21)), scalar));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._31), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._31)), scalar));
		DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(&result._41), DirectX::XMVectorScale(DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(&m._41)), scalar));
		return result;
	}

	struct translation_matrix
		: public matrix
	{
		__forceinline translation_matrix()
			: matrix()
		{}

		__forceinline translation_matrix(const vector3 &translation)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&translation)));
		}
	};

	struct rotation_matrix
		: public matrix
	{
		__forceinline rotation_matrix()
			: matrix()
		{}

		__forceinline rotation_matrix(const quat &rotation)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation)));
		}
	};

	struct scaling_matrix
		: public matrix
	{
		__forceinline scaling_matrix()
			: matrix()
		{}

		__forceinline scaling_matrix(const vector3 &scaling)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scaling)));
		}
	};

	struct view_matrix
		: public matrix
	{
		__forceinline view_matrix()
			: matrix()
		{}

		__forceinline view_matrix(const vector3 &eye, const vector3 &target, const vector3 &up)
		{
			DirectX::XMStoreFloat4x4(
				this,
				DirectX::XMMatrixLookAtLH(
					DirectX::XMLoadFloat3(&eye),
					DirectX::XMLoadFloat3(&target),
					DirectX::XMLoadFloat3(&up)));
		}
	};

	struct projection_matrix
		: public matrix
	{
		__forceinline projection_matrix()
			: matrix()
		{}

		__forceinline projection_matrix(float fov, float aspect_ratio, float near_plane_distance, float far_plane_distance)
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixPerspectiveFovLH(fov, aspect_ratio, near_plane_distance, far_plane_distance));
		}
	};
}
