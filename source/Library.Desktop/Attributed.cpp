#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(Attributed)

const std::string Attributed::ACTION_CONTAINER = "Actions";
const std::string Attributed::ENTITY_CONTAINER = "Entities";
const std::string Attributed::SECTOR_CONTAINER = "Sectors";
const std::string Attributed::REACTION_CONTAINER = "Reactions";

Attributed::Attributed() : Scope(), mNumPrescribedSigs(0)
{
	(*this)["this"] = this;
	++mNumPrescribedSigs;
}

Attributed::Attributed(const Attributed& otherAttribute) : Scope(otherAttribute), mNumPrescribedSigs(otherAttribute.mNumPrescribedSigs)
{
	Datum* thisDatum = Find("this");
	*thisDatum = this;
}

Attributed::Attributed(Attributed&& otherAttribute) : Scope(std::move(otherAttribute)), mNumPrescribedSigs(otherAttribute.mNumPrescribedSigs)
{
	Datum* thisDatum = Find("this");
	*thisDatum = this;

	otherAttribute.mNumPrescribedSigs = 0;
}

Attributed& Attributed::operator=(const Attributed& otherAttribute)
{
	if (this != &otherAttribute)
	{
		Scope::operator=(otherAttribute);

		Datum* thisDatum = Find("this");
		*thisDatum = this;

		mNumPrescribedSigs = otherAttribute.mNumPrescribedSigs;
	}

	return *this;
}

Attributed& Attributed::operator=(Attributed&& otherAttribute)
{
	Scope::operator=(std::move(otherAttribute));

	if (this != &otherAttribute)
	{
		Datum* thisDatum = Find("this");
		*thisDatum = this;

		otherAttribute.mNumPrescribedSigs = 0;
	}

	return *this;
}

Attributed::~Attributed()
{
}

bool Attributed::operator==(const Attributed & otherAttribute) const
{
	if (Size() != otherAttribute.Size())
	{
		return false;
	}

	return Scope::operator==(otherAttribute);
}

bool Attributed::operator!=(const Attributed & otherAttribute) const
{
	return !(operator==(otherAttribute));
}

void Attributed::AddInternalAttribute(const std::string& name, int initialValue, std::uint32_t size)
{
	Datum& attribute = AppendDatumForAttribute(name);

	for (std::uint32_t i = 0; i < size; i++)
	{
		attribute.PushBack(initialValue);
	}
}

void Attributed::AddInternalAttribute(const std::string& name, float initialValue, std::uint32_t size)
{
	Datum& attribute = AppendDatumForAttribute(name);

	for (std::uint32_t i = 0; i < size; i++)
	{
		attribute.PushBack(initialValue);
	}
}

void Attributed::AddInternalAttribute(const std::string& name, const glm::vec4& initialValue, std::uint32_t size)
{
	Datum& attribute = AppendDatumForAttribute(name);

	for (std::uint32_t i = 0; i < size; i++)
	{
		attribute.PushBack(initialValue);
	}
}

void Attributed::AddInternalAttribute(const std::string& name, const glm::mat4& initialValue, std::uint32_t size)
{
	Datum& attribute = AppendDatumForAttribute(name);

	for (std::uint32_t i = 0; i < size; i++)
	{
		attribute.PushBack(initialValue);
	}
}

void Attributed::AddNestedScopeAttribute(const std::string& name, std::uint32_t size)
{
	if (size == 0)
	{
		Datum& emptyScopeDatum = Append(name);
		emptyScopeDatum.SetType(Datum::DatumType::Table);
	}
	else
	{
		for (std::uint32_t i = 0; i < size; i++)
		{
			AppendScope(name);
		}
	}

	++mNumPrescribedSigs;
}

void Attributed::AddInternalAttribute(const std::string& name, const std::string& initialValue, std::uint32_t size)
{
	Datum& attribute = AppendDatumForAttribute(name);

	for (std::uint32_t i = 0; i < size; i++)
	{
		attribute.PushBack(initialValue);
	}
}

void Attributed::AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size)
{
	Datum& attribute = AppendDatumForAttribute(name);

	for (std::uint32_t i = 0; i < size; i++)
	{
		attribute.PushBack(initialValue);
	}
}

