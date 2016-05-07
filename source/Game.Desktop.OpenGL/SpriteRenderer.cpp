/** @file SpriteRenderer.cpp
*   @brief A sprite rendering object
*
*   @author Alex Stopar (amstopar)
*/

#include "pch.h"
#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

static GLfloat vertices[] =
{
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

static const GLuint indices[] =
{
	0, 1, 2,
	0, 3, 1
};

SpriteRenderer::SpriteRenderer(const Shader & shader) : mShader(shader)
{
	initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->mVAO);
}

void SpriteRenderer::DrawSprite(Sprite& sprite)
{
	mShader.Use();

	//Set texture vertices based on frame
	vertices[2] = sprite.GetFramePosition().x * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalXFrames()));
	vertices[3] = (sprite.GetFramePosition().y + 1.0f) * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalYFrames()));
	vertices[6] = (sprite.GetFramePosition().x + 1.0f) * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalXFrames()));
	vertices[7] = (sprite.GetFramePosition().y) * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalYFrames()));
	vertices[10] = sprite.GetFramePosition().x * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalXFrames()));
	vertices[11] = (sprite.GetFramePosition().y) * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalYFrames()));
	vertices[14] = (sprite.GetFramePosition().x + 1.0f) * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalXFrames()));
	vertices[15] = (sprite.GetFramePosition().y + 1.0f) * (1.0f / static_cast<float>(sprite.GetTexture().GetTotalYFrames()));

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glm::mat4 modelMatrix;

	auto setPosition = sprite.GetPosition();
	if (sprite.IsYFlipped())
	{
		setPosition.y += sprite.GetSize().y;
	}

	if (sprite.IsXFlipped())
	{
		setPosition.x += sprite.GetSize().x;
	}

	//First translate
	modelMatrix = glm::translate(modelMatrix, glm::vec3(setPosition, 0.0f));

	//Translate origin of rotation to center of quad and rotate along z-axis
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * sprite.GetSize().x, 0.5f * sprite.GetSize().y, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, sprite.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * sprite.GetSize().x, -0.5f * sprite.GetSize().y, 0.0f));

	//Now apply scaling
	modelMatrix = glm::scale(modelMatrix, glm::vec3(sprite.GetSize().x * (sprite.IsXFlipped() ? -1.0f : 1.0f), 
		sprite.GetSize().y * (sprite.IsYFlipped() ? -1.0f : 1.0f), 1.0f));

	mShader.SetMatrix("model", modelMatrix);
	mShader.SetVector3f("spriteColor", sprite.GetColor());
	mShader.SetVector2f("translation", sprite.GetTranslation());

	glActiveTexture(GL_TEXTURE0);
	sprite.GetTexture().Bind();

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{	
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
