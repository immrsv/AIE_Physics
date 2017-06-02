#pragma once

#include <glm\glm\glm.hpp>

#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	// PLane Normal, and distance _TO_ Origin along Normal
	Plane(glm::vec2 posn, glm::vec2 normal);
	~Plane();

	glm::vec2 m_v2Normal;

	
	virtual void draw();
	virtual void update(float deltaTime);

	virtual void collideWithPlane(PhysicsObject*);
	virtual void collideWithCircle(PhysicsObject*);
	virtual void collideWithBox(PhysicsObject*);

	// Kinematic object, ignore forces
	virtual void applyForce(glm::vec2 force, glm::vec2 posn = glm::vec2()) {};

private:
	glm::vec2 m_v2Parallel;
};

