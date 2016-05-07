#include "pch.h"

using namespace UnitTestLibraryDesktop;
using namespace LibraryDesktop;

RTTI_DEFINITIONS(Foo)

Foo::Foo() : mData(0), mPointerData(new int(0))
{
}

Foo::Foo(int data) : mData(data), mPointerData(new int(data + 1))
{
}

Foo::Foo(const Foo& otherFoo) : mData(0), mPointerData(nullptr)
{
	operator=(otherFoo);
}

Foo::Foo(Foo&& otherFoo) : mData(otherFoo.mData), mPointerData(otherFoo.mPointerData)
{
	otherFoo.mData = 0;
	otherFoo.mPointerData = nullptr;
}

Foo& Foo::operator=(const Foo& otherFoo)
{
	if (this != &otherFoo)
	{
		mData = otherFoo.mData;

		delete mPointerData;
		mPointerData = new int(*(otherFoo.mPointerData));
	}

	return *this;
}

Foo& Foo::operator=(Foo&& otherFoo)
{
	if (this != &otherFoo)
	{
		mData = otherFoo.mData;

		delete mPointerData;
		mPointerData = otherFoo.mPointerData;

		otherFoo.mData = 0;
		otherFoo.mPointerData = nullptr;
	}

	return *this;
}

Foo::~Foo()
{
	if (this)
	{
		delete mPointerData;
		mPointerData = nullptr;
	}
}

bool Foo::operator==(const Foo& otherFoo) const
{
	return ((mData == otherFoo.mData) && ((*mPointerData) == (*(otherFoo.mPointerData))));
}

/*bool Foo::Equals(const RTTI* rhs) const
{
	if(rhs->Is("Foo"))
	return operator==(*(rhs));
}*/

bool Foo::operator!=(const Foo& otherFoo) const
{
	return !(operator==(otherFoo));
}

int &Foo::Data()
{
	return mData;
}

const int Foo::Data() const
{
	return mData;
}

int* Foo::PointerData()
{
	return mPointerData;
}

const int* Foo::PointerData() const
{
	return mPointerData;
}

std::string Foo::ToString() const
{
	std::string fooString = "";

	fooString += ("{mData: " + std::to_string(mData) + ", mPointerData: " + std::to_string(*mPointerData) + "}");
	return fooString;
}
