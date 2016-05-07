#pragma once

#include "Shader.h"
#include "Texture2D.h"

class PostProcessor
{
public:

	/** \brief Constructs the post processor object
	*   \param shader The shader used by this object
	*	\param width The width of the scene
	*	\param height The height of the scene
	*/
	PostProcessor(Shader& shader, GLuint width, GLuint height);
	
	//Prepares the framebuffer operations before rendering
	void BeginRender();

	//Stores rendered data into texture
	void EndRender();

	/** \brief Renders the screen quad
	*   \param time The time used in rendering post-process effects
	*/
	void Render(GLfloat time);

	/** \brief Is the edge effect being used?
	*   \return Whether it is being used or not
	*/
	bool IsEdgeUsed();

	/** \brief Sets if edge effect is being used
	*   \param used Whether it is being used or not
	*/
	void SetIsEdgeUsed(bool used);

	/** \brief Is the blur effect being used?
	*   \return Whether it is being used or not
	*/
	bool IsBlurUsed();

	/** \brief Sets if blur effect is being used
	*   \param used Whether it is being used or not
	*/
	void SetIsBlurUsed(bool used);

	/** \brief Is the wave effect being used?
	*   \return Whether it is being used or not
	*/
	bool IsWaveUsed();

	/** \brief Sets if wave effect is being used
	*   \param used Whether it is being used or not
	*/
	void SetIsWaveUsed(bool used);

	//Initialize render data for the object
	void InitRenderData();

	/** \brief Binds framebuffer based on scene
	*	\param width The width of the scene
	*	\param height The height of the scene
	*/
	void BindFramebuffer(GLuint width, GLuint height);

private:
	//The shader to use
	Shader* mShader;

	//The texture of the scene
	Texture2D mTexture;

	//The width and height of the scene
	GLuint mWidth;
	GLuint mHeight;

	//Is the edge effect used?
	bool mEdgeUsed;

	//Is the blur effect used?
	bool mBlurUsed;

	//Is the wave effect used?
	bool mWaveUsed;

	//The multisampled and regular frame buffer object
	GLuint mMSFBO, mFBO;

	//The renderbuffer object
	GLuint mRBO;

	//The vertex array object
	GLuint mVAO;

};

