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
	virtual void update(float deltaTime);

	virtual void collideWithPlane(PhysicsObject*);
	virtual void collideWithCircle(PhysicsObject*);
	virtual void collideWithBox(PhysicsObject*);

	virtual void applyForce(glm::vec2 force, glm::vec2 posn = glm::vec2());

private:
	glm::mat4 m_m4Transform;

};

