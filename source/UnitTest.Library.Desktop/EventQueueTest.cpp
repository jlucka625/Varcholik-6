#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventQueueTest)
	{
	public:
		TEST_METHOD_INITIALIZE(PrepForTests)
		{
#if defined(DEBUG) | defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(CleanupTestObjects)
		{
#if defined(DEBUG) | defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(EventQueueTestConstruction)
		{
			EventQueue eventQueue;
			Assert::IsTrue(eventQueue.IsEmpty());
		}

		TEST_METHOD(EventQueueTestEnqueue)
		{
			EventQueue eventQueue;
			Assert::IsTrue(eventQueue.IsEmpty());

			Foo testFoo(13);
			std::shared_ptr<Event<Foo>> fooEventPtr = std::make_shared<Event<Foo>>(testFoo);

			FooEventSubscriber fooSubscriber;
			AttributedFooEventSubscriber aFooSubscriber;
			fooEventPtr->Subscribe(fooSubscriber);
			fooEventPtr->Subscribe(aFooSubscriber);

			std::shared_ptr<Event<std::uint32_t>> intEventPtr = std::make_shared<Event<std::uint32_t>>(24);

			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);

			eventQueue.Enqueue(fooEventPtr, gameTime);
			Assert::IsFalse(eventQueue.IsEmpty());
			Assert::AreEqual(eventQueue.Size(), 1U);
			Assert::IsTrue(fooEventPtr->IsExpired(gameTime.CurrentTime()));
			Assert::IsTrue(fooEventPtr->Delay() == std::chrono::milliseconds::zero());

			Assert::ExpectException<std::exception>([&] {eventQueue.Enqueue(fooEventPtr, gameTime); });

			eventQueue.Enqueue(intEventPtr, gameTime, std::chrono::milliseconds(5000));
			Assert::AreEqual(eventQueue.Size(), 2U);
			Assert::IsFalse(intEventPtr->IsExpired(gameTime.CurrentTime()));
			Assert::IsTrue(intEventPtr->Delay() == std::chrono::milliseconds(5000));

			fooEventPtr->UnsubscribeAll();
		}

		TEST_METHOD(EventQueueTestSend)
		{
			EventQueue eventQueue;

			//AttributedFoo test
			AttributedFoo aFoo;
			aFoo["Health"] = 100;
			aFoo["Money"] = 100.0f;
			aFoo["Name"] = "Me";
			aFoo["Position"] = glm::vec4(50, 50, 50, 50);
			aFoo["CurrentFieldGrid"] = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			AttributedFooEventSubscriber aFooSubscriber;

			Assert::IsTrue(aFooSubscriber.Health() == 0);
			Assert::IsTrue(aFooSubscriber.Money() == 0.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(0, 0, 0, 0));

			std::shared_ptr<Event<AttributedFoo>> aFooEventPtr = std::make_shared<Event<AttributedFoo>>(aFoo);
			aFooEventPtr->Subscribe(aFooSubscriber);

			eventQueue.Send(aFooEventPtr);

			Assert::IsTrue(aFooSubscriber.Health() == 100);
			Assert::IsTrue(aFooSubscriber.Money() == 100.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "Me");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(50, 50, 50, 50));

			//int test
			IntEventSubscriber intSubscriber;
			Assert::AreEqual(0U, intSubscriber.Number());

			std::shared_ptr<Event<std::uint32_t>> intEventPtr = std::make_shared<Event<std::uint32_t>>(66);
			intEventPtr->Subscribe(intSubscriber);

			eventQueue.Send(intEventPtr);
			Assert::AreEqual(66U, intSubscriber.Number());

			//Foo test
			FooEventSubscriber fooSubscriber;

			std::shared_ptr<Event<Foo>> fooEventPtr = std::make_shared<Event<Foo>>(13);
			fooEventPtr->Subscribe(fooSubscriber);

			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);

			eventQueue.Enqueue(fooEventPtr, gameTime);

			Assert::AreEqual(0, fooSubscriber.Data());
			Assert::AreEqual(0, fooSubscriber.PointerData());
			eventQueue.Send(fooEventPtr);
			Assert::AreEqual(13, fooSubscriber.Data());
			Assert::AreEqual(14, fooSubscriber.PointerData());

			aFooEventPtr->UnsubscribeAll();
			fooEventPtr->UnsubscribeAll();
			intEventPtr->UnsubscribeAll();
		}

		TEST_METHOD(EventQueueTestUpdate)
		{
			FooEventSubscriber fooSubscriber;
			AttributedFooEventSubscriber aFooSubscriber;
			IntEventSubscriber intSubscriber;

			EventQueue eventQueue;

			Foo testFoo(13);
			std::shared_ptr<Event<Foo>> fooEventPtr = std::make_shared<Event<Foo>>(testFoo);
			fooEventPtr->Subscribe(fooSubscriber);
			fooEventPtr->Subscribe(intSubscriber);
			fooEventPtr->Subscribe(aFooSubscriber);

			Foo otherTestFoo(42);
			std::shared_ptr<Event<Foo>> otherFooEventPtr = std::make_shared<Event<Foo>>(otherTestFoo);

			std::shared_ptr<Event<std::uint32_t>> intEventPtr = std::make_shared<Event<std::uint32_t>>(24);
			intEventPtr->Subscribe(fooSubscriber);
			intEventPtr->Subscribe(intSubscriber);
			intEventPtr->Subscribe(aFooSubscriber);

			AttributedFoo testAFoo;
			std::shared_ptr<Event<AttributedFoo>> aFooEventPtr = std::make_shared<Event<AttributedFoo>>(testAFoo);
			aFooEventPtr->Subscribe(fooSubscriber);
			aFooEventPtr->Subscribe(intSubscriber);
			aFooEventPtr->Subscribe(aFooSubscriber);

			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);

			eventQueue.Enqueue(fooEventPtr, gameTime);
			eventQueue.Enqueue(otherFooEventPtr, gameTime, std::chrono::milliseconds(800));
			eventQueue.Enqueue(aFooEventPtr, gameTime, std::chrono::milliseconds(1500));
			eventQueue.Enqueue(intEventPtr, gameTime, std::chrono::milliseconds(2500));

			Assert::AreEqual(4U, eventQueue.Size());
			eventQueue.Update(gameTime);
			Assert::AreEqual(3U, eventQueue.Size());
			Assert::AreEqual(13, fooSubscriber.Data());
			Assert::AreEqual(14, fooSubscriber.PointerData());

			while (!eventQueue.IsEmpty())
			{
				gameClock.UpdateGameTime(gameTime);
				eventQueue.Update(gameTime);
			}

			Assert::AreEqual(42, fooSubscriber.Data());
			Assert::AreEqual(43, fooSubscriber.PointerData());

			fooEventPtr->UnsubscribeAll();
			intEventPtr->UnsubscribeAll();
			aFooEventPtr->UnsubscribeAll();
		}

		TEST_METHOD(EnqueueTestClear)
		{
			std::shared_ptr<Event<Foo>> fooEventPtr = std::make_shared<Event<Foo>>(13);

			std::shared_ptr<Event<std::uint32_t>> intEventPtr = std::make_shared<Event<std::uint32_t>>(24);

			AttributedFoo aFoo;
			std::shared_ptr<Event<AttributedFoo>> aFooEventPtr = std::make_shared<Event<AttributedFoo>>(aFoo);

			GameTime gameTime;
			EventQueue eventQueue;

			eventQueue.Enqueue(fooEventPtr, gameTime);
			eventQueue.Enqueue(intEventPtr, gameTime);
			eventQueue.Enqueue(aFooEventPtr, gameTime);

			Assert::IsFalse(eventQueue.IsEmpty());
			Assert::AreEqual(3U, eventQueue.Size());

			eventQueue.Clear();

			Assert::IsTrue(eventQueue.IsEmpty());
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EventQueueTest::startMemState;
#endif
}