#include "pch.h"

#include <algorithm>

using namespace LibraryDesktop;

const Datum::ToStringFunc Datum::toStringFuncTable[static_cast<std::uint32_t>(Datum::DatumType::MAX)] = 
{
	[](Datum& datum, const std::uint32_t& index = 0){ UNREFERENCED_PARAMETER(datum); UNREFERENCED_PARAMETER(index); return ""; },
	[](Datum& datum, const std::uint32_t& index = 0){ return std::to_string(datum.Get<int>(index)); },
	[](Datum& datum, const std::uint32_t& index = 0){ return std::to_string(datum.Get<float>(index)); },
	[](Datum& datum, const std::uint32_t& index = 0){ return glm::to_string(datum.Get<glm::vec4>(index)); },
	[](Datum& datum, const std::uint32_t& index = 0){ return glm::to_string(datum.Get<glm::mat4>(index)); },
	[](Datum& datum, const std::uint32_t& index = 0){ UNREFERENCED_PARAMETER(datum); UNREFERENCED_PARAMETER(index); return ""; },
	[](Datum& datum, const std::uint32_t& index = 0){ return datum.Get<std::string>(index); },
	[](Datum& datum, const std::uint32_t& index = 0){ return ((datum.Get<RTTI*>(index))->ToString()); }
};

const Datum::SetSizeFunc Datum::setSizeFuncTable[static_cast<std::uint32_t>(Datum::DatumType::MAX)] =
{
	nullptr,
	&Resize<int>,
	&Resize<float>,
	&Resize<glm::vec4>,
	&Resize<glm::mat4>,
	&Resize<Scope*>,
	&Resize<std::string>,
	&Resize<RTTI*>
};

const Datum::SetFromStringFunc Datum::setFromStringFuncTable[static_cast<std::uint32_t>(Datum::DatumType::MAX)] = 
{
	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) { UNREFERENCED_PARAMETER(datum); UNREFERENCED_PARAMETER(value); UNREFERENCED_PARAMETER(index); },
	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) { datum.Set(stoi(value), index); },
	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) { datum.Set(stof(value), index); },

	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) 
	{ 
		glm::vec4 tempVec;

		sscanf_s(value.c_str(), "%f%f%f%f", &tempVec.x, &tempVec.y, &tempVec.z, &tempVec.w);
		datum.Set(tempVec, index);
	},

	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) 
	{ 
		glm::mat4 tempMat4;

		sscanf_s(value.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
			&tempMat4[0][0], &tempMat4[0][1], &tempMat4[0][2], &tempMat4[0][3], 
			&tempMat4[1][0], &tempMat4[1][1], &tempMat4[1][2], &tempMat4[1][3],
			&tempMat4[2][0], &tempMat4[2][1], &tempMat4[2][2], &tempMat4[2][3],
			&tempMat4[3][0], &tempMat4[3][1], &tempMat4[3][2], &tempMat4[3][3]
		);

		datum.Set(tempMat4, index);
	},

	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) { UNREFERENCED_PARAMETER(datum); UNREFERENCED_PARAMETER(value); UNREFERENCED_PARAMETER(index); },
	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) { datum.Set(value, index); },
	[](Datum& datum, const std::string& value, const std::uint32_t& index = 0) { UNREFERENCED_PARAMETER(datum); UNREFERENCED_PARAMETER(value); UNREFERENCED_PARAMETER(index); }
};

const Datum::DeepCopyFunc Datum::deepCopyFuncTable[static_cast<std::uint32_t>(Datum::DatumType::MAX)] =
{
	[&](Datum& lhs, const Datum& rhs) { UNREFERENCED_PARAMETER(lhs); UNREFERENCED_PARAMETER(rhs); },

	[&](Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			lhs.Set(rhs.Get<int>(i), i);
		}
	},

	[&](Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			lhs.Set(rhs.Get<float>(i), i);
		}
	},

	[&](Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			lhs.Set(rhs.Get<glm::vec4>(i), i);
		}
	},

	[&](Datum& lhs, const Datum& rhs)
	{
		lhs.SetSize(lhs.Capacity());

		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			lhs.Set(rhs.Get<glm::mat4>(i), i);
		}
	},

	[&](Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			lhs = rhs.Get<Scope*>(i);
		}
	},

	[&](Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			lhs.Set(rhs.Get<std::string>(i), i);
		}
	},

	[&](Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			lhs.Set(rhs.Get<RTTI*>(i), i);
		}
	}
};

