#pragma once

extern float DegreesToRadians;

enum Result
{
	BSP_FILE_INVALID,
	BSP_VERTICES_INVALID,
	BSP_TEXTURES_INVALID,
	BSP_LIGHTMAPS_INVALID,
	BSP_INDICES_INVALID,
	BSP_HEADER_INVALID,
	BSP_FACES_INVALID,
	
	OPENGL_SPF_FAILED,
	OPENGL_CPF_FAILED,
	OPENGL_MAKE_FAILED,

	ALL_OK
};

class Vec3
{
public:
	Vec3() {}

	Vec3(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	Vec3 operator+(Vec3 Vector) { return Vec3(Vector.x + x, Vector.y + y, Vector.z + z); }
	Vec3 operator-(Vec3 Vector) { return Vec3(x - Vector.x, y - Vector.y, z - Vector.z); }
	Vec3 operator*(float Num) { return Vec3(x * Num, y * Num, z * Num); }
	Vec3 operator/(float Num) { return Vec3(x / Num, y / Num, z / Num); }

	float x, y, z;
};

class Vec2
{
public:
	Vec2() {}

	Vec2(float X, float Y) { x = X; y = Y; }
	Vec2 operator+(Vec2 Vector) { return Vec2(Vector.x + x, Vector.y + y); }
	Vec2 operator-(Vec2 Vector) { return Vec2(x - Vector.x, y - Vector.y); }
	Vec2 operator*(float Num) { return Vec2(x * Num, y * Num); }
	Vec2 operator/(float Num) { return Vec2(x / Num, y / Num); }

	float x, y;
};