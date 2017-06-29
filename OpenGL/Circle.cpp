

#include <aie/Gizmos.h>
#include <iostream>

#include "Circle.h"
#include "Plane.h"


Circle::Circle(glm::vec2 posn, glm::vec2 vel, float radius)
	:RigidBody(PhysicsObjectType::CIRCLE, posn, vel),
	m_fRadius(radius)
{
	m_fMoment = 0.5f * m_fMass * m_fRadius * m_fRadius;
}


Circle::~Circle()
{
}

void Circle::draw() {
	glm::vec4 color = m_bIsAwake ? glm::vec4(1, 0, 0, 1) : glm::vec4(0.4, 0, 0, 1);
	Gizmos::add2DCircle(m_v2Position, m_fRadius, 32, color);

	vec2 highlight = toWorld(vec2(0,m_fRadius / 2.0f));
	Gizmos::add2DCircle(highlight, m_fRadius/5.0f, 32, glm::vec4(0.5, 0.1, 0.5, 1));
}



void Circle::collideWithPlane(PhysicsObject* obj) {

	Plane* plane = (Plane*)obj;

	// Height above plane
	float dist = glm::dot((m_v2Position - plane->m_v2Position), plane->m_v2Normal);

	float radialV = glm::dot(m_v2LinearVelocity, plane->m_v2Normal);


	if ((dist > 0 && dist < m_fRadius && radialV < 0) || (dist <0 && dist> -m_fRadius && radialV > 0)) {
		glm::vec2 deltaF = -m_fMass * (radialV * plane->m_v2Normal) * (1 + sm_fCoeffRestitution);
		applyForce(deltaF);
	}
}

void Circle::collideWithCircle(PhysicsObject* obj) {
	Circle* circle = dynamic_cast<Circle*>(obj);


	glm::vec2 collisionNormal = glm::normalize(m_v2Position - circle->m_v2Position);

	// Height above circle
	float dist = glm::length(m_v2Position - circle->m_v2Position) - (m_fRadius + circle->m_fRadius);

	float radialV = glm::dot(m_v2LinearVelocity - circle->m_v2LinearVelocity, collisionNormal);


	if (dist < 0 && radialV < 0) {
		if (m_bIsAwake || circle->m_bIsAwake) {
			m_bIsAwake = true;
			circle->m_bIsAwake = true;
		}
		else
			return; // Neither is awake

		glm::vec2 deltaF = -m_fMass * (radialV * collisionNormal) * (1 + sm_fCoeffRestitution);
		applyForce(0.5f * deltaF);
		circle->applyForce(-0.5f * deltaF);
	}
}

void Circle::collideWithBox(PhysicsObject* obj) {
	obj->collideWithCircle(this);
}
