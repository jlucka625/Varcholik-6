#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

/*namespace LibraryDesktop
{
	template <class AbstractProductT> typename Factory<AbstractProductT>::FactoryTable Factory<AbstractProductT>::sFactoryTable;
}*/

namespace UnitTestLibraryDesktop
{
	ConcreteFactory(RTTI, Foo)
	ConcreteFactory(RTTI, AttributedFoo)
	ConcreteFactory(RTTI, Scope)
	ConcreteFactory(RTTI, AttributedBar)

	TEST_CLASS(FactoryTest)
	{
	public:
		TEST_METHOD_INITIALIZE(PrepForTests)
		{
#if defined(DEBUG) | defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&startMemState);
#endif
		}

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_CLEANUP(CleanupTestObjects)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(FactoryTestInitialization)
		{
			Assert::IsTrue(Factory<RTTI>::IsEmpty());
		}

		TEST_METHOD(FactoryTestAddAndRemove)
		{
			std::string factoryNames[] = { "Scope", "AttributedBar", "AttributedFoo", "Foo" };
			Assert::IsTrue(Factory<RTTI>::IsEmpty());

			{
				FooFactory fooFactory;

				Assert::IsTrue(!Factory<RTTI>::IsEmpty());
				Assert::IsTrue(Factory<RTTI>::Find("Foo") != nullptr);
				Assert::AreEqual(Factory<RTTI>::Size(), 1U);

				{
					AttributedFooFactory attributedFooFactory;

					Assert::IsTrue(Factory<RTTI>::Find("AttributedFoo") != nullptr);
					Assert::AreEqual(Factory<RTTI>::Size(), 2U);

					{
						ScopeFactory scopeFactory;

						Assert::IsTrue(Factory<RTTI>::Find("Scope") != nullptr);
						Assert::AreEqual(Factory<RTTI>::Size(), 3U);

						{
							AttributedBarFactory attributedBarFactory;

							Assert::IsTrue(Factory<RTTI>::Find("AttributedBar") != nullptr);
							Assert::AreEqual(Factory<RTTI>::Size(), 4U);

							int i = 0;

							for (Factory<RTTI>::FactoryTable::Iterator it = Factory<RTTI>::begin(); it != Factory<RTTI>::end(); ++it)
							{
								Assert::IsTrue(it->second->ClassName() == factoryNames[i]);
								++i;
							}
						}

						Assert::IsTrue(Factory<RTTI>::Find("AttributedBar") == nullptr);
						Assert::AreEqual(Factory<RTTI>::Size(), 3U);
					}

					Assert::IsTrue(Factory<RTTI>::Find("Scope") == nullptr);
					Assert::AreEqual(Factory<RTTI>::Size(), 2U);
				}

				Assert::IsTrue(Factory<RTTI>::Find("AttributedFoo") == nullptr);
				Assert::AreEqual(Factory<RTTI>::Size(), 1U);
			}

			Assert::IsTrue(Factory<RTTI>::Find("Foo") == nullptr);
			Assert::IsTrue(Factory<RTTI>::IsEmpty());
		}

		TEST_METHOD(FactoryTestCreation)
		{
			FooFactory fooFactory;

			RTTI* newFoo = Factory<RTTI>::Create("Foo");
			Assert::IsTrue(newFoo->Is("Foo"));

			Assert::ExpectException<std::exception>([&] {Factory<RTTI>::Create("AttributedBar"); });

			delete newFoo;
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState FactoryTest::startMemState;
#endif
}