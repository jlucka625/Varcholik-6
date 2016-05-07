#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTest)
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

		TEST_METHOD(EventTestConstruction)
		{
			Foo testFoo(13);
			Event<Foo> fooEvent(testFoo);
			Assert::IsTrue(testFoo == fooEvent.Message());
		}

		TEST_METHOD(EventTestInvalidSubscribe)
		{
			FooEventSubscriber fooSubscriber;
			IntEventSubscriber intSubscriber;
			AttributedFooEventSubscriber aFooSubscriber;

			//int test
			Event<std::uint32_t> intEvent(42);

			intEvent.Subscribe(fooSubscriber);
			intEvent.Subscribe(aFooSubscriber);
			intEvent.Subscribe(intSubscriber);

			Assert::ExpectException<std::exception>([&] { intEvent.Subscribe(intSubscriber); });
			Assert::ExpectException<std::exception>([&] { intEvent.Subscribe(fooSubscriber); });
			Assert::ExpectException<std::exception>([&] { intEvent.Subscribe(aFooSubscriber); });

			//Foo test
			Foo testFoo(13);
			Event<Foo> fooEvent(testFoo);

			fooEvent.Subscribe(fooSubscriber);
			fooEvent.Subscribe(aFooSubscriber);
			fooEvent.Subscribe(intSubscriber);

			Assert::ExpectException<std::exception>([&] { fooEvent.Subscribe(intSubscriber); });
			Assert::ExpectException<std::exception>([&] { fooEvent.Subscribe(fooSubscriber); });
			Assert::ExpectException<std::exception>([&] { fooEvent.Subscribe(aFooSubscriber); });

			//AttributedFoo test
			AttributedFoo testAFoo;
			Event<AttributedFoo> aFooEvent(testAFoo);

			aFooEvent.Subscribe(fooSubscriber);
			aFooEvent.Subscribe(aFooSubscriber);
			aFooEvent.Subscribe(intSubscriber);

			Assert::ExpectException<std::exception>([&] { aFooEvent.Subscribe(intSubscriber); });
			Assert::ExpectException<std::exception>([&] { aFooEvent.Subscribe(fooSubscriber); });
			Assert::ExpectException<std::exception>([&] { aFooEvent.Subscribe(aFooSubscriber); });

			intEvent.UnsubscribeAll();
			fooEvent.UnsubscribeAll();
			aFooEvent.UnsubscribeAll();
		}

		TEST_METHOD(FooEventTestDeliver)
		{
			Foo testFoo(13);
			Event<Foo> fooEvent(testFoo);

			FooEventSubscriber fooSubscriber;
			IntEventSubscriber intSubscriber;
			AttributedFooEventSubscriber aFooSubscriber;

			fooEvent.Subscribe(fooSubscriber);
			fooEvent.Subscribe(intSubscriber);
			fooEvent.Subscribe(aFooSubscriber);

			Assert::AreEqual(0, fooSubscriber.Data());
			Assert::AreEqual(0, fooSubscriber.PointerData());
			Assert::AreEqual(0U, intSubscriber.Number());
			Assert::IsTrue(aFooSubscriber.Health() == 0);
			Assert::IsTrue(aFooSubscriber.Money() == 0.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(0, 0, 0, 0));

			fooEvent.Deliver();

			Assert::AreEqual(13, fooSubscriber.Data());
			Assert::AreEqual(14, fooSubscriber.PointerData());
			Assert::AreEqual(0U, intSubscriber.Number());
			Assert::IsTrue(aFooSubscriber.Health() == 0);
			Assert::IsTrue(aFooSubscriber.Money() == 0.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(0, 0, 0, 0));

			fooEvent.Unsubscribe(fooSubscriber);
			fooEvent.UnsubscribeAll();
		}

		TEST_METHOD(IntEventTestDeliver)
		{
			//Foo test
			Event<std::uint32_t> intEvent(42);

			FooEventSubscriber fooSubscriber;
			IntEventSubscriber intSubscriber;
			AttributedFooEventSubscriber aFooSubscriber;

			intEvent.Subscribe(fooSubscriber);
			intEvent.Subscribe(intSubscriber);
			intEvent.Subscribe(aFooSubscriber);

			Assert::AreEqual(0, fooSubscriber.Data());
			Assert::AreEqual(0, fooSubscriber.PointerData());
			Assert::AreEqual(0U, intSubscriber.Number());
			Assert::IsTrue(aFooSubscriber.Health() == 0);
			Assert::IsTrue(aFooSubscriber.Money() == 0.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(0, 0, 0, 0));

			intEvent.Deliver();

			Assert::AreEqual(0, fooSubscriber.Data());
			Assert::AreEqual(0, fooSubscriber.PointerData());
			Assert::AreEqual(42U, intSubscriber.Number());
			Assert::IsTrue(aFooSubscriber.Health() == 0);
			Assert::IsTrue(aFooSubscriber.Money() == 0.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(0, 0, 0, 0));

			intEvent.Unsubscribe(intSubscriber);
			intEvent.UnsubscribeAll();
		}

		TEST_METHOD(AttributedFooEventTestDeliver)
		{
			//Foo test
			AttributedFoo aFoo;
			aFoo["Health"] = 100;
			aFoo["Money"] = 100.0f;
			aFoo["Name"] = "Me";
			aFoo["Position"] = glm::vec4(50, 50, 50, 50);
			aFoo["CurrentFieldGrid"] = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
			Event<AttributedFoo> aFooEvent(aFoo);

			FooEventSubscriber fooSubscriber;
			IntEventSubscriber intSubscriber;
			AttributedFooEventSubscriber aFooSubscriber;

			aFooEvent.Subscribe(fooSubscriber);
			aFooEvent.Subscribe(intSubscriber);
			aFooEvent.Subscribe(aFooSubscriber);

			Assert::AreEqual(0, fooSubscriber.Data());
			Assert::AreEqual(0, fooSubscriber.PointerData());
			Assert::AreEqual(0U, intSubscriber.Number());
			Assert::IsTrue(aFooSubscriber.Health() == 0);
			Assert::IsTrue(aFooSubscriber.Money() == 0.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(0, 0, 0, 0));

			aFooEvent.Deliver();

			Assert::AreEqual(0, fooSubscriber.Data());
			Assert::AreEqual(0, fooSubscriber.PointerData());
			Assert::AreEqual(0U, intSubscriber.Number());
			Assert::IsTrue(aFooSubscriber.Health() == 100);
			Assert::IsTrue(aFooSubscriber.Money() == 100.0f);
			Assert::IsTrue(aFooSubscriber.Identity() == "Me");
			Assert::IsTrue(aFooSubscriber.CurrentFieldGrid() == glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
			Assert::IsTrue(aFooSubscriber.Position() == glm::vec4(50, 50, 50, 50));

			aFooEvent.Unsubscribe(aFooSubscriber);
			aFooEvent.UnsubscribeAll();
		}

		TEST_METHOD(FooEventTestCopySemantics)
		{
			//Copy constructor
			Foo testFoo(13);
			Event<Foo> fooEvent(testFoo);

			Event<Foo> otherFooEvent(fooEvent);

			Assert::IsTrue(fooEvent == otherFooEvent);
			Assert::IsTrue(fooEvent.Message() == otherFooEvent.Message());

			//Copy assignment operator
			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);
			std::chrono::high_resolution_clock::time_point currentTime = gameTime.CurrentTime();

			Foo otherTestFoo(16);
			Event<Foo> finalFooEvent(otherTestFoo);
			
			Assert::IsTrue(otherFooEvent != finalFooEvent);

			otherFooEvent = finalFooEvent;
			Assert::IsTrue(otherFooEvent.Message() == finalFooEvent.Message());

			finalFooEvent.SetTime(currentTime, std::chrono::milliseconds(3000));
			Assert::IsTrue(otherFooEvent != finalFooEvent);

			otherFooEvent.SetTime(currentTime, std::chrono::milliseconds(2000));
			Assert::IsTrue(otherFooEvent != finalFooEvent);
			
			otherFooEvent.SetTime(currentTime, std::chrono::milliseconds(3000));

			Assert::IsTrue(otherFooEvent == finalFooEvent);
		}

		TEST_METHOD(FooEventTestMoveSemantics)
		{
			//Move constructor
			Foo testFoo(13);
			Event<Foo> fooEvent(testFoo);

			Event<Foo> otherFooEvent(std::move(fooEvent));

			Assert::IsTrue(otherFooEvent.Message() == testFoo);

			//Move assignment operator
			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);
			std::chrono::high_resolution_clock::time_point currentTime = gameTime.CurrentTime();
			std::chrono::milliseconds delay = std::chrono::milliseconds(3000);
			otherFooEvent.SetTime(currentTime, delay);

			Foo otherTestFoo(16);
			Event<Foo> finalFooEvent(otherTestFoo);

			finalFooEvent = std::move(otherFooEvent);
			Assert::IsTrue(finalFooEvent.Message() == testFoo);
			Assert::IsTrue(finalFooEvent.TimeEnqueued() == currentTime);
			Assert::IsTrue(finalFooEvent.Delay() == delay);
		}

		TEST_METHOD(AttributedFooEventTestCopySemantics)
		{
			//Copy constructor
			AttributedFoo testAFoo;
			Event<AttributedFoo> aFooEvent(testAFoo);

			Event<AttributedFoo> otherAFooEvent(aFooEvent);

			Assert::IsTrue(aFooEvent == otherAFooEvent);
			Assert::IsTrue(aFooEvent.Message() == otherAFooEvent.Message());

			//Copy assignment operator
			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);
			std::chrono::high_resolution_clock::time_point currentTime = gameTime.CurrentTime();

			AttributedFoo otherTestAFoo;
			Event<AttributedFoo> finalAFooEvent(otherTestAFoo);

			Assert::IsTrue(otherAFooEvent != finalAFooEvent);

			otherAFooEvent = finalAFooEvent;
			Assert::IsTrue(otherAFooEvent.Message() == finalAFooEvent.Message());

			finalAFooEvent.SetTime(currentTime, std::chrono::milliseconds(3000));
			Assert::IsTrue(otherAFooEvent != finalAFooEvent);

			otherAFooEvent.SetTime(currentTime, std::chrono::milliseconds(2000));
			Assert::IsTrue(otherAFooEvent != finalAFooEvent);

			otherAFooEvent.SetTime(currentTime, std::chrono::milliseconds(3000));

			Assert::IsTrue(otherAFooEvent == finalAFooEvent);
		}

		TEST_METHOD(AttributedFooEventTestMoveSemantics)
		{
			//Move constructor
			AttributedFoo testAFoo;
			Event<AttributedFoo> aFooEvent(testAFoo);

			Event<AttributedFoo> otherAFooEvent(std::move(aFooEvent));

			Assert::IsTrue(otherAFooEvent.Message() == testAFoo);

			//Move assignment operator
			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);
			std::chrono::high_resolution_clock::time_point currentTime = gameTime.CurrentTime();
			std::chrono::milliseconds delay = std::chrono::milliseconds(3000);
			otherAFooEvent.SetTime(currentTime, delay);

			AttributedFoo otherTestAFoo;
			Event<AttributedFoo> finalAFooEvent(otherTestAFoo);

			finalAFooEvent = std::move(otherAFooEvent);
			Assert::IsTrue(finalAFooEvent.Message() == testAFoo);
			Assert::IsTrue(finalAFooEvent.TimeEnqueued() == currentTime);
			Assert::IsTrue(finalAFooEvent.Delay() == delay);
		}

		TEST_METHOD(IntEventTestCopySemantics)
		{
			//Copy constructor
			Event<std::uint32_t> intEvent(32);

			Event<std::uint32_t> otherIntEvent(intEvent);

			Assert::IsTrue(intEvent == otherIntEvent);
			Assert::IsTrue(intEvent.Message() == otherIntEvent.Message());

			//Copy assignment operator
			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);
			std::chrono::high_resolution_clock::time_point currentTime = gameTime.CurrentTime();

			Event<std::uint32_t> finalIntEvent(999);

			Assert::IsTrue(otherIntEvent != finalIntEvent);

			otherIntEvent = finalIntEvent;
			Assert::IsTrue(otherIntEvent.Message() == finalIntEvent.Message());

			finalIntEvent.SetTime(currentTime, std::chrono::milliseconds(3000));
			Assert::IsTrue(otherIntEvent != finalIntEvent);

			otherIntEvent.SetTime(currentTime, std::chrono::milliseconds(2000));
			Assert::IsTrue(otherIntEvent != finalIntEvent);

			otherIntEvent.SetTime(currentTime, std::chrono::milliseconds(3000));

			Assert::IsTrue(otherIntEvent == finalIntEvent);
		}

		TEST_METHOD(IntEventTestMoveSemantics)
		{
			//Move constructor
			Event<std::uint32_t> intEvent(32);

			Event<std::uint32_t> otherIntEvent(std::move(intEvent));

			Assert::IsTrue(otherIntEvent.Message() == 32U);

			//Move assignment operator
			GameClock gameClock;
			GameTime gameTime;

			gameClock.Reset();
			gameClock.UpdateGameTime(gameTime);
			std::chrono::high_resolution_clock::time_point currentTime = gameTime.CurrentTime();
			std::chrono::milliseconds delay = std::chrono::milliseconds(3000);
			otherIntEvent.SetTime(currentTime, delay);

			Event<std::uint32_t> finalIntEvent(74U);

			finalIntEvent = std::move(otherIntEvent);
			Assert::IsTrue(finalIntEvent.Message() == 32U);
			Assert::IsTrue(finalIntEvent.TimeEnqueued() == currentTime);
			Assert::IsTrue(finalIntEvent.Delay() == delay);
		}

		TEST_METHOD(FooEventTestRTTI)
		{
			Foo testFoo(13);
			Event<Foo> fooEvent(testFoo);

			RTTI* testRTTI = &fooEvent;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("EventPublisher"));
			Assert::IsTrue(testRTTI->Is("Event"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			EventPublisher* otherPublisher = testRTTI->As<EventPublisher>();
			Assert::IsTrue(otherPublisher != nullptr);
			Assert::IsTrue(&fooEvent == otherPublisher);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Event<Foo>* otherEvent = testRTTI->As<Event<Foo>>();
			Assert::IsTrue(otherEvent != nullptr);

			Assert::IsTrue(testRTTI->Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(otherPublisher->TypeIdInstance()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			otherTestRTTI = testRTTI->QueryInterface(Event<Foo>::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(AttributedFooEventTestRTTI)
		{
			AttributedFoo testAFoo;
			Event<AttributedFoo> aFooEvent(testAFoo);

			RTTI* testRTTI = &aFooEvent;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("EventPublisher"));
			Assert::IsTrue(testRTTI->Is("Event"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			EventPublisher* otherPublisher = testRTTI->As<EventPublisher>();
			Assert::IsTrue(otherPublisher != nullptr);
			Assert::IsTrue(&aFooEvent == otherPublisher);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Event<AttributedFoo>* otherEvent = testRTTI->As<Event<AttributedFoo>>();
			Assert::IsTrue(otherEvent != nullptr);

			Assert::IsTrue(testRTTI->Is(Event<AttributedFoo>::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(otherPublisher->TypeIdInstance()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			otherTestRTTI = testRTTI->QueryInterface(Event<AttributedFoo>::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(IntEventTestRTTI)
		{
			Event<std::uint32_t> intEvent(47);

			RTTI* testRTTI = &intEvent;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("EventPublisher"));
			Assert::IsTrue(testRTTI->Is("Event"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			EventPublisher* otherPublisher = testRTTI->As<EventPublisher>();
			Assert::IsTrue(otherPublisher != nullptr);
			Assert::IsTrue(&intEvent == otherPublisher);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Event<std::uint32_t>* otherEvent = testRTTI->As<Event<std::uint32_t>>();
			Assert::IsTrue(otherEvent != nullptr);

			Assert::IsTrue(testRTTI->Is(Event<std::uint32_t>::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(otherPublisher->TypeIdInstance()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			otherTestRTTI = testRTTI->QueryInterface(Event<std::uint32_t>::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(EventMessageAttributedEventTestRTTI)
		{
			EventMessageAttributed aEventMessage;
			Event<EventMessageAttributed> emaEvent(aEventMessage);

			RTTI* testRTTI = &emaEvent;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("EventPublisher"));
			Assert::IsTrue(testRTTI->Is("Event"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			EventPublisher* otherPublisher = testRTTI->As<EventPublisher>();
			Assert::IsTrue(otherPublisher != nullptr);
			Assert::IsTrue(&emaEvent == otherPublisher);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Event<EventMessageAttributed>* otherEvent = testRTTI->As<Event<EventMessageAttributed>>();
			Assert::IsTrue(otherEvent != nullptr);

			Assert::IsTrue(testRTTI->Is(Event<EventMessageAttributed>::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(otherPublisher->TypeIdInstance()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			otherTestRTTI = testRTTI->QueryInterface(Event<EventMessageAttributed>::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EventTest::startMemState;
#endif
}