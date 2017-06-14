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

	virtual void collideWithPlane(PhysicsObject*);
	virtual void collideWithCircle(PhysicsObject*);
	virtual void collideWithBox(PhysicsObject*);
	

};

