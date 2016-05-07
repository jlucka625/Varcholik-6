/** @file ResourceManager.cpp
*   @brief A global resource manager
*
*   @author Alex Stopar (amstopar)
*/

#include "pch.h"
#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL.h>

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader& ResourceManager::LoadShader(const GLchar * vertexFile, const GLchar * fragmentFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vertexFile, fragmentFile);
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(const GLchar * file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto& iter : Shaders)
	{
		glDeleteProgram(iter.second.GetProgram());
	}

	for (const auto& iter : Textures)
	{
		auto idRef = iter.second.GetID();
		glDeleteTextures(1, &idRef);
	}
}

Shader ResourceManager::loadShaderFromFile(const GLchar * vertexFile, const GLchar * fragmentFile)
{
	//Get the shaders from a file path
	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//Ensures exception throwing
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexShaderSource = vShaderStream.str();
		fragmentShaderSource = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR - Shader file not successfully read!" << std::endl;
	}

	const GLchar* vShaderCode = vertexShaderSource.c_str();
	const GLchar* fShaderCode = fragmentShaderSource.c_str();

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode);

	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar * file, GLboolean alpha)
{
	Texture2D texture;

	if (alpha)
	{
		texture.SetInternalFormat(GL_RGBA);
		texture.SetImageFormat(GL_RGBA);
	}

	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.GetImageFormat() == 
		GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	texture.Generate(width, height, image);

	SOIL_free_image_data(image);
	return texture;
}