const Datum::EqualityFunc Datum::equalityFuncTable[static_cast<std::uint32_t>(Datum::DatumType::MAX)] =
{
	[&](const Datum& lhs, const Datum& rhs) { UNREFERENCED_PARAMETER(lhs); UNREFERENCED_PARAMETER(rhs); return true; },

	[&](const Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			if (lhs.Get<int>(i) != rhs.Get<int>(i))
			{
				return false;
			}
		}

		return true;
	},

	[&](const Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			if (lhs.Get<float>(i) != rhs.Get<float>(i))
			{
				return false;
			}
		}

		return true;
	},

	[&](const Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			if (lhs.Get<glm::vec4>(i) != rhs.Get<glm::vec4>(i))
			{
				return false;
			}
		}

		return true;
	},

	[&](const Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			if (lhs.Get<glm::mat4>(i) != rhs.Get<glm::mat4>(i))
			{
				return false;
			}
		}

		return true;
	},

	[&](const Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			if (!(lhs[i] == rhs[i]))
			{
				return false;
			}
		}

		return true;
	},

	[&](const Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			if (lhs.Get<std::string>(i) != rhs.Get<std::string>(i))
			{
				return false;
			}
		}

		return true;
	},

	[&](const Datum& lhs, const Datum& rhs)
	{
		for (std::uint32_t i = 0; i < lhs.Size(); i++)
		{
			if (lhs.Get<RTTI*>(i) != rhs.Get<RTTI*>(i))
			{
				return false;
			}
		}

		return true;
	}
};

Datum::Datum() : mType(DatumType::Unknown), mCapacity(0), mSize(0), mIsExternal(false), 
mData(DatumValues::DatumValues())
{
}

Datum::Datum(const Datum& otherDatum) : mType(DatumType::Unknown), mIsExternal(false),
mData(DatumValues::DatumValues())
{
	operator=(otherDatum);
}

Datum::Datum(Datum && otherDatum) : mType(otherDatum.mType), mIsExternal(otherDatum.mIsExternal), mSize(otherDatum.mSize), mCapacity(otherDatum.mCapacity),
mData(otherDatum.mData)
{
	otherDatum.mType = DatumType::Unknown;
	otherDatum.mIsExternal = false;
	otherDatum.mSize = otherDatum.mCapacity = 0;
}

Datum& Datum::operator=(const Datum& otherDatum)
{
	if (this != &otherDatum)
	{
		SetType(otherDatum.mType);

		if (!mIsExternal)
		{
			Clear();
		}
		else
		{
			mData.Clear();
			mSize = mCapacity = 0;
		}

		SetType(otherDatum.mType);
		mIsExternal = otherDatum.mIsExternal;

		if (mType != DatumType::Unknown)
		{
			if (!mIsExternal)
			{
				SetSize(otherDatum.mCapacity);
			}
			else
			{
				mCapacity = otherDatum.mCapacity;
				mSize = otherDatum.mSize;
			}

			if (otherDatum.mIsExternal)
			{
				mData = otherDatum.mData;
			}
			else
			{
				deepCopyFuncTable[static_cast<std::uint32_t>(mType)](*this, otherDatum);
			}
		}
		else
		{
			mSize = mCapacity = 0;
		}
	}

	return *this;
}

