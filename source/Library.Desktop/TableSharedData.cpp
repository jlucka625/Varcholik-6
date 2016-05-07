#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(TableSharedData)

TableSharedData::TableSharedData() : XmlParseMaster::SharedData(), mScope(new Scope())
{
}

TableSharedData::~TableSharedData()
{
	while (mScope != nullptr)
	{
		Scope* parent = mScope->GetParent();
		delete mScope;
		mScope = parent;
	}
}

XmlParseMaster::SharedData* TableSharedData::Clone() const
{
	return new TableSharedData();
}

Scope & LibraryDesktop::TableSharedData::GetCurrentScope() const
{
	assert(mScope != nullptr);
	return *mScope;
}

void TableSharedData::Clear()
{
	mScope->Clear();
}
