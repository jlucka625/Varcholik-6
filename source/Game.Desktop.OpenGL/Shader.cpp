/** @file Shader.cpp
*   @brief A Shader program object
*
*   @author Alex Stopar (amstopar)
*/

#include "pch.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : mProgram(0)
{
}

Shader& Shader::Use()
{
	glUseProgram(mProgram);
	return *this;
}

void Shader::Compile(const GLchar * vShaderCode, const GLchar * fShaderCode)
{
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	//Try the vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "Error - Vertex shader compilation failed\n" << infoLog << std::endl;
	}

	//Try the fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "Error - Fragment shader compilation failed\n" << infoLog << std::endl;
	}

	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	glLinkProgram(mProgram);

	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
}

GLuint Shader::GetProgram()
{
	return mProgram;
}

void Shader::SetFloat(const GLchar * name, GLfloat value, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform1f(glGetUniformLocation(mProgram, name), value);
}

void Shader::SetInteger(const GLchar * name, GLint value, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform1i(glGetUniformLocation(mProgram, name), value);
}

void Shader::SetVector2f(const GLchar * name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform2f(glGetUniformLocation(mProgram, name), x, y);
}

void Shader::SetVector2f(const GLchar * name, const glm::vec2 & value, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform2f(glGetUniformLocation(mProgram, name), value.x, value.y);
}

void Shader::SetVector3f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform3f(glGetUniformLocation(mProgram, name), x, y, z);
}

void Shader::SetVector3f(const GLchar * name, const glm::vec3 & value, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform3f(glGetUniformLocation(mProgram, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform4f(glGetUniformLocation(mProgram, name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar * name, const glm::vec4 & value, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniform4f(glGetUniformLocation(mProgram, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix(const GLchar * name, const glm::mat4x4 & value, GLboolean useShader)
{
	if (useShader)
	{
		Use();
	}

	glUniformMatrix4fv(glGetUniformLocation(mProgram, name), 1, GL_FALSE, glm::value_ptr(value));
}
