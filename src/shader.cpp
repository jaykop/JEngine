#include <shader.hpp>
#include <fstream>
#include <debug_renderer.hpp>

#include <mat4.hpp>
#include <vec4.hpp>
#include <vec2.hpp>

jeBegin

std::vector<const char*> Shader::vsDirectory_;
std::vector<const char*> Shader::fsDirectory_;

Shader::Shader() : programId_(0), vertexId_(0),
	fragmentId_(0), geometryId_(0),
	infoLogLength_(0), result_(GL_FALSE) {}

void Shader::create_shader(const char* file_path, Type type, unsigned shaderNumber)
{
	// Create the shader
	GLuint localShader = 0;

	switch (type) {

	case Type::VERTEX:
		vertexId_ = glCreateShader(GL_VERTEX_SHADER);
		localShader = vertexId_;
		break;
	case Type::PIXEL:
		fragmentId_ = glCreateShader(GL_FRAGMENT_SHADER);
		localShader = fragmentId_;
		break;
	case Type::GEOMETRY:
		geometryId_ = glCreateShader(GL_GEOMETRY_SHADER);
		localShader = geometryId_;
		break;
	default:
		jeDebugPrint("!Shader - No type for shader %s\n", file_path);
		break;
	}

	std::string shaderCode;
	std::ifstream shaderStream(file_path, std::ios::in);
	if (shaderStream.is_open()) {
		std::string Line = "";
		while (getline(shaderStream, Line))
			shaderCode += "\n" + Line;
		shaderStream.close();
	}
	
	char const* sourcePointer = shaderCode.c_str();
	int length = int(shaderCode.size());
	
	glShaderSource(localShader, 1, &sourcePointer, &length);
	glCompileShader(localShader);

	// Check shader
	glGetShaderiv(localShader, GL_COMPILE_STATUS, &result_);
	glGetShaderiv(localShader, GL_INFO_LOG_LENGTH, &infoLogLength_);

	if (infoLogLength_ > 0) {
		std::vector<char> ShaderErrorMessage(infoLogLength_ + int(1));
		glGetShaderInfoLog(localShader, infoLogLength_, nullptr, &ShaderErrorMessage[0]);
		jeDebugPrint("!Shader - %4s / %s\n", &ShaderErrorMessage[0], file_path);
	}
}

void Shader::combine_shaders(unsigned shaderNumber)
{
	programId_ = glCreateProgram();

	if (programId_ == 0)
		jeDebugPrint("!Shader - Shader %i couldn't get program id.\n", shaderNumber);

	else {

		// Combine several shaders into the program
		if (vertexId_)
			glAttachShader(programId_, vertexId_);
		if (fragmentId_)
			glAttachShader(programId_, fragmentId_);
		if (geometryId_)
			glAttachShader(programId_, geometryId_);

		glLinkProgram(programId_);

		//// Check the program
		//glGetProgramiv(programId_, GL_LINK_STATUS, &result_);
		//glGetShaderiv(programId_, GL_INFO_LOG_LENGTH, &infoLogLength_);

		//// Check if linked properly
		//if (infoLogLength_ > 0) {
		//	std::vector<char> ProgramErrorMessage(infoLogLength_ + 1);
		//	glGetShaderInfoLog(programId_, infoLogLength_, nullptr, &ProgramErrorMessage[0]);
		//	jeDebugPrint("!Shader - %4s / %i\n", &ProgramErrorMessage[0], shaderNumber);

		//}

		glUseProgram(programId_);	// Start using shade  program

		if (vertexId_) {
			glDetachShader(programId_, vertexId_);
			glDeleteShader(vertexId_);
		}

		if (fragmentId_) {
			glDetachShader(programId_, fragmentId_);
			glDeleteShader(fragmentId_);
		}

		if (geometryId_) {
			glDetachShader(programId_, geometryId_);
			glDeleteShader(geometryId_);
		}
	}
}

void Shader::set_bool(const char* name, bool toggle)
{
	glUniform1i(glGetUniformLocation(programId_, name), toggle);
}

void Shader::set_uint(const char* name, unsigned uInt)
{
	glUniform1ui(glGetUniformLocation(programId_, name), uInt);
}

void Shader::set_vec4(const char* name, const vec4& vector)
{
	glUniform4f(glGetUniformLocation(programId_, name),
		vector.x, vector.y, vector.z, vector.w);
}

void Shader::set_vec3(const char* name, const vec3& vector)
{
	glUniform3f(glGetUniformLocation(programId_, name),
		vector.x, vector.y, vector.z);
}

void Shader::set_vec2(const char* name, const vec2& vector)
{
	glUniform2f(glGetUniformLocation(programId_, name),
		vector.x, vector.y);
}

void Shader::use()
{
	glUseProgram(programId_);
}

void Shader::set_int(const char* name, int number)
{
	glUniform1i(glGetUniformLocation(programId_, name), number);
}

void Shader::set_float(const char* name, float number)
{
	glUniform1f(glGetUniformLocation(programId_, name), number);
}

void Shader::set_enum(const char* name, int type)
{
	glUniform1i(glGetUniformLocation(programId_, name), type);
}

void Shader::set_matrix(const char* name, const mat4& mat4)
{
	glUniformMatrix4fv(glGetUniformLocation(programId_, name),
		1, GL_FALSE, &mat4.m[0][0]);
}

jeEnd
