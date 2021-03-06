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

	virtual bool collideWithPlane(PhysicsObject*);
	virtual bool collideWithCircle(PhysicsObject*);
	virtual bool collideWithBox(PhysicsObject*);

private:
	glm::vec2 m_v2Parallel;
};

