/** @file Texture2D.h
*   @brief Prototypes for a texture object
*
*   @author Alex Stopar (amstopar)
*/
#pragma once

#include <gl3w.h>

class Texture2D
{
public:
	///Default constructor for this object
	Texture2D();

	/** \brief Generates a texture from image data
	*   \param width The width of the texture
	*	\param height The height of the texture
	*	\param data The image data to use for generation
	*/
	void Generate(GLuint width, GLuint height, unsigned char* data);

	///Binds the texture as the active GL_TEXTURE_2D texture object
	void Bind() const;

	/** \brief Gets the ID of the texture
	*   \return The ID of the texture
	*/
	GLuint GetID() const;

	/** \brief Gets the width of the texture
	*   \return The width of the texture
	*/
	GLuint GetWidth() const;

	/** \brief Sets the width of the texture
	*   \param width The width of the texture
	*/
	void SetWidth(GLuint width);

	/** \brief Gets the height of the texture
	*   \return The height of the texture
	*/
	GLuint GetHeight() const;

	/** \brief Sets the height of the texture
	*   \param height The height of the texture
	*/
	void SetHeight(GLuint height);

	/** \brief Gets the format of the texture
	*   \return The format of the texture
	*/
	GLuint GetInternalFormat() const;

	/** \brief Sets the format of the texture
	*   \param format The format of the texture
	*/
	void SetInternalFormat(GLuint format);

	/** \brief Gets the format of the loaded image
	*   \return The format of the loaded image
	*/
	GLuint GetImageFormat() const;

	/** \brief Sets the format of the loaded image
	*   \param format The format of the loaded image
	*/
	void SetImageFormat(GLuint format);

	/** \brief Gets the S-axis wrap mode of the texture
	*   \return The S-axis wrap mode of the texture
	*/
	GLuint GetWrapS() const;

	/** \brief Sets the S-axis wrap mode of the texture
	*   \param mode The S-axis wrap mode of the texture
	*/
	void SetWrapS(GLuint mode);

	/** \brief Gets the T-axis wrap mode of the texture
	*   \return The T-axis wrap mode of the texture
	*/
	GLuint GetWrapT() const;

	/** \brief Sets the T-axis wrap mode of the texture
	*   \param mode The T-axis wrap mode of the texture
	*/
	void SetWrapT(GLuint mode);

	/** \brief Gets the minification filter of the texture
	*   \return The minification filter of the texture
	*/
	GLuint GetMinFilter() const;

	/** \brief Sets the minification filter of the texture
	*   \param filter The minification filter of the texture
	*/
	void SetMinFilter(GLuint filter);

	/** \brief Gets the magnification filter of the texture
	*   \return The magnification filter of the texture
	*/
	GLuint GetMagFilter() const;

	/** \brief Sets the magnification filter of the texture
	*   \param filter The magnification filter of the texture
	*/
	void SetMagFilter(GLuint filter);

	/** \brief Gets the total horizontal frames of the texture
	*   \return The total horizontal frames of the texture
	*/
	float GetTotalXFrames() const;

	/** \brief Sets the total horizontal frames of the texture
	*   \param frames The total horizontal frames of the texture
	*/
	void SetTotalXFrames(float frames);

	/** \brief Gets the total vertical frames of the texture
	*   \return The total vertical frames of the texture
	*/
	float GetTotalYFrames() const;

	/** \brief Sets the total vertical frames of the texture
	*   \param frames The total vertical frames of the texture
	*/
	void SetTotalYFrames(float frames);
	
private:
	//The id of the texture object
	GLuint mID;

	//The width of the texture
	GLuint mWidth;

	//The height of the texture
	GLuint mHeight;

	//The format of the texture object
	GLuint mInternalFormat;

	//The format of the loaded image
	GLuint mImageFormat;

	//The wrapping mode on the S-axis
	GLuint mWrapS;

	//The wrapping mode on the T-axis
	GLuint mWrapT;

	//The filtering mode for minification (texels < pixels)
	GLuint mMinFilter;

	//The filtering mode for magnification (texels > pixels)
	GLuint mMagFilter;

	//The total amount of horizontal frames in the texture
	float mTotalXFrames;

	//The total amount of vertical frames in the texture
	float mTotalYFrames;

};

