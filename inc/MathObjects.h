#pragma once

#include <cmath>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() = default;
	Vector3(float _x, float _y = 0.0f, float _z = 0.0f) :
		x(_x),
		y(_y),
		z(_z)
	{}

	Vector3 operator+(const Vector3& v) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	Vector3 operator-(const Vector3& v) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	Vector3 operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	Vector3 operator/(float scalar) const {
		return { x / scalar, y / scalar, z / scalar };
	}

	Vector3 operator-() const {
		return { -x , -y , -z };
	}

	Vector3 cross(const Vector3& v) const {
		return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x };
	}

	Vector3 operator^(const Vector3& v) const {
		return cross(v);
	}

	float size() const {
		return sqrtf(x * x + y * y + z * z);
	}

	Vector3 normalize() const {
		float invLength = 1.0f / size();
		//float length = sqrt(x * x + y * y + z * z);
		return{ x * invLength,
				y * invLength,
				z * invLength };
	}

	float dot(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	float operator|(const Vector3& v) const {
		return dot(v);
	}

};

//Row Major
struct Matrix4 {

	Matrix4() = default;

	void LookAt(const Vector3& eyePos, const Vector3& targetPos, const Vector3& upDir) 
	{
		const Vector3 ZAxis = (eyePos - targetPos).normalize();
		const Vector3 XAxis = (upDir ^ ZAxis).normalize();
		const Vector3 YAxis = ZAxis ^ XAxis;

		m[0][0] = XAxis.x;
		m[1][0] = XAxis.y;
		m[2][0] = XAxis.z;

		m[0][1] = YAxis.x;
		m[1][1] = YAxis.y;
		m[2][1] = YAxis.z;

		m[0][2] = ZAxis.x;
		m[1][2] = ZAxis.y;
		m[2][2] = ZAxis.z;
		
		m[0][3] = 0.0f;
		m[1][3] = 0.0f;
		m[2][3] = 0.0f;

		m[3][0] = eyePos | XAxis;
		m[3][1] = eyePos | YAxis;
		m[3][2] = eyePos | ZAxis;
		m[3][3] = 1.0f;

	}

	void Perspective(float halfFOV, float widthScreen, float heightScreen, float MinZ, float MaxZ) 
	{
		float plane0[4] = { 1.0f / tanf(halfFOV),		0.0f,							0.0f,							0.0f};
		float plane1[4] = { 0.0f,						widthScreen / tanf(halfFOV),	0.0f,							0.0f};
		float plane2[4] = { 0.0f,						0.0f,							MaxZ / (MaxZ - MinZ),			1.0f };
		float plane3[4] = { 0.0f,						0.0f,							-MinZ * MaxZ / (MaxZ - MinZ),	0.0f };
	
		m[0][0] = plane0[0];
		m[0][1] = plane0[1];
		m[0][2] = plane0[2];
		m[0][3] = plane0[3];

		m[1][0] = plane1[0];
		m[1][1] = plane1[1];
		m[1][2] = plane1[2];
		m[1][3] = plane1[3];

		m[2][0] = plane2[0];
		m[2][1] = plane2[1];
		m[2][2] = plane2[2];
		m[2][3] = plane2[3];

		m[3][0] = plane3[0];
		m[3][1] = plane3[1];
		m[3][2] = plane3[2];
		m[3][3] = plane3[3];
	}

	void Transpose() 
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float temp = m[i][j];

				m[i][j] = m[j][i];
				m[j][i] = temp;

			}
		}
	}
	

	float m[4][4];
};

struct Camera {
	//TODO
	void SetLookAt(const Vector3& eyePos, const Vector3& targetPos);

	Vector3 position;
	Vector3 target;
	Vector3 up;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
};