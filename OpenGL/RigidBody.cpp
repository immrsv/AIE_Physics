//#include "stdafx.h"

#include <glm\glm\ext.hpp>

#include "aie/Gizmos.h"
#include "RigidBody.h"

#include <iostream>

RigidBody::RigidBody(PhysicsObjectType objectType, glm::vec2 posn, glm::vec2 vel)
	: PhysicsObject(objectType, posn), m_v2LinearVelocity(vel), m_fAngularVelocity(0), m_fMass(1), m_fMoment(1), m_bIsKinematic(false)
{
}

RigidBody::~RigidBody()
{
}

//void RigidBody::draw() {
//	Gizmos::add2DCircle(m_v2Position, 1, 32, glm::vec4(1, 0, 0, 1));
//}
//
void RigidBody::update(float deltaTime) {

	// Motion
	m_v2Position += (m_v2LinearVelocity * deltaTime);
	m_fRotation += (m_fAngularVelocity * deltaTime);


	m_m4Transform = glm::eulerAngleZ(m_fRotation);
}
//
//void RigidBody::collideWithPlane(PhysicsObject*) {}
//void RigidBody::collideWithCircle(PhysicsObject*) {}
//void RigidBody::collideWithBox(PhysicsObject*) {}


// contact in world coords

void RigidBody::resolveCollision(RigidBody* other, vec2 contact, vec2* direction) {

	// find the vector between centres, or use provided direction
	vec2 unitDisp = direction ? *direction : glm::normalize(other->m_v2Position - m_v2Position);

	// get component along axis for each object
	vec2 unitParallel(-unitDisp.y, unitDisp.x);

	// determine linear and angular velocities
	float r1 = glm::dot(contact - m_v2Position, unitParallel);
	float r2 = glm::dot(contact - other->m_v2Position, -unitParallel);
	float v1 = glm::dot(contact - m_v2Position, unitDisp) + r1 * m_fAngularVelocity;
	float v2 = glm::dot(contact - other->m_v2Position, -unitDisp) + r2 * other->m_fAngularVelocity;

	if ( v1 > v2 ) { // they're moving closer
		float effMass1 = 1.0f / (1.0f / m_fMass + (r1 * r1) / m_fMoment);
		float effMass2 = 1.0f / (1.0f / other->m_fMass + (r2 * r2) / other->m_fMoment);

		vec2 force = (1.0f + sm_fCoeffRestitution) * effMass1 * effMass2 / (effMass1 + effMass2) * (v1 - v2) * unitDisp;

		applyForce(-force, contact - m_v2Position);
		other->applyForce(force, contact - other->m_v2Position);
	}
}

void RigidBody::applyForce(vec2 force, vec2 posn) {

	//if (m_bIsKinematic) return; // Kinematic (non-reactive) object

	glm::vec2 linearAccel = force / m_fMass;
	float angularAccel = 0;
	
	if (glm::length(posn) != 0)
		angularAccel = (force.y * posn.x - force.x * posn.y) / m_fMoment;

	m_v2LinearVelocity += linearAccel;
	m_fAngularVelocity += angularAccel;
}