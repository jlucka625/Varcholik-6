#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(ActionContainer)

ActionContainer::ActionContainer() : Attributed()
{ 
	AddNestedScopeAttribute(ACTION_CONTAINER); 
	AddNestedScopeAttribute(REACTION_CONTAINER);
};

Datum & LibraryDesktop::ActionContainer::Actions()
{
	return (*this)[ACTION_CONTAINER];
}

Datum & LibraryDesktop::ActionContainer::Reactions()
{
	return (*this)[REACTION_CONTAINER];
}