Datum& Datum::operator=(Datum && otherDatum)
{
	if (this != &otherDatum)
	{
		mSize = otherDatum.mSize;
		mCapacity = otherDatum.mCapacity;
		mIsExternal = otherDatum.mIsExternal;
		mData = otherDatum.mData;
		mType = otherDatum.mType;

		otherDatum.mSize = 0;
		otherDatum.mCapacity = 0;
		otherDatum.mIsExternal = 0;
		otherDatum.mType = DatumType::Unknown;
	}

	return *this;
}

Datum::~Datum()
{
	Clear();
}

Datum& Datum::operator=(const int & data)
{
	Set(data, 0);
	return *this;
}

Datum& Datum::operator=(const float & data)
{
	Set(data, 0);
	return *this;
}

Datum& Datum::operator=(const glm::vec4 & data)
{
	Set(data, 0);
	return *this;
}

Datum& Datum::operator=(const glm::mat4 & data)
{
	Set(data, 0);
	return *this;
}

Datum& Datum::operator=(Scope& data)
{
	Set(data, 0);
	return *this;
}

Datum& Datum::operator=(const std::string & data)
{
	Set(data, 0);
	return *this;
}

Datum& Datum::operator=(RTTI* const& data)
{
	Set(data, 0);
	return *this;
}

template<> inline int& Datum::Get<int>(const std::uint32_t& index) const
{
	if (mType != DatumType::Integer)
	{
		throw std::exception("Error: this Datum is not of type Integer, so an int value cannot be obtained");
	}
	else if (index >= mSize)
	{
		throw std::exception("Error: index is out of bounds");
	}

	return mData.mIntArray[index];
}

template<> inline float& Datum::Get<float>(const std::uint32_t& index) const
{
	if (mType != DatumType::Float)
	{
		throw std::exception("Error: this Datum is not of type Float, so a float value cannot be obtained");
	}
	else if (index >= mSize)
	{
		throw std::exception("Error: index is out of bounds");
	}

	return mData.mFloatArray[index];
}

template<> inline glm::vec4& Datum::Get<glm::vec4>(const std::uint32_t& index) const
{
	if (mType != DatumType::Vector)
	{
		throw std::exception("Error: this Datum is not of type Vector, so a glm::vec4 value cannot be obtained");
	}
	else if (index >= mSize)
	{
		throw std::exception("Error: index is out of bounds");
	}

	return mData.mVectorArray[index];
}

template<> inline glm::mat4& Datum::Get<glm::mat4>(const std::uint32_t& index) const
{
	if (mType != DatumType::Matrix)
	{
		throw std::exception("Error: this Datum is not of type Matrix, so a glm::mat4 value cannot be obtained");
	}
	else if (index >= mSize)
	{
		throw std::exception("Error: index is out of bounds");
	}

	return mData.mMatrixArray[index];
}

template<> inline Scope*& Datum::Get<Scope*>(const std::uint32_t& index) const
{
	if (mType != DatumType::Table)
	{
		throw std::exception("Error: this Datum is not of type Table, so a Scope* value cannot be obtained");
	}
	else if (index >= mSize)
	{
		throw std::exception("Error: index is out of bounds");
	}

	return mData.mTableArray[index];
}

template<> inline std::string& Datum::Get<std::string>(const std::uint32_t& index) const
{
	if (mType != DatumType::String)
	{
		throw std::exception("Error: this Datum is not of type String, so an std::string value cannot be obtained");
	}
	if (index >= mSize)
	{
		throw std::exception("Error: index is out of bounds");
	}

	return mData.mStringArray[index];
}

template<> inline RTTI*& Datum::Get<RTTI*>(const std::uint32_t& index) const
{
	if (mType != DatumType::Pointer)
	{
		throw std::exception("Error: this Datum is not of type Pointer, so an RTTI* value cannot be obtained");
	}
	if (index >= mSize)
	{
		throw std::exception("Error: index is out of bounds");
	}

	return mData.mPointerArray[index];
}

