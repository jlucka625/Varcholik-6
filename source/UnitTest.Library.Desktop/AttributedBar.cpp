#include "pch.h"

using namespace UnitTestLibraryDesktop;
using namespace LibraryDesktop;

RTTI_DEFINITIONS(AttributedBar)

AttributedBar::AttributedBar() : AttributedFoo(), classID(0), description()
{
	AddExternalAttribute("ClassID", 1, &classID);
	AddExternalAttribute("Description", 1, &description);
}

AttributedBar::~AttributedBar()
{
}
