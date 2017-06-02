#include "stdafx.h"
#include "PhysicsObject.h"


glm::vec2 PhysicsObject::sm_v2Gravity = glm::vec2();
float PhysicsObject::sm_fCoeffRestitution = 1.0;

PhysicsObject::PhysicsObject(PhysicsObjectType objectType, glm::vec2 posn)
	: m_iObjectType(objectType), m_v2Position(posn), m_fRotation(0)
{
}

PhysicsObject::~PhysicsObject()
{
}


void PhysicsObject::checkCollision(PhysicsObject* obj) {
	switch (obj->m_iObjectType) {
	case PhysicsObjectType::PLANE:
		collideWithPlane(obj);
		break;
	case PhysicsObjectType::CIRCLE:
		collideWithCircle(obj);
		break;
	case PhysicsObjectType::TRIANGLE:
		//collideWithTriangle(obj);
		break;
	case PhysicsObjectType::BOX:
		collideWithBox(obj);
		break;
	}
}