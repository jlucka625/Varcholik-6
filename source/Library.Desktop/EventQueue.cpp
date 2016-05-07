#include "pch.h"

using namespace LibraryDesktop;

EventQueue::EventQueue() : mQueue()
{
}

EventQueue::~EventQueue()
{
	Clear();
}

void EventQueue::Enqueue(const std::shared_ptr<EventPublisher>& publisher, const GameTime& gameTime, const std::chrono::milliseconds& delay)
{
	if (mQueue.Find(publisher) != mQueue.end())
	{
		throw std::exception("Error: this publisher has already been enqueued");
	}

	publisher->SetTime(gameTime.CurrentTime(), delay);
	mQueue.PushBack(publisher);
}

void EventQueue::Send(const std::shared_ptr<EventPublisher>& publisher)
{
	publisher->Deliver();
}

void EventQueue::Update(const GameTime & gameTime)
{
	Vector<std::shared_ptr<EventPublisher>> mQueueCopy = mQueue;

	for (const auto& publisher : mQueueCopy)
	{
		if (publisher->IsExpired(gameTime.CurrentTime()))
		{
			Send(publisher);
			mQueue.Remove(publisher);
		}
	}
}

void EventQueue::Clear()
{
	mQueue.Clear();
}

bool EventQueue::IsEmpty() const
{
	return mQueue.IsEmpty();
}

std::uint32_t EventQueue::Size() const
{
	return mQueue.Size();
}
