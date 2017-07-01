#include "stdafx.h"

#include "Spring.h"

#include <aie\Gizmos.h>


Spring::Spring(RigidBody* body1, vec2 contact1, RigidBody* body2, vec2 contact2, float length, float strength, float damping)
	: PhysicsObject(PhysicsObjectType::SPRING, vec2(0)),
	m_pBody1(body1), m_v2Contact1(contact1),
	m_pBody2(body2), m_v2Contact2(contact2),
	m_fRestLength(length),
	m_fStrength(strength),
	m_fDamping(damping)
{
}


Spring::~Spring()
{
}

void Spring::draw() {

	glm::vec2 start(m_pBody1->toWorld(m_v2Contact1));
	glm::vec2 end(m_pBody2->toWorld(m_v2Contact2));

	Gizmos::add2DLine(start, end, glm::vec4(1, 1, 1, 1));
}

void Spring::update(float deltaTime) {

	glm::vec2 p1 = m_pBody1->toWorld(m_v2Contact1);
	glm::vec2 p2 = m_pBody2->toWorld(m_v2Contact2);
	
	glm::vec2 dist = p2 - p1;

	float length = glm::length(dist);

	// apply damping
	glm::vec2 dv = m_pBody2->m_v2LinearVelocity - m_pBody1->m_v2LinearVelocity;
	glm::vec2 force = dist * m_fStrength * (m_fRestLength - length) - m_fDamping * dv;

	m_pBody1->applyForce(-force * deltaTime, p1 - m_pBody1->m_v2Position);
	m_pBody2->applyForce( force * deltaTime, p2 - m_pBody2->m_v2Position);

}
