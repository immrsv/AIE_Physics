#pragma once

#include "RigidBody.h"

using glm::vec2;

class Box : public RigidBody
{
public:
	Box(glm::vec2 posn, vec2 vel, glm::vec2 size = glm::vec2(1,1));
	~Box();

	glm::vec2 m_v2Size;

	virtual void draw();

	virtual bool collideWithPlane(PhysicsObject*);
	virtual bool collideWithCircle(PhysicsObject*);
	virtual bool collideWithBox(PhysicsObject*);
	
	virtual bool checkBoxCorners(Box* box, vec2& contact, int& numContacts, float& penetration, vec2& edgeNormal);

	virtual bool IsInside(glm::vec2 pt);
};

