#pragma once
#include <unordered_map>
#include "Factory.h"
#include "EntityTile.h"
#include "EntityPlayer.h"
#include "EntityBackground.h"
//#include "EntityInputHandler.h"
#include "EntityMovable.h"
#include "ActionSpriteDraw.h"
#include "ActionSpriteUpdate.h"
#include "ActionList.h"
#include "Entity.h"

class LevelParser final
{
	static const std::uint32_t maxRow = 19;
	static const std::uint32_t maxCol = 25;

public:
	LevelParser(Game& game, Camera& camera);
	~LevelParser();

	LibraryDesktop::Sector& LoadLevel();
	void NextLevel();

private:
	void PopulateSymbolTable();
	void PopulateLevelList();

	std::uint32_t mLevelIndex;

	LibraryDesktop::Sector& ParseLevel(const std::string& levelString);
	EntityTile& CreateTile(Entity* entity, float x, float y);
	EntityPlayer& CreatePlayer(Entity* entity, float x, float y);
	EntityMovable& CreateMovable(Entity* entity, float x, float y);

	std::unordered_map<char, std::string> mSymbolTable;
	Vector<std::string> mLevelList;

	static const std::uint32_t MAP_SIZE;
	static const float TILE_WIDTH;
	static const float TILE_HEIGHT;
	static const std::string VELOCITY_X_KEY;
	static const std::string VELOCITY_Y_KEY;
	static const std::string TEXTURE_NAME_KEY;
	static const std::string FRAME_X_KEY;
	static const std::string FRAME_Y_KEY;
	static const std::string WIDTH_KEY;
	static const std::string HEIGHT_KEY;
	static const std::string HARMFUL_KEY;
	static const std::string VISIBLE_KEY;
	static const std::string EXIT_KEY;
	static const std::string FLIP_KEY;
	static const std::string SPEED_KEY;
	static const std::string ANIMATION_KEY;
	static const std::string ANIMATION_NAME_KEY;
	static const std::string IDLE_KEY;
	static const std::string WALK_KEY;
	static const std::string ACTIONS_KEY;
	static const std::string WORLDS_KEY;
	static const std::string ENTITIES_KEY;
	static const std::string FRAME_KEY;
	static const std::string DIRECTION_KEY;
	static const std::string CONTENT_PATH;
	static const std::string SYMBOLS_PATH;
	static const std::string LEVELS_PATH;

	static EntityTileFactory tileFactory;
	static EntityPlayerFactory playerFactory;
	static EntityBackgroundFactory backgroundFactory;
	static EntityMovableFactory movableFactory;
	static ActionSpriteDrawFactory drawFactory;
	static ActionSpriteUpdateFactory updateFactory;
	static ActionListFactory actionListFactory;

	Game* mGame;
	Camera* mCamera;
};


