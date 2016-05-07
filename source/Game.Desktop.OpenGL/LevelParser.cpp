#include "pch.h"
#include "LevelParser.h"
#include <fstream>
#include <iostream>

using namespace LibraryDesktop;

const std::uint32_t LevelParser::MAP_SIZE = 13;
const float LevelParser::TILE_WIDTH = 32.0f;
const float LevelParser::TILE_HEIGHT = 32.0f;

const std::string LevelParser::VELOCITY_X_KEY = "VelocityX";
const std::string LevelParser::VELOCITY_Y_KEY = "VelocityY";
const std::string LevelParser::TEXTURE_NAME_KEY = "texName";
const std::string LevelParser::FRAME_X_KEY = "u";
const std::string LevelParser::FRAME_Y_KEY = "v";
const std::string LevelParser::WIDTH_KEY = "width";
const std::string LevelParser::HEIGHT_KEY = "height";
const std::string LevelParser::HARMFUL_KEY = "Harmful";
const std::string LevelParser::VISIBLE_KEY = "isVisible";
const std::string LevelParser::EXIT_KEY = "isExit";
const std::string LevelParser::FLIP_KEY = "isFlip";
const std::string LevelParser::SPEED_KEY = "Speed";
const std::string LevelParser::ANIMATION_KEY = "Animation";
const std::string LevelParser::ANIMATION_NAME_KEY = "AnimationName";
const std::string LevelParser::IDLE_KEY = "Idle";
const std::string LevelParser::WALK_KEY = "Walk";
const std::string LevelParser::ACTIONS_KEY = "Actions";
const std::string LevelParser::WORLDS_KEY = "Worlds";
const std::string LevelParser::ENTITIES_KEY = "Entities";
const std::string LevelParser::FRAME_KEY = "Frame";
const std::string LevelParser::DIRECTION_KEY = "Positive";
const std::string LevelParser::CONTENT_PATH = "Content\\TileXML\\";
const std::string LevelParser::SYMBOLS_PATH = "Content\\TileXML\\TileSymbols.xml";
const std::string LevelParser::LEVELS_PATH = "Content\\TileXML\\LevelConfig.xml";

EntityTileFactory LevelParser::tileFactory;
EntityPlayerFactory LevelParser::playerFactory;
EntityBackgroundFactory LevelParser::backgroundFactory;
EntityMovableFactory LevelParser::movableFactory;
ActionSpriteDrawFactory LevelParser::drawFactory;
ActionSpriteUpdateFactory LevelParser::updateFactory;
ActionListFactory LevelParser::actionListFactory;

LevelParser::LevelParser() : mSymbolTable(MAP_SIZE), mLevelList(), mLevelIndex(0)
{
	PopulateSymbolTable();
	PopulateLevelList();
}

void LevelParser::PopulateSymbolTable()
{
	SymbolSharedData sharedData;
	XmlParseMaster master(sharedData);
	TileXmlParseHelper helper;

	master.AddHelper(helper);

	master.ParseFromFile(SYMBOLS_PATH);

	mSymbolTable = sharedData.GetTilePaths();
}

void LevelParser::PopulateLevelList()
{
	LevelSharedData sharedData;
	XmlParseMaster master(sharedData);
	LevelConfigXmlParseHelper helper;

	master.AddHelper(helper);

	master.ParseFromFile(LEVELS_PATH);

	mLevelList = sharedData.GetLevelPaths();
}

void LevelParser::NextLevel()
{
	++mLevelIndex;
	if (mLevelIndex == mLevelList.Size())
		mLevelIndex = 0;
}

Sector& LevelParser::LoadLevel()
{
	std::string levelString = "", line = "";
	std::ifstream stream;
	stream.open(mLevelList.At(mLevelIndex));
	while (std::getline(stream, line))
	{
		std::cout << line << std::endl;
		levelString.append(line);
		levelString.append("\n");
	}
	stream.close();

	return ParseLevel(levelString);
}

