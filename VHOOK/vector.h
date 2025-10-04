#pragma once
#include "includes.h"

#ifndef	Q_MATH_H
#define	Q_MATH_H

//Globals
#define M_PI 3.14159265358979323846f
#define ANG_CLIP( ang )		if( ang > 180.0f ) { ang -= 360.0f; } else if( ang <- 180.0f ) { ang += 360.0f; }
#define	GENTITYNUM_BITS		10
#define	MAX_GENTITIES		( 1 << GENTITYNUM_BITS )

struct Color;
struct FVector2D;
struct FVector;
struct FRotator;

struct Color
{
	float R;
	float G;
	float B;
	float A;

	Color();
	Color(int _R, int _G, int _B, int _A);
	Color(float _R, float _G, float _B);

	float GetRed();
	float GetGreen();
	float GetBlue();
	float GetAlpha();

	void SetRed(float _R);
	void SetGreen(float _G);
	void SetBlue(float _B);
	void SetAlpha(float _A);

	void SetRed(int _R);
	void SetGreen(int _G);
	void SetBlue(int _B);
	void SetAlpha(int _A);

	Color FromHSB(float hue, float saturation, float brightness);
};


struct FRotator
{
	float Pitch;
	float Yaw;
	float Roll;

	FRotator();
	FRotator(float _Pitch, float _Yaw, float _Roll);

	float GetPitch();
	float GetYaw();
	float GetRoll();
	float Size();
	void Normalize();
	//FRotator FRotator::Normalize180 ();

	FRotator operator+ (const FRotator& A);
	FRotator operator- (const FRotator& A);
	FRotator operator/ (float A);
	FRotator operator/= (float A);
	FRotator operator* (float A);

	void GetVectors(FVector* Forward, FVector* Right, FVector* Up);

	FVector ToVector();
	float Length();
};

struct FVector2D
{
	float X;
	float Y;

	FVector2D();
	FVector2D(float _X, float _Y);

	FVector2D operator+ (const FVector2D& A);
	FVector2D operator+ (const float A);
	FVector2D operator+= (const FVector2D& A);
	FVector2D operator+= (const float A);
	FVector2D operator- (const FVector2D& A);
	FVector2D operator- (const float A);
	FVector2D operator-= (const FVector2D& A);
	FVector2D operator-= (const float A);
	FVector2D operator* (const FVector2D& A);
	FVector2D operator* (const float A);
	FVector2D operator*= (const FVector2D& A);
	FVector2D operator*= (const float A);
	FVector2D operator/ (const FVector2D& A);
	FVector2D operator/ (const float A);
	FVector2D operator/= (const FVector2D& A);
	FVector2D operator/= (const float A);

	bool operator== (const FVector2D& A);
	bool operator!= (const FVector2D& A);

	float Distance(FVector2D v);
	float GetX();
	float GetY();

	float Size();
	bool IsValid();

};


struct FVector
{
	float X;
	float Y;
	float Z;

	FVector();
	FVector(float _X, float _Y, float _Z);

	FVector operator+ (const FVector& A);
	FVector operator+ (const float A);
	FVector operator+= (const FVector& A);
	FVector operator+= (const float A);
	FVector operator- (const FVector& A);
	FVector operator- (const float A);
	FVector operator-= (const FVector& A);
	FVector operator-= (const float A);
	FVector operator* (const FVector& A);
	FVector operator* (const float A);
	FVector operator*= (const FVector& A);
	FVector operator*= (const float A);
	FVector operator/ (const FVector& A);
	FVector operator/ (const float A);
	FVector operator/= (const FVector& A);
	FVector operator/= (const float A);

	bool operator== (const FVector& A);
	bool operator!= (const FVector& A);

	FVector GetSafeNormal();
	FVector GetDirectionUnitVector(FVector To);

	float GetX();
	float GetY();
	float GetZ();

	float Size();
	//float Dot (const FVector V1,const FVector V2);
	FRotator ToRotator();

