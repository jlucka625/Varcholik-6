/** @file Shader.h
*   @brief Prototypes for a shader program object
*
*   @author Alex Stopar (amstopar)
*/
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "gl3w.h"
#include "GLFW/glfw3.h"

class Shader
{
public:

	///The default constructor for this object
	Shader();

	///Uses the shader program
	Shader& Use();

	/** \brief Compiles the shader
	*   \param vShaderCode The vertex shader code
	*	\param fShaderCode The fragment shader code
	*/
	void Compile(const GLchar* vShaderCode, const GLchar* fShaderCode);

	/** \brief Gets the program of the object
	*   \return The program of the object
	*/
	GLuint GetProgram();

	/** \brief Sets a float value in the shader
	*   \param name The name of the uniform value
	*	\param value The value to set
	*	\param useShader Does this call use the shader?
	*/
	void SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);


	/** \brief Sets an integer value in the shader
	*   \param name The name of the uniform value
	*	\param value The value to set
	*	\param useShader Does this call use the shader?
	*/
	void SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);


	/** \brief Sets a vec2 value in the shader
	*   \param name The name of the uniform value
	*	\param x The value to set (x)
	*	\param y The value to set (y)
	*	\param useShader Does this call use the shader?
	*/
	void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);


	/** \brief Sets a vec2 value in the shader
	*   \param name The name of the uniform value
	*	\param value The value to set
	*	\param useShader Does this call use the shader?
	*/
	void SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);


	/** \brief Sets a vec3 value in the shader
	*   \param name The name of the uniform value
	*	\param x The value to set (x)
	*	\param y The value to set (y)
	*	\param z The value to set (z)
	*	\param useShader Does this call use the shader?
	*/
	void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);


	/** \brief Sets a vec3 value in the shader
	*   \param name The name of the uniform value
	*	\param value The value to set
	*	\param useShader Does this call use the shader?
	*/
	void SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);


	/** \brief Sets a vec4 value in the shader
	*   \param name The name of the uniform value
	*	\param x The value to set (x)
	*	\param y The value to set (y)
	*	\param z The value to set (z)
	*	\param w The value to set (w)
	*	\param useShader Does this call use the shader?
	*/
	void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);


	/** \brief Sets a vec4 value in the shader
	*   \param name The name of the uniform value
	*	\param value The value to set
	*	\param useShader Does this call use the shader?
	*/
	void SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);


	/** \brief Sets a mat4x4 value in the shader
	*   \param name The name of the uniform value
	*	\param value The value to set
	*	\param useShader Does this call use the shader?
	*/
	void SetMatrix(const GLchar* name, const glm::mat4x4& value, GLboolean useShader = false);

private:

	//The shader program to use
	GLuint mProgram;
};

