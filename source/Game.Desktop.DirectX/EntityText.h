#pragma once
#include "Entity.h"

///An entity used to display text to the screen
class EntityText final : public Entity
{
	RTTI_DECLARATIONS(EntityText, Entity)
public:
	///The default constructor for this object
	EntityText();

	///Draw the sprite
	void Draw();

	/** @brief					Gets the text of the entity
	*   @return                 The text of the entity
	*/
	const std::string& Text() const;

	/** @brief					Sets the text of the entity
	*   @param text				The text to set
	*/
	void SetText(const std::string& text);

	/** @brief					Gets the font of the entity
	*   @return                 The font of the entity
	*/
	const std::string& Font() const;

	/** @brief					Sets the font of the entity
	*   @param font				The font to set
	*/
	void SetFont(const std::string& font);

	/** @brief					Gets the color
	*   @return                 The color of the entity
	*/
	glm::vec3 Color() const;

	/** @brief					Sets the color of the entity
	*   @param r				The red
	*   @param g				The green
	*   @param b				The blue
	*/
	void SetColor(float r, float g, float b);

	/** @brief					Gets the size/position info (x, y, scale, fontSize)
	*   @return                 The size/position info
	*/
	glm::vec4 SizePositionInfo() const;

	/** @brief					Sets the size/position info
	*   @param x				The horizontal position
	*   @param y				The vertical position
	*   @param scale			The scale
	*   @param fontSize			The font size
	*/
	void SetSizePositionInfo(float x, float y, float scale, float fontSize);

	///Loads the specified font and font size into the text
	void Load();

private:

	//The size and position of the text (x, y, scale, fontSize)
	glm::vec4 mSizePositionInfo;

	//The color of the text
	glm::vec4 mColor;

	//The text of the entity
	std::string mText;

	//The file that contains the text font
	std::string mFont;

	///The name of the text attribute
	static const std::string TEXT_ATTRIBUTE;

	///The name of the font attribute
	static const std::string FONT_ATTRIBUTE;

	///The name of the color attribute
	static const std::string COLOR_ATTRIBUTE;

	///The name of the size/position info attribute
	static const std::string SIZE_POSITION_ATTRIBUTE;
};

ConcreteFactory(RTTI, EntityText);

