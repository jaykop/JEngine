#include <macros.hpp>
#include <cmath>
#include <mat4.hpp>
#include <vec3.hpp>

mat4 mat4::translate(const vec3& vec)
{
	mat4 toReturn;

	toReturn.m[0][3] = vec.x;
	toReturn.m[1][3] = vec.y;
	toReturn.m[2][3] = vec.z;

	return toReturn;
}

mat4 mat4::scale(const vec3& vec)
{
	mat4 toReturn;

	toReturn.m[0][0] = vec.x;
	toReturn.m[1][1] = vec.y;
	toReturn.m[2][2] = vec.z;
	toReturn.m[3][3] = 1.f;

	return toReturn;
}

mat4 mat4::rotate_x(float radian)
{
	mat4 rotation;
	float c = std::cosf(radian);
	float s = std::sinf(radian);

	rotation.m[0][0] = 1.f;
	rotation.m[1][1] = c;
	rotation.m[1][2] = -s;
	rotation.m[2][1] = s;
	rotation.m[2][2] = c;
	rotation.m[3][3] = 1.f;

	return rotation;
}

mat4 mat4::rotate_y(float radian)
{
	mat4 rotation;
	float c = std::cosf(radian);
	float s = std::sinf(radian);

	rotation.m[0][0] = c;
	rotation.m[1][1] = 1.f;
	rotation.m[0][2] = s;
	rotation.m[2][0] = -s;
	rotation.m[2][2] = c;
	rotation.m[3][3] = 1.f;

	return rotation;
}

mat4 mat4::rotate_z(float radian)
{
	mat4 rotation;
	float c = std::cosf(radian);
	float s = std::sinf(radian);

	rotation.m[0][0] = c;
	rotation.m[0][1] = -s;
	rotation.m[1][0] = s;
	rotation.m[1][1] = c;
	rotation.m[2][2] = 1.f;
	rotation.m[3][3] = 1.f;

	return rotation;
}

mat4 mat4::rotate(float radian, const vec3& vec)
{
	mat4 toReturn;

	float cosine = cos(radian);
	float sine = sin(radian);

	vec3 norm = vec.normalized();
	vec3 offset = norm * (1.f - cosine);

	toReturn.m[0][0] = offset.x * norm.x + cosine;
	toReturn.m[1][0] = offset.x * norm.y + norm.z * sine;
	toReturn.m[2][0] = offset.x * norm.z - norm.y * sine;

	toReturn.m[0][1] = offset.x * norm.y - norm.z * sine;
	toReturn.m[1][1] = offset.y * norm.y + cosine;
	toReturn.m[2][1] = offset.y * norm.z + norm.x * sine;

	toReturn.m[0][2] = offset.x * norm.z + norm.y * sine;
	toReturn.m[1][2] = offset.y * norm.z - norm.x * sine;
	toReturn.m[2][2] = offset.z * norm.z + cosine;

	toReturn.m[3][3] = 1.f;

	return toReturn;
}

mat4 mat4::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float distance = 1.f / std::tanf(fovy * .5f);

	mat4 toReturn;

	toReturn.m[0][0] = distance / aspect;
	toReturn.m[1][1] = distance;
	toReturn.m[2][2] = (zFar + zNear) / (zNear - zFar);

	toReturn.m[2][3] = (2 * zFar * zNear) / (zNear - zFar);
	toReturn.m[3][2] = -1.f;

	return toReturn;
}

mat4 mat4::orthogonal(float left, float right, float bottom, float top, float zNear, float zFar)
{
	mat4 toReturn;

	toReturn.m[0][0] = 2.f / (right - left);
	toReturn.m[1][1] = 2.f / (top - bottom);
	toReturn.m[2][2] = 2.f / (zNear - zFar);
	toReturn.m[0][3] = (right + left) / (left - right);
	toReturn.m[1][3] = (top + bottom) / (bottom - top);
	toReturn.m[2][3] = (zFar + zNear) / (zNear - zFar);
	toReturn.m[3][3] = 1.f;

	return toReturn;
}

mat4 mat4::orthogonal(float left, float right, float bottom, float top)
{
	mat4 toReturn;

	toReturn.m[0][0] = 2.f / (right - left);
	toReturn.m[1][1] = 2.f / (top - bottom);
	toReturn.m[2][2] = 1.f;
	toReturn.m[0][3] = (right + left) / (left - right);
	toReturn.m[1][3] = (top + bottom) / (bottom - top);
	toReturn.m[3][3] = 1.f;
	return toReturn;
}

mat4 mat4::look_at(const vec3& eye, const vec3& target, const vec3& up)
{
	vec3 l, u, r;

	if (eye == target)
		jeDebugPrint("!mat4 - LookAt's position and target are identical.\n");

	l = (eye - target).normalized();
	u = up;
	r = up.cross(l);
	u = l.cross(r);

	mat4 toReturn;

	toReturn.m[0][0] = r.x;
	toReturn.m[0][1] = r.y;
	toReturn.m[0][2] = r.z;
	toReturn.m[0][3] = (-r).dot(eye);

	toReturn.m[1][0] = u.x;
	toReturn.m[1][1] = u.y;
	toReturn.m[1][2] = u.z;
	toReturn.m[1][3] = (-u).dot(eye);

	toReturn.m[2][0] = l.x;
	toReturn.m[2][1] = l.y;
	toReturn.m[2][2] = l.z;
	toReturn.m[2][3] = (-l).dot(eye);

	toReturn.m[3][3] = 1.f;

	return toReturn;
}