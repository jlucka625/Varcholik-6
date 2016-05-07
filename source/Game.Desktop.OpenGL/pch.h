#pragma once
#define OPEN_GL 1

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "HashMap.h"
#include "Common.h"
#include "Datum.h"
#include "RTTI.h"
#include "Scope.h"
#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"

#include "IntegerXmlParseHelper.h"
#include "FloatXmlParseHelper.h"
#include "VectorXmlParseHelper.h"
#include "MatrixXmlParseHelper.h"
#include "TableXmlParseHelper.h"
#include "StringXmlParseHelper.h"
#include "EntityXmlParseHelper.h"
#include "SectorXmlParseHelper.h"
#include "WorldXmlParseHelper.h"
#include "ActionXmlParseHelper.h"
#include "ReactionXmlParseHelper.h"
#include "TileXmlParseHelper.h"
#include "LevelConfigXmlParseHelper.h"

#include "FooSharedData.h"
#include "TableSharedData.h"
#include "SymbolSharedData.h"
#include "LevelSharedData.h"

#include "Factory.h"

#include "GameClock.h"
#include "GameTime.h"

#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"
#include "ActionContainer.h"

//#include "Enemy.h"

#include "Action.h"
#include "ActionList.h"

#include "ActionIf.h"
#include "ActionListWhile.h"
#include "ActionIncrement.h"
#include "ActionDecrement.h"
#include "ActionLoadSector.h"
#include "ActionUnloadSector.h"

#include "EventSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "EventQueue.h"

#include "Constants.h"
#include "ContactListener.h"

#include <Windows.h>
#include <exception>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "ActionMovePlayer.h"
#include "ActionFlipPlayer.h"
#include "ActionSpriteDraw.h"
#include "ActionSpriteUpdate.h"
#include "Animation.h"
#include "ResourceManager.h"
#include "fmod.hpp"
#include "SoundManager.h"
#include "Sprite.h"
#include "EntityTile.h"
#include "EntityPlayer.h"
#include "EntityInputHandler.h"
#include "EntityMovable.h"
#include "EntityText.h"
#include "EntityBackground.h"
#include "LevelParser.h"

#include <Box2D\Box2D.h>

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#if defined(OPENGL)
#include "gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/simd_mat4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/core/func_trigonometric.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#define DeleteObject(object) if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if((objects) != NULL) { delete[] objects; objects = NULL; }
