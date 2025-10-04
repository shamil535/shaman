#pragma once

#include "includes.h"
#include "vector.h"

#define TORAD(x) ((x) * 0.01745329251f)

namespace math
{
	static inline D3DMATRIX MatrixMultiplication(const D3DMATRIX p_m1, const D3DMATRIX p_m2)
	{
		D3DMATRIX p_out;
		p_out._11 = p_m1._11 * p_m2._11 + p_m1._12 * p_m2._21 + p_m1._13 * p_m2._31 + p_m1._14 * p_m2._41;
		p_out._12 = p_m1._11 * p_m2._12 + p_m1._12 * p_m2._22 + p_m1._13 * p_m2._32 + p_m1._14 * p_m2._42;
		p_out._13 = p_m1._11 * p_m2._13 + p_m1._12 * p_m2._23 + p_m1._13 * p_m2._33 + p_m1._14 * p_m2._43;
		p_out._14 = p_m1._11 * p_m2._14 + p_m1._12 * p_m2._24 + p_m1._13 * p_m2._34 + p_m1._14 * p_m2._44;
		p_out._21 = p_m1._21 * p_m2._11 + p_m1._22 * p_m2._21 + p_m1._23 * p_m2._31 + p_m1._24 * p_m2._41;
		p_out._22 = p_m1._21 * p_m2._12 + p_m1._22 * p_m2._22 + p_m1._23 * p_m2._32 + p_m1._24 * p_m2._42;
		p_out._23 = p_m1._21 * p_m2._13 + p_m1._22 * p_m2._23 + p_m1._23 * p_m2._33 + p_m1._24 * p_m2._43;
		p_out._24 = p_m1._21 * p_m2._14 + p_m1._22 * p_m2._24 + p_m1._23 * p_m2._34 + p_m1._24 * p_m2._44;
		p_out._31 = p_m1._31 * p_m2._11 + p_m1._32 * p_m2._21 + p_m1._33 * p_m2._31 + p_m1._34 * p_m2._41;
		p_out._32 = p_m1._31 * p_m2._12 + p_m1._32 * p_m2._22 + p_m1._33 * p_m2._32 + p_m1._34 * p_m2._42;
		p_out._33 = p_m1._31 * p_m2._13 + p_m1._32 * p_m2._23 + p_m1._33 * p_m2._33 + p_m1._34 * p_m2._43;
		p_out._34 = p_m1._31 * p_m2._14 + p_m1._32 * p_m2._24 + p_m1._33 * p_m2._34 + p_m1._34 * p_m2._44;
		p_out._41 = p_m1._41 * p_m2._11 + p_m1._42 * p_m2._21 + p_m1._43 * p_m2._31 + p_m1._44 * p_m2._41;
		p_out._42 = p_m1._41 * p_m2._12 + p_m1._42 * p_m2._22 + p_m1._43 * p_m2._32 + p_m1._44 * p_m2._42;
		p_out._43 = p_m1._41 * p_m2._13 + p_m1._42 * p_m2._23 + p_m1._43 * p_m2._33 + p_m1._44 * p_m2._43;
		p_out._44 = p_m1._41 * p_m2._14 + p_m1._42 * p_m2._24 + p_m1._43 * p_m2._34 + p_m1._44 * p_m2._44;
		return p_out;
	}

	static inline D3DXMATRIX Matrix(const FVector rot, const FVector origin = FVector(0, 0, 0))
	{
		const auto rad_pitch = (rot.X * static_cast<float>(PI) / 180.f);
		const auto rad_yaw = (rot.Y * static_cast<float>(PI) / 180.f);
		const auto rad_roll = (rot.Z * static_cast<float>(PI) / 180.f);

		const auto sp = sinf(rad_pitch);
		const auto cp = cosf(rad_pitch);
		const auto sy = sinf(rad_yaw);
		const auto cy = cosf(rad_yaw);
		const auto sr = sinf(rad_roll);
		const auto cr = cosf(rad_roll);

		D3DMATRIX matrix;
		matrix.m[0][0] = cp * cy;
		matrix.m[0][1] = cp * sy;
		matrix.m[0][2] = sp;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = sr * sp * cy - cr * sy;
		matrix.m[1][1] = sr * sp * sy + cr * cy;
		matrix.m[1][2] = -sr * cp;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(cr * sp * cy + sr * sy);
		matrix.m[2][1] = cy * sr - cr * sp * sy;
		matrix.m[2][2] = cr * cp;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.X;
		matrix.m[3][1] = origin.Y;
		matrix.m[3][2] = origin.Z;
		matrix.m[3][3] = 1.f;
		return matrix;
	}

	static float GetDistance3D(FVector src, FVector dst)
	{
		return sqrt(pow(dst.X - src.X, 2) + pow(dst.Y - src.Y, 2) + pow(dst.Z - src.Z, 2));
	}

	static float GetDistance2D(FVector2D src, FVector2D dst)
	{
		return sqrt(pow(dst.X - src.X, 2) + pow(dst.Y - src.Y, 2));
	}

	static FVector TransformVec(FVector src, FVector ang, float dist)
	{
		FVector v;
		v.X = src.X + (cosf(TORAD(ang.Y)) * dist);
		v.Y = src.Y + (sinf(TORAD(ang.Y)) * dist);
		v.Z = src.Z + (tanf(TORAD(ang.X)) * dist);
		return v;
	}
}