Sector& LevelParser::ParseLevel(const std::string& levelString)
{
	Sector* levelSector = new Sector;
	levelSector->SetName(levelString);

	TableSharedData sharedData;
	XmlParseMaster master(sharedData);

	ActionXmlParseHelper actionHelper;
	master.AddHelper(actionHelper);

	EntityXmlParseHelper entityHelper;
	master.AddHelper(entityHelper);

	SectorXmlParseHelper sectorHelper;
	master.AddHelper(sectorHelper);

	WorldXmlParseHelper worldHelper;
	master.AddHelper(worldHelper);

	FloatXmlParseHelper floatHelper;
	master.AddHelper(floatHelper);

	IntegerXmlParseHelper intHelper;
	master.AddHelper(intHelper);

	MatrixXmlParseHelper matrixHelper;
	master.AddHelper(matrixHelper);

	ReactionXmlParseHelper reactionHelper;
	master.AddHelper(reactionHelper);

	StringXmlParseHelper stringHelper;
	master.AddHelper(stringHelper);

	TableXmlParseHelper tableHelper;
	master.AddHelper(tableHelper);

	VectorXmlParseHelper vectorHelper;
	master.AddHelper(vectorHelper);

	float x = 0.0f, y = 0.0f;

	for (std::uint32_t i = 0; i < levelString.length(); ++i)
	{
		const char& symbol = levelString.at(i);
		if (symbol != '\n')
		{
			if (symbol != '.')
			{
				std::string path = CONTENT_PATH;
				master.ParseFromFile(path.append(mSymbolTable[symbol]));

				World* world = sharedData.GetCurrentScope()[WORLDS_KEY][0].As<World>();
				Sector* sector = world->Sectors().Get<Scope*>()->As<Sector>();
				Entity* entity = sector->Entities().Get<Scope*>()->As<Entity>();

				if (entity)
					if (entity->Is(EntityTile::TypeIdClass()))
						levelSector->Adopt(CreateTile(entity, x, y), ENTITIES_KEY);
					else if (entity->Is(EntityPlayer::TypeIdClass()))
						levelSector->Adopt(CreatePlayer(entity, x, y), ENTITIES_KEY);
					else if (entity->Is(EntityMovable::TypeIdClass()))
						levelSector->Adopt(CreateMovable(entity, x, y), ENTITIES_KEY);
			}
			x += 1.0f;
		}
		else
		{
			y += 1.0f;
			x = 0.0f;
		}
		sharedData.Clear();
	}

	return *levelSector;
}

EntityTile& LevelParser::CreateTile(Entity* entity, float x, float y)
{
	EntityTile& tile = *(entity->As<EntityTile>());

	std::string textureName = entity->Find(TEXTURE_NAME_KEY)->Get<std::string>();

	float u = entity->Find(FRAME_X_KEY)->Get<float>();
	float v = entity->Find(FRAME_Y_KEY)->Get<float>();

	float width = entity->Find(WIDTH_KEY)->Get<float>();
	float height = entity->Find(HEIGHT_KEY)->Get<float>();

	tile.CreateSprite(textureName, u, v, width, height);
	tile.GetSprite().SetPosition(glm::vec2(x * width, y * height));

	tile.GetSprite().SetVisible(true);

	int harmful = entity->Find(HARMFUL_KEY)->Get<int>();
	tile.SetIsHarmful(harmful);

	int visible = entity->Find(VISIBLE_KEY)->Get<int>();
	tile.GetSprite().SetVisible(visible != 0);

	int exit = entity->Find(EXIT_KEY)->Get<int>();
	tile.SetExit(exit != 0);

	int flip = entity->Find(FLIP_KEY)->Get<int>();
	tile.SetIsGravityFlip(flip);

	return tile;
}

