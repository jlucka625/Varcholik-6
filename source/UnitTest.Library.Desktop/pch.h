#pragma once

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <type_traits>
#include <Windows.h>

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "CppUnitTest.h"
#include "DefaultIncrementFunctor.h"
#include "ZeroIncrementFunctor.h"
#include "DoubleSizeIncrementFunctor.h"
#include "Foo.h"
#include "CustomHashFunctor.h"
#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "HashMap.h"
#include "Attributed.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "Datum.h"
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

#include "FooSharedData.h"
#include "TableSharedData.h"

#include "Factory.h"

#include "GameClock.h"
#include "GameTime.h"

#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"
#include "ActionContainer.h"

#include "Enemy.h"

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

#include "IntEventSubscriber.h"
#include "FooEventSubscriber.h"
#include "AttributedFooEventSubscriber.h"

#include "Reaction.h"