template<> inline void Datum::Resize<int>(Datum& datum, const std::uint32_t& size)
{
	if (datum.mCapacity < size)
	{
		datum.mCapacity = size;

		int* newIntArray = new int[datum.mCapacity];

		if (datum.mData.mIntArray != nullptr)
		{
			for (std::uint32_t i = 0; i < datum.mSize; i++)
			{
				newIntArray[i] = datum.mData.mIntArray[i];
			}

			delete[] datum.mData.mIntArray;
		}

		datum.mData.mIntArray = newIntArray;
	}

	datum.mSize = size;
}

template<> inline void Datum::Resize<float>(Datum& datum, const std::uint32_t& size)
{
	if (datum.mCapacity < size)
	{
		datum.mCapacity = size;

		float* newFloatArray = new float[datum.mCapacity];

		if (datum.mData.mFloatArray != nullptr)
		{
			for (std::uint32_t i = 0; i < datum.mSize; i++)
			{
				newFloatArray[i] = datum.mData.mFloatArray[i];
			}

			delete[] datum.mData.mFloatArray;
		}

		datum.mData.mFloatArray = newFloatArray;
	}

	datum.mSize = size;
}

template<> inline void Datum::Resize<glm::vec4>(Datum& datum, const std::uint32_t& size)
{
	if (datum.mCapacity < size)
	{
		datum.mCapacity = size;

		glm::vec4* newVectorArray = new glm::vec4[size];

		if (datum.mData.mVectorArray != nullptr)
		{
			for (std::uint32_t i = 0; i < datum.mSize; i++)
			{
				newVectorArray[i] = datum.mData.mVectorArray[i];
			}

			delete[] datum.mData.mVectorArray;
		}

		datum.mData.mVectorArray = newVectorArray;
	}

	datum.mSize = size;
}

template<> inline void Datum::Resize<glm::mat4>(Datum& datum, const std::uint32_t& size)
{
	if (datum.mCapacity < size)
	{
		datum.mCapacity = size;

		glm::mat4* newMatrixArray = new glm::mat4[size];

		if (datum.mData.mMatrixArray != nullptr)
		{
			for (std::uint32_t i = 0; i < datum.mSize; i++)
			{
				newMatrixArray[i] = datum.mData.mMatrixArray[i];
			}

			delete[] datum.mData.mMatrixArray;
		}

		datum.mData.mMatrixArray = newMatrixArray;
	}

	datum.mSize = size;
}

template<> inline void Datum::Resize<Scope*>(Datum& datum, const std::uint32_t& size)
{
	if (datum.mCapacity < size)
	{
		datum.mCapacity = size;

		Scope** newTableArray = new Scope*[size];

		if (datum.mData.mTableArray != nullptr)
		{
			for (std::uint32_t i = 0; i < datum.mSize; i++)
			{
				newTableArray[i] = datum.mData.mTableArray[i];
			}

			delete[] datum.mData.mTableArray;
		}

		datum.mData.mTableArray = newTableArray;
	}

	datum.mSize = size;
}

template<> inline void Datum::Resize<std::string>(Datum& datum, const std::uint32_t& size)
{
	if (datum.mCapacity < size)
	{
		datum.mCapacity = size;

		std::string* newStringArray = new std::string[size];

		if (datum.mData.mStringArray != nullptr)
		{
			for (std::uint32_t i = 0; i < datum.mSize; i++)
			{
				newStringArray[i] = datum.mData.mStringArray[i];
			}

			delete[] datum.mData.mStringArray;
		}

		datum.mData.mStringArray = newStringArray;
	}

	datum.mSize = size;
}

template<> inline void Datum::Resize<RTTI*>(Datum& datum, const std::uint32_t& size)
{
	if (datum.mCapacity < size)
	{
		datum.mCapacity = size;

		RTTI** newPointerArray = new RTTI*[size];

		if (datum.mData.mPointerArray != nullptr)
		{
			for (std::uint32_t i = 0; i < datum.mSize; i++)
			{
				newPointerArray[i] = datum.mData.mPointerArray[i];
			}

			delete[] datum.mData.mPointerArray;
		}

		datum.mData.mPointerArray = newPointerArray;
	}

	datum.mSize = size;
}

