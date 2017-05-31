#pragma once

#include <glm\glm\glm.hpp>

class PhysicsObject
{
public:

	enum PhysicsObjectType {
		PLANE,
		CIRCLE,
		BOX,
		SPRING
	};

	PhysicsObject(PhysicsObjectType objectType, glm::vec2 posn);
	~PhysicsObject();

	static glm::vec2 s_v2Gravity;

	glm::vec2 m_v2Position;

	float m_fRotation;

	PhysicsObjectType m_iObjectType;

	virtual void draw() = 0;
	virtual void update(float deltaTime) = 0;

	void checkCollision(PhysicsObject* rhs);

	virtual void collideWithPlane(PhysicsObject*) = 0;
	virtual void collideWithCircle(PhysicsObject*) = 0;
	virtual void collideWithBox(PhysicsObject*) = 0;
};