	/*char* toString ()
	{
		static char buffer[ 256 ];
		ZeroMemory( buffer, 256 );

		sprintf( buffer, "vector( %.2f, %.2f, %.2f )", this->X, this->Y, this->Z );
		return buffer;
	};*/
	float		Distance(FVector v);
	float		Dot(FVector pStart, FVector pEnd);
	bool		InAngle(FVector pStart, FVector pEnd, FVector pRotation, int nAngle);
	int			round(float fValue);
	FVector2D	GetAngleToOrigin(FRotator ViewAngles, FVector vec_Start, FVector vec_End);
	//static	float		GetDistance(centity_t* pEntity, FVector Location);
	//static	bool		WorldToScreen(FVector vec_Location, FVector& vec_Return);

	bool IsValid();
};

struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};

struct FTransform
{
	FQuat rot;
	FVector translation;
	char pad[4];
	FVector scale;
	char pad1[4];

	D3DMATRIX ToMatrixWithScale() const
	{
		D3DMATRIX m;
		m._41 = translation.X;
		m._42 = translation.Y;
		m._43 = translation.Z;

		const auto x2 = rot.x + rot.x;
		const auto y2 = rot.y + rot.y;
		const auto z2 = rot.z + rot.z;

		const auto xx2 = rot.x * x2;
		const auto yy2 = rot.y * y2;
		const auto zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.X;
		m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.Z;

		const auto yz2 = rot.y * z2;
		const auto wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.Z;
		m._23 = (yz2 + wx2) * scale.Y;

		const auto xy2 = rot.x * y2;
		const auto wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.Y;
		m._12 = (xy2 + wz2) * scale.X;

		const auto xz2 = rot.x * z2;
		const auto wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.Z;
		m._13 = (xz2 - wy2) * scale.X;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};
extern FVector FVec;
#endif

struct Vector2 {
	float x, y;

	Vector2 operator-(const Vector2& other) const {
		return { x - other.x, y - other.y };
	}

	float Length() const {
		return std::sqrt(x * x + y * y);
	}
};

class Vector3
{
public:
	float x, y, z;

	Vector3() {}
	Vector3(const float x, const float y, const float z) :x(x), y(y), z(z) {}

	Vector3 operator + (const Vector3& rhs)const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs)const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs)const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs)const { return Vector3(x / rhs, y / rhs, z / rhs); }

	Vector3& operator +=(const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -=(const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *=(const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /=(const float& rhs) { return *this = *this / rhs; }

	float Length()const { return sqrtf(x * x + y * y + z * z); }
	Vector3 Normalize()const { return *this * (1 / Length()); }

	float Distance(const Vector3& rhs)const {
		return sqrtf(
			(x - rhs.x) * (x - rhs.x) +
			(y - rhs.y) * (y - rhs.y) +
			(z - rhs.z) * (z - rhs.z)
		);
	}

	float dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }

	// Função estática para criar um Vector3 a partir de um __m128 
	static Vector3 FromM128(__m128 v) {
		Vector3 vec;
		alignas(16) float temp[4];
		_mm_store_ps(temp, v);
		vec.x = temp[0];
		vec.y = temp[1];
		vec.z = temp[2];
		return vec;
	}
};

struct Rotation {
	float yaw, pitch, roll;
};

inline Vector3 get_forward_vector(const Rotation& rot) {
	float length = std::sqrt(rot.yaw * rot.yaw + rot.pitch * rot.pitch + rot.roll * rot.roll);
	if (length != 0) {
		return { rot.yaw / length, rot.pitch / length, rot.roll / length };
	}
	return { 0, 0, 0 };
}

struct Vector4
{
	float x, y, z, w;
};

using Vec3 = Vector3;
using vec3 = Vector3;
using Vec = Vector3;
using vec = Vector3;
using vec3_t = Vector3;
using vec4 = Vector4;
using vec2 = Vector2;

enum Bone
{
	Head,
	LeftToe,
	RightToe,
	LeftAnkle,
	RightAnkle,
	LeftHand,
	RightHand,
	Neck,
	Abdomen
};