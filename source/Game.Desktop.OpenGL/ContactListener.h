#pragma once

#include <Box2D\Box2D.h>

class ContactListener : public b2ContactListener
{
public:
	ContactListener() = default;
	~ContactListener() = default;

	//Evaluates the start of the collision.
	void BeginContact(class b2Contact *contact);

	//Evaluates the end of the collision
	void EndContact(class b2Contact *contact);
};

