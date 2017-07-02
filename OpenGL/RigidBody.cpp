//#include "stdafx.h"

#include <glm\glm\ext.hpp>

#include "aie/Gizmos.h"
#include "RigidBody.h"

#include <iostream>
#include <limits>


float RigidBody::sm_fSleepSpeed = 0.00001f; // linear and angular speed
int RigidBody::sm_iSleepDelay = 30; // Number of physics frames

RigidBody::RigidBody(PhysicsObjectType objectType, glm::vec2 posn, glm::vec2 vel)
	: PhysicsObject(objectType, posn),
	m_v2LinearVelocity(vel),
	m_bIsAwake(true),
	m_fAngularVelocity(0),
	m_fMass(1),
	m_fMoment(1),
	m_bIsKinematic(false),
	m_iSleepCounter(sm_iSleepDelay)
{
	if (!m_bIsAwake) {
		m_v2LinearVelocity = vec2();
		m_fAngularVelocity = 0.0f;
	}
}

RigidBody::~RigidBody()
{
}


vec2 RigidBody::toWorld(vec2 local) {
	return m_v2Position + vec2(getRotation() * glm::vec4(local, 0, 1));
}


void RigidBody::update(float deltaTime) {

	if (m_bIsKinematic || !m_bIsAwake)
	{
		m_v2LinearVelocity = vec2();
		m_fAngularVelocity = 0.0f;

		return;
	}

	// Motion
	m_v2Position += (m_v2LinearVelocity * deltaTime);
	m_fRotation += (m_fAngularVelocity * deltaTime);

	m_bIsRotationDirty |= (m_fAngularVelocity != 0.0f); // Dirty only if angular velocity not zero

	if (glm::length(m_v2LinearVelocity) < sm_fSleepSpeed && m_fAngularVelocity < sm_fSleepSpeed) {
		if (m_iSleepCounter-- == 0) {
			m_bIsAwake = false;
			m_v2LinearVelocity = vec2();
			m_fAngularVelocity = 0.0f;
		}
	}
	else
		m_iSleepCounter = sm_iSleepDelay;
}



/// point of contact in world space
/// direction of impetus in world space

void RigidBody::resolveCollision(RigidBody* other, vec2 contact, vec2* direction) {

	if (m_bIsAwake || other->m_bIsAwake) {
		m_bIsAwake = true;
		other->m_bIsAwake = true;
	}

	else
		return; // Neither is awake

	// find the vector between centres, or use provided direction
	vec2 unitDisp = glm::normalize(direction ? *direction : (other->m_v2Position - m_v2Position));

	// get component along axis for each object
	vec2 unitParallel(unitDisp.y, -unitDisp.x);

	// determine linear and angular velocities
	float r1 = glm::dot(contact - m_v2Position, unitParallel);
	float r2 = glm::dot(contact - other->m_v2Position, -unitParallel);
	float v1 = glm::dot(m_v2LinearVelocity, unitDisp) + r1 * m_fAngularVelocity;
	float v2 = glm::dot(other->m_v2LinearVelocity, unitDisp) - r2 * other->m_fAngularVelocity;

	if ( v1 > v2 ) { // they're moving closer
		float effMass1 = m_bIsKinematic ? 999999 : 1.0f / (1.0f / m_fMass + (r1 * r1) / m_fMoment);
		float effMass2 = other->m_bIsKinematic ? 999999 : 1.0f / (1.0f / other->m_fMass + (r2 * r2) / other->m_fMoment);

		vec2 force = (1.0f + sm_fCoeffRestitution) * effMass1 * effMass2 / (effMass1 + effMass2) * (v1 - v2) * unitDisp;

		applyForce(-force, contact - m_v2Position);
		other->applyForce(force, contact - other->m_v2Position);
	}
}


/// posn in local space
void RigidBody::applyForce(vec2 force, vec2 posn) {

	glm::vec2 linearAccel = force / m_fMass;
	float angularAccel = 0;
	
	if (glm::length(posn) != 0)
		angularAccel = (force.y * posn.x - force.x * posn.y) / m_fMoment;

	m_v2LinearVelocity += linearAccel;
	m_fAngularVelocity += angularAccel;
}