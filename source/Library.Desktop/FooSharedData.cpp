#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(FooSharedData)

FooSharedData::FooSharedData() : XmlParseMaster::SharedData()
{
}

XmlParseMaster::SharedData* FooSharedData::Clone() const
{
	return new FooSharedData();
}