Scope& Datum::operator[](std::uint32_t index)
{
	return *Get<Scope*>(index);
}

const Scope& LibraryDesktop::Datum::operator[](std::uint32_t index) const
{
	return *Get<Scope*>(index);
}

std::uint32_t Datum::PushBack(const int & value)
{
	if (mType != DatumType::Integer)
	{
		SetType(DatumType::Integer);
	}

	SetSize((mSize + 1));
	mData.mIntArray[mSize - 1] = value;
	return (mSize - 1);
}

std::uint32_t Datum::PushBack(const float & value)
{
	if (mType != DatumType::Float)
	{
		SetType(DatumType::Float);
	}

	SetSize((mSize + 1));
	mData.mFloatArray[mSize - 1] = value;
	return (mSize - 1);
}

std::uint32_t Datum::PushBack(const glm::vec4 & value)
{
	if (mType != DatumType::Vector)
	{
		SetType(DatumType::Vector);
	}

	SetSize((mSize + 1));
	mData.mVectorArray[mSize - 1] = value;
	return (mSize - 1);
}

std::uint32_t Datum::PushBack(const glm::mat4 & value)
{
	if (mType != DatumType::Matrix)
	{
		SetType(DatumType::Matrix);
	}

	SetSize((mSize + 1));
	mData.mMatrixArray[mSize - 1] = value;
	return (mSize - 1);
}

std::uint32_t Datum::PushBack(Scope& value)
{
	if (mType != DatumType::Table)
	{
		SetType(DatumType::Table);
	}

	SetSize((mSize + 1));
	mData.mTableArray[mSize - 1] = &value;
	return (mSize - 1);
}

std::uint32_t Datum::PushBack(const std::string & value)
{
	if (mType != DatumType::String)
	{
		SetType(DatumType::String);
	}

	SetSize((mSize + 1));
	mData.mStringArray[mSize - 1] = value;
	return (mSize - 1);
}

std::uint32_t Datum::PushBack(RTTI * const & value)
{
	if (mType != DatumType::Pointer)
	{
		SetType(DatumType::Pointer);
	}

	SetSize((mSize + 1));
	mData.mPointerArray[mSize - 1] = value;
	return (mSize - 1);
}

void Datum::Set(const int& value, const std::uint32_t& index)
{
	SetType(DatumType::Integer);

	if (index >= mSize)
	{
		PushBack(value);
	}
	else
	{
		mData.mIntArray[index] = value;
	}
}

void Datum::Set(const float& value, const std::uint32_t& index)
{
	SetType(DatumType::Float);

	if (index >= mSize)
	{
		PushBack(value);
	}
	else
	{
		mData.mFloatArray[index] = value;
	}
}

void Datum::Set(const glm::vec4& value, const std::uint32_t& index)
{
	SetType(DatumType::Vector);

	if (index >= mSize)
	{
		PushBack(value);
	}
	else
	{
		mData.mVectorArray[index] = value;
	}
}

void Datum::Set(const glm::mat4& value, const std::uint32_t& index)
{
	SetType(DatumType::Matrix);

	if (index >= mSize)
	{
		PushBack(value);
	}
	else
	{
		mData.mMatrixArray[index] = value;
	}
}

void Datum::Set(Scope& value, const std::uint32_t& index)
{
	SetType(DatumType::Table);

	if (index >= mSize)
	{
		PushBack(value);
	}
	else
	{
		mData.mTableArray[index] = &value;
	}
}

void Datum::Set(const std::string& value, const std::uint32_t& index)
{
	SetType(DatumType::String);

	if (index >= mSize)
	{
		PushBack(value);
	}
	else
	{
		mData.mStringArray[index] = value;
	}
}

void Datum::Set(RTTI* const& value, const std::uint32_t& index)
{
	SetType(DatumType::Pointer);

	if (index >= mSize)
	{
		PushBack(value);
	}
	else
	{
		mData.mPointerArray[index] = value;
	}
}

