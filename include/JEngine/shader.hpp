/******************************************************************************/
/*!
\file   shader.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/18(yy/mm/dd)

\description
Contains the definition of Shader class
*/
/******************************************************************************/
#pragma once

#include <glew.h>
#include <macros.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
#include <string>
#include <vector>

jeBegin

class Shader {

	friend class GLManager;
	friend class AssetManager;

	static std::vector<const char*> vsDirectory_;
	static std::vector<const char*> fsDirectory_;

public:

	void use();

	void set_int(const char* name, int number);
	void set_bool(const char* name, bool toggle);
	void set_enum(const char* name, int typr);
	void set_matrix(const char* name, const mat4& mat4);
	void set_uint(const char* name, unsigned uInt);
	void set_float(const char* name, float number);
	void set_vec3(const char* name, const vec3& vector);
	void set_vec4(const char* name, const vec4& vector);

private:

	enum class Type { VERTEX, GEOMETRY, PIXEL };

	friend class GLManager;
	friend class GraphicSystem;
	friend class AssetManager;

	void create_shader(const char* file_path, Type type);
	void combine_shaders();

	GLuint  programId_, vertexId_, fragmentId_, geometryId_;
	int	    infoLogLength_;
	GLint   result_;

	Shader();
	~Shader() {};
	Shader(Shader&&) = delete;
	Shader(const Shader&) = delete;
	Shader& operator=(Shader&&) = delete;
	Shader& operator=(const Shader&) = delete;

};

jeEnd