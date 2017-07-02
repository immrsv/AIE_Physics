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

	glm::vec4 m_v4Color;

	int tag;

	glm::mat4& getRotation();
	vec2& getLocalX();
	vec2& getLocalY();

	PhysicsObjectType m_iObjectType;

	virtual void draw() = 0;
	virtual void update(float deltaTime) = 0;

	bool checkCollision(PhysicsObject* rhs);

	virtual bool collideWithPlane(PhysicsObject*) = 0;
	virtual bool collideWithCircle(PhysicsObject*) = 0;
	virtual bool collideWithBox(PhysicsObject*) = 0;

	virtual bool isInside(glm::vec2 pt) { return false; }

protected:
	bool m_bIsRotationDirty = true;
	glm::mat4 cm_m4Rotation;
	vec2 cm_v2LocalX; // Calculated Member
	vec2 cm_v2LocalY;

	void updateRotation();
};