void Attributed::AddExternalAttribute(const std::string & name, std::uint32_t size, int* externalValues)
{
	Datum& attribute = AppendDatumForAttribute(name);
	attribute.SetStorage(externalValues, size);
}

void Attributed::AddExternalAttribute(const std::string & name, std::uint32_t size, float* externalValues)
{
	Datum& attribute = AppendDatumForAttribute(name);
	attribute.SetStorage(externalValues, size);
}

void Attributed::AddExternalAttribute(const std::string & name, std::uint32_t size, glm::vec4* externalValues)
{
	Datum& attribute = AppendDatumForAttribute(name);
	attribute.SetStorage(externalValues, size);
}

void Attributed::AddExternalAttribute(const std::string & name, std::uint32_t size, glm::mat4* externalValues)
{
	Datum& attribute = AppendDatumForAttribute(name);
	attribute.SetStorage(externalValues, size);
}

void Attributed::AddExternalAttribute(const std::string & name, std::uint32_t size, std::string* externalValues)
{
	Datum& attribute = AppendDatumForAttribute(name);
	attribute.SetStorage(externalValues, size);
}

void Attributed::AddExternalAttribute(const std::string & name, std::uint32_t size, RTTI** externalValues)
{
	Datum& attribute = AppendDatumForAttribute(name);
	attribute.SetStorage(externalValues, size);
}

void Attributed::UpdateExternalStorage(const std::string& name, std::uint32_t size, int* externalValues)
{
	Find(name)->SetStorage(externalValues, size);
}

void Attributed::UpdateExternalStorage(const std::string& name, std::uint32_t size, float* externalValues)
{
	Find(name)->SetStorage(externalValues, size);
}

void Attributed::UpdateExternalStorage(const std::string& name, std::uint32_t size, glm::vec4* externalValues)
{
	Find(name)->SetStorage(externalValues, size);
}

void Attributed::UpdateExternalStorage(const std::string& name, std::uint32_t size, glm::mat4* externalValues)
{
	Find(name)->SetStorage(externalValues, size);
}

void Attributed::UpdateExternalStorage(const std::string& name, std::uint32_t size, std::string* externalValues)
{
	Find(name)->SetStorage(externalValues, size);
}

void Attributed::UpdateExternalStorage(const std::string& name, std::uint32_t size, RTTI** externalValues)
{
	Find(name)->SetStorage(externalValues, size);
}

void Attributed::ReceiveAuxiliaryAttributes(Attributed& otherAttributed)
{
	otherAttributed.DistributeAuxiliaryAttributes(*(const_cast<Attributed*>(this)));
}

void Attributed::DistributeAuxiliaryAttributes(Attributed& otherAttributed)
{
	for (std::uint32_t i = AuxiliaryBegin(); i < Size(); ++i)
	{
		SymbolTable::PairType& pair = *(mOrderVector[i]);
		otherAttributed[pair.first] = pair.second;
	}
}

bool Attributed::IsPrescribedAttribute(const std::string& name) const
{
	if (name == "this")
	{
		return true;
	}

	std::uint32_t index = 0;

	return ((GetSignature(name, index) != nullptr) && (index < mNumPrescribedSigs));
}

bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
{
	return (IsAttribute(name) && !(IsPrescribedAttribute(name)));
}

bool Attributed::IsAttribute(const std::string& name) const
{
	return (Find(name) != nullptr);
}

Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
{
	if (IsPrescribedAttribute(name))
	{
		throw std::exception("Error: this is the name of a prescribed attribute");
	}

	Datum& newAttribute = Append(name);
	return newAttribute;
}

const std::uint32_t Attributed::AuxiliaryBegin() const
{
	return mNumPrescribedSigs;
}

Datum* Attributed::GetSignature(const std::string& name, std::uint32_t& sigIndex) const
{	
	for (Vector<SymbolTable::PairType*>::Iterator it = mOrderVector.begin(); it != mOrderVector.end(); ++it)
	{
		SymbolTable::PairType* pair = (*it);

		if (pair->first == name)
		{
			return &(pair->second);
		}

		++sigIndex;
	}

	return nullptr;
}

Datum& Attributed::AppendDatumForAttribute(const std::string & name)
{
	Datum& attribute = Append(name);
	++mNumPrescribedSigs;

	return attribute;
}
