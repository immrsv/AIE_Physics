#pragma once

#include "RigidBody.h"

class Circle : public RigidBody
{
public:
	Circle(glm::vec2 posn, glm::vec2 vel, float radius);
	~Circle();

	float m_fRadius;

	virtual void draw();
	//virtual void update(float deltaTime);

	virtual bool collideWithPlane(PhysicsObject*);
	virtual bool collideWithCircle(PhysicsObject*);
	virtual bool collideWithBox(PhysicsObject*);

	virtual bool isInside(glm::vec2 pt);
};

