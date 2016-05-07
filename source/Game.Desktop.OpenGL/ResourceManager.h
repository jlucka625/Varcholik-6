/** @file ResourceManager.h
*   @brief Prototypes for a global resource manager
*
*   @author Alex Stopar (amstopar)
*/

#pragma once

#include <map>
#include <string>
#include <gl3w.h>
#include "Texture2D.h"
#include "Shader.h"

class ResourceManager
{
public:
	
	//Resource storage for shaders
	static std::map<std::string, Shader> Shaders;

	//Resource storage for textures
	static std::map<std::string, Texture2D> Textures;

	/** \brief Loads the shader file
	*   \param vertexFile The file for the vertex shader
	*   \param fragmentFile The file for the fragment shader
	*   \param name The name of the shader
	*   \return The loaded shader
	*/
	static Shader& LoadShader(const GLchar* vertexFile, const GLchar* fragmentFile, std::string name);

	/** \brief Gets the shader file by name
	*   \param name The name of the shader
	*   \return The shader
	*/
	static Shader& GetShader(std::string name);

	static Texture2D& LoadTexture(const GLchar* file, GLboolean alpha, std::string name);

	/** \brief Gets the texture object by name
	*   \param name The name of the texture
	*   \return The texture
	*/
	static Texture2D& GetTexture(std::string name);

	///De-allocates all resources
	static void Clear();

private:
	///The default constructor for this object
	ResourceManager() {}

	//Loads a shader from a file
	static Shader loadShaderFromFile(const GLchar* vertexFile, const GLchar* fragmentFile);

	//Loads a texture from a file
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};

