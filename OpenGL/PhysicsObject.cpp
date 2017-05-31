#include "stdafx.h"
#include "PhysicsObject.h"


glm::vec2 PhysicsObject::s_v2Gravity = glm::vec2();


PhysicsObject::PhysicsObject(PhysicsObjectType objectType, glm::vec2 posn)
	: m_iObjectType(objectType), m_v2Position(posn), m_fRotation(0)
{
}

PhysicsObject::~PhysicsObject()
{
}


void PhysicsObject::checkCollision(PhysicsObject* rhs) {

}