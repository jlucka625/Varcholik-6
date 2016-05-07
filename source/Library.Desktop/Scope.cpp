#include "pch.h"

RTTI_DEFINITIONS(Scope)

using namespace LibraryDesktop;

Scope::Scope(const std::uint32_t& size) : mSymbolTable(size), mOrderVector(), mParent(nullptr), mParentName("")
{
}

Scope::Scope(const std::string& name, const std::uint32_t& size) : mSymbolTable(size), mOrderVector(), mParent(nullptr), mParentName(name)
{
}

Scope::Scope(const Scope& otherScope) : mParent(nullptr), mParentName("")
{
	operator=(otherScope);
}

Scope::Scope(Scope&& otherScope) : mParent(otherScope.mParent), mParentName(otherScope.mParentName), mSymbolTable(std::move(otherScope.mSymbolTable)), mOrderVector(std::move(otherScope.mOrderVector))
{
	Reparent();
	otherScope.mParent = nullptr;
	otherScope.mParentName = "";
}

Scope& Scope::operator=(const Scope& otherScope)
{
	if (this != &otherScope)
	{
		Orphan();
		Clear();

		for (auto& pair : otherScope.mOrderVector)
		{
			if (pair->second.Type() == Datum::DatumType::Table)
			{
				for (std::uint32_t i = 0; i < pair->second.Size(); i++)
				{
					Scope& appended = AppendScope(pair->first);
					Scope* parentCopy = appended.mParent;
					appended.mParent = nullptr;
					appended = pair->second[i];
					appended.mParent = parentCopy;
				}
			}
			else
			{
				bool wasAppended;
				Datum& appended = Append(pair->first, wasAppended);
				appended = pair->second;
			}
		}
	}

	return *this;
}

Scope& Scope::operator=(Scope&& otherScope)
{
	if (this != &otherScope)
	{
		Orphan();
		Clear();

		mParent = otherScope.mParent;
		mParentName = otherScope.mParentName;
		mSymbolTable = std::move(otherScope.mSymbolTable);
		mOrderVector = std::move(otherScope.mOrderVector);

		Reparent();

		otherScope.mParent = nullptr;
		otherScope.mParentName = "";
	}

	return *this;
}

Scope::~Scope()
{
	Orphan();
	Clear();
}

bool Scope::operator==(const Scope& otherScope) const
{
	if (this == &otherScope)
	{
		return true;
	}

	if (mOrderVector.Size() != otherScope.mOrderVector.Size())
	{
		return false;
	}

	for (std::uint32_t i = 0; i < mOrderVector.Size(); i++)
	{
		if ((mOrderVector[i]->first != "this") && (otherScope.mOrderVector[i]->first != "this") && *(mOrderVector[i]) != *(otherScope.mOrderVector[i]))
		{
			return false;
		}
	}

	return true;
}

bool Scope::operator!=(const Scope& otherScope) const
{
	return !(operator==(otherScope));
}

std::uint32_t Scope::Size() const
{
	return mSymbolTable.Size();
}

std::uint32_t Scope::Capacity() const
{
	return mSymbolTable.Capacity();
}

Datum* Scope::Find(const std::string& name) const
{
	SymbolTable::Iterator found = mSymbolTable.Find(name);

	if (found != mSymbolTable.end())
	{
		return &((*found).second);
	}
	else
	{
		return nullptr;
	}
}

Datum* Scope::Search(const std::string& name, Scope** scope)
{
	SymbolTable::Iterator found = mSymbolTable.Find(name);

	if (found == mSymbolTable.end())
	{
		if (mParent == nullptr)
		{
			return nullptr;
		}
		else
		{
			return mParent->Search(name, scope);
		}
	}
	else
	{
		if (scope != nullptr)
		{
			*scope = this;
		}

		return &((*found).second);
	}
}

Datum& Scope::Append(const std::string& name, bool& wasAppended)
{
	if (name == "")
	{
		throw std::exception("Error: cannot use an empty string as the key");
	}

	bool wasInserted;
	SymbolTable::Iterator insertion = mSymbolTable.Insert(SymbolTable::PairType(name, Datum()), wasInserted);
	wasAppended = wasInserted;

	if (wasInserted)
	{
		mOrderVector.PushBack(&(*insertion));
	}

	return (*insertion).second;
}

Datum& Scope::Append(const std::string & name)
{
	bool wasAppended;

	return Append(name, wasAppended);
}

Scope* Scope::GetParent()
{
	return mParent;
}

Datum& Scope::operator[](const std::string& name)
{
	bool wasAppended;
	return Append(name, wasAppended);
}

Datum & LibraryDesktop::Scope::operator[](const std::string & name) const
{
	Datum* result = Find(name);

	if (result == nullptr)
	{
		throw std::exception("Error: a Datum with this name does not exist");
	}

	return *result;
}

Datum& Scope::operator[](const std::uint32_t& index)
{
	return (mOrderVector[index]->second);
}

Scope& Scope::AppendScope(const std::string& name)
{
	if (name == "")
	{
		throw std::exception("Error: cannot use an empty string as the key");
	}

	bool wasAppended;
	Datum& newScopeDatum = Append(name, wasAppended);
	newScopeDatum.SetType(Datum::DatumType::Table);

	Scope* newScope = new Scope(name);
	newScope->mParent = this;
	newScopeDatum.PushBack(*newScope);

	return *newScope;
}

void Scope::Adopt(Scope& scope, const std::string& name)
{
	if (this != &scope)
	{
		bool wasAppended;
		Datum& targetScope = Append(name, wasAppended);

		scope.Orphan();

		scope.mParent = this;
		scope.mParentName = name;
		targetScope.PushBack(scope);
	}
}

std::string Scope::FindName(const Scope& otherScope) const
{
	std::string found = "";

	for (const auto& pair : mOrderVector)
	{
		if (pair->second.Type() == Datum::DatumType::Table)
		{
			for (std::uint32_t i = 0; i < pair->second.Size(); i++)
			{
				if (pair->second.Get<Scope*>(i) == &otherScope)
				{
					found = pair->first;
					break;
				}
			}
		}
	}

	return found;
}

void Scope::Reparent()
{
	for (auto& pair : mOrderVector)
	{
		if (pair->second.Type() == Datum::DatumType::Table)
		{
			for (std::uint32_t i = 0; i < pair->second.Size(); i++)
			{
				pair->second.Get<Scope*>(i)->mParent = this;
			}
		}
	}
}

void Scope::Orphan()
{
	if (mParent != nullptr)
	{
		Datum* found = mParent->Find(mParentName);

		if (found != nullptr)
		{
			found->Remove(*this);
			mParent = nullptr;
			mParentName = "";
		}
	}
}

void Scope::Clear()
{
	for (auto& pair : mOrderVector)
	{
		if (pair->second.Type() == Datum::DatumType::Table)
		{
			for (std::uint32_t i = 0; i < pair->second.Size(); i++)
			{
				Scope* currentScope = pair->second.Get<Scope*>(i);
				assert(currentScope->mParent == this);
				currentScope->mParent = nullptr;
				delete currentScope;
			}
		}
	}

	mSymbolTable.Clear();
	mOrderVector.Clear();
}
