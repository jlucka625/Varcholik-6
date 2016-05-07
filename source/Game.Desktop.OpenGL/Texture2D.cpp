/** @file Texture2D.cpp
*   @brief A texture object
*
*   @author Alex Stopar (amstopar)
*/

#include "pch.h"
#include "Texture2D.h"


Texture2D::Texture2D() : mWidth(0), mHeight(0), mInternalFormat(GL_RGBA), mImageFormat(GL_RGBA), 
mWrapS(GL_CLAMP_TO_EDGE), mWrapT(GL_CLAMP_TO_EDGE), mMinFilter(GL_LINEAR), mMagFilter(GL_LINEAR), mTotalXFrames(1U), mTotalYFrames(1U), mID(0)
{
	glGenTextures(1, &mID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char * data)
{
	mWidth = width;
	mHeight = height;

	//Create texture and bind
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mImageFormat, GL_UNSIGNED_BYTE, data);

	//Set texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, mID);
}

GLuint Texture2D::GetID() const
{
	return mID;
}

GLuint Texture2D::GetWidth() const
{
	return mWidth;
}

void Texture2D::SetWidth(GLuint width)
{
	mWidth = width;
}

GLuint Texture2D::GetHeight() const
{
	return mHeight;
}

void Texture2D::SetHeight(GLuint height)
{
	mHeight = height;
}

GLuint Texture2D::GetInternalFormat() const
{
	return mInternalFormat;
}

void Texture2D::SetInternalFormat(GLuint format)
{
	mInternalFormat = format;
}

GLuint Texture2D::GetImageFormat() const
{
	return mImageFormat;
}

void Texture2D::SetImageFormat(GLuint format)
{
	mImageFormat = format;
}

GLuint Texture2D::GetWrapS() const
{
	return mWrapS;
}

void Texture2D::SetWrapS(GLuint mode)
{
	mWrapS = mode;
}

GLuint Texture2D::GetWrapT() const
{
	return mWrapT;
}

void Texture2D::SetWrapT(GLuint mode)
{
	mWrapT = mode;
}

GLuint Texture2D::GetMinFilter() const
{
	return mMinFilter;
}

void Texture2D::SetMinFilter(GLuint filter)
{
	mMinFilter = filter;
}

GLuint Texture2D::GetMagFilter() const
{
	return mMagFilter;
}

void Texture2D::SetMagFilter(GLuint filter)
{
	mMagFilter = filter;
}

float Texture2D::GetTotalXFrames() const
{
	return mTotalXFrames;
}

void Texture2D::SetTotalXFrames(float frames)
{
	if (frames > 0.0f)
	{
		mTotalXFrames = frames;
	}
	else
	{
		mTotalXFrames = 1.0f;
	}
}

float Texture2D::GetTotalYFrames() const
{
	return mTotalYFrames;
}

void Texture2D::SetTotalYFrames(float frames)
{
	if (frames > 0.0f)
	{
		mTotalYFrames = frames;
	}
	else
	{
		mTotalYFrames = 1.0f;
	}
}

