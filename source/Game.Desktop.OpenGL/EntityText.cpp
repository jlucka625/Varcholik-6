#include "pch.h"
#include "EntityText.h"
#include "ResourceManager.h"

RTTI_DEFINITIONS(EntityText);

const std::string EntityText::TEXT_ATTRIBUTE = "Text";
const std::string EntityText::FONT_ATTRIBUTE = "Font";
const std::string EntityText::SIZE_POSITION_ATTRIBUTE = "SizePosition";
const std::string EntityText::COLOR_ATTRIBUTE = "Color";

const std::string defaultFont = "Content/ModerDOS.ttf";

const float defaultFontSize = 22.0f;

EntityText::EntityText() : mTextRenderer(ResourceManager::GetShader("TextShader")), mSizePositionInfo(glm::vec4(0, 0, 1.0f, defaultFontSize)), mFont(defaultFont)
{
	AddExternalAttribute(TEXT_ATTRIBUTE, 1, &mText);
	AddExternalAttribute(FONT_ATTRIBUTE, 1, &mFont);
	AddExternalAttribute(SIZE_POSITION_ATTRIBUTE, 1, &mSizePositionInfo);
	AddExternalAttribute(COLOR_ATTRIBUTE, 1, &mColor);
}

void EntityText::Draw()
{
	mTextRenderer.RenderText(mText, mSizePositionInfo.x, mSizePositionInfo.y, mSizePositionInfo.z, glm::vec3(mColor.r, mColor.g, mColor.b));
}

const std::string & EntityText::Text() const
{
	return mText;
}

void EntityText::SetText(const std::string & text)
{
	mText = text;
}

const std::string & EntityText::Font() const
{
	return mFont;
}

void EntityText::SetFont(const std::string & font)
{
	mFont = font;
}

glm::vec3 EntityText::Color() const
{
	return glm::vec3(mColor.r, mColor.g, mColor.b);
}

void EntityText::SetColor(float r, float g, float b)
{
	mColor = glm::vec4(r, g, b, 1.0f);
}

glm::vec4 EntityText::SizePositionInfo() const
{
	return mSizePositionInfo;
}

void EntityText::SetSizePositionInfo(float x, float y, float scale, float fontSize)
{
	mSizePositionInfo = glm::vec4(x, y, scale, fontSize);
}

void EntityText::Load()
{
	mTextRenderer.Load(mFont, static_cast<GLuint>(mSizePositionInfo.w));
}
