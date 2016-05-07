#include "pch.h"

using namespace UnitTestLibraryDesktop;
using namespace LibraryDesktop;

RTTI_DEFINITIONS(AttributedFoo)

AttributedFoo::AttributedFoo() : Attributed(), health(0), money(0.0f), identity(), position(glm::vec4(0, 0, 0, 0)), currentFieldGrid(glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)), dummyFoo(13), pointer(&dummyFoo)
{
	AddInternalAttribute("Stats", 0, 4);
	AddInternalAttribute("DamageModifiers", 1.0f, 3);
	AddInternalAttribute("Powers", "", 6);
	AddNestedScopeAttribute("OuterScope", 1);
	AddInternalAttribute("ItemDrops", glm::vec4(0, 0, 0, 0), 2);
	AddInternalAttribute("Map", glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
	AddInternalAttribute("TestFoo", &dummyFoo);

	AddExternalAttribute("Health", 1, &health);
	AddExternalAttribute("Money", 1, &money);
	AddExternalAttribute("Position", 1, &position);
	AddExternalAttribute("CurrentFieldGrid", 1, &currentFieldGrid);
	AddExternalAttribute("Name", 1, &identity);
	AddExternalAttribute("Pointer", 1, &pointer);
}

AttributedFoo::AttributedFoo(const AttributedFoo& otherAFoo) : Attributed(otherAFoo), health(otherAFoo.health), money(otherAFoo.money), identity(otherAFoo.identity), 
position(otherAFoo.position), currentFieldGrid(otherAFoo.currentFieldGrid), dummyFoo(otherAFoo.dummyFoo), pointer(otherAFoo.pointer)
{
	UpdateExternalStorage("Health", 1, &health);
	UpdateExternalStorage("Money", 1, &money);
	UpdateExternalStorage("Position", 1, &position);
	UpdateExternalStorage("CurrentFieldGrid", 1, &currentFieldGrid);
	UpdateExternalStorage("Name", 1, &identity);
	UpdateExternalStorage("Pointer", 1, &pointer);
}

AttributedFoo::AttributedFoo(AttributedFoo&& otherAFoo) : Attributed(std::move(otherAFoo)), health(otherAFoo.health), money(otherAFoo.money), identity(otherAFoo.identity),
position(otherAFoo.position), currentFieldGrid(otherAFoo.currentFieldGrid), dummyFoo(std::move(otherAFoo.dummyFoo)), pointer(otherAFoo.pointer)
{
	UpdateExternalStorage("Health", 1, &health);
	UpdateExternalStorage("Money", 1, &money);
	UpdateExternalStorage("Position", 1, &position);
	UpdateExternalStorage("CurrentFieldGrid", 1, &currentFieldGrid);
	UpdateExternalStorage("Name", 1, &identity);
	UpdateExternalStorage("Pointer", 1, &pointer);

	otherAFoo.health = 0;
	otherAFoo.money = 0.0f;
	otherAFoo.position = glm::vec4(0, 0, 0, 0);
	otherAFoo.currentFieldGrid = glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	otherAFoo.identity = "";
	otherAFoo.pointer = nullptr;
}

AttributedFoo& AttributedFoo::operator=(const AttributedFoo& otherAFoo)
{
	Attributed::operator=(otherAFoo);

	if (this != &otherAFoo)
	{
		health = otherAFoo.health;
		money = otherAFoo.money;
		identity = otherAFoo.identity;
		position = otherAFoo.position;
		currentFieldGrid = otherAFoo.currentFieldGrid;
		dummyFoo = otherAFoo.dummyFoo;
		pointer = otherAFoo.pointer;

		UpdateExternalStorage("Health", 1, &health);
		UpdateExternalStorage("Money", 1, &money);
		UpdateExternalStorage("Position", 1, &position);
		UpdateExternalStorage("CurrentFieldGrid", 1, &currentFieldGrid);
		UpdateExternalStorage("Name", 1, &identity);
		UpdateExternalStorage("Pointer", 1, &pointer);
	}

	return *this;
}

AttributedFoo& AttributedFoo::operator=(AttributedFoo&& otherAFoo)
{
	Attributed::operator=(std::move(otherAFoo));

	if (this != &otherAFoo)
	{
		health = otherAFoo.health;
		money = otherAFoo.money;
		identity = otherAFoo.identity;
		position = otherAFoo.position;
		currentFieldGrid = otherAFoo.currentFieldGrid;
		dummyFoo = std::move(otherAFoo.dummyFoo);
		pointer = otherAFoo.pointer;

		UpdateExternalStorage("Health", 1, &health);
		UpdateExternalStorage("Money", 1, &money);
		UpdateExternalStorage("Position", 1, &position);
		UpdateExternalStorage("CurrentFieldGrid", 1, &currentFieldGrid);
		UpdateExternalStorage("Name", 1, &identity);
		UpdateExternalStorage("Pointer", 1, &pointer);

		otherAFoo.health = 0;
		otherAFoo.money = 0.0f;
		otherAFoo.position = glm::vec4(0, 0, 0, 0);
		otherAFoo.currentFieldGrid = glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		otherAFoo.identity = "";
		otherAFoo.pointer = nullptr;
	}

	return *this;
}

AttributedFoo::~AttributedFoo()
{
}
