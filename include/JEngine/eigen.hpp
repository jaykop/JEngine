#pragma once
#include <mat3.hpp>

class eigen
{
public:
	// static members
	static unsigned num_jacobi_iterations;

	// public methods
	static void jacobi(mat3& a, mat3& v);
	static void symmetric_schur(mat3& a, int p, int q, float& c, float& s);
	static void eigen_vectors(const mat3& covariance,
		vec3& vx, vec3& vy, vec3& vz);

}; // class eigen