EntityPlayer& LevelParser::CreatePlayer(Entity* entity, float x, float y)
{
	EntityPlayer& player = *(entity->As<EntityPlayer>());

	std::string textureName = entity->Find(TEXTURE_NAME_KEY)->Get<std::string>();

	float u = entity->Find(FRAME_X_KEY)->Get<float>();
	float v = entity->Find(FRAME_Y_KEY)->Get<float>();

	float width = entity->Find(WIDTH_KEY)->Get<float>();
	float height = entity->Find(HEIGHT_KEY)->Get<float>();

	player.CreateSprite(textureName, u, v, width, height);

	float moveSpeed = entity->Find(SPEED_KEY)->Get<float>();
	player.SetMoveSpeed(moveSpeed);

	float fallSpeed = entity->Find("FallSpeed")->Get<float>();
	player.SetFallSpeed(fallSpeed);

	player.GetSprite().SetPosition(glm::vec2(x * width, y * height/2.0f));
	player.GetSprite().SetAnimationTime(0.1f);
	player.GetSprite().SetVisible(true);

	Datum& actions = *(entity->Find(ACTIONS_KEY));
	for (std::uint32_t i = 0; i < actions.Size(); ++i)
	{
		Action& animation = *(actions.Get<Scope*>(i)->As<Action>());
		if (&animation && animation.Name() == ANIMATION_KEY)
		{
			Datum& name = *animation.Find(ANIMATION_NAME_KEY);
			Datum& frames = *animation.Find(FRAME_KEY);
			Animation anim(frames.Size());
			for (std::uint32_t j = 0; j < frames.Size(); ++j)
			{
				anim.AddFrame(frames.Get<glm::vec4>(j));
			}
			player.GetSprite().AddAnimation(name.Get<std::string>(), anim);

		}
	}
	player.ChangeAnimation(IDLE_KEY);

	return player;
}

EntityMovable & LevelParser::CreateMovable(Entity * entity, float x, float y)
{
	EntityMovable& movable = *(entity->As<EntityMovable>());

	std::string textureName = entity->Find(TEXTURE_NAME_KEY)->Get<std::string>();

	float u = entity->Find(FRAME_X_KEY)->Get<float>();
	float v = entity->Find(FRAME_Y_KEY)->Get<float>();

	float width = entity->Find(WIDTH_KEY)->Get<float>();
	float height = entity->Find(HEIGHT_KEY)->Get<float>();

	movable.CreateSprite(textureName, u, v, width, height);

	movable.GetSprite().SetPosition(glm::vec2(x * TILE_WIDTH, y * TILE_HEIGHT));
	movable.GetSprite().SetAnimationTime(0.1f);

	Datum& actions = *(entity->Find(ACTIONS_KEY));
	if (&actions)
	{
		for (std::uint32_t i = 0; i < actions.Size(); ++i)
		{
			Action& animation = *(actions.Get<Scope*>(i)->As<Action>());
			if (&animation && animation.Name() == ANIMATION_KEY)
			{
				Datum& name = *animation.Find(ANIMATION_NAME_KEY);
				Datum& frames = *animation.Find(FRAME_KEY);
				Animation anim(frames.Size());
				for (std::uint32_t j = 0; j < frames.Size(); ++j)
				{
					anim.AddFrame(frames.Get<glm::vec4>(j));
				}
				movable.GetSprite().AddAnimation(name.Get<std::string>(), anim);
				movable.ChangeAnimation(name.Get<std::string>());
			}
		}
	}

	int harmful = entity->Find(HARMFUL_KEY)->Get<int>();
	movable.SetIsHarmful(harmful);

	float speedX = entity->Find(VELOCITY_X_KEY)->Get<float>();
	float speedY = entity->Find(VELOCITY_Y_KEY)->Get<float>();
	movable.SetSpeed(speedX, speedY);

	int positive = entity->Find(DIRECTION_KEY)->Get<int>();
	if (speedX != 0.0f)
		positive != 0 ? movable.SetIsPositiveX(true) : movable.SetIsPositiveX(false);

	if (speedY != 0.0f)
		positive != 0 ? movable.SetIsPositiveY(true) : movable.SetIsPositiveY(false);

	return movable;
}

LevelParser::~LevelParser()
{
}
