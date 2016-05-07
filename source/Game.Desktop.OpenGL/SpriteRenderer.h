/** @file SpriteRenderer.h
*   @brief Prototypes for a sprite rendering object
*
*   @author Alex Stopar (amstopar)
*/

#pragma once

#include "Texture2D.h"
#include "Shader.h"
#include "Sprite.h"

class SpriteRenderer
{
public:

	/** \brief Constructor for the object
	*   \param shader The shader that this renderer uses
	*/
	explicit SpriteRenderer(const Shader& shader);

	///The default destructor for this object
	~SpriteRenderer();

	/** \brief Draws a sprite to the screen
	*   \param sprite The sprite to draw
	*/
	void DrawSprite(Sprite& sprite);

private:
	//The shader used by the renderer
	Shader mShader;

	//The vertex array object of the renderer
	GLuint mVAO;

	//The vertex buffer object of the renderer
	GLuint mVBO;

	//The element buffer object of the renderer
	GLuint mEBO;

	//Initializes all necessary render data after construction of objects
	void initRenderData();

	
};

