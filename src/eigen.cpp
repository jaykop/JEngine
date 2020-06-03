#include <eigen.hpp>
#include <vec3.hpp>

unsigned eigen::num_jacobi_iterations = 50;

void eigen::jacobi(mat3& a, mat3& v)
{
	// init v
	v = mat3::identity;
	float prev_off = 0.0f;

	for (unsigned n = 0; n < num_jacobi_iterations; ++n)
	{
		int p = 0;
		int q = 1;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (i == j)
					continue;

				if (std::abs(a.m[i][j]) > abs(a.m[p][q]))
				{
					p = i;
					q = j;
				}
			}
		}

		float c, s;
		mat3 J = mat3::identity;

		// compute jacobi rotation matrix J(p, q, theta)
		symmetric_schur(a, p, q, c, s);

		J.m[p][p] = c;
		J.m[p][q] = s;
		J.m[q][p] = -s;
		J.m[q][q] = c;

		// cumulate roations
		v = v * J;

		a = (J.transposed() * a) * J;

		float off = 0.0f;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (i == j)
					continue;
				off += a.m[i][j] * a.m[i][j];
			}
		}

		// stop when not decreasing
		if (n > 2 && off >= prev_off)
			return;

		prev_off = off;
	}
}

void eigen::symmetric_schur(mat3& a, int p, int q, float& c, float& s)
{
	if (abs(a.m[p][q]) > 0.0001f)
	{
		float r = (a.m[q][q] - a.m[p][p]) / (2.0f * a.m[p][q]);
		float t;
		if (r >= 0.0f)
			t = 1.0f / (r + sqrt(1.0f + r * r));
		else
			t = -1.0f / (-r + sqrt(1.0f + r * r));
		c = 1.0f / sqrt(1.0f + t * t);
		s = t * c;
	}
	else
	{
		c = 1.0f;
		s = 0.0f;
	}
}

static void vec_swap(vec3& lhs, vec3& rhs)
{
	vec3 temp = lhs;
	lhs = rhs;
	rhs = temp;
}

void eigen::eigen_vectors(const mat3& covariance,
	vec3& vx, vec3& vy, vec3& vz)
{
	mat3 eigen_values(covariance);
	mat3 eigen_vecs;

	eigen::jacobi(eigen_values, eigen_vecs);

	float values[3] = { eigen_values.m00, eigen_values.m11, eigen_values.m22 };
	vec3 vecs[3] = {
					  vec3(eigen_vecs.m00, eigen_vecs.m10, eigen_vecs.m20),
					  vec3(eigen_vecs.m01, eigen_vecs.m11, eigen_vecs.m21),
					  vec3(eigen_vecs.m02, eigen_vecs.m12, eigen_vecs.m22)
	};

	if (values[0] < values[1])
	{
		std::swap(values[0], values[1]);
		vec_swap(vecs[0], vecs[1]);
	}
	if (values[0] < values[2])
	{
		std::swap(values[0], values[2]);
		vec_swap(vecs[0], vecs[2]);
	}
	if (values[1] < values[2])
	{
		std::swap(values[1], values[2]);
		vec_swap(vecs[1], vecs[2]);
	}

	vx = vecs[0];
	vy = vecs[1];
	vz = vecs[2];
}