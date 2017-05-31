//#include "stdafx.h"

#include "aie/Gizmos.h"
#include "RigidBody.h"

#include <iostream>

RigidBody::RigidBody(PhysicsObjectType objectType, glm::vec2 posn, glm::vec2 vel)
	: PhysicsObject(objectType, posn), m_v2Velocity(vel), m_fSpin(0)
{
}

RigidBody::~RigidBody()
{
}

//void RigidBody::draw() {
//	Gizmos::add2DCircle(m_v2Position, 1, 32, glm::vec4(1, 0, 0, 1));
//}
//
//void RigidBody::update(float deltaTime) {
//
//	// Acceleration
//	m_v2Velocity += (s_v2Gravity * deltaTime);
//
//
//	// Collision
//
//
//
//	// Motion
//	m_v2Position += (m_v2Velocity * deltaTime);
//	m_fRotation += (m_fSpin * deltaTime);
//}
//
//void RigidBody::collideWithPlane(PhysicsObject*) {}
//void RigidBody::collideWithCircle(PhysicsObject*) {}
//void RigidBody::collideWithBox(PhysicsObject*) {}