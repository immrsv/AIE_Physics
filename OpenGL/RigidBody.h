#pragma once

#include <glm\glm\glm.hpp>

#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(PhysicsObjectType objectType, glm::vec2 posn, glm::vec2 vel);
	~RigidBody();

	glm::vec2 m_v2Velocity;
	
	float m_fSpin;

	//virtual void draw();
	//virtual void update(float deltaTime);

	//virtual void collideWithPlane(PhysicsObject*);
	//virtual void collideWithCircle(PhysicsObject*);
	//virtual void collideWithBox(PhysicsObject*);
};

