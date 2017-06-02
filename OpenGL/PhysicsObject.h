#pragma once

#include <glm\glm\glm.hpp>

using glm::vec2;

class PhysicsObject
{
public:

	enum PhysicsObjectType {
		PLANE,
		CIRCLE,
		TRIANGLE,
		BOX,
		SPRING
	};

	PhysicsObject(PhysicsObjectType objectType, glm::vec2 posn);
	~PhysicsObject();

	static glm::vec2 sm_v2Gravity;
	static float sm_fCoeffRestitution;

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