void Datum::Remove(const int& value)
{
	if (mType != DatumType::Integer)
	{
		throw std::exception("Error: cannot remove an int from a Datum that is not of type Integer");
	}
	else if (mIsExternal)
	{
		throw std::exception("Error: cannot remove external data");
	}

	std::uint32_t i = 0;

	for (i = 0; i < mSize; i++)
	{
		if (mData.mIntArray[i] == value)
		{
			break;
		}
	}

	if (i != mSize)
	{
		--mSize;
		auto size = ((mSize - i) * sizeof(int));

		if (size > 0)
		{
			memmove_s(&mData.mIntArray[i], size, &mData.mIntArray[i + 1], size);
		}
	}
}

void Datum::Remove(const float& value)
{
	if (mType != DatumType::Float)
	{
		throw std::exception("Error: cannot remove a float from a Datum that is not of type Float");
	}
	else if (mIsExternal)
	{
		throw std::exception("Error: cannot remove external data");
	}

	std::uint32_t i = 0;

	for (i = 0; i < mSize; i++)
	{
		if (mData.mFloatArray[i] == value)
		{
			break;
		}
	}

	if (i != mSize)
	{
		--mSize;
		auto size = ((mSize - i) * sizeof(float));

		if (size > 0)
		{
			memmove_s(&mData.mFloatArray[i], size, &mData.mFloatArray[i + 1], size);
		}
	}
}

void Datum::Remove(const glm::vec4& value)
{
	if (mType != DatumType::Vector)
	{
		throw std::exception("Error: cannot remove a glm::vec4 from a Datum that is not of type Vector");
	}
	else if (mIsExternal)
	{
		throw std::exception("Error: cannot remove external data");
	}

	std::uint32_t i = 0;

	for (i = 0; i < mSize; i++)
	{
		if (mData.mVectorArray[i] == value)
		{
			break;
		}
	}

	if (i != mSize)
	{
		--mSize;
		auto size = ((mSize - i) * sizeof(glm::vec4));

		if (size > 0)
		{
			memmove_s(&mData.mVectorArray[i], size, &mData.mVectorArray[i + 1], size);
		}
	}
}

void Datum::Remove(const glm::mat4& value)
{
	if (mType != DatumType::Matrix)
	{
		throw std::exception("Error: cannot remove a glm::mat4 from a Datum that is not of type Matrix");
	}
	else if (mIsExternal)
	{
		throw std::exception("Error: cannot remove external data");
	}

	std::uint32_t i = 0;

	for (i = 0; i < mSize; i++)
	{
		if (mData.mMatrixArray[i] == value)
		{
			break;
		}
	}

	if (i != mSize)
	{
		--mSize;
		auto size = ((mSize - i) * sizeof(glm::mat4));

		if (size > 0)
		{
			memmove_s(&mData.mMatrixArray[i], size, &mData.mMatrixArray[i + 1], size);
		}
	}
}

void Datum::Remove(Scope& value)
{
	if (mType != DatumType::Table)
	{
		throw std::exception("Error: cannot remove a Scope* from a Datum that is not of type Table");
	}

	std::uint32_t i = 0;

	for (i = 0; i < mSize; i++)
	{
		if (operator[](i) == value)
		{
			break;
		}
	}

	if (i != mSize)
	{
		--mSize;

		auto size = ((mSize - i) * sizeof(Scope*));

		if (size > 0)
		{
			memmove_s(&mData.mTableArray[i], size, &mData.mTableArray[i + 1], size);
		}

	}
}

void Datum::Remove(const std::string& value)
{
	if (mType != DatumType::String)
	{
		throw std::exception("Error: cannot remove an std::string from a Datum that is not of type String");
	}
	else if (mIsExternal)
	{
		throw std::exception("Error: cannot remove external data");
	}

	std::uint32_t i = 0;

	for (i = 0; i < mSize; i++)
	{
		if (mData.mStringArray[i] == value)
		{
			break;
		}
	}

	if (i != mSize)
	{
		for (i += 1; i < mSize; i++)
		{
			mData.mStringArray[i - 1] = mData.mStringArray[i];
		}

		--mSize;
	}
}

