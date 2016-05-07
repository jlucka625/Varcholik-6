/** @file TextRenderer.cpp
*   @brief A text rendering object
*
*   @author Alex Stopar (amstopar)
*/

#include "pch.h"
#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(Shader & shader) : mShader(&shader), mVAO(0), mVBO(0), mCharacters()
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, GLuint fontSize)
{
	//Clear the characters first
	mCharacters.clear();

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Error! Could not init FreeType library" << std::endl;
	}

	//Load font as face
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		std::cout << "Error! Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	//Disable byte-alignment restrictions
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load and store the first 128 ASCII characters
	for (GLubyte characterByte = 0; characterByte < 128; ++characterByte)
	{
		if (FT_Load_Char(face, characterByte, FT_LOAD_RENDER))
		{
			std::cout << "Error! Failed to load glyph " << characterByte << std::endl;
			continue;
		}

		//Generate texture for character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
			GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		//Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Store character for future use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};

		mCharacters.insert(std::pair<GLchar, Character>(characterByte, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	mShader->Use();
	mShader->SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVAO);

	//Iteratre through all characters in text
	std::string::const_iterator charIter;
	for (charIter = text.begin(); charIter != text.end(); ++charIter)
	{
		auto character = mCharacters[*charIter];
		GLfloat xpos = x + character.Bearing.x * scale;
		GLfloat ypos = y + (mCharacters['H'].Bearing.y - character.Bearing.y) * scale;

		GLfloat width = character.Size.x * scale;
		GLfloat height = character.Size.y * scale;

		//Update the VBO
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + height,   0.0, 1.0 },
			{ xpos + width, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + height,   0.0, 1.0 },
			{ xpos + width, ypos + height,   1.0, 1.0 },
			{ xpos + width, ypos,       1.0, 0.0 }
		};

		//Render glyph texture and update content of VBO memory
		glBindTexture(GL_TEXTURE_2D, character.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Advance cursors for next glyph - bitshift by 6 to get value in pixels (1/64 * 2^6) = 64
		x += (character.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
