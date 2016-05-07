#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(FooTest)
	{
	public:
		
		TEST_METHOD(FooTestConstructor)
		{
			Foo dummyFoo;

			Assert::AreEqual(0, dummyFoo.Data());

			int testInteger = 10;
			Foo fooInstance(10);
			Assert::AreEqual(testInteger, fooInstance.Data());
		}

		TEST_METHOD(FooTestMoveSemantics)
		{
			Foo testFoo(12);

			Foo otherFoo = std::move(testFoo);

			Assert::AreEqual(12, otherFoo.Data());
		}
	};
}