void Datum::Remove(RTTI* const& value)
{
	if (mType != DatumType::Pointer)
	{
		throw std::exception("Error: cannot remove an RTTI* from a Datum that is not of type Pointer");
	}
	else if (mIsExternal)
	{
		throw std::exception("Error: cannot remove external data");
	}

	std::uint32_t i = 0;

	for (i = 0; i < mSize; i++)
	{
		if (mData.mPointerArray[i] == value)
		{
			break;
		}
	}

	if (i != mSize)
	{
		--mSize;

		auto size = ((mSize - i) * sizeof(RTTI*));

		if (size > 0)
		{
			memmove_s(&mData.mPointerArray[i], size, &mData.mPointerArray[i + 1], size);
		}
	}
}

void Datum::SetFromString(const std::string& value, DatumType type, const std::uint32_t& index)
{
	if (type == DatumType::Pointer || type == DatumType::Table)
	{
		throw std::exception("Error: cannot convert from a pointer element to an std::string");
	}

	setFromStringFuncTable[static_cast<std::uint32_t>(type)](*this, value, index);
}

bool Datum::operator==(const Datum& otherDatum) const
{
	if (mType != otherDatum.mType || mCapacity != otherDatum.mCapacity || mSize != otherDatum.mSize)
	{
		return false;
	}

	return equalityFuncTable[static_cast<std::uint32_t>(mType)](*this, otherDatum);
}

bool Datum::operator!=(const Datum& otherDatum) const
{
	return !(operator==(otherDatum));
}

bool Datum::operator==(const int& data) const
{
	return ((mType == DatumType::Integer) && (mSize > 0) && (mData.mIntArray[0] == data));
}

bool Datum::operator!=(const int& data) const
{
	return !(operator==(data));
}

bool Datum::operator==(const float& data) const
{
	return ((mType == DatumType::Float) && (mSize > 0) && (mData.mFloatArray[0] == data));
}

bool Datum::operator!=(const float& data) const
{
	return !(operator==(data));
}

bool Datum::operator==(const glm::vec4& data) const
{
	return ((mType == DatumType::Vector) && (mSize > 0) && (mData.mVectorArray[0] == data));
}

bool Datum::operator!=(const glm::vec4& data) const
{
	return !(operator==(data));
}

bool Datum::operator==(const glm::mat4& data) const
{
	return ((mType == DatumType::Matrix) && (mSize > 0) && (mData.mMatrixArray[0] == data));
}

bool Datum::operator!=(const glm::mat4& data) const
{
	return !(operator==(data));
}

bool Datum::operator==(Scope& data) const
{
	return ((mType == DatumType::Table) && (mSize > 0) && (operator[](0) == data));
}

bool Datum::operator!=(Scope& data) const
{
	return !(operator==(data));
}

bool Datum::operator==(const std::string& data) const
{
	return ((mType == DatumType::String) && (mSize > 0) && (mData.mStringArray[0] == data));
}

bool Datum::operator!=(const std::string& data) const
{
	return !(operator==(data));
}

bool Datum::operator==(RTTI* const& data) const
{
	return ((mType == DatumType::Pointer) && (mSize > 0) && (mData.mPointerArray[0] == data));
}

bool Datum::operator!=(RTTI* const& data) const
{
	return !(operator==(data));
}

bool Datum::IsExternal() const
{
	return mIsExternal;
}

std::uint32_t Datum::Size() const
{
	return mSize;
}

void Datum::SetSize(const std::uint32_t& size)
{
	if (mType == DatumType::Unknown)
	{
		throw std::exception("Error: type must be set before any allocation can be done");
	}
	else if (mIsExternal)
	{
		throw std::exception("Error: cannot tamper with any memory allocated for external data");
	}

	setSizeFuncTable[static_cast<std::uint32_t>(mType)](*this, size);
}

