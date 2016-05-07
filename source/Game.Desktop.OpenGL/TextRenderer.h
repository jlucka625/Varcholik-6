/** @file TextRenderer.h
*   @brief Prototypes for a text rendering object
*
*   @author Alex Stopar (amstopar)
*/

#pragma once

#include "Shader.h"

///Holds all state information relevant to a character from FreeType
struct Character
{
	GLuint TextureID; //ID handle of the glyph texture
	glm::ivec2 Size; //Size of glyph
	glm::ivec2 Bearing; //Offset from baseline to left/top of glyph
	GLuint Advance; //Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:

	/** \brief The constructor for this object
	*	\param shader The shader to use
	*/
	explicit TextRenderer(Shader& shader);

	/** \brief Loads characters from the given font file
	*	\param font The font file to read from
	*	\param fontSize The size of the font
	*/
	void Load(std::string font, GLuint fontSize);

	/** \brief Renders text to the screen
	*	\param text The text to render
	*	\param x The horizontal location to draw
	*	\param y The vertical location to draw
	*	\param color The color of the text
	*/
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

private:

	//The list of pre-compiled Characters to use
	std::map<GLchar, Character> mCharacters;

	//The shader to use
	Shader* mShader;

	//The vertex array object
	GLuint mVAO;

	//The vertex buffer object
	GLuint mVBO;
};