void Datum::SetStorage(int* const container, const std::uint32_t & size)
{
	if (mType != DatumType::Unknown)
	{
		if (mType != DatumType::Integer)
		{
			throw std::exception("Error: cannot change the type of a Datum once it has been set");
		}

		if (!mIsExternal)
		{
			Clear();
		}
	}

	mType = DatumType::Integer;
	mSize = mCapacity = size;
	mIsExternal = true;
	mData.mIntArray = container;
}

void Datum::SetStorage(float* const container, const std::uint32_t & size)
{
	if (mType != DatumType::Unknown)
	{
		if (mType != DatumType::Float)
		{
			throw std::exception("Error: cannot change the type of a Datum once it has been set");
		}

		if (!mIsExternal)
		{
			Clear();
		}
	}

	mType = DatumType::Float;
	mSize = mCapacity = size;
	mIsExternal = true;
	mData.mFloatArray = container;
}

void Datum::SetStorage(glm::vec4* const container, const std::uint32_t & size)
{
	if (mType != DatumType::Unknown)
	{
		if (mType != DatumType::Vector)
		{
			throw std::exception("Error: cannot change the type of a Datum once it has been set");
		}

		if (!mIsExternal)
		{
			Clear();
		}
	}

	mType = DatumType::Vector;
	mSize = mCapacity = size;
	mIsExternal = true;
	mData.mVectorArray = container;
}

void Datum::SetStorage(glm::mat4* const container, const std::uint32_t & size)
{
	if (mType != DatumType::Unknown)
	{
		if (mType != DatumType::Matrix)
		{
			throw std::exception("Error: cannot change the type of a Datum once it has been set");
		}

		if (!mIsExternal)
		{
			Clear();
		}
	}

	mType = DatumType::Matrix;
	mSize = mCapacity = size;
	mIsExternal = true;
	mData.mMatrixArray = container;
}

void Datum::SetStorage(std::string* const container, const std::uint32_t & size)
{
	if (mType != DatumType::Unknown)
	{
		if (mType != DatumType::String)
		{
			throw std::exception("Error: cannot change the type of a Datum once it has been set");
		}

		if (!mIsExternal)
		{
			Clear();
		}
	}

	mType = DatumType::String;
	mSize = mCapacity = size;
	mIsExternal = true;
	mData.mStringArray = container;
}

void Datum::SetStorage(RTTI** const container, const std::uint32_t & size)
{
	if (mType != DatumType::Unknown)
	{
		if (mType != DatumType::Pointer)
		{
			throw std::exception("Error: cannot change the type of a Datum once it has been set");
		}

		if (!mIsExternal)
		{
			Clear();
		}
	}

	mType = DatumType::Pointer;
	mSize = mCapacity = size;
	mIsExternal = true;
	mData.mPointerArray = container;
}

std::uint32_t Datum::Capacity() const
{
	return mCapacity;
}

Datum::DatumType Datum::Type() const
{
	return mType;
}

void Datum::SetType(const Datum::DatumType& type)
{
	if (mType != DatumType::Unknown && mType != type)
	{
		throw std::exception("Error: cannot change the type of a Datum once it has been set");
	}

	if (type != DatumType::MAX)
	{
		mType = type;
	}
}

std::string Datum::ToString(const std::uint32_t& index) 
{
	return toStringFuncTable[static_cast<std::uint32_t>(mType)](*this, index);
}

void Datum::Clear()
{
	if (mType != DatumType::Unknown && !mIsExternal)
	{
		if (mType == DatumType::String)
		{
			delete[] mData.mStringArray;
		}
		else
		{
			delete[] mData.mIntArray;
		}
	}

	mData.Clear();
	mSize = mCapacity = 0;
	mType = DatumType::Unknown;
	mIsExternal = false;
